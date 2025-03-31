# Instrucciones para Docker

Este documento describe cómo ejecutar el sistema de Parking Inteligente usando Docker.

## Requisitos Previos

- Docker instalado en tu sistema
- Docker Compose instalado en tu sistema

## Construcción y Ejecución

1. Construir y ejecutar el contenedor:
   ```bash
   docker-compose up --build
   ```

2. Acceder a la aplicación:
   - Abrir el navegador y visitar: `http://localhost:3000`
   - Para el panel de administración: `http://localhost:3000/admin.html`

## Detener la Aplicación

Para detener la aplicación:
```bash
docker-compose down
```

## Subir la Imagen a Docker Hub

1. Inicia sesión en Docker Hub:
   ```bash
   docker login
   ```

2. Etiqueta la imagen con tu nombre de usuario de Docker Hub:
   ```bash
   docker tag parking-inteligente-app tu-usuario/parking-inteligente-app:latest
   ```

3. Sube la imagen a Docker Hub:
   ```bash
   docker push tu-usuario/parking-inteligente-app:latest
   ```

## Notas Importantes

- El servidor se ejecuta en el puerto 3000
- Los cambios en los archivos de src y public se reflejarán automáticamente
- La aplicación se reiniciará automáticamente si ocurre algún error
- Para usar la imagen desde Docker Hub: `docker pull tu-usuario/parking-inteligente-app:latest`