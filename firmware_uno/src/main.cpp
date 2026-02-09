#include <Arduino.h>
#include "MotorControl.h"

// --- Configuración de Pines ---
const int ENA = 5;  // PWM Motor Izq
const int IN1 = 6;
const int IN2 = 7;
const int ENB = 10; // PWM Motor Der
const int IN3 = 8;
const int IN4 = 9;

// Instancia de control de motores
MotorControl motores(ENA, IN1, IN2, ENB, IN3, IN4);

void setup() {
  Serial.begin(115200);
  
  motores.begin();
  
  Serial.println("Arduino Uno listo para comandos (Motores modularizados).");
}

void loop() {
  // Escuchar comandos del Wemos (m1:val, m2:val)
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    
    if (cmd.startsWith("m1:")) {
      motores.mover(1, cmd.substring(3).toInt());
    } else if (cmd.startsWith("m2:")) {
      motores.mover(2, cmd.substring(3).toInt());
    }
  }

  // Ejemplo de telemetría (se enviaría al Wemos cada cierto tiempo)
  static unsigned long lastSent = 0;
  if (millis() - lastSent > 200) {
    lastSent = millis();
    // Formato que la web ya entiende: dist:F,B
    int distF = 200; // Ejemplo
    int distB = 255; // Ejemplo
    Serial.print("dist:");
    Serial.print(distF);
    Serial.print(",");
    Serial.println(distB);
  }
}
