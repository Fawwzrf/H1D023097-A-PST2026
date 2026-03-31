# 📘 Praktikum Sistem Tertanam - Modul 1 Percabangan

## Pertanyaan Praktikum

1. Pada kondisi apa program masuk ke blok `if`?  
2. Pada kondisi apa program masuk ke blok `else`?  
3. Apa fungsi dari perintah `delay(timeDelay)`?  
4. Jika program memiliki alur **mati → lambat → cepat → reset (mati)**, ubah menjadi:
   **lambat → cepat → sedang → mati (tanpa reset langsung)** dan jelaskan setiap baris kode.

---

## ✅ Jawaban

### 1. Kondisi Masuk ke Blok `if`

Program akan masuk ke blok `if` ketika kondisi berikut terpenuhi:

```cpp
if (timeDelay <= 100)
```

**Penjelasan:**
Program akan masuk ke kondisi `if` ketika nilai `timeDelay` kurang dari atau sama dengan 100 ms, yang berarti LED sudah berada pada kecepatan kedip yang sangat cepat dan akan masuk ke kondisi reset(mengatur ulang nilai timeDelay menjadi 1000 ms).

---

### 2. Kondisi Masuk ke Blok `else`

Program akan masuk ke blok `else` ketika kondisi `if` **tidak terpenuhi**, yaitu:

```cpp
timeDelay > 100
```

Di dalam blok `else` terdapat perintah:

```cpp
timeDelay -= 100;
```

**Penjelasan:**
Ketika nilai `timeDelay` lebih besar dari 100 ms(belum memenuhi kondisi `if`), maka program akan masuk ke kondisi `else` dan mengurangi nilai `timeDelay` sebesar 100 ms setiap siklus, sehingga LED berkedip semakin cepat secara bertahap.

---

### 3. Fungsi `delay(timeDelay)`

Fungsi:

```cpp
delay(timeDelay);
```

**Penejelasan:**
Fungsi dari `delay(timeDelay)` adalah untuk memberikan jeda waktu dalam satuan milidetik (ms) pada program. 
---

## 4. Modifikasi Program  
### Alur Baru: **Lambat → Cepat → Sedang → Mati (Tanpa Reset)**

### 📌 Source Code

```cpp
const int ledPin = 13;     // Menentukan pin LED pada pin digital 13
int timeDelay = 1000;     // Delay awal (LED berkedip lambat)

void setup() { 
  pinMode(ledPin, OUTPUT);   // Mengatur pin LED sebagai output
}

void loop() { 
  // Menyalakan LED
  digitalWrite(ledPin, HIGH);  
  delay(timeDelay);           // Menunggu sesuai nilai delay (LED ON)

  // Mematikan LED
  digitalWrite(ledPin, LOW);  
  delay(timeDelay);           // Menunggu sesuai nilai delay (LED OFF)

  // Percabangan untuk mengatur perubahan kecepatan LED
  if (timeDelay <= 200) {     
    // Jika delay sudah sangat kecil (LED sangat cepat)
    timeDelay = 500;          
    // Ubah menjadi kecepatan sedang

  } else if (timeDelay <= 500) {  
    // Jika delay berada pada kecepatan sedang
    timeDelay = 0;            
    // Ubah menjadi mati (tidak berkedip)

  } else {  
    // Jika LED masih dalam kondisi lambat
    timeDelay -= 200;         
    // Kurangi delay agar LED semakin cepat
  }

  // Kondisi untuk menghentikan program
  if (timeDelay == 0) {
    digitalWrite(ledPin, LOW); // Pastikan LED benar-benar mati
    while(true);               // Menghentikan program secara permanen
  }
}
```

---

### Penjelasan Alur Program

1. **Kondisi Awal**
   - `timeDelay = 1000 ms`
   - LED berkedip lambat  

2. **Percepatan Bertahap**
   - Program masuk ke blok `else`
   - `timeDelay` dikurangi (`-200`)
   - LED menjadi semakin cepat  

3. **Kondisi Cepat**
   - Saat `timeDelay ≤ 200`
   - Program masuk ke blok `if`
   - Delay diubah menjadi `500 ms` (kecepatan sedang)  

4. **Kondisi Sedang ke Mati**
   - Saat `timeDelay ≤ 500`
   - Delay diubah menjadi `0`  
   - LED berhenti berkedip  

5. **Program Berhenti**
   - LED dimatikan  
   - `while(true)` menjaga program tetap berhenti (tidak reset ulang)  

---

## Kesimpulan

- Percabangan `if-else` digunakan untuk mengatur perubahan kecepatan LED  
- Nilai `timeDelay` berperan sebagai pengontrol utama kecepatan  
- Modifikasi program menghasilkan alur yang lebih halus:

  **Lambat → Cepat → Sedang → Mati**

- Program tidak kembali ke awal (tidak reset), melainkan berhenti permanen  