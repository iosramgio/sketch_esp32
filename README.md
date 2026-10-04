# ESP32 Firmware - IoT Smart Glove for BISINDO Sign Language Translation

Repositori ini berisi kode sumber (*firmware*) mikrokontroler **ESP32** yang digunakan pada perangkat *smart glove* (sarung tangan pintar) untuk sistem penerjemahan Bahasa Isyarat Indonesia (**BISINDO**). Perangkat ini berfungsi untuk membaca data sensor fisik (*flex sensor* dan IMU), melakukan prapemrosesan data, dan mengirimkannya secara *real-time* melalui protokol **MQTT**.

---

## 🏗️ Arsitektur Sistem

Berikut adalah diagram arsitektur menyeluruh dari sistem *smart glove* IoT hingga pemrosesan model *deep learning* dan antarmuka pengguna:

##🛠️ Komponen Perangkat Keras
Mikrokontroler: ESP32 NodeMCU Module (Wi-Fi Integrated)

Sensor Lentur (Flex Sensor): Dipasang pada jari-jari sarung tangan untuk mendeteksi tekukan.

Sensor Gerak (IMU MPU6050): Mengukur orientasi, percepatan, dan kecepatan sudut tangan.

Catu Daya: Baterai Li-Po yang terintegrasi dengan modul pengisi daya.

##📡 Alur Komunikasi Data
Pembacaan Sensor: ESP32 mengumpulkan data mentah dari flex sensor dan IMU secara kontinyu.

Konversi & Prapemrosesan: Data dikonversi menjadi format deret waktu (time-series).

Transmisi MQTT: Data dikirimkan secara nirkabel menggunakan protokol MQTT ke broker tujuan untuk diproses oleh model 1D CNN-LSTM.

##⚙️ Panduan Instalasi & Penggunaan
Prasyarat
Arduino IDE atau PlatformIO

Papan ESP32 Board Manager terinstal di IDE

Pustaka (Libraries) pendukung:

WiFi.h

PubSubClient (untuk komunikasi MQTT)

Pustaka sensor IMU (misal: Adafruit MPU6050 atau pengikut sensor terkait)

Konfigurasi
Ubah kredensial jaringan Wi-Fi pada file konfigurasi kode (SSID dan Password).

Masukkan alamat broker MQTT yang digunakan pada variabel mqtt_server.

Unggah (upload) kode ke papan ESP32 menggunakan kabel USB.
