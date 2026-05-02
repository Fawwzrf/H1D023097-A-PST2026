# 📘 Praktikum Sistem Tertanam - Modul 4 ADC (Analog to Digital Converter)

## Pertanyaan Praktikum

1. Apa fungsi perintah analogRead() pada rangkaian praktikum ini?
2. Mengapa diperlukan fungsi map() dalam program tersebut?
3. Modifikasi program berikut agar servo hanya bergerak dalam rentang 30° hingga 150°, meskipun potensiometer tetap memiliki rentang ADC 0–1023. Jelaskan program pada file README.md

---

## ✅ Jawaban

### 1. Fungsi perintah analogRead()
Fungsi `analogRead()` berperan sebagai jembatan konversi *Analog-to-Digital* (ADC) yang bertugas untuk menyampling sinyal tegangan kontinu (analog) yang masuk melalui pin input (dalam hal ini dari *wiper* potensiometer pada pin A0). Secara teknis, perintah ini membaca level tegangan antara 0V hingga 5V dan mentransformasikannya menjadi nilai digital diskrit 10-bit, yang menghasilkan representasi angka bulat antara **0 (untuk 0V)** hingga **1023 (untuk 5V)** agar dapat diproses secara matematis oleh logika program mikrokontroler.

### 2. Urgensi penggunaan fungsi map()
Fungsi `map()` diperlukan untuk melakukan prosedur **Normalisasi** atau penskalaan linier antara dua domain data yang memiliki rentang resolusi berbeda. Tanpa fungsi ini, nilai ADC (0–1023) tidak dapat langsung digunakan untuk mengontrol sudut servo yang hanya memiliki batas operasional 0 hingga 180 derajat. Dengan `map()`, sistem secara otomatis menghitung rasio proporsional sehingga setiap perubahan posisi mekanis pada potensiometer akan menghasilkan pergerakan sudut servo yang akurat dan sinkron sesuai batas yang diinginkan.

### 3. Modifikasi Program (Rentang 30° - 150°)

### 📌 Source Code
```cpp
#include <Servo.h> // Mengimpor library Servo untuk mengontrol modul motor servo melalui sinyal PWM standar

Servo myservo; // Instansiasi objek 'myservo' dari class Servo untuk mengelola abstraksi kontrol perangkat

// ===================== PIN SETUP =====================
const int potensioPin = A0;   // Menetapkan pin A0 sebagai jalur input analog untuk membaca tegangan dari potensiometer
const int servoPin = 9;      // Menetapkan pin digital 9 sebagai jalur output PWM untuk mengirim sinyal posisi ke servo

// ===================== VARIABEL =====================
int pos = 0; // Variabel penampung hasil pemetaan (mapping) untuk menyimpan nilai sudut akhir servo
int val = 0; // Variabel penampung nilai mentah (raw data) hasil pembacaan ADC dari potensiometer

void setup() {
  myservo.attach(servoPin); // Menginisialisasi dan menghubungkan objek servo ke pin fisik 9 pada board Arduino
  Serial.begin(9600);       // Membuka jalur komunikasi serial dengan kecepatan baud rate 9600 bit per detik untuk keperluan monitoring
}

void loop() {
  // ===================== PEMBACAAN ADC =====================
  val = analogRead(potensioPin); // Mengambil sampel data analog dari pin A0 dan menyimpannya dalam variabel 'val' (range 0-1023)

  // ===================== KONVERSI DATA (MODIFIKASI) =====================
  // Melakukan pemetaan nilai ADC (0-1023) ke rentang sudut terbatas (30-150 derajat)
  pos = map(val, 0, 1023, 30, 150); 

  // ===================== OUTPUT SERVO =====================
  myservo.write(pos); // Mengirimkan instruksi sinyal posisi ke motor servo berdasarkan hasil kalkulasi mapping 'pos'

  // ===================== MONITORING DATA =====================
  Serial.print("ADC Potensio: ");
  Serial.print(val);           // Menampilkan nilai ADC mentah ke Serial Monitor
  Serial.print(" | Sudut Servo: ");
  Serial.println(pos);         // Menampilkan nilai sudut yang telah dipetakan ke Serial Monitor

  // ===================== STABILISASI =====================
  delay(100); // Memberikan jeda waktu 100ms agar pergerakan mekanis servo stabil dan tidak membebani pemrosesan
}
```
