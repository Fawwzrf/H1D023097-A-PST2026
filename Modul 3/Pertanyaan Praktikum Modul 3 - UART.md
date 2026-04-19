# 📘 Praktikum Sistem Tertanam - Modul 3 Komunikasi Serial UART

## Pertanyaan Praktikum 3.5.4

1. Jelaskan proses dari input keyboard hingga LED menyala/mati!
2. Mengapa digunakan `Serial.available()` sebelum membaca data? Apa yang terjadi jika baris tersebut dihilangkan?
3. Modifikasi program agar LED berkedip (blink) ketika menerima input '2' dengan kondisi jika '2' aktif maka LED akan terus berkedip sampai perintah selanjutnya diberikan dan berikan penjelasan disetiap baris kode nya dalam bentuk README.md!
4. Tentukan apakah menggunakan `delay()` atau `millis()`! Jelaskan pengaruhnya terhadap sistem.

---

## ✅ Jawaban

### 1. Proses dari input keyboard hingga LED menyala/mati
1. **Input user:** Pengguna mengetik karakter (misal '1') di komputer/laptop melalui Serial Monitor IDE, lalu menekan ENTER (mengirimkan data string secara serial menggunakan komunikasi USB).
2. **Transmisi UART:** Chip antarmuka (seperti CH340 atau ATmega16U2) pada antarmuka input papan Arduino secara langsung mengubah sinyal USB dari komputer tsb menjadi data serial TTL (UART) yang diteruskan kepada pin `RX` (Receive) pada mikrokontroler.
3. **Pembacaan Data:** Mikrokontroler inti (ATmega328P) menyimpan data yang masuk seketika di *buffer* memori serial. Kondisional fungsi `Serial.available()` akan mendeteksi keberadaan muatan data aktual (jumlah *byte*), lalu ketika *aware* diaktifkan, fungsi sintaks `Serial.read()` dipanggil untuk mendistribusikan / memindahtempatkan nilai byte tadi dari array buffer ke sebuah variabel komputasi logik (misalnya var `data`).
4. **Eksekusi Kondisional:** Struktur kendali program (`if`) membandingkan variabel karakter yang diterima. Jika isinya cocok menjadi `'1'`, program memanggil fungsi instruksi `digitalWrite(PIN_LED, HIGH)` untuk menyalakan LED. Jika cocok `'0'`, output direndahkan menjadi `LOW` (mematikan LED). Pada akhirnya, program juga menggemakan output text balasan `Serial.println("LED ...")` dengan menginstruksinya melewati pin *TX* komputer.

### 2. Penggunaan `Serial.available()` sebelum membaca data
`Serial.available()` diutilisasi di awal percabangan untuk memeriksa secara pasti apakah ada jejak kepingan data/karakter aktual yang terparkir antri dan siap dibaca di dalam register/buffer *receiver* hardware serial. 
**Kasus jika dihilangkan:** Jika program bertendensi untuk langsung membobrok alias memanggil pasrah `Serial.read()` secara berulang *non-stop* pada loop tanpa menjamin status ketersediaan *buffer* aktual, fungsi tersebut akan merespons me-return *blank value* yakni nilai `-1` (atau `255`/0xFF merepresentasikan non-kehadiran). Dikarenakan nilai bit `255` tersebut tidak berkorelasi dengan indeks ASCII `'1'`, `'0'`, maupun spasi karakter, program akan memicu terus-menerus klausa default akhir `else` sehingga mencetak string error *"Perintah tidak dikenal"* bertubi-tubi tanpa jeda (spam). Implikasinya: ia bisa menyita saturasi trafik I/O, mengebom memori log monitor PC sampai *crash*, serta mengheksos siklus *Thread* kinerja prosesor dari yang seharusnya hanya mematung diam dan menunggu.

### 3. Modifikasi program agar LED berkedip (blink) ketika input '2'
Berikut implementasinya yang dibangun mengusung format sistem *State Machine* sehingga program dapat merekam perintah terakhir untuk di-looping dan terhindar dari cacat algoritma *blocking*:

