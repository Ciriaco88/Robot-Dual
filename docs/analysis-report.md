# 🧠 Analysis Report: Robot-Dual Project

Este documento sirve como referencia técnica para que una IA comprenda rápidamente la arquitectura, el flujo de datos y las dependencias de este proyecto robótico de dos placas.

## 🏗️ Resumen de Arquitectura (Dual-Board)
El proyecto utiliza una arquitectura de "Cerebro Dividido" para separar las tareas de alta latencia (WiFi/Red) de las tareas de tiempo real (Motores/Sensores).

### 1. Nodo de Comunicaciones (Wemos D1 / ESP8266)
- **Directorio:** `firmware_wemos/`
- **Responsabilidad:** Gateway de red y servidor de activos web.
- **Tecnologías:** `ESPAsyncWebServer`, `WebSockets`, `LittleFS`.
- **Flujo de Entrada:** Interfaz multitáctil (WebSocket) -> Serial (115200 bps).
- **Flujo de Salida:** Serial (Telemetría) -> WebSocket (Dashboard).

### 2. Nodo de Ejecución (Arduino Uno / AVR)
- **Directorio:** `firmware_uno/`
- **Responsabilidad:** Control de motores y adquisición de datos de sensores.
- **Tecnologías:** Control PWM nativo, I2C para sensores ToF.
- **Flujo de Entrada:** Serial (Comandos) -> Accionamiento de motores.
- **Flujo de Salida:** Lectura de sensores -> Serial (Telemetría en formato `dist:F,B`).

## 📡 Protocolo de Comunicación Serial
La comunicación entre ambas placas es bidireccional vía UART a **115200 baudios**.

### Comandos (Web -> Wemos -> Uno)
- `m1:VAL`: Velocidad motor izquierdo (-255 a 255).
- `m2:VAL`: Velocidad motor derecho (-255 a 255).
*Los comandos se terminan con un carácter de nueva línea (`\n`).*

### Telemetría (Uno -> Wemos -> Web)
- `dist:F,B`: String que representa las distancias frontal y trasera en mm.
*Ejemplo:* `dist:150,200\n`

## 🎨 Interfaz de Usuario (Dashboard)
- **Localización:** `firmware_wemos/data/`
- **Mecánica Táctica:** Implementación de sliders táctiles personalizados en `script.js` que utilizan el `touchId` para permitir el **multitouch real** sin interferencia de gestos del navegador (zoom/scroll bloqueados vía `touch-action: none`).

## 🔌 Requisitos de Hardware Críticos
- **Divisor de Tensión:** Necesario en la línea `TX (Uno) -> RX (Wemos)` (5V a 3.3V).
- **Masa Común:** Ambos GND deben estar unidos.
- **Alimentación:** Asegurar corriente suficiente para los motores (L298N) independientemente de la lógica.

## 🛠️ Notas para Depuración
- Para ver qué comandos envía el Wemos, conectar el Wemos al PC y observar el Monitor Serie.
- Para simular comandos hacia el Arduino, enviar strings tipo `m1:200` desde el Monitor Serie con el Arduino conectado al PC.
