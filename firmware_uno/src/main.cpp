#include <Arduino.h>

// --- Configuración de Pines ---
// Definir aquí los pines de tus drivers de motores (L298N, etc.)
const int ENA = 5;  // PWM Motor Izq
const int IN1 = 6;
const int IN2 = 7;
const int ENB = 10; // PWM Motor Der
const int IN3 = 8;
const int IN4 = 9;

void setup() {
  Serial.begin(115200);
  
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  Serial.println("Arduino Uno listo para comandos.");
}

void moverMotor(int motor, int velocidad) {
  int pwPin = (motor == 1) ? ENA : ENB;
  int inA = (motor == 1) ? IN1 : IN3;
  int inB = (motor == 1) ? IN2 : IN4;

  if (velocidad == 0) {
    digitalWrite(inA, LOW);
    digitalWrite(inB, LOW);
    analogWrite(pwPin, 0);
  } else if (velocidad > 0) {
    digitalWrite(inA, HIGH);
    digitalWrite(inB, LOW);
    analogWrite(pwPin, min(255, velocidad));
  } else {
    digitalWrite(inA, LOW);
    digitalWrite(inB, HIGH);
    analogWrite(pwPin, min(255, abs(velocidad)));
  }
}

void loop() {
  // Escuchar comandos del Wemos (m1:val, m2:val)
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    
    if (cmd.startsWith("m1:")) {
      moverMotor(1, cmd.substring(3).toInt());
    } else if (cmd.startsWith("m2:")) {
      moverMotor(2, cmd.substring(3).toInt());
    }
  }

  // Ejemplo de telemetría (se enviaría al Wemos cada cierto tiempo)
  static unsigned long lastSent = 0;
  if (millis() - lastSent > 200) {
    lastSent = millis();
    // Formato que la web ya entiende: dist:F,B
    // Por ahora enviamos valores de prueba o lecturas reales si ya tienes los sensores
    int distF = 200; // Ejemplo
    int distB = 255; // Ejemplo
    Serial.print("dist:");
    Serial.print(distF);
    Serial.print(",");
    Serial.println(distB);
  }
}
