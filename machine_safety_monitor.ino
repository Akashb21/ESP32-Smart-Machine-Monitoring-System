#include "DHT.h"

#define DHTPIN 4
#define DHTTYPE DHT11

#define IR_PIN 15
#define BUZZER_PIN 25
#define RELAY_PIN 26

#define TEMP_LIMIT 38.0

DHT dht(DHTPIN, DHTTYPE);

bool machineShutdown = false;

void setup() {
  Serial.begin(115200);

  dht.begin();

  pinMode(IR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);

  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(RELAY_PIN, HIGH);   // Machine running simulation

  Serial.println("ESP32 Smart Machine Safety Monitor Started");
  Serial.println("-----------------------------------------");
}

void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  int irState = digitalRead(IR_PIN);

  bool objectDetected = (irState == LOW);
  bool highTemperature = false;

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("DHT11 Read Failed!");
    delay(2000);
    return;
  }

  if (temperature >= TEMP_LIMIT) {
    highTemperature = true;
  }

  if (objectDetected || highTemperature) {
    machineShutdown = true;
  } else {
    machineShutdown = false;
  }

  Serial.println();
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  Serial.print("Object Near Machine: ");
  Serial.println(objectDetected ? "YES" : "NO");

  if (machineShutdown) {
    Serial.println("ALERT: Unsafe Condition Detected!");
    Serial.println("Machine Status: SHUTDOWN");
    Serial.println("Relay Status: OFF");

    digitalWrite(BUZZER_PIN, HIGH);
    digitalWrite(RELAY_PIN, LOW);
  } else {
    Serial.println("Machine Status: RUNNING");
    Serial.println("Relay Status: ON");

    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(RELAY_PIN, HIGH);
  }

  Serial.println("-----------------------------------------");

  delay(2000);
}
