# 🧤 ESP32 Firmware: IoT Smart Glove for BISINDO Sign Language Translation

![ESP32](https://img.shields.io/badge/Hardware-ESP32-blue?logo=espressif)
![Protocol](https://img.shields.io/badge/Protocol-MQTT-green?logo=eclipse-mosquitto)
![Framework](https://img.shields.io/badge/Framework-Arduino%20IDE%20%2F%20PlatformIO-orange)
![License](https://img.shields.io/badge/License-MIT-brightgreen)

Firmware ini merupakan bagian dari sistem penerjemah **Bahasa Isyarat Indonesia (BISINDO)** berbasis *wearable device* dan kecerdasan buatan hibrida **1D CNN-LSTM**. Kode sumber ini berjalan pada mikrokontroler **ESP32**, bertugas melakukan akuisisi data sensor kinematika secara real-time, prapemrosesan sinyal mentah, serta transmisi *payload* time-series melalui protokol **MQTT**.

---

## 📑 Daftar Isi
- [Fitur Utama](#-fitur-utama)
- [Arsitektur & Diagram Sistem](#-arsitektur--diagram-sistem)
- [Skema Pinout & Spesifikasi Perangkat Keras](#-skema-pinout--spesifikasi-perangkat-keras)
- [Struktur Repositori](#-struktur-repositori)
- [Format & Struktur Data MQTT](#-format--struktur-data-mqtt)
- [Panduan Instalasi & Pengunggahan](#-panduan-instalasi--pengunggahan)
- [Kalibrasi Sensor](#-kalibrasi-sensor)
- [Pengujian Latensi & Performa](#-pengujian-latensi--performa)
- [Sitasi & Konteks Akademis](#-sitasi--konteks-akademis)

---

## 🚀 Fitur Utama

- **Akuisisi Multi-Sensor Sinkron:** Mengambil data pembacaan dari 5 *flex sensor* (kemiringan jari) dan IMU MPU6050 (3-axis *Accelerometer* & 3-axis *Gyroscope*).
- **Filtering & Normalisasi Data:** Menerapkan teknik *moving average filter* pada ESP32 untuk meredam *noise* sinyal analog.
- **Komunikasi Berlatensi Rendah:** Menggunakan protokol ringan **MQTT over TCP/IP** via Wi-Fi dengan interval *sampling rate* yang dapat dikonfigurasi (30–50 Hz).
- **Auto-reconnect Mechanism:** Penanganan otomatis koneksi terputus (*reconnection loop*) baik pada jaringan Wi-Fi maupun MQTT Broker.
- **Manajemen Daya Efisien:** Mengoptimalkan konsumsi daya transmisi ESP32 tanpa mengorbankan stabilitas pengiriman paket.

---

## 🏗️ Arsitektur & Diagram Sistem

Berikut adalah aliran data dan arsitektur hulu-ke-hilir sistem penerjemah BISINDO:

```mermaid
graph TD
    subgraph Hardware Layer [Layer 1: Smart Glove Hardware]
        A1[Flex Sensors x5] -->|Analog Read ADC1| ESP[ESP32 Microcontroller]
        A2[MPU6050 IMU] -->|I2C Protocol - SDA/SCL| ESP
    end

    subgraph Communication Layer [Layer 2: Transmisi Data]
        ESP -->|Prapemrosesan Sinyal & JSON Serialization| MQTT_PUB[MQTT Publisher]
        MQTT_PUB -->|Wi-Fi / TCP IP - Port 1883| BROKER[MQTT Broker / Eclipse Mosquitto]
    end

    subgraph Processing & UI Layer [Layer 3: Processing & Interface]
        BROKER -->|MQTT Subscriber| APP[Desktop Application GUI]
        APP -->|Windowing Time-Series Data| AI[1D CNN-LSTM Deep Learning Model]
        AI -->|Prediksi Kelas Gestur| DISPLAY[Tampilan Teks BISINDO & Log Latensi]
    end

    style ESP fill:#007acc,stroke:#fff,stroke-width:2px,color:#fff
    style BROKER fill:#43a047,stroke:#fff,stroke-width:2px,color:#fff
    style AI fill:#e53935,stroke:#fff,stroke-width:2px,color:#fff
