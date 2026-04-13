# ESP32 Relay & OLED Control – PlatformIO + Wokwi

Proyek ini adalah simulasi sistem **kontrol relay dengan umpan balik OLED** menggunakan **ESP32**, dikembangkan dengan **PlatformIO di VS Code** dan disimulasikan menggunakan **Wokwi**.  
Proyek ini cocok sebagai bahan praktikum *Sistem Akuisisi Data* dan contoh integrasi workflow modern (VS Code + PlatformIO + Wokwi).

## Fitur Utama

- Board: **ESP32 DevKit** (Arduino framework)
- Kontrol **relay** dengan logika otomatis berbasis timer
- Tampilan status sistem pada **OLED SSD1306** (mode, status relay, interval)
- Simulasi penuh di **Wokwi** (tanpa hardware fisik)
- Struktur proyek rapi dengan **PlatformIO** (mudah dipindahkan ke hardware nyata)

## Prasyarat

Sebelum memulai, siapkan:

- **Visual Studio Code**
- Extension:
  - **PlatformIO IDE**
  - **Wokwi** untuk VS Code
- PlatformIO sudah terpasang dan bisa membuat project ESP32

> Catatan: Pastikan koneksi internet aktif saat pertama kali build untuk mengunduh library yang diperlukan (Adafruit GFX & Adafruit SSD1306).

## Struktur Proyek

```text
.
├─ platformio.ini
├─ wokwi.toml
├─ diagram.json
├─ src/
│  └─ main.cpp
└─ README.md
```

Penjelasan:

- `platformio.ini` – konfigurasi PlatformIO (board, framework, library, serial speed)
- `wokwi.toml` – konfigurasi Wokwi untuk mengaitkan firmware PlatformIO dengan simulator
- `diagram.json` – skema rangkaian di Wokwi (ESP32, relay, LED, OLED, tombol jika ada)
- `src/main.cpp` – kode program utama ESP32
- `README.md` – dokumentasi proyek

## Konfigurasi PlatformIO

```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
monitor_speed = 115200

lib_deps =
  adafruit/Adafruit GFX Library
  adafruit/Adafruit SSD1306
```

- `board = esp32dev` menargetkan board ESP32 DevKit
- `monitor_speed` harus sama dengan nilai di `Serial.begin()` di `main.cpp`
- `lib_deps` memastikan library OLED diunduh otomatis saat build

## Konfigurasi Wokwi

```toml
[wokwi]
version = 1
firmware = ".pio/build/esp32dev/firmware.bin"
elf = ".pio/build/esp32dev/firmware.elf"
```

- `firmware` menunjuk ke file `.bin` hasil build PlatformIO
- `elf` digunakan Wokwi untuk debug yang lebih detail  
Pastikan environment (`esp32dev`) sama dengan yang digunakan di `platformio.ini`.

## Diagram Wokwi

File `diagram.json` mendeskripsikan rangkaian di Wokwi, misalnya:

- 1x **ESP32 DevKit**
- 1x **Relay module**
- 1x LED + resistor sebagai indikator beban relay
- 1x **OLED SSD1306** (I2C, alamat 0x3C)
- (Opsional) Push button untuk mengganti mode/manual toggle

Koneksi umum:

- Relay IN → GPIO23  
- OLED SDA → GPIO21  
- OLED SCL → GPIO22  
- VCC untuk relay + OLED → 3V3  
- Semua GND → GND ESP32

Detail aktual koneksi ada di dalam `diagram.json`.

## Kode Utama (`src/main.cpp`)

Secara garis besar, program melakukan:

1. Inisialisasi serial (`Serial.begin(115200)`), pin relay, dan OLED SSD1306
2. Menampilkan informasi status di OLED (mode, status relay, interval)
3. Menjalankan logika **mode otomatis** (relay toggle dengan interval tertentu) atau **mode manual** (berbasis command atau push button, tergantung versi)
4. Mengirim log status ke serial terminal untuk debugging

Lihat isi lengkap `src/main.cpp` di repo ini untuk implementasi detail.

## Cara Menjalankan

### 1. Clone Repository

```bash
git clone https://github.com/<username>/<nama-repo>.git
cd <nama-repo>
```

Ganti `<username>` dan `<nama-repo>` sesuai GitHub kamu.

### 2. Buka di VS Code

- Buka folder proyek di VS Code
- Pastikan extension **PlatformIO IDE** dan **Wokwi** sudah aktif

### 3. Build dengan PlatformIO

Dari VS Code:

- Buka panel PlatformIO
- Pilih environment `esp32dev`
- Klik **Build**

Atau via command line:

```bash
pio run
```

Jika build sukses, firmware akan ada di `.pio/build/esp32dev/firmware.bin`.

### 4. Jalankan Simulasi di Wokwi

- Jalankan perintah Wokwi di VS Code (mis. “Start Simulation”)  
- Extension akan membaca `wokwi.toml` dan `diagram.json`, lalu memuat firmware ESP32

Saat simulasi berjalan, Anda akan melihat:

- OLED menampilkan status sistem (mode, relay, interval)
- Relay dan LED berubah sesuai mode/interval yang diprogram
- Serial terminal menampilkan log status

## Perilaku Sistem

- **Mode AUTO**  
  Relay melakukan toggle ON/OFF dengan interval tertentu (misal 1000 ms).  
  OLED menampilkan `Mode: AUTO`, status relay, dan interval.

- **Mode MANUAL**  
  Relay diubah ON/OFF berdasarkan input (serial atau push button).  
  OLED menampilkan `Mode: MANUAL` dan status relay saat ini.

Jika menggunakan terminal di VS Code + Wokwi, perhatikan bahwa sebagian versi hanya nyaman untuk melihat output, sehingga push button di `diagram.json` biasanya menjadi antarmuka utama untuk user.

## Ide Pengembangan Lanjutan

- Versi dengan **4 relay / 4 LED** (multi-channel control)
- Integrasi dengan **Blynk Cloud** untuk kontrol dari dashboard web/mobile
- Penambahan sensor (DHT22, LDR, dsb.) sehingga relay menjadi aktuator berbasis kondisi real
- Refactor kode menjadi modul-modul terpisah (mis. `display.cpp`, `logic.cpp`) untuk arsitektur lebih bersih

## Lisensi

Tentukan lisensi yang kamu inginkan, contoh:

```text
MIT License
```

Atau:

> Proyek ini dirilis dengan lisensi MIT. Silakan gunakan, modifikasi, dan kembangkan sesuai kebutuhan.