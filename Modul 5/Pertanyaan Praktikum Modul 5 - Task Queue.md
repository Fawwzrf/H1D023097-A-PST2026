# 📘 Praktikum Sistem Tertanam - Modul 5 Komunikasi Task (Queue)

## Pertanyaan Praktikum

1. Apakah kedua task berjalan secara bersamaan atau bergantian? Jelaskan mekanismenya!
2. Apakah program ini berpotensi mengalami race condition? Jelaskan!
3. Modifikasilah program dengan menggunakan sensor DHT sesungguhnya sehingga informasi yang ditampilkan dinamis. Bagaimana hasilnya? Jelaskan program pada file README.md.

---

## ✅ Jawaban

### 1. Mekanisme Eksekusi dengan Antrean (Queue)
Kedua task (`read_data` dan `display`) berjalan secara **bergantian** yang dikelola oleh scheduler FreeRTOS. Mekanisme komunikasi antara kedua task dilakukan melalui fitur **Queue (Antrean)**.
Fungsi `read_data` akan mengirimkan data statis ke dalam antrean `my_queue` menggunakan `xQueueSend`. Saat data masuk, fungsi `display` yang awalnya berada dalam status menunggu (`blocked state` pada `xQueueReceive`) akan langsung diaktifkan oleh scheduler untuk mengambil dan membaca data tersebut. Hal ini menciptakan komunikasi satu arah (Producer-Consumer) yang tersinkronisasi.

### 2. Potensi Race Condition
Program ini **tidak berpotensi mengalami race condition** pada data yang dilewatkan melalui Queue. FreeRTOS menjamin bahwa operasi pengiriman (`xQueueSend`) dan penerimaan (`xQueueReceive`) bersifat **thread-safe** dan dieksekusi secara atomik. RTOS akan menyalin data secara penuh (Deep Copy) ke dalam buffer antrean dengan aman, sehingga data tidak akan terkorupsi oleh interupsi atau task lain yang mencoba mengakses memori di saat bersamaan. Namun, *race condition* bisa terjadi jika resource luar (seperti `Serial.print`) digunakan berbarengan oleh berbagai task tanpa perlindungan khusus.

### 3. Modifikasi Program (Sensor DHT)
Modifikasi dilakukan dengan mengganti data statis menjadi data pembacaan langsung dari sensor DHT11 menggunakan library `<DHT.h>`. Pada fungsi `read_data`, suhu dan kelembapan dibaca lalu dikirim ke dalam queue menggunakan `xQueueSend`.
**Hasilnya**: Serial Monitor sekarang dapat menampilkan data suhu dan kelembapan ruangan secara real-time dan dinamis. Task pembacaan data (sensing) terpisah dengan baik dari task penampil data (display), sehingga program berjalan secara lebih efisien dan modular.

### 📌 Source Code Modifikasi
```cpp
#include <Arduino_FreeRTOS.h> // Library RTOS
#include <queue.h>            // Library untuk fitur Queue
#include <DHT.h>              // Library sensor DHT

#define DHTPIN 2       // Pin data DHT terhubung ke pin 2 digital
#define DHTTYPE DHT11  // Menggunakan sensor DHT11

DHT dht(DHTPIN, DHTTYPE); // Inisialisasi sensor DHT

// Struktur data untuk mengirim suhu dan kelembapan sekaligus
struct readings {
  float temp;
  float h;
};

QueueHandle_t my_queue; // Deklarasi variabel queue

void read_data(void *pvParameters);
void display(void *pvParameters);

void setup() {
  Serial.begin(9600); // Inisialisasi Serial Monitor
  dht.begin();        // Mengaktifkan sensor DHT

  // Membuat antrean tunggal yang dapat menampung 1 struktur data readings
  my_queue = xQueueCreate(1, sizeof(struct readings));

  if (my_queue != NULL) { 
    // Mendaftarkan task pembaca data dan penampil data
    xTaskCreate(read_data, "read sensors", 128, NULL, 1, NULL);
    xTaskCreate(display, "display", 128, NULL, 1, NULL);
    vTaskStartScheduler(); // Memulai scheduler
  }
}

void loop() {
  // Kosong, scheduler yang mengatur eksekusi
}

void read_data(void *pvParameters) {
  struct readings x;
  for (;;) {
    // Membaca data kelembapan dan suhu
    x.h = dht.readHumidity();
    x.temp = dht.readTemperature();

    // Memastikan data valid (bukan NaN) sebelum dikirim
    if (!isnan(x.h) && !isnan(x.temp)) {
      xQueueSend(my_queue, &x, portMAX_DELAY); // Kirim data ke queue
    }
    
    // Memberikan jeda minimal 2 detik sesuai standar sensor DHT11
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}

void display(void *pvParameters) {
  struct readings x;
  for (;;) {
    // Menunggu (blocking) hingga ada data baru di dalam queue
    if (xQueueReceive(my_queue, &x, portMAX_DELAY) == pdPASS) {
      // Menampilkan output data ke Serial Monitor
      Serial.print("🌡️ Suhu Udara: ");
      Serial.print(x.temp);
      Serial.print(" °C  |  💧 Kelembapan: ");
      Serial.print(x.h);
      Serial.println(" %");
    }
  }
}
```
