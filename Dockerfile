# Usar una imagen base de Node.js
FROM node:18-slim

# Crear directorio de la aplicación
WORKDIR /app

# Copiar archivos de dependencias
COPY package*.json ./

# Instalar dependencias
RUN npm install

# Copiar el resto de archivos de la aplicación
COPY . .

# Exponer el puerto que usa la aplicación
EXPOSE 3000

# Comando para iniciar la aplicación
CMD ["npm", "start"]