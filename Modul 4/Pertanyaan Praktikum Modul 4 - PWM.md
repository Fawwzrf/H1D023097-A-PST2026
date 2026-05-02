# 📘 Praktikum Sistem Tertanam - Modul 4 PWM (Pulse Width Modulation)

## Pertanyaan Praktikum

1. Jelaskan mengapa LED dapat diatur kecerahannya menggunakan fungsi analogWrite()!
2. Apa hubungan antara nilai ADC (0–1023) dan nilai PWM (0–255)?
3. Modifikasilah program berikut agar LED hanya menyala pada rentang kecerahan sedang, yaitu hanya ketika nilai PWM berada pada rentang 50 sampai 200. Jelaskan program pada file README.md.

---

## ✅ Jawaban

### 1. Mekanisme Pengaturan Kecerahan LED via analogWrite()
LED dapat diatur kecerahannya menggunakan `analogWrite()` karena fungsi ini menerapkan teknik **Pulse Width Modulation (PWM)**. Secara fisik, pin digital tidak bisa mengeluarkan tegangan analog murni, namun dengan PWM, Arduino mensimulasikan tegangan antara 0V dan 5V dengan cara menyalakan dan mematikan sinyal digital secara sangat cepat (frekuensi tinggi). Dengan mengubah *Duty Cycle* (durasi waktu 'ON' terhadap total periode), kita dapat memanipulasi **Tegangan Rata-rata** yang diterima LED. Karena frekuensi switching ini melampaui batas ambang persepsi mata manusia (*Persistence of Vision*), kita tidak melihat LED berkedip, melainkan merasakannya sebagai perubahan intensitas cahaya atau tingkat kecerahan (*dimming*).

### 2. Hubungan Korelasi Nilai ADC dan Nilai PWM
Hubungan antara keduanya adalah perbandingan proporsionalitas linear berdasarkan resolusi bit perangkat. Pembacaan **ADC** menggunakan resolusi **10-bit** (memiliki $2^{10} = 1024$ tingkat, yaitu 0–1023), sedangkan output **PWM** pada Arduino standar menggunakan resolusi **8-bit** (memiliki $2^8 = 256$ tingkat, yaitu 0–255). Secara matematis, rasio perbandingannya adalah **4:1**. Artinya, setiap kenaikan 4 satuan pada nilai input ADC akan dikonversi menjadi kenaikan 1 satuan pada nilai output PWM untuk menjaga keseimbangan rasio tegangan dari input ke output.

### 3. Modifikasi Program (Rentang PWM 50 - 200)

### 📌 Source Code
```cpp
#include <Arduino.h> // Library dasar inti untuk fungsionalitas Arduino

// ===================== PIN SETUP =====================
const int potPin = A0;   // Mendefinisikan pin A0 sebagai input untuk membaca nilai dari potensiometer
const int ledPin = 9;    // Mendefinisikan pin 9 (mendukung PWM) sebagai output untuk menggerakkan LED

// ===================== VARIABEL =====================
int nilaiADC = 0;  // Variabel untuk menampung data digital hasil pembacaan sensor analog
int pwm = 0;       // Variabel untuk menyimpan hasil konversi skala ke nilai PWM

void setup() {
  pinMode(ledPin, OUTPUT); // Mengonfigurasi pin LED sebagai output elektrikal
  Serial.begin(9600);      // Menginisialisasi komunikasi serial untuk memantau data pada Serial Monitor
}

void loop() {
  // ===================== PEMBACAAN SENSOR =====================
  nilaiADC = analogRead(potPin); // Membaca level tegangan potensiometer (0-1023)

  // ===================== PEMROSESAN DATA (SCALING) =====================
  // Mengubah rentang input ADC 10-bit menjadi output PWM 8-bit secara linier
  pwm = map(nilaiADC, 0, 1023, 0, 255); 

  // ===================== OUTPUT PWM DENGAN KONDISI (MODIFIKASI) =====================
  // Implementasi logika gate: LED hanya menyala jika intensitas berada pada rentang sedang (50-200)
  if (pwm >= 50 && pwm <= 200) {
    analogWrite(ledPin, pwm); // Menyalakan LED dengan intensitas sesuai nilai PWM aktif
  } else {
    analogWrite(ledPin, 0);   // Mematikan LED (0) jika di luar rentang 50-200 (terlalu redup atau terlalu terang)
  }

  // ===================== MONITORING DATA =====================
  Serial.print("ADC: ");
  Serial.print(nilaiADC);    // Mencetak nilai input sensor ke terminal
  Serial.print(" | PWM: ");
  Serial.println(pwm);       // Mencetak nilai kalkulasi PWM ke terminal

  // ===================== STABILISASI SISTEM =====================
  delay(50); // Memberikan jeda sampling 50ms untuk stabilitas pembacaan dan visualisasi data
}
```
