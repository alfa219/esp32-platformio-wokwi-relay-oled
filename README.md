# ESP32 PlatformIO Wokwi Relay OLED Blynk

Proyek ini adalah simulasi dan pengembangan sistem kontrol **relay/LED berbasis ESP32** dengan **PlatformIO** di **VS Code**, **Wokwi** sebagai simulator hardware, **OLED SSD1306** sebagai display lokal, dan **Blynk Cloud** sebagai antarmuka kontrol jarak jauh. [file:1][web:146]

Project ini dibuat sebagai pengembangan dari praktikum *Sistem Akuisisi Data*, khususnya pada bagian komunikasi platform, simulasi ESP32, penambahan relay/LED/OLED, dan integrasi Blynk Cloud. [file:1]

## Fitur

- ESP32 DevKit sebagai mikrokontroler utama. [file:1]
- Simulasi rangkaian dengan Wokwi. [file:1][web:146]
- Pengembangan firmware menggunakan PlatformIO di VS Code. [web:177]
- Kontrol output melalui Blynk menggunakan Virtual Pin. [web:147]
- Tampilan status perangkat pada OLED SSD1306. [file:1]
- Siap dikembangkan ke multi-channel LED/relay atau sensor IoT lain. [file:1][web:180]

## Arsitektur Sistem

Sistem bekerja dengan alur berikut:

1. ESP32 terhubung ke WiFi Wokwi (`Wokwi-GUEST`). [web:146][web:180]
2. ESP32 login ke Blynk Cloud menggunakan `BLYNK_TEMPLATE_ID`, `BLYNK_TEMPLATE_NAME`, dan `BLYNK_AUTH_TOKEN`. [file:1][web:148][web:151]
3. Widget di dashboard Blynk mengirim nilai ke Virtual Pin, misalnya `V0`. [file:1][web:147]
4. ESP32 membaca nilai tersebut melalui `BLYNK_WRITE(V0)`, lalu menyalakan atau mematikan relay/LED fisik. [web:147][web:180]
5. OLED menampilkan status koneksi WiFi, koneksi Blynk, dan status output lokal. [file:1]

## Komponen yang Digunakan

- ESP32 DevKit
- Relay module / LED output
- OLED SSD1306 I2C
- Wokwi Simulator
- PlatformIO IDE
- Blynk Cloud Dashboard [file:1][web:146]

## Struktur Folder

```text
.
├── .gitignore
├── README.md
├── platformio.ini
├── wokwi.toml
├── diagram.json
├── src/
│   └── main.cpp
├── include/
├── lib/
└── test/
```

## Konfigurasi `platformio.ini`

Pastikan `platformio.ini` memuat library yang dibutuhkan, termasuk Blynk dan OLED:

```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
monitor_speed = 115200

lib_deps =
  blynkkk/Blynk
  adafruit/Adafruit GFX Library
  adafruit/Adafruit SSD1306
```

## Konfigurasi Blynk

Agar firmware bisa terhubung ke Blynk Cloud, lakukan langkah berikut:

1. Buat akun di Blynk Cloud. [file:1]
2. Buat **Template** baru untuk ESP32 dengan koneksi WiFi. [file:1]
3. Tambahkan **Datastream** Virtual Pin, misalnya:
   - `V0` untuk kontrol ON/OFF output
   - `V1` untuk LED indikator status (opsional) [file:1][web:147][web:170]
4. Tambahkan widget di dashboard:
   - **Switch** untuk kontrol perangkat
   - **LED widget** untuk indikator status (opsional) [web:147][web:170]
5. Buat **Device** dari template.
6. Salin `Template ID`, `Template Name`, dan `Auth Token` ke file `src/main.cpp`. [file:1][web:148][web:151][web:154]

## WiFi untuk Wokwi

Untuk simulasi Wokwi, gunakan:

```cpp
char ssid[] = "Wokwi-GUEST";
char pass[] = "";
```

Wokwi menyediakan access point virtual untuk ESP32 sehingga perangkat dapat mensimulasikan koneksi internet. [web:140][web:143]

## Cara Menjalankan

1. Clone repository:
```bash
git clone https://github.com/alfa219/esp32-platformio-wokwi-relay-oled.git
cd esp32-platformio-wokwi-relay-oled
```

2. Buka project di VS Code.

3. Pastikan extension berikut sudah terpasang:
- PlatformIO IDE
- Wokwi for VS Code [web:177]

4. Build project:
```bash
pio run
```

5. Jalankan simulasi Wokwi dari VS Code.

6. Pastikan kredensial Blynk sudah benar di `src/main.cpp`.

7. Buka dashboard Blynk dan uji kontrol output dari widget yang terhubung ke Virtual Pin. [web:147][web:170]

## Catatan Keamanan

`BLYNK_AUTH_TOKEN` adalah kredensial perangkat dan sebaiknya tidak dipublikasikan di repository publik. Jika token sudah terlanjur dipublikasikan, lakukan regenerasi token dari Blynk Device Info. Praktik ini penting dalam keamanan sistem IoT berbasis cloud. [web:151][web:154]

## Pengembangan Lanjutan

Beberapa arah pengembangan selanjutnya:

- Kontrol 4 LED / 4 relay dari Blynk, sesuai latihan lanjutan pada laporan. [file:1]
- Penambahan sensor seperti DHT22 atau LDR.
- Sinkronisasi dashboard Blynk dengan OLED lokal.
- Pemisahan kode menjadi beberapa modul agar struktur firmware lebih bersih. [file:1][web:180]

## Repository

GitHub:  
`https://github.com/alfa219/esp32-platformio-wokwi-relay-oled`

## License

Tambahkan lisensi sesuai kebutuhan, misalnya MIT License.