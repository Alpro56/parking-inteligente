// Librerías necesarias
#include <Servo.h>                // Librería para manejar servomotores
#include <LiquidCrystal_I2C.h>      // Librería para manejar la pantalla LCD (I2C)
#include <IRremote.h>


// PINES ,CONFIGURACIONES Y OBJETOS
// Configuración del LCD I2C (dirección 0x27 y tamaño 16x2)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Sensores ultrasónicos
const int trig1 = 3, echo1 = 7;  // Sensor ultrasónico 1 (Entrada)
const int trig2 = 2, echo2 = 4;  // Sensor ultrasónico 2 (Salida)

// Servomotores
Servo servo1, servo2;
const int pinServo1 = 6;    // Pin del servo de entrada
const int pinServo2 = 5;     // Pin del servo de salida

// LEDs
const int leds[6] = {8, 9, 10, 11, 13, A0};  // Pines para los LEDs de plazas

// Sensor IR
const int sensorIR = 12;
IRrecv irrecv(sensorIR);   // Instancia del receptor IR
decode_results results;         // Variable para almacenar datos recibidos

// Array para almacenar los códigos de los botones del mando
long mandos[6] = {0xFFA25D, 0xFF629D, 0xFFE21D, 0xFF22DD, 0xFF02FD, 0xFFC23D};


// Configuraciones
const int servo1cerrado = 90;
const int servo1abierto = 0;
const int servo2cerrado = 90;
const int servo2abierto = 0;

int distanciaCoche = 10;        // Distancia (cm) mínima para detectar un coche
int tiempoEspera = 3000;        // Tiempo (ms) que permanece abierta la barrera después de que el coche se vaya

float precioMinuto = 0.02;      // Precio que los clientes pagarán por cada minuto en el parking


// Variables de control
bool plazas[6] = {1, 1, 1, 1, 1, 1};    // 1 = libre, 0 = ocupada
long int tiempoEntrada[6] = {0, 0, 0, 0, 0, 0};    // Almacena el momento que un coche entró a una plaza
long int tiempoEstancia[6] = {0, 0, 0, 0, 0, 0};        // Almacena el tiempo que está un coche en una plaza
int plazaAsignada = 0;


void setup() {
  // Inicializa el monitor serial a 9600 baudios
  Serial.begin(9600);

  // Inicializar LCD
  lcd.init();
  lcd.begin(16,2);
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Configurando...");

  // Configuración de pines de los sensores ultrasónicos
  pinMode(trig1, OUTPUT);
  pinMode(echo1, INPUT);
  pinMode(trig2, OUTPUT);
  pinMode(echo2, INPUT);

  // Configuración del sensor IR
  pinMode(sensorIR, INPUT);
  irrecv.enableIRIn(); // Habilitar receptor infrarrojo
  
  // Inicializar servos
  servo1.attach(pinServo1);
  servo2.attach(pinServo2);

  servo1.write(servo1cerrado);
  servo2.write(servo2cerrado);

  // Configuración de pines de los LEDs
  for (int i = 0; i < 6; i++) {
    pinMode(leds[i], OUTPUT);
  }
  
  // Encender LEDs
  for (int i = 0; i < 6; i++) {
    digitalWrite(leds[i], HIGH);  // LED encendido (plaza libre)
    delay(300);
  }


  lcd.clear();
  lcd.print("Sistema Iniciado");
  delay(1000);
  lcd.clear();
  irrecv.resume();
}
void loop() {
  gestionar1();
  calcularEstancia(0, 0);
  gestionar2();
  gestionarPantalla();
  administracion();
  logs();
}

// Función para gestionar la entrada de un coche
void gestionar1() {
    if (detectarCoche(trig1, echo1) && comprobarPlazas()) {
        asignarPlaza();
    } else if (!comprobarPlazas()){
        // No hay plazas libres: muestra mensaje de parking lleno
        lcd.clear();
        lcd.print("Parking lleno");
    }
}

