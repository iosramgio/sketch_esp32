# ESP32 Firmware - IoT Smart Glove for BISINDO Sign Language Translation

Dokumentasi mengenai komponen perangkat keras, alur komunikasi data, serta panduan instalasi dan konfigurasi sistem.

---

## 🏗️ Arsitektur Sistem
![alt text](https://github.com/iosramgio/sketch_esp32/blob/main/public/diagram.png)

---

![alt text](https://github.com/iosramgio/sketch_esp32/blob/main/public/circuit_image.png)

---
## 🛠️ Komponen Perangkat Keras
- **Mikrokontroler:** ESP32 NodeMCU Module (Wi-Fi Integrated)
- **Sensor Lentur (Flex Sensor):** Dipasang pada jari-jari sarung tangan untuk mendeteksi tekukan.
- **Sensor Gerak (IMU MPU6050):** Mengukur orientasi, percepatan, dan kecepatan sudut tangan.
- **Catu Daya:** Baterai Li-Po yang terintegrasi dengan modul pengisi daya.

---

## 📡 Alur Komunikasi Data
1. **Pembacaan Sensor:** ESP32 mengumpulkan data mentah dari flex sensor dan IMU secara kontinyu.
2. **Konversi & Prapemrosesan:** Data dikonversi menjadi format deret waktu (*time-series*).
3. **Transmisi MQTT:** Data dikirimkan secara nirkabel menggunakan protokol MQTT ke broker tujuan untuk diproses oleh model 1D CNN-LSTM.

---

## ⚙️ Panduan Instalasi & Penggunaan

### Prasyarat
- Arduino IDE atau PlatformIO
- Papan ESP32 Board Manager terinstal di IDE
- **Pustaka (Libraries) Pendukung:**
  - `WiFi.h`
  - `PubSubClient` (untuk komunikasi MQTT)
  - Pustaka sensor IMU (misal: `Adafruit_MPU6050` atau library sensor terkait)

### Konfigurasi
1. Ubah kredensial jaringan Wi-Fi pada file konfigurasi kode (`SSID` dan `Password`).
2. Masukkan alamat broker MQTT yang digunakan pada variabel `mqtt_server`.
3. Unggah (*upload*) kode ke papan ESP32 menggunakan kabel USB.
