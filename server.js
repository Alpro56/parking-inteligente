const express = require('express');
const app = express();
const http = require('http').Server(app);
const io = require('socket.io')(http);

// Modo simulación
let simulationMode = true;

// Estado del parking
let parkingState = {
  lcd: {
    line1: '',
    line2: ''
  },
  plazas: [true, true, true, true, true, true], // true = libre, false = ocupada
  barreras: {
    entrada: false, // false = cerrada, true = abierta
    salida: false
  },
  leds: [true, true, true, true, true, true] // true = encendido, false = apagado
};

// Servir archivos estáticos
app.use(express.static('public'));
app.use(express.json());

// Rutas API
app.get('/api/status', (req, res) => {
  res.json(parkingState);
});

// Conexión Socket.IO
io.on('connection', (socket) => {
  console.log('Cliente conectado');
  socket.emit('status', parkingState);

  // Eventos de administración
  socket.on('toggleBarrera', (data) => {
    if (data.barrera === 'entrada') {
      parkingState.barreras.entrada = data.estado;
    } else {
      parkingState.barreras.salida = data.estado;
    }
    io.emit('status', parkingState);
  });

  socket.on('toggleLed', (data) => {
    parkingState.leds[data.plaza - 1] = data.estado;
    io.emit('status', parkingState);
  });

  socket.on('togglePlaza', (data) => {
    parkingState.plazas[data.plaza - 1] = data.estado;
    io.emit('status', parkingState);
  });
});

// Simulación de actualizaciones del LCD
setInterval(() => {
  const messages = [
    { line1: "Bienvenido!", line2: `Plazas: ${parkingState.plazas.filter(p => p).length}/6` },
    { line1: "Precio/min: 0.02€", line2: `Plazas: ${parkingState.plazas.filter(p => p).length}/6` },
    { line1: `Hay ${parkingState.plazas.filter(p => p).length} plazas`, line2: "disponibles" }
  ];
  parkingState.lcd = messages[Math.floor(Math.random() * messages.length)];
  io.emit('status', parkingState);
}, 3000);

// Iniciar servidor
const PORT = process.env.PORT || 3000;
http.listen(PORT, () => {
  console.log(`Servidor web iniciado en puerto ${PORT}`);
});