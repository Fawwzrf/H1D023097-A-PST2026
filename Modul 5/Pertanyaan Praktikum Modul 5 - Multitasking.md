# 📘 Praktikum Sistem Tertanam - Modul 5 Multitasking

## Pertanyaan Praktikum

1. Apakah ketiga task berjalan secara bersamaan atau bergantian? Jelaskan mekanismenya!
2. Bagaimana cara menambahkan task keempat? Jelaskan langkahnya!
3. Modifikasilah program dengan menambah sensor (misalnya potensiometer), lalu gunakan nilainya untuk mengontrol kecepatan LED! Bagaimana hasilnya? Jelaskan program pada file README.md.

---

## ✅ Jawaban

### 1. Eksekusi Task (Bersamaan vs Bergantian)
Ketiga task (`TaskBlink1`, `TaskBlink2`, dan `Taskprint`) berjalan secara **bergantian**. Mikrokontroler Arduino Uno hanya memiliki satu core (single-core), sehingga CPU hanya dapat memproses satu instruksi pada satu waktu. Mekanisme ini diatur oleh **RTOS Scheduler** yang membagi waktu CPU untuk masing-masing task. Saat sebuah task berada pada fase delay (`vTaskDelay`), scheduler akan memindahkan kontrol CPU ke task lain yang sudah siap dieksekusi, sehingga memberikan efek seolah-olah semua task berjalan secara bersamaan.

### 2. Cara Menambahkan Task Keempat
Langkah-langkah untuk menambahkan task keempat adalah:
1. **Deklarasi fungsi**: Buat prototipe fungsi untuk task baru, misalnya `void TaskBlink3(void *pvParameters);`.
2. **Implementasi task**: Buat fungsi utama task yang berisi *infinite loop* (`while(1)` atau `for(;;)`). Pastikan untuk menambahkan `vTaskDelay()` di dalam loop agar tidak memonopoli CPU.
3. **Mendaftarkan task**: Daftarkan task baru pada `setup()` menggunakan fungsi `xTaskCreate(TaskBlink3, "task4", 128, NULL, 1, NULL);`.

### 3. Modifikasi Program (Potensiometer)
Modifikasi dilakukan dengan menambahkan pembacaan nilai analog dari potensiometer (`analogRead()`) pada `TaskBlink1`. Nilai ADC (0-1023) kemudian dipetakan menjadi nilai delay (50-1000 ms) menggunakan fungsi `map()`.
**Hasilnya**: Kecepatan kedipan LED dapat diubah secara langsung menggunakan potensiometer. Perubahan kecepatan pada task ini terjadi secara independen dan **tidak mengganggu kecepatan eksekusi** dari task lain karena RTOS menangani delay secara asinkron.

### 📌 Source Code Modifikasi
```cpp
#include <Arduino_FreeRTOS.h> // Library RTOS untuk Arduino

void TaskBlink1( void *pvParameters );
void TaskBlink2( void *pvParameters );
void Taskprint( void *pvParameters );

void setup() {
  Serial.begin(9600); // Inisialisasi Serial Monitor

  // Membuat dan mendaftarkan ketiga task ke scheduler
  xTaskCreate(TaskBlink1, "task1", 128, NULL, 1, NULL);
  xTaskCreate(TaskBlink2, "task2", 128, NULL, 1, NULL);
  xTaskCreate(Taskprint, "task3", 128, NULL, 1, NULL);
  
  vTaskStartScheduler(); // Memulai scheduler RTOS
}

void loop() {
  // Kosong, karena loop utama dikontrol oleh RTOS Scheduler
}

void TaskBlink1(void *pvParameters) {
  pinMode(8, OUTPUT);         // Pin LED kuning
  const int potensioPin = A0; // Pin potensiometer
  
  while(1) {
    int val = analogRead(potensioPin); // Membaca nilai potensiometer
    int delayTime = map(val, 0, 1023, 50, 1000); // Konversi ke nilai delay
    
    Serial.print("Task1 | Kecepatan LED: ");
    Serial.print(delayTime);
    Serial.println(" ms");

    digitalWrite(8, HIGH); // Nyalakan LED
    vTaskDelay( delayTime / portTICK_PERIOD_MS ); // Delay sesuai nilai potensiometer
    
    digitalWrite(8, LOW);  // Matikan LED
    vTaskDelay( delayTime / portTICK_PERIOD_MS ); // Delay sesuai nilai potensiometer
  }
}

void TaskBlink2(void *pvParameters) {
  pinMode(7, OUTPUT); // Pin LED merah
  while(1) {
    digitalWrite(7, HIGH);
    vTaskDelay( 300 / portTICK_PERIOD_MS ); // Delay statis 300ms
    digitalWrite(7, LOW);
    vTaskDelay( 300 / portTICK_PERIOD_MS ); // Delay statis 300ms
  }
}

void Taskprint(void *pvParameters) {
  int counter = 0;
  while(1) {
    counter++; 
    vTaskDelay(500 / portTICK_PERIOD_MS); // Delay statis 500ms
  }
}
```
