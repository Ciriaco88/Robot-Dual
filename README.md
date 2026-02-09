# 🤖 Robot Dual-Board (Wemos D1 + Arduino Uno)

Este proyecto evoluciona el robot original hacia una arquitectura de dos controladores para maximizar pines, potencia y escalabilidad. Utiliza una **Wemos D1 (ESP8266)** para la gestión inalámbrica y un **Arduino Uno** para el control físico de motores y sensores.

## 🚀 Arquitectura Dual
El sistema se divide en dos grandes bloques:

1.  **Wemos D1 (Comunicaciones):**
    *   Gestiona el Punto de Acceso WiFi (`Robot-Dual`).
    *   Sirve la interfaz web multitáctil (HTML/CSS/JS).
    *   Actúa como puente, reenviando comandos web al Arduino vía **Serial (115200 baudios)**.
2.  **Arduino Uno (Acción):**
    *   Recibe comandos de movimiento (ej: `m1:200`).
    *   Controla directamente el driver de motores.
    *   Gestiona los sensores (VL53L0X, ultrasónicos, etc.) y envía la telemetría al Wemos.

## 🛠️ Conexión entre Placas (Wemos ↔ Uno)

| Wemos D1 | Arduino Uno | Función |
| :--- | :--- | :--- |
| **TX** | **Pin 0 (RX)** | Comandos (Web -> Motores) |
| **RX** | **Pin 1 (TX)** | Telemetría (Sensores -> Web) |
| **GND** | **GND** | Masa común (Obligatorio) |

> [!CAUTION]
> **Voltaje de Señal:** El Arduino Uno opera a 5V y el Wemos a 3.3V. En la línea `TX (Uno) -> RX (Wemos)`, se recomienda usar un **divisor de tensión** (o convertidor de nivel lógico) para no dañar el Wemos.

## 📁 Estructura del Proyecto
*   `/firmware_wemos`: Código fuente para la Wemos D1 y carpeta `data/` con la interfaz web.
*   `/firmware_uno`: Código fuente para el Arduino Uno.
*   `.gitignore`: Filtros para Git (excluye carpetas `.pio`).

## 💻 Instalación
Este proyecto se gestiona con **PlatformIO**:

1.  Abre la carpeta raíz `Robot-Dual` en VS Code.
2.  PlatformIO detectará los dos entornos.
3.  Usa el entorno `env:d1` para subir el código y la web al Wemos.
4.  Usa el entorno `env:uno` para subir el código al Arduino Uno.

## 📱 Control
1.  Conéctate a la red Wi-Fi `Robot-Dual` (Pass: `12345678`).
2.  Navega a `http://192.168.4.1`.
3.  Usa los nuevos controles multitáctiles para manejar el robot.

---
Proyecto desarrollado como evolución del Robot Upcycled original.
