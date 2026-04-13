# ESP32 PlatformIO Wokwi — Relay & OLED dengan Blynk Cloud

Sistem kontrol berbasis **ESP32** yang mensimulasikan kendali relay/LED secara remote melalui **Blynk Cloud**, dengan tampilan status lokal pada **OLED SSD1306**. Proyek dikembangkan menggunakan **PlatformIO** di VS Code dan disimulasikan melalui **Wokwi**.

> Dibuat sebagai pengembangan dari praktikum *Sistem Akuisisi Data* — mencakup komunikasi platform IoT, simulasi hardware ESP32, dan integrasi Blynk Cloud.

---

## Fitur Utama

- Kontrol relay/LED via Blynk Cloud menggunakan Virtual Pin
- Tampilan status koneksi dan output pada OLED SSD1306 (I2C)
- Simulasi hardware penuh menggunakan Wokwi (tanpa hardware fisik)
- Firmware dikembangkan dengan PlatformIO + Arduino framework
- Mudah dikembangkan ke multi-channel relay, sensor DHT22, LDR, dll.

---

## Arsitektur Sistem

```
[Blynk Dashboard] → (Virtual Pin V0) → [Blynk Cloud]
                                              ↓
                                       [ESP32 via WiFi]
                                         ↙       ↘
                                  [Relay/LED]   [OLED SSD1306]
```

**Alur kerja:**
1. ESP32 terhubung ke WiFi (`Wokwi-GUEST` untuk simulasi).
2. ESP32 autentikasi ke Blynk Cloud menggunakan Auth Token.
3. Widget Switch di dashboard Blynk mengirim nilai ke Virtual Pin `V0`.
4. ESP32 membaca nilai via `BLYNK_WRITE(V0)` lalu menyalakan/mematikan relay.
5. OLED menampilkan status WiFi, koneksi Blynk, dan kondisi output saat ini.

---

## Komponen

| Komponen | Keterangan |
|---|---|
| ESP32 DevKit | Mikrokontroler utama |
| Relay / LED | Output yang dikontrol |
| OLED SSD1306 | Display status lokal via I2C |
| Wokwi | Simulator hardware |
| PlatformIO | IDE & build system |
| Blynk Cloud | Platform kontrol IoT remote |

---

## Struktur Proyek

```
esp32-platformio-wokwi-relay-oled/
├── src/
│   └── main.cpp          # Kode utama firmware
├── include/
├── lib/
├── test/
├── platformio.ini        # Konfigurasi PlatformIO & library
├── wokwi.toml            # Konfigurasi simulasi Wokwi
├── diagram.json          # Skema rangkaian Wokwi
└── README.md
```

---

## Setup & Konfigurasi

### 1. Konfigurasi `platformio.ini`

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

### 2. Konfigurasi Blynk Cloud

1. Buat akun di [Blynk Cloud](https://blynk.io).
2. Buat **Template** baru → pilih hardware ESP32, koneksi WiFi.
3. Tambahkan **Datastream** Virtual Pin:
   - `V0` → kontrol ON/OFF relay/LED
   - `V1` → LED indikator status (opsional)
4. Tambahkan widget di **Dashboard**:
   - Widget **Switch** → dihubungkan ke `V0`
   - Widget **LED** → indikator status (opsional)
5. Buat **Device** dari template, lalu salin:
   - `BLYNK_TEMPLATE_ID`
   - `BLYNK_TEMPLATE_NAME`
   - `BLYNK_AUTH_TOKEN`
6. Tempel ketiga nilai tersebut ke bagian atas `src/main.cpp`.

### 3. Konfigurasi WiFi (Wokwi)

Wokwi menyediakan access point virtual. Gunakan kredensial berikut di `main.cpp`:

```cpp
char ssid[] = "Wokwi-GUEST";
char pass[] = "";
```

---

## Cara Menjalankan

```bash
# 1. Clone repository
git clone https://github.com/alfa219/esp32-platformio-wokwi-relay-oled.git
cd esp32-platformio-wokwi-relay-oled
```

2. Buka folder proyek di **VS Code**.
3. Pastikan extension berikut sudah terpasang:
   - [PlatformIO IDE](https://platformio.org/install/ide?install=vscode)
   - [Wokwi for VS Code](https://marketplace.visualstudio.com/items?itemName=wokwi.wokwi-vscode)
4. Isi kredensial Blynk di `src/main.cpp` (Template ID, Template Name, Auth Token).
5. Build firmware:
   ```bash
   pio run
   ```
6. Jalankan simulasi melalui Wokwi di VS Code.
7. Buka dashboard Blynk → uji kontrol relay via widget Switch.

---

## Catatan Keamanan

> **Jangan commit `BLYNK_AUTH_TOKEN` ke repository publik.**

Auth Token adalah identitas unik perangkat kamu di Blynk Cloud. Jika sudah terlanjur terpublikasi, segera lakukan **regenerasi token** melalui menu *Device Info* di Blynk.

Untuk proyek yang lebih aman, simpan token di file `.env` atau gunakan mekanisme provisioning.

---

## Pengembangan Lanjutan

- [ ] Kontrol 4 relay/LED secara independen dari Blynk
- [ ] Integrasi sensor DHT22 (suhu & kelembaban) atau LDR (cahaya)
- [ ] Sinkronisasi data sensor ke dashboard Blynk sekaligus OLED
- [ ] Refactor kode ke beberapa modul untuk struktur firmware yang lebih bersih
- [ ] Tambahkan OTA (Over-the-Air) firmware update

---

## Repository

**GitHub:** [alfa219/esp32-platformio-wokwi-relay-oled](https://github.com/alfa219/esp32-platformio-wokwi-relay-oled)

---

## Lisensi

Proyek ini dilisensikan di bawah [MIT License](LICENSE).