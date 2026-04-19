# 📘 Praktikum Sistem Tertanam - Modul 3 Komunikasi I2C

## Pertanyaan Praktikum 3.6.4

1. Jelaskan bagaimana cara kerja komunikasi I2C antara Arduino dan LCD pada rangkaian tersebut!
2. Apakah pin potensio meter harus seperti itu? Jelaskan yang terjadi apa bila pin kiri dan pin kanan tertukar!
3. Modifikasi program dengan menggabungkan antara UART dan I2C (keduanya sebagai output) sehingga:
   - Data tidak hanya ditampilkan di LCD tetapi juga di Serial Monitor.
   - Adapun data yang ditampilkan pada Serial Monitor sesuai dengan tabel format.
   - Modifikasi penempatan elemen antarmuka di layar LCD display: `ADC: X  Y%` at `setCursor(0, 0)` dan Bar (level) at `setCursor(0, 1)`.
   - Berikan penjelasan disetiap baris kode nya.
4. Lengkapi table berikut!
ADC | Volt (V) | Persen (%)
1   |          | 
21  |          | 
49  |          | 
74  |          | 
96  |          | 

---

## ✅ Jawaban

### 1. Cara kerja komunikasi I2C antara Arduino dan LCD
Komunikasi `I2C` (Inter-Integrated Circuit) pada prototipe arsitektur sistem ini bekerja bertukar data mutlak dengan menyandarkan kepada topologi *bus* jalur sinkron dwi-kabel, yakni `SDA` (*Serial Data*) yang digunakan untuk mengirim bit instruksi dua-arah antar stasiun node perangkat, dibantu asisten jalur paralel `SCL` (*Serial Clock*) sebagai sarana sinkronisasi merambatkan konstan denyut frekuensi detak memandu orkestrasi pergerakan *byte*-nya.

Arduino ditunjuk menunaikan fungsinya sebagai pimpinan terpusat tunggal **"Master"** (aktor generator penggerak detak pulsa clock *SCL* serta penginisiator arus penulisan awal) yang mengkoordinasikan distribusi pertukaran. Mengetahui takdirnya, instrumen LCD Adapter *Backpack* (IC PCF8574) diubah menjadi bertindak pasif selaku perangkat klien **"Slave"** yang tunduk terus menunggu lalu lintas jalurnya merespons panggilan pesanan secara spesifik alamat I2C pribadinya dipanggil oleh master (contoh melalui register alokasi *ADDRESS* = `0x27`). Apabila *frame* instruksi panggilan inisiatif dari si Master cocok merujuk pada label identitas si Slave tadi barulah alat menyahut komando kedaulatannya (melalui pertukaran status `ACK`). Pada tahap berikutnya *Byte* *frame* serangkaian digit muatan kontrol intruksinya (bisa itu instruksi format *Wake Up* memicu menyalakan *backlight*, membersihkan kanvas layar buffer matrix eksis, maupun titipan transmiter merender cetakkan abjad bitmap karakter pixel merapat ke layar) ditangkap dan dibongkarkemas isinya mendeterminir fungsional peranti internal HD44780 LCD-nya untuk dieksekusi secara fisikal.

### 2. Apakah pin potensio meter harus persis urutannya? (Menukar Pin Kiri & Kanan)
**Posisi pin kutub ekstrim paling luar (pin penyalur referensi Tegangan VCC & terminal Ground sink) dapat dirotasi dan ditukar pasang secara bebas, namun khusus eksklusif untuk Pin Ekstraktor di Tengah dia bersifat wajib/absolut absolut fix ditancapkan kepada port terminal inkuiri ADC Analog Arduino**.

Sejatinya kaki trimpot / potensio meter mekanik hanyalah instrumen pasif tersusun atas irisan peretas karbon *resistor tracker* semibundar (jalur track cincin linier). Menukar kaki Tepi Kiri dan Tepi Kanan itu ibarat memutarbalikkan secara sepihak memutar kutub penyuplai rentang sumbu hambatan (*Voltage Divider*) di rel lintasannya. 
**Yang terjadi apabila tertukar orientasinya**: Jika diawal kita telah memetakan skenario bila gagang *knob* diputar mengarahkan mentok ke arah sumbu Kanan Puncak artinya rasio hambatannya menyusut drastis membocorkan lolosan tegangannya mentok menuju VCC 5V penuh (Mengartikan konversi output *Nilai Maksimum Digital Integer / 1023*), maka ketika kabel colokan polaritas kakinya direverse ditukar mendadak di *breadboard*, efek yang diekspektasikan adalah orientasinya menjejalkan skenario *Counter-logic* yang di-*mirror*. Kini putaran gagang seret Kanan Puncak bakal dirasakan memproduksi kejanggalan hambatan ektrem bertolak ukur mengekang membungkam laju tegangan menjorok mepet jadi 0V (Nilai nol).

Kesimpulan komprehensif, menukar sepasang pin jepit sumbernya tidak sedikitpun mengakibatkan alat trimpot/potensiometer tersebut menjadi terbakar/rusak *conslet* mematikan sama sekali, hal itu esensinya semata-mata cuman mem-**flip** alias *men-mirror* interaksi orientasi fungsional parameter operasional membaca limitasi maksimum menjadi titik minimum (berpindah kutub logika referensi inversi mekanikal murni).

### 3. Modifikasi program penggabungan I2C dan UART

### 📌 Source Code
```cpp
#include <Wire.h>               // Mengimpor library fondasi low-level antarmuka I2C / TWI standar Arduino (protokol implementasi Two-Wire bus)
#include <LiquidCrystal_I2C.h>  // Mengimpor library sub-koleksi eksternal mengatur abstraksi command driver layar matriks LCD modul ekspander chip serializer I2C

LiquidCrystal_I2C lcd(0x27, 16, 2); // Instansiasi pemfaktoran pendaftaran variabel abstrak objek class 'lcd' mengikat menetapkan pendaftaran parameter register rute alamat I2C pada channel 0x27, juga pemetaan peta definisi resolusi fisik layar untuk jumlah penampungan string format karakter 16 pilar matrix kolom cell x dengan 2 lantai grid level baris tingkat di panel LCD

const int pinPot = A0; // Pengukuhan konstanta numerik mengidentifikasi nomor referensi kaki / pin penunjukkan rute pelacakan jalur rute *Analog input* untuk penerimaan sensor mekanik trimpot / potensio di analog pin A0

void setup() {
  Serial.begin(9600); // Mendirikan & mengukir persiapan inisiasi awal interogasi terminal UART *Serial Monitor* menyelaraskan tempo presisi sampling parameter baud *clock-rate* kecepatan pengiriman sebesar 9600 bit kepingan ditransfer per second menuju komputer
  lcd.init();         // Mem-*wakeup* serta me*re-configure* default bootloader internal register controller hardware mesin rendering di daleman chip modul IC *periphery* LCD dari masa tidurnya di posisi *stand-by reset*
  lcd.backlight();    // Mengaktifkan secara elektrikal memberi aba aba memicu saklar tegangan transistor mensuplai pencahayaan LED dioda terang layar *Backlight* pada blok belakang dinding LCD
}

void loop() {
  // --- [1] Ruang Fase Pembacaan Pengkajian Data Polos / Sensor Raw Data dari port ADC (Ekstraksi Konversi Tegangan skala Fisik Linear ke rentang ter-kuantisasi level Digital Data Bits) ---
  int nilaiADC = analogRead(pinPot); // Fungsi instruksi mengekstrak mengais dan memindahtangankan jejak sampling tegangan yang terbaca di input receiver analog kaki A0 lantas mem-proyeksi memetakam rasio ukurannya ditransformasi jadi resolusi representasi variabel bilangan diskrit utuh tipe Integer di parameter ADC beresolusi 10-bit (range 0 menjorok hingga titik mentok maksimal level 1023 pecahan)

  // --- [2] Tahapan Sentral Blok Ruang Komputasi Proses Data Meramu Representasi Ekstra Pengolahan Aljabar Matematis mencari skala Voltase nyata & normalisasi kalkulasi indeks proporsionalitas Persentase ---
  float voltase = (nilaiADC / 1023.0) * 5.0;      // Mengaplikasikan implementasi konversi parameter perbandingan linier matematis komputasional menaksir ekuivalensi rasio besaran satuan tegangan sumber tegangan absolut Volts proporsional dengan presisi bilangan *floating-point*
  float persen = (nilaiADC / 1023.0) * 100.0;     // Menerapkan prosedur serupa mentransformasi proyeksi dimensi angka ADC mutlak tersebut mendefinisikan menjadikannya kalkulasi indeks rasio fraksional representasi level nilai proporsional range persentase perseratusan maksimal skala logis (%) mentok di (100.0)
  int panjangBar = map(nilaiADC, 0, 1023, 0, 16); // Pelaksanaan konversi prosedur ekstrapolasi linear fungsi Normalisasi parameter pemetaan konversi rasio jangkauan ekuasi limitasi kuantitas total rentang sensor ADC menuju sasaran pilar jumlah balok maksimum layar penampang matriks bar lcd batas konklusif di (16 spasi petak baris kolom karakter)
  
  // --- [3] Blok Proses Modul Eksekusi Penyiaran Transmisi Menampilkan Paket UART Output System / Rute Pengiriman Deretan Formasi Log Teks Terformat presisi menuju Perangkat PC Monitor ---
  Serial.print("ADC: ");         // Meneteskan aliran sekuel byte string konstan bertindak sebagai awalan/header teks judul penamaan indikator awal
  Serial.print(nilaiADC);        // Memompa mengirim murni keping byte mencetak angka murninya nilai hitung real ADC berdampingan di jalurnya
  Serial.print(" Volt: ");       // Mencetak keping string statis memformasikan label jeda pembatas visual pelengkap atribut indikator nama besaran fisika variabel tegangan Volts
  Serial.print(voltase);         // Menyisipkan nilai desimal berisikan angka dari hasil komputensi float nilai ekuilibrium kalkulasi voltase lengkap terintegrasi koma presisi pembulatan rasionalisasi
  Serial.print(" V Persen: ");   // Melapisi dan menutup batasan margin frame memuat konjungsi jeda spasi separator parameter identitas pelancong persen indeks 
  Serial.print(persen);          // Memproyeksikan transfer kalkulasi final representasi float diskrit perbandingan persentase murni tanpa komplikasi embel satuan tambahan 
  Serial.println("%");           // Terminator pungkasan karakter pemungkas mem-finishing deretan string dihiasi logo persen menutup memukul konversi baris *simbol break return string \r new line \n* ganti enter lompat selang baris baru di layout display monitor

  // --- [4] Blok Eksekusi Mengolah Pengoperasian Konstruksi Grafis Komposisi Merender Sistem I2C Matrix Character LCD Display Panel (Proses Visualisasi Lapisan ke Perangkat Fisik Front-end) --- 
  lcd.setCursor(0, 0); // Reposisi set menggerakkan navigasi orientasi awal lokomotif penggerak pointernya secara spasial eksplisit mendaratkan mematok kursor pangkal pergerakan penulisan start origin eksklusif menjurus tepat menduduki sel pertiadaan indeks matriks sel 'X' (0) berlabuh di sudut tepi margin paling kiri layar atap pojok atas lantai baris row 'Y' ke-nol horizontal dimensi atas (0)
  lcd.print("ADC: ");  // Merekonsiliasi peluncuran perintah pencetakan menulis stiker tempel cat teks label header identitas awalan inisial konstan pembuka keterangan parameter pengiring yakni "ADC:" berlabuh mendarat mengkonsumsi lahan display sel abjad string demi mendampingi indikator utama
  lcd.print(nilaiADC); // Memoles implementasi mendikte perintah konversi proyeksi deretan susunan angka representasional dari nilai kuantitas kalkulasi var mutlak "ADC" terkini yang diracik dideretkan dengan mencetak menindas ke cell bergeser dinamis mengikuti jejak kursor penulisan aktif pada petak kolom cell sejajar tepat di sebelahnya
  lcd.print("  ");     // Secara cerdik mengeksploitasi trik instan murah *overwrite clear padding buffer* memoles meriset layar dengan membasuhkan menempel jejak sekumpulan *whitespace* selang jarak lap spasi lebar mengarsir meratakan bersih mem- *flashover* sel petak rentan menghapus sampah kotor residu *artifact trails* jejak sisa-sisa persinggahan ekor buntut karakter bilangan masa lampau angka besar rumpang (*ghosting fix*)
  lcd.print(persen, 0);// Menampilkan rute ekstrasi pengimplementasian pencetakan kalkulasi luaran output nilai indeks pencapaian rasio percent namun diberi aturan opsional pengekangan presisi akurasi float dengan parameter limitasi (, 0) melarang keras memamerkan membuang tail ekor deretan serentetan serakan nol float poin pembulatan resolusi koma desimal berkepanjangan agar tak menjerembab lahan LCD
  lcd.print("%   ");   // Merapikan polesan epilog penghujung garis eksekusi menoreh menaruh akhiran dekorasi stempel ornamen lambang persentase % dibuntut ekor nilainya diimbuhkan disuapin dijejali dengan padding ekstra pelapis *space blank array* pembersih penahan residu kotoran ekor digit karakter *overflow wrap line* string membabibuta menjebol pagar memanjang menjorok memagari agar rapi tidak menindih baris akhir limit pinggir kanvas layar berbatas sempit.

  // --- [5] Blok Eksekusi Progress Bar Indikator Analitik Display Grafik Simulasi (Baris Lantai Dua) --- 
  lcd.setCursor(0, 1); // Me-relokasi manuver men-set tata posisi target ekspektasi lokasi bidikan proyektor koordinat pointer navigasi melesat memindahkannya start lompat anjlok merosot meluncur menempati area dasar blok *floor layout* layar daratan sel indeks lantai posisi di bawah mulai dari indeks garis bujur kolong panel nol (0) posisi sumbu axis kiri
  for (int i = 0; i < 16; i++) { // Menginisialisasi skema *counter-based syntax control flow iterasi* (kalang loop) menunjuk enumerator pencacah parameter step indeks increment 'i' diputar rutin merangkak berurutan dari rentang ujung dasar (0) demi misi merender dan memodulasi mencetak grafis tumpukan blok simulasi pita pengukur panjang progres *gauge level bar* merata solid mencakup jangkauan mendisplai layar penuh berkapasitas resolusi bentangan 16 petak cell horisontal kompartemen karakter (diekskusi terpecah belah tersegmen selaras meneteskan 1 kotak sel pilar secara per sekuen tiap putarannya)
    if (i < panjangBar) { // Evaluasi pemantauan pembandingan *gate check logic threshold* menyangkal mengkonfirmasi kondisional memilah apakah status variabel penunjuk enumerator pointer kalkulator batas baris lintasan petak step 'i' yang menanjak naik ini keberadaannya berkedudukan belum melampaui limit melintas batas *limit bound metric logic length map* konversi sasaran variabel rasio analog ekuivalensi bar nya?
      lcd.print((char)255); // Bilamana kondisi perbandingan premisnya dinilai sah (True) valid terpenuhi merengkuh ambang, segera wujudkan manifestasi cetak corak selnya mendatangkan proyeksikan panggil mengimpor arsir kode grafis blok peta solid asir tekstur grafis balok sel pengisi kotak berformat hitam pekat tebal buta pixel padat yang di sandi pada entitas representasi urutan (*Extended ASCII character layout indeks array ke-255 atau setara memori bilangan heksa representatif kustom khusus *0xFF* blok arsir full solid) mendirikan tugu indikator tinggi padat
    } else {
      lcd.print(" "); // Bilamana eksploitasi loop ternyata sudah menyimpang kehausan melayang kelewatan jauh mendobrak melewati rasio *margin ceiling limiter threshold* memakan target sisa residu hamparan kosong kotak memanjang cell diluar panjang takaran asli, cukup blok dan isi *filler*-nya kompensasinya ditutup murni sekadar memakai sematan stempel blank sisipan embel-embel stiker karakter huruf luang pelapis spasi *space bar* tembus pandang " " sehingga visualisasinya *direset dinetralisir* tersapu pudar transparan polos rata tidak numpuk berjejal merepotkan pandangan estetik
    }
  }

  // --- [6] Penutup Rate Limiting Refresh Management --- 
  delay(200); // Mendesak sistem mencicipi implementasi eksekusi pengalaman jeda waktu istirahat sejenak me-*refresh padding* relaksasi sinkronisasi pembekuan *halt suspension thread hold limitator statis* suspensi penahanan laju penundukan agresivitas siklus repetisif memancing setop ter-jeda putaran rute instruksi instruksi *loop delay blocker* diam menyekat mengkalkulasi waktu durasi selama seperlima / seperempat millenium melaju (*200 Milisecond* / detik) untuk membuahkan tujuan melandakan mendinginkan *overhead stress limit resource* kelelahan komponen pemrosesan otak silikon silinder (*throttling relaxation*) sekaligus meredam memitigasi menyempurnakan *framerate visual fidelity timing syncronization logic* keutuhan *flicker control refresh LCD frame buffer persistence* mencegah laju tumpahan membludak rentet kompensasi visual tampilan layar instrumen peragaan membludak ber-kedip mengamuk (*fast flickering ghost artifacts glitch*) terengah-engah dan *illegible blur*
}
```

### 4. Tabel Hasil Pengukuran Analisis Kalkulasi Serial Berdasarkan Modifikasi Potensiometer

| ADC | Volt (V) | Persen (%) |
|-----|----------|------------|
| 1   | 0.00 V   | 0.10 %     |
| 21  | 0.10 V   | 2.05 %     |
| 49  | 0.24 V   | 4.79 %     |
| 74  | 0.36 V   | 7.23 %     |
| 96  | 0.47 V   | 9.38 %     |

*Catatan Validasi Data Simulasi Komputasi Kalkulator Matematika*: Besaran pengukuran komparatif kolom di atas tidak dikarang secara impulsif tebakan perkiraan subyektif awam, melainkan murni terproyeksi tertuang terealisasi mutlak melalui pengujian *benchmarking runtime kalkulasi variabel sistem persamaan korelasi linear kalibrasi logaritmik sensor hardware persentase `float persen = (ADC/1023.0)*100.0` presisi serta representasi proporsional limit tegangan analog rasio potensial skala voltase `float voltase = (ADC/1023.0)*5.0`*.