void calcularEstancia(int a, int p) {
    switch (a) {
        case 0: {
            for (int i = 0; i < 6; i++) {
                if (tiempoEntrada[i] != 0) {
                    tiempoEstancia[i] = millis()/1000 - tiempoEntrada[i];
                }
            }
            break;
        }
        case 1: {
            long segundos = tiempoEstancia[p];
            int horas = segundos / 3600;
            int minutos = (segundos % 3600) / 60;
            int segs = segundos % 60;
            float costeTotal = (segundos / 60.0) * precioMinuto;
            
            Serial.println("=================================");
            Serial.println("        TICKET DE ESTANCIA       ");
            Serial.println("=================================");
            Serial.print("  Plaza número: ");
            Serial.println(p + 1);
            Serial.print("  Tiempo de estancia: ");
            Serial.print(horas);
            Serial.print("h ");
            Serial.print(minutos);
            Serial.print("m ");
            Serial.print(segs);
            Serial.println("s");
            Serial.print("  Coste por minuto: ");
            Serial.print(precioMinuto, 2);
            Serial.println("€");
            Serial.print("  Coste total: ");
            Serial.print(costeTotal, 2);
            Serial.println("€");
            Serial.println("---------------------------------");
            Serial.println("  Fecha: *FECHA NO SINCRONIZADA*");
            Serial.println("  Hora: *HORA NO SINCRONIZADA*");
            Serial.println("---------------------------------");
            Serial.println("¡Gracias por confiar en nosotros!");
            Serial.println("=================================");
            tiempoEntrada[p] = 0;
            tiempoEstancia[p] = 0;
            break;
        }
    }
}

// Función para gestionar la salida de un coche
void gestionar2() {
  irrecv.resume();
    while (detectarCoche(trig2, echo2)) {
        lcd.clear();
        lcd.print("Esperando señal");
        lcd.setCursor(0,1);
        lcd.print("del mando");
        if (irrecv.decode(&results)) {
            Serial.print("Código recibido: ");
            Serial.println(results.value, HEX);
            
            for (int i = 0; i < 6; i++) {
                if (results.value == mandos[i]) {
                    liberarPlaza(i + 1);
                    break;
                }
            }
            
            irrecv.resume();
            return;
        }
        delay(100); // Pequeña pausa para no saturar el procesador
    }
    lcd.clear();
}

bool mantenerBarrera(int trig, int echo) {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  unsigned long duracion = pulseIn(echo, HIGH);
  int distancia = (duracion * 0.0343) / 2;
  return distancia <= distanciaCoche;
}

// Detectar coche usando sensor ultrasónico
bool detectarCoche(int trig, int echo) {
    const int LECTURAS = 15;  // Número de lecturas para confirmar
    int lecturas = 0;        // Contador de lecturas positivas
    
    for(int i = 0; i < LECTURAS; i++) {
        digitalWrite(trig, LOW);
        delayMicroseconds(2);
        digitalWrite(trig, HIGH);
        delayMicroseconds(10);
        digitalWrite(trig, LOW);

        unsigned long duracion = pulseIn(echo, HIGH);
        int distancia = (duracion * 0.0343) / 2;
        
        if(distancia <= distanciaCoche) {
            lecturas++;
        }
        
        delay(50);  // Pequeño retraso entre lecturas
    }
    
    return lecturas >= (LECTURAS - 1);  // Retorna true si la mayoría de lecturas detectan coche
}

// Gestionar barrera
void gestionarBarrera(Servo &servo, int trig, int echo, int anguloAbierto, int anguloCerrado) {
    while (mantenerBarrera(trig, echo)) {
        servo.write(anguloAbierto);
    }
    delay(tiempoEspera);
    servo.write(anguloCerrado);
}

// Comprobar si hay plazas libres
bool comprobarPlazas() {
    for (int i = 0; i < 6; i++) {
        if (plazas[i] == 1) {
            return true;
        }
    }
    return false;
}

// Asignar una plaza libre (se ocupa la plaza y se apaga el LED)
void asignarPlaza() {
    for (plazaAsignada = 0; plazaAsignada < 6; plazaAsignada++) {
        if (plazas[plazaAsignada] == 1) {
            plazas[plazaAsignada] = 0; // Ocupa la plaza
            lcd.clear();
            lcd.print("Dirijase a");
            lcd.setCursor(0, 1);
            lcd.print("la plaza: ");
            lcd.print(plazaAsignada + 1);
            tiempoEntrada[plazaAsignada] = millis()/1000;
            break;
        }
    }
    digitalWrite(leds[plazaAsignada], LOW);
    gestionarBarrera(servo1, trig1, echo1, servo1abierto, servo1cerrado);
    // Efecto de parpadeo en el LED de la plaza
    for (int rep = 1; rep <= 4; rep++) {
        digitalWrite(leds[plazaAsignada], HIGH);
        delay(200);
        digitalWrite(leds[plazaAsignada], LOW);
        delay(200);
    }
    digitalWrite(leds[plazaAsignada], LOW);
}

// Liberar una plaza ocupada (se libera la plaza y se enciende el LED)
void liberarPlaza(int numPlaza) {
  
  int index = numPlaza - 1;  // Convertir de 1-indexado a 0-indexado
  
  // Comprueba si la plaza está ocupada
  if (plazas[index] == 0) {
    plazas[index] = 1; // Libera la plaza
    digitalWrite(leds[index], HIGH);
    lcd.clear();
    lcd.print("Plaza liberada:");
    lcd.setCursor(0, 1);
    lcd.print("Plaza "); 
    lcd.print(numPlaza);
    calcularEstancia(1, index);
    gestionarBarrera(servo2, trig2, echo2, servo2abierto, servo2cerrado);
  } else {
    lcd.clear();
    lcd.print("Plaza ya libre");
    lcd.setCursor(0, 1);
    lcd.print("Plaza ");
    lcd.print(numPlaza);
    delay(1000);
  }
  irrecv.resume();
}

void administracion() {
    int y = 0;
  while (y < 5) {
    if (irrecv.decode(&results)) {
        switch (results.value) {
            case 0xFFE01F: // Botón 7: Abrir barrera entrada
                servo1.write(servo1abierto);
                lcd.clear();
                lcd.print("Entrada abierta");
                break;
            case 0xFF6897: // Botón *: Cerrar barrera entrada
                servo1.write(servo1cerrado);
                lcd.clear();
                lcd.print("Entrada cerrada");
                break;
            case 0xFF906F: // Botón 9: Abrir barrera salida
                servo2.write(servo2abierto);
                lcd.clear();
                lcd.print("Salida abierta");
                break;
            case 0xFFB04F: // Botón #: Cerrar barrera salida
                servo2.write(servo2cerrado);
                lcd.clear();
                lcd.print("Salida cerrada");
                break;
            case 0xFFA857: // Botón 8: Encender LED de plaza
            case 0xFF9867: // Botón 0: Apagar LED de plaza
            case 0xFF18E7: // Botón flecha arriba: Liberar plaza
            case 0xFF4AB5: // Botón flecha abajo: Ocupar plaza
                int accion = results.value == 0xFFA857 ? 1 : // Encender LED
                            results.value == 0xFF9867 ? 2 : // Apagar LED
                            results.value == 0xFF18E7 ? 3 : // Liberar plaza
                            4; // Ocupar plaza
                
                String mensajes[] = {"para encender", "para apagar", "para liberar", "para ocupar"};
                lcd.clear();
                lcd.print("Seleccione plaza");
                lcd.setCursor(0, 1);
                lcd.print(mensajes[accion - 1]);
                
                irrecv.resume();
                while (!irrecv.decode(&results)) {
                    delay(100);
                }
                
                int plaza = -1;
                switch (results.value) {
                    case 0xFFA25D: plaza = 0; break; // Botón 1
                    case 0xFF629D: plaza = 1; break; // Botón 2
                    case 0xFFE21D: plaza = 2; break; // Botón 3
                    case 0xFF22DD: plaza = 3; break; // Botón 4
                    case 0xFF02FD: plaza = 4; break; // Botón 5
                    case 0xFFC23D: plaza = 5; break; // Botón 6
                }
                
                if (plaza >= 0) {
                    switch (accion) {
                        case 1: // Encender LED
                            digitalWrite(leds[plaza], HIGH);
                            lcd.clear();
                            lcd.print("LED encendido");
                            break;
                        case 2: // Apagar LED
                            digitalWrite(leds[plaza], LOW);
                            lcd.clear();
                            lcd.print("LED apagado");
                            break;
                        case 3: // Liberar plaza
                            plazas[plaza] = 1;
                            digitalWrite(leds[plaza], HIGH);
                            tiempoEntrada[plaza] = 0;
                            tiempoEstancia[plaza] = 0;
                            lcd.clear();
                            lcd.print("Plaza liberada");
                            break;
                        case 4: // Ocupar plaza
                            plazas[plaza] = 0;
                            digitalWrite(leds[plaza], LOW);
                            tiempoEntrada[plaza] = millis()/1000;
                            lcd.clear();
                            lcd.print("Plaza ocupada");
                            break;
                    }
                    lcd.setCursor(0, 1);
                    lcd.print("Plaza ");
                    lcd.print(plaza + 1);
                }
                break;
            case 0xFF38C7: // Botón OK: Reiniciar parking
                // Reiniciar todas las plazas a libres
                for (int i = 0; i < 6; i++) {
                    plazas[i] = 1;
                    digitalWrite(leds[i], HIGH);
                    tiempoEntrada[i] = 0;
                    tiempoEstancia[i] = 0;
                }
                // Cerrar barreras
                servo1.write(servo1cerrado);
                servo2.write(servo2cerrado);
                lcd.clear();
                lcd.print("Parking");
                lcd.setCursor(0, 1);
                lcd.print("reiniciado");
                break;
        }
        irrecv.resume();
    }
    y++;
  }
}

void logs() {
  Serial.println("\n===== ESTADO DEL PARKING =====\n");
  
  // Estado de las plazas
  Serial.println("PLAZAS:");
  for (int i = 0; i < 6; i++) {
    Serial.print("Plaza ");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.print(plazas[i] ? "LIBRE" : "OCUPADA");
    if (!plazas[i]) {
      Serial.print(" (Tiempo: ");
      long segundos = tiempoEstancia[i];
      int horas = segundos / 3600;
      int minutos = (segundos % 3600) / 60;
      int segs = segundos % 60;
      Serial.print(horas);
      Serial.print("h ");
      Serial.print(minutos);
      Serial.print("m ");
      Serial.print(segs);
      Serial.print("s)");
    }
    Serial.println();
  }
  
  // Estado de las barreras
  Serial.println("\nBARRERAS:");
  Serial.print("Entrada: ");
  Serial.println(servo1.read() == servo1cerrado ? "CERRADA" : "ABIERTA");
  Serial.print("Salida: ");
  Serial.println(servo2.read() == servo2cerrado ? "CERRADA" : "ABIERTA");
  
  // Resumen
  int plazasLibres = 0;
  for (int i = 0; i < 6; i++) {
    if (plazas[i]) plazasLibres++;
  }
  Serial.println("\nRESUMEN:");
  Serial.print("Plazas libres: ");
  Serial.print(plazasLibres);
  Serial.print("/6");
  Serial.println(plazasLibres == 0 ? " (COMPLETO)" : "");
  
  Serial.println("\n============================\n");
  delay(100);
}

void gestionarPantalla() {
    int libres = 0;
    for (int i = 0; i < 6; i++) {
        if (plazas[i] == 1) {
            libres++;
        }
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    String line1, line2;
    switch (random(0,2)) {
        case 0: {
            line1 = "Bienvenido!";
            line2 = "Plazas: " + String(libres) + "/6";
            break;
        }
            
        case 1: {
            line1 = "Precio/min: " + String(precioMinuto, 2) + "€";
            line2 = "Plazas: " + String(libres) + "/6";
            break;
        }

        case 2:{
            if (!comprobarPlazas()) {
                line1 = "Parking lleno";
                line2 = "Vuelva pronto!";
            } else {
                line1 = "Hay " + String(libres) + " plazas";
                line2 = "disponibles";
            }
            break;
        }
    }
    lcd.print(line1);
    lcd.setCursor(0, 1);
    lcd.print(line2);
    Serial.print("lcd:");
    Serial.print(line1);
    Serial.print("|");
    Serial.println(line2);
    delay(500);
}