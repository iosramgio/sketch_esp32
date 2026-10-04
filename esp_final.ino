#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <ArduinoJson.h>

// ==========================================
// 1. PENGATURAN JARINGAN & MQTT
// ==========================================
const char* ssid = "wifii";          
const char* password = "satuduatiga";
const char* mqtt_server = "10.32.227.195";

WiFiClient espClient;
PubSubClient client(espClient);

// ==========================================
// 2. PENGATURAN SENSOR FLEX 
// ==========================================
const int flexPins[] = {32, 35, 34, 39, 36}; 
const int numFlex = 5;

// Angka kalibrasi 
int valLurus[] = {1895, 100, 1845, 2140, 2095}; 
int valTekuk[] = {1865, 30, 1830, 2130, 2070}; 

// Variabel Smoothing
const int SAMPLE_SIZE = 8; 
int readings[5][SAMPLE_SIZE]; 
int readIndex = 0;
long total[5] = {0};
int average[5] = {0};
int persenFlex[5] = {0};

// ==========================================
// 3. PENGATURAN MPU 
// ==========================================
const int MPU_ADDR = 0x68; // Alamat I2C MPU lo yang terdeteksi

// Timer pengiriman data
unsigned long lastMsg = 0;
const int delayMS = 20; // 50x update per detik

// ==========================================
// FUNGSI WIFI & MQTT
// ==========================================
void setup_wifi() {
  delay(10);
  Serial.println("\nMenghubungkan ke WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n☑ WiFi terhubung!");
  Serial.print("IP Address ESP32: ");
Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Mencoba terhubung ke MQTT Broker...");
    if (client.connect("ESP32_Glove_Client")) {
      Serial.println("☑ Terhubung!");
    } else {
      Serial.print("☒ Gagal, rc=");
      Serial.print(client.state());
      Serial.println(" Coba lagi dalam 5 detik");
      delay(5000);
    }
  }
}

// ==========================================
// SETUP
// ==========================================
void setup() {
  Serial.begin(115200);
  
  Serial.println("Menunggu listrik stabil...");
  delay(2000); 

  // Inisialisasi Wi-Fi dan MQTT
  setup_wifi();
  client.setServer(mqtt_server, 1883);

  // Inisialisasi I2C dan MPU6500 (Raw Mode)
  Wire.begin(21, 22);
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B); // Register Power Management
  Wire.write(0);    // Bangunkan MPU (set ke 0)
  Wire.endTransmission(true);
  Serial.println("☑ MPU-6500 Siap (Raw Mode)!");

  // Set pin Flex Sensor dan kosongkan buffer smoothing
  for (int i = 0; i < numFlex; i++) {
    pinMode(flexPins[i], INPUT);
    for (int j = 0; j < SAMPLE_SIZE; j++) {
      readings[i][j] = 0;
    }
  }
}

// ==========================================
// MAIN LOOP
// ==========================================
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > delayMS) {
    lastMsg = now;

    // --- 1. BACA FLEX SENSOR & SMOOTHING ---
    for (int i = 0; i < numFlex; i++) {
      total[i] = total[i] - readings[i][readIndex];
      readings[i][readIndex] = analogRead(flexPins[i]);
      total[i] = total[i] + readings[i][readIndex];
      average[i] = total[i] / SAMPLE_SIZE;
      
      persenFlex[i] = map(average[i], valLurus[i], valTekuk[i], 0, 100);
      persenFlex[i] = constrain(persenFlex[i], 0, 100);
    }
    readIndex = (readIndex + 1) % SAMPLE_SIZE;

    // --- 2. BACA MPU-6500 (RAW I2C) ---
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x3B); // Mulai dari register Accel X+
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_ADDR, 14, true);

    // Ambil data mentah lalu konversi ke skala standar Float
    float ax = (Wire.read() << 8 | Wire.read()) / 16384.0;
    float ay = (Wire.read() << 8 | Wire.read()) / 16384.0;
    float az = (Wire.read() << 8 | Wire.read()) / 16384.0;
    Wire.read(); Wire.read(); // Lewati data Suhu internal (2 byte)
    float gx = (Wire.read() << 8 | Wire.read()) / 131.0;
    float gy = (Wire.read() << 8 | Wire.read()) / 131.0;
    float gz = (Wire.read() << 8 | Wire.read()) / 131.0;

    // --- 3. BUNGKUS FORMAT JSON ---
    StaticJsonDocument<384> doc;
    doc["timestamp"] = now;
    
    // Array Flex (0-100)
    JsonArray flexArray = doc.createNestedArray("flex");
    for (int i = 0; i < numFlex; i++) {
      flexArray.add(persenFlex[i]);
    }

    // Array Akselerometer
    JsonArray accelArray = doc.createNestedArray("accel");
    accelArray.add(ax);
    accelArray.add(ay);
    accelArray.add(az);

    // Array Gyroscope
    JsonArray gyroArray = doc.createNestedArray("gyro");
    gyroArray.add(gx);
    gyroArray.add(gy);
    gyroArray.add(gz);

    // --- 4. UBAH KE STRING & PUBLISH KE MQTT ---
    char jsonString[384];
    serializeJson(doc, jsonString);
    
    client.publish("bisindo/sensor/data", jsonString);
    
    // Tampilkan di Serial Monitor
    Serial.println(jsonString);
  }
}