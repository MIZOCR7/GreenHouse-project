#include "DHT.h"

#define DHTPIN 4
#define DHTTYPE DHT11

#define PIR_PIN 25
#define SOIL_PIN 32
#define LIGHT_PHOTO_PIN 12   // حساس الضوء للنبات

#define BUZZER_PIN 33

#define FAN_RELAY 26
#define LIGHT_RELAY 27
#define PUMP_RELAY 14
#define HEATER_RELAY 13

#define LED1 21
#define LED2 19
#define LED3 22
#define LED4 18

DHT dht(DHTPIN, DHTTYPE);

// ⏱️ تحديث كل 5 ثواني (مناسب للعرض)
unsigned long lastReadTime = 0;
const long interval = 5000;

// 📊 متوسط القراءات
int getAverage(int pin) {
  long sum = 0;
  for (int i = 0; i < 10; i++) {
    sum += analogRead(pin);
    delay(5);
  }
  return sum / 10;
}

void setup() {
  Serial.begin(115200);
  dht.begin();

  pinMode(PIR_PIN, INPUT);
  pinMode(SOIL_PIN, INPUT);
  pinMode(LIGHT_PHOTO_PIN, INPUT);

  pinMode(BUZZER_PIN, OUTPUT);

  pinMode(FAN_RELAY, OUTPUT);
  pinMode(PUMP_RELAY, OUTPUT);
  pinMode(LIGHT_RELAY, OUTPUT);
  pinMode(HEATER_RELAY, OUTPUT);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);

  Serial.println("SMART GREENHOUSE SYSTEM STARTED 🌱");
}

void loop() {

  handleSecurity(); // شغال دايمًا

  if (millis() - lastReadTime >= interval) {
    lastReadTime = millis();

    float temp = dht.readTemperature();
    float hum = dht.readHumidity();
    int soil = getAverage(SOIL_PIN);
    int light = getAverage(LIGHT_PHOTO_PIN);

    Serial.println("\n===== SYSTEM DATA =====");

    // ✅ تأكيد قراءة الحساس
    if (isnan(temp) || isnan(hum)) {
      Serial.println("❌ DHT ERROR");
    } else {
      Serial.print("🌡️ Temp: "); Serial.println(temp);
      Serial.print("💧 Humidity: "); Serial.println(hum);
    }

    Serial.print("🌱 Soil: "); Serial.println(soil);
    Serial.print("☀️ Light: "); Serial.println(light);

    controlTemperature(temp);
    controlSoil(soil);
    controlLight(light);
  }
}

// ================= SYSTEM FUNCTIONS =================

// 🌡️ الحرارة → مروحة + سخان
void controlTemperature(float t) {

  if (t > 30) {
    digitalWrite(FAN_RELAY, HIGH);
    digitalWrite(HEATER_RELAY, LOW);
    Serial.println("🔥 Fan ON (Cooling)");
  }
  else if (t < 22) {
    digitalWrite(FAN_RELAY, LOW);
    digitalWrite(HEATER_RELAY, HIGH);
    Serial.println("🔥 Heater ON (Warming)");
  }
  else {
    digitalWrite(FAN_RELAY, LOW);
    digitalWrite(HEATER_RELAY, LOW);
    Serial.println("✅ Temp Stable");
  }
}

// 💧 التربة → مضخة + LEDs
void controlSoil(int soil) {

  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);

  if (soil > 3000) {
    digitalWrite(LED1, HIGH);
    digitalWrite(PUMP_RELAY, HIGH);
    Serial.println("🚨 Soil VERY DRY → Pump ON");
  }
  else if (soil > 2200) {
    digitalWrite(LED2, HIGH);
    digitalWrite(PUMP_RELAY, LOW);
    Serial.println("⚠️ Soil DRY");
  }
  else if (soil > 1500) {
    digitalWrite(LED3, HIGH);
    digitalWrite(PUMP_RELAY, LOW);
    Serial.println("✅ Soil GOOD");
  }
  else {
    digitalWrite(LED4, HIGH);
    digitalWrite(PUMP_RELAY, LOW);
    Serial.println("💧 Soil WET");
  }
}

// 💡 الضوء → لمبة نمو
void controlLight(int light) {

  if (light < 800) {
    digitalWrite(LIGHT_RELAY, HIGH);
    Serial.println("💡 Grow Light ON");
  } else {
    digitalWrite(LIGHT_RELAY, LOW);
    Serial.println("☀️ Enough Light");
  }
}

// 🚨 الأمان → PIR + Buzzer
void handleSecurity() {

  if (digitalRead(PIR_PIN) == HIGH) {
    tone(BUZZER_PIN, 1000);
    Serial.println("🚨 Motion Detected!");
  } else {
    noTone(BUZZER_PIN);
  }
}