### 📌 Source Code
```cpp
const int PIN_LED = 13;
char currentCommand = '0';          // Variabel state untuk menyimpan status perintah aktif terakhir
unsigned long previousMillis = 0;   // Variabel untuk menyimpan jejak waktu (timestamp) operasi kedip terakhir
const long interval = 500;          // Variabel konstan untuk menentukan delay/jeda rentabilitas blinking rate berdurasi milidetik
bool ledState = LOW;                // Variabel yang merepresentasikan status nyala boolean komponen lampu LED

void setup() {
  Serial.begin(9600); // Menginisialisasi komunikasi antarmuka serial dengan rasio baud 9600 bit per sekon
  Serial.println("Ketik '1': ON, '0': OFF, '2': BLINK"); // Menurunkan output intsruksi UI antarmuka ke layar monitor PC
  pinMode(PIN_LED, OUTPUT); // Mengkonfigurasi parameter mode kaki pin LED (13) mendefinisikan bertindak memancarkan sinyal Output
}

void loop() {
  // Mengevaluasi dan menghitung jumlah bit baru yang tertampung/available secara teratur (pooling) di bufer UART
  if (Serial.available() > 0) {
    char data = Serial.read(); // Mengekstraksi memori sisa byte yang tertampung di depan antrian untuk disimpan ke variabel program
    
    // Melompati dan mem-bypass perlakuan terhadap sisa kontrol newline & carriage return sisipan auto-enter
    if (data == '\n' || data == '\r') {
      return; 
    }
    
    // Verifikasi validasi menu perintah / menimpakan update jika instruksi sesuai kesepakatan spesifikasi index
    if (data == '1' || data == '0' || data == '2') {
      currentCommand = data; // Persitensi menetapkan input terakhir bertengger pada variabel memory state `currentCommand`
      Serial.print("Perintah aktif berubah mode menjadi: "); // Mencetak narasi label string update di Console Serial
      Serial.println(currentCommand); // Men-spill isi variabel konfirmasi command state
    } else {
      Serial.println("Perintah tidak dikenal"); // Mengeluarkan trigger notifikasi error ketika karakter input keliru
    }
  }

  // --- Blok Eksekusi Engine Utama (Melakukan iterasi tanpa henti menyesuaikan Command State terakhir) ---
  
  if (currentCommand == '1') {
    digitalWrite(PIN_LED, HIGH); // Mengeluarkan logika signal positif 5V menyalakan pijaran LED yang stabil/ajeg
  } 
  else if (currentCommand == '0') {
    digitalWrite(PIN_LED, LOW);  // Me-nol-kan pengeluaran sinkronisasi tegangan 0V, meredam LED dalam mode istirahat (mati konstan)
  } 
  else if (currentCommand == '2') {
    // Mengeksekusi manuver manipulasi kedip asinkron (non-block) memanfaatkan kalkulasi waktu riwayat komputasi dari start boot
    unsigned long currentMillis = millis(); // Menangkap rekaman nilai stopwatch presisi dari *uptime* mikrokontroler MCU yang berjalan saat iterasi tsb
    
    // Jika margin perselisihan dari real-time dengan timer lama telah tercapai mempampouhi selisih threshold sasaran (500) ...
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis; // Reset/sinkronisasikan limitator base timer dengan timeline referensi *timestamp* baru
      ledState = !ledState;           // Toggle interupsi: membalik logika sasaran target variable boolean (true menjadi false, false menjadi true)
      digitalWrite(PIN_LED, ledState);// Mengubah perintah pergerakan aktuator hardware pada pin mendedikasikan sesuai refleksi memori manipulasi LED tadi (berkedip)
    }
  }
}
```

### 4. Tentukan apakah menggunakan `delay()` atau `millis()`! Jelaskan pengaruhnya terhadap sistem
Program di atas **HARUS MUTLAK** diusung dengan mereprentasikan implementasi metode **`millis()`** ketimbang manipulasi `delay()`.
Eksistensi pemanggilan operasi `delay(500)` memiliki karakteristik fatal yang bersifat *blocking / Halt*. Berarti dalam praktiknya, apabila kita berkeras menjadwalkan timer blinking via `delay()`, seluruh alur run-time mikrokontroler dan pencacah intruksi *program counter* akan di-*pause* kaku untuk tidak melangkah ke komputasi sintaks lain apa pun selama setengah detik penuh. Jika persis dalam jeda tidur sesaat itu ada perintah baru mengalir via rute Serial (contoh pengguna berubah pikiran pindah dari ke opsi '0'), maka perintah tak di-*acknowledge* (*lag*) hingga *delay* menuntaskan sisa jam tidurnya. Implikasinya: navigasi instrumen terkesan lemot dan lamban menanti respon.

Sebaliknya, *Framework State Engine* fleksibel berbasis arsitektur **`millis()`** memangkas secara mulus kelemahan ini via algoritma *non-blocking*. Di sini, core siklus *loop* akan didelegasi berlarian merespon kecekatan super tinggi dalam terus menyeleksi sinyal input komando yang menabrak asinkron dengan antrian (`listening process`), melintas berulang ratusan kali per detik sembari di saat yang sama secara teliti mengobservasi selisih delta interval jam kalkulasi matetmatika. Pergantian arus pin LED diekskusi layaknya pemicu interupsi ketika delta waktu sudah pas jatuh tempo membelah interval (tanpa menghentikan keluwesan *listening process*).
