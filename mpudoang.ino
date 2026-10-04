#include <Wire.h>

// Variabel untuk mendeteksi alamat aktif (0x68 atau 0x69)
int mpuAddress = 0x68; 
bool mpuTerdeteksi = false;

void setup() {
  Serial.begin(115200);
  
  Serial.println("\n--- MEMULAI TES MANDIRI MPU-6500 ---");
  delay(2000); // Tunggu arus listrik stabil

  // Paksa ESP32 membuka jalur I2C di pin default: SDA (21) dan SCL (22)
  Wire.begin(21, 22); 

  // --- CEK ALAMAT 0x68 ---
  Wire.beginTransmission(0x68);
  Wire.write(0x6B); // Register PWR_MGMT_1
  Wire.write(0);    // Bangunkan MPU (set ke 0)
  if (Wire.endTransmission() == 0) {
    mpuAddress = 0x68;
    mpuTerdeteksi = true;
    Serial.println("✅ SENSOR MERESPON DI ALAMAT: 0x68");
  } 
  // --- CEK ALAMAT ALTERNATIF 0x69 ---
  else {
    Wire.beginTransmission(0x69);
    Wire.write(0x6B);
    Wire.write(0);
    if (Wire.endTransmission() == 0) {
      mpuAddress = 0x69;
      mpuTerdeteksi = true;
      Serial.println("✅ SENSOR MERESPON DI ALAMAT: 0x69");
    }
  }

  if (!mpuTerdeteksi) {
    Serial.println("❌ ERROR: MPU-6500 Tidak Merespon di 0x68 maupun 0x69!");
    Serial.println("👉 SOLUSI: Cek kabel VCC (Hubungkan ke 3.3V), GND, SDA (21), SCL (22). Pastikan solderan kokoh!");
  } else {
    Serial.println("🚀 Mulai membaca data akselerometer & gyro...\n");
  }
}

void loop() {
  if (!mpuTerdeteksi) {
    // Jika tidak terdeteksi, diam di sini agar tidak membanjiri serial monitor dengan sampah
    delay(1000);
    return;
  }

  // Minta data 14 byte dari register MPU (Mulai dari data Accel X hingga Gyro Z)
  Wire.beginTransmission(mpuAddress);
  Wire.write(0x3B); // Alamat awal register data
  Wire.endTransmission(false);
  Wire.requestFrom(mpuAddress, 14, true);

  // Gabungkan bit HIGH dan LOW untuk masing-masing sumbu
  int16_t raw_ax = Wire.read() << 8 | Wire.read();
  int16_t raw_ay = Wire.read() << 8 | Wire.read();
  int16_t raw_az = Wire.read() << 8 | Wire.read();
  int16_t raw_temp = Wire.read() << 8 | Wire.read(); // Data Suhu internal
  int16_t raw_gx = Wire.read() << 8 | Wire.read();
  int16_t raw_gy = Wire.read() << 8 | Wire.read();
  int16_t raw_gz = Wire.read() << 8 | Wire.read();

  // Cetak data mentah ke Serial Monitor
  Serial.print("ACCEL -> X: "); Serial.print(raw_ax);
  Serial.print(" \tY: "); Serial.print(raw_ay);
  Serial.print(" \tZ: "); Serial.print(raw_az);
  
  Serial.print("   ||   GYRO -> X: "); Serial.print(raw_gx);
  Serial.print(" \tY: "); Serial.print(raw_gy);
  Serial.print(" \tZ: "); Serial.println(raw_gz);

  delay(200); // Delay 200ms agar mata kita sempat membaca angkanya
}