#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>

// --- Configuración y Variables Globales ---
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

// --- Manejador de mensajes WebSocket (Wemos -> Arduino via Serial) ---
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client,
             AwsEventType type, void *arg, uint8_t *data, size_t len) {
  if (type == WS_EVT_DATA) {
    // Reenviar el mensaje recibido desde la web directamente al puerto Serial
    // El Arduino Uno estará escuchando este puerto.
    Serial.write(data, len);
    Serial.println(); // Añadir salto de línea para facilitar el parseo en el Uno
  }
}

void setup() {
  // Comunicación con Arduino Uno (y para depuración)
  Serial.begin(115200);

  // Iniciar sistema de archivos para la web
  if (!LittleFS.begin()) {
    Serial.println("¡ERROR! LittleFS no montado.");
    return;
  }

  // Configuración de Red (Punto de Acceso)
  WiFi.softAP("Robot-Dual", "12345678");
  IPAddress myIP = WiFi.softAPIP();

  // Configuración de WebSockets y Servidor
  ws.onEvent(onEvent);
  server.addHandler(&ws);
  server.serveStatic("/", LittleFS, "/").setDefaultFile("index.html");

  server.begin();
  
  Serial.println("Wemos listo. IP: " + myIP.toString());
}

void loop() {
  ws.cleanupClients();

  // Escuchar datos que vienen del Arduino Uno (Sensores -> Web)
  if (Serial.available()) {
    String telemetry = Serial.readStringUntil('\n');
    if (telemetry.length() > 0) {
      // Reenviar telemetría a todos los clientes web (Dashboard)
      ws.textAll(telemetry);
    }
  }

  yield();
}
