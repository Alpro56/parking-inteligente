# Parking Inteligente con Arduino

## Información General

### Bienvenida

Bienvenido/a a la documentación del proyecto asignado por el profesor Juan Carlos Alejandres en el grupo FPBI 2ºA del Instituto IES Palomeras-Vallecas durante el curso 24/25.

**Realizado por los integrantes del Equipo1:**  
Álvaro Hernández, Iván Fernández y David Pérez.

**Título del Proyecto:** Parking Inteligente con Arduino  
**Autores:** Álvaro Hernández, Iván Fernández, David Pérez  
**Tutor:** Juan Carlos Alejandres  
**Institución:** IES Palomeras-Vallecas  
**Curso y Especialidad:** Formación Profesional Básica de Informática  
**Fecha de entrega estimada:** 11 de Abril de 2025

Explora cada sección para obtener información detallada y recursos adicionales.

---

## Índice General

1. [Bienvenida](#bienvenida)
2. [Índice General](#índice-general)
3. [Memoria Descriptiva](#memoria-descriptiva)
   - Introducción
   - Objetivo y Alcance
   - Antecedentes
   - Normas y Referencias
   - Definiciones y Abreviaturas
   - Requisitos de Diseño
   - Análisis de Soluciones
   - Resultados Finales
4. [Especificaciones Técnicas](#especificaciones-técnicas)
   - Descripción del Sistema
   - Componentes y Materiales
   - Diagrama Funcional
   - Diagrama Eléctrico
   - Programa Arduino
5. [Anexos y Recursos](#anexos-y-recursos)
   - Esquemas y Planos
   - Listado de Códigos y Comentarios
   - Fotografías del Montaje
   - Presupuesto
   - Fichero de Presentación
   - Presentación Power Point
   - Video de Funcionamiento
6. [Conclusiones](#conclusiones)

---

## Memoria Descriptiva

### Introducción

El proyecto **"Parking Inteligente con Arduino"** surge ante la necesidad de optimizar la gestión de espacios de estacionamiento en zonas urbanas o edificios con plazas limitadas. La automatización y monitorización en tiempo real son fundamentales para evitar el uso ineficiente de recursos y mejorar la experiencia del usuario. Mediante la integración de sensores, servomotores y una interfaz visual, se busca controlar la ocupación de las plazas, gestionar el acceso y salida de vehículos y calcular el coste de la estancia.

Este sistema ha sido diseñado para un entorno de prueba y desarrollo. Para su implementación en espacios públicos o comerciales, podrían requerirse modificaciones adicionales según normativas vigentes.

### Objetivo y Alcance

**Objetivo:**
- **Automatización del Acceso:** Controlar el acceso mediante barreras automatizadas que se activan al detectar la presencia de un vehículo.
- **Gestión de Plazas:** Visualización en tiempo real de las plazas disponibles y asignación de la plaza en función del orden de llegada.
- **Registro y Facturación:** Medición del tiempo de estancia y cálculo de la tarifa (1,20 €/hora) para la emisión de un ticket.
- **Interfaz Amigable:** Mostrar información relevante (estado de plazas, número asignado, etc.) en una pantalla LCD y mediante indicadores LED.

**Alcance:**
- **Hardware:** Integración de Arduino, servomotores, sensores ultrasónicos (HC-SR05), sensor IR, pantalla LCD y LEDs indicativos para 6 plazas.
- **Software:** Desarrollo de un programa en Arduino que gestione la detección, asignación, temporización, y comunicación visual y serial.
- **Documentación:** Elaboración completa de la documentación, memoria, esquemas, código fuente, pruebas, presupuesto y presentación final.

### Antecedentes

El proyecto se plantea en el contexto de la digitalización y automatización de sistemas de aparcamiento, aprovechando la versatilidad y bajo coste de la plataforma Arduino. Estudios previos sobre sensores de distancia y control de servomotores han permitido definir una solución robusta y escalable. Además, la integración de un sistema de control remoto mediante sensor IR facilita la gestión manual cuando sea necesaria.

### Normas y Referencias

- **Normativa Eléctrica:** Normas de instalaciones eléctricas y de baja tensión aplicables en el entorno educativo.
- **Documentación Arduino:** Manuales oficiales y guías de uso para Arduino Uno y módulos complementarios.
- **Fuentes Bibliográficas:** Se han consultado artículos técnicos, tutoriales y manuales de sensores HC-SR05, servomotores y pantallas LCD.

### Definiciones y Abreviaturas

- **Arduino:** Plataforma de prototipado electrónico de código abierto.
- **HC-SR05:** Sensor ultrasónico utilizado para la detección de obstáculos.
- **LCD:** Pantalla de cristal líquido utilizada para la visualización de datos.
- **Módulo I2C:** A este módulo se conectan todos los pines de la pantalla LCD para que solo se conecten dos pines a Arduino.
- **LED:** Diodo emisor de luz, usado en este proyecto para indicar el estado de cada plaza.
- **PWM:** Modulación por ancho de pulso, empleada para el control de servomotores.
- **IR:** Infrarrojo, utilizado para la transmisión de señales a distancia (mando a distancia).

---

## Requisitos de Diseño

### Requisitos Funcionales
- **Control de Acceso:** Subida y bajada automática de barreras mediante servomotores.
- **Detección de Vehículos:** Uso de sensores HC-SR05 para detectar la presencia de un vehículo en las zonas de entrada y salida.
- **Indicadores de Estado:** Cada plaza cuenta con un LED que indica si está libre (encendido) u ocupada (apagado).
- **Interfaz de Usuario:** Pantalla LCD para informar sobre la disponibilidad de plazas y la plaza asignada.
- **Gestión de Tiempo y Facturación:** Registro del tiempo de estancia para calcular el precio a pagar.
- **Control Remoto:** Mando IR para realizar funciones específicas (subir/bajar barreras y reiniciar el sistema).

### Requisitos Técnicos
- **Plataforma:** Arduino Uno o placa compatible.
- **Integración:** Conexión de al menos 6 LEDs, 2 sensores HC-SR05, 2 servomotores, 1 pantalla LCD, 1 módulo I2C y 1 sensor IR.
- **Comunicación:** Emisión de datos vía monitor serial para el ticket de salida.
- **Energía:** Fuente de alimentación estable y adecuada para todos los componentes.

### Requisitos Económicos
- **Presupuesto Ajustado:** Selección de componentes con buena relación calidad/precio sin sacrificar el rendimiento.

---

## Análisis de Soluciones

- **Tamaño del Parking:** Relación entre el coste de los materiales y el tamaño del parking.
- **Posición de los Servomotores:** Ubicación de los servomotores en las zonas de entrada y salida del parking.
- **Herramientas Usadas:** 
  - Arduino IDE
  - Visual Studio Code
  - Tinkercad
  - Wokwi
  - Trae
- **Diseño y Accesibilidad:** Compartimento inferior para almacenar cables y componentes, y fácil acceso para reparaciones.

---

## Resultados Finales

El sistema implementado ha cumplido con los objetivos propuestos:

- **Automatización:** Las barreras funcionan correctamente en respuesta a la detección de vehículos.
- **Gestión de Plazas:** La pantalla LCD muestra en tiempo real la disponibilidad de plazas.
- **Registro de Estancia:** Se registra el tiempo de cada vehículo y se calcula el importe a pagar de forma automática.
- **Interacción Usuario:** El mando IR permite el control manual y el reinicio del sistema, facilitando la operación en situaciones de fallo.

---

## Conclusiones

El proyecto ha logrado automatizar el acceso a las plazas de estacionamiento, permitiendo una gestión eficiente de los espacios disponibles. El sistema es escalable y adaptable, con posibilidad de ser implementado en diferentes entornos. La implementación en un entorno real requeriría algunas modificaciones y pruebas adicionales.
