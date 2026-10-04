// ==========================================
// KONFIGURASI PIN SENSOR FLEX
// ==========================================
// Sesuaikan dengan urutan pin sarung tangan lo
const int PIN_FLEX[] = {32, 35, 34, 39, 36}; 
const int JUMLAH_JARI = 5;
const String NAMA_JARI[] = {"IBU", "TLJ", "TGH", "MNS", "KLK"};

void setup() {
  Serial.begin(115200);
  
  // Setup Pin Flex Sensor
  for (int i = 0; i < JUMLAH_JARI; i++) {
    pinMode(PIN_FLEX[i], INPUT);
  }

  Serial.println("\n=== MEMULAI BACA RAW DATA: 5 FLEX SENSOR SAJA ===");
  delay(2000);
}

void loop() {
  Serial.print("DATA FLEX ->  ");
  
  for (int i = 0; i < JUMLAH_JARI; i++) {
    int nilaiMentah = analogRead(PIN_FLEX[i]);
    
    Serial.print(NAMA_JARI[i]);
    Serial.print(": ");
    Serial.print(nilaiMentah);
    
    if (i < JUMLAH_JARI - 1) {
      Serial.print("   |   "); 
    }
  }

  Serial.println(); // Pindah baris baru
  
  // Delay 300ms agar mudah dibaca mata untuk proses pencatatan
  delay(50); 
}