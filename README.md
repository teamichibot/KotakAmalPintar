# Infaq Pintar dengan ESP32

Proyek ini bertujuan untuk membuat sistem infaq pintar menggunakan ESP32, sensor warna TCS3200, layar OLED, dan integrasi Telegram. Sistem ini dapat mendeteksi nominal uang berdasarkan warna, menampilkan saldo di layar OLED, serta mengirimkan notifikasi ke bot Telegram.

## Fitur Utama

1. **Deteksi Nominal Uang:** Menggunakan sensor warna untuk mengenali nominal uang berdasarkan nilai RGB.
2. **Tampilan Saldo:** Menampilkan saldo terkini di layar OLED.
3. **Notifikasi Telegram:** Mengirimkan saldo terbaru ke bot Telegram.
4. **WiFi Auto Connect:** Memanfaatkan WiFiManager untuk koneksi otomatis ke WiFi.

## Komponen yang Dibutuhkan

| No | Komponen                  | Jumlah |
|----|---------------------------|--------|
| 1  | ESP32                     | 1      |
| 2  | Sensor Warna (TCS3200)    | 1      |
| 3  | Layar OLED 128x64 (I2C)   | 1      |
| 4  | Push Button               | 1      |
| 5  | Kapasitor 10uF 25V        | 1      |
| 6  | Kabel Jumper              | Beberapa |
| 7  | Breadboard                | 1      |

## Rangkaian

| Komponen         | Pin ESP32       | Pin Komponen     | Keterangan                  |
|------------------|-----------------|------------------|-----------------------------|
| Sensor TCS3200   | 2               | S2               | Pin kontrol sensor          |
| Sensor TCS3200   | 4               | S3               | Pin kontrol sensor          |
| Sensor TCS3200   | 15              | OUT              | Output frekuensi warna      |
| Sensor TCS3200   | 3.3V            | VCC              | Daya sensor                 |
| Sensor TCS3200   | GND             | GND              | Ground sensor               |
| OLED Display     | 21              | SDA              | Data I2C                    |
| OLED Display     | 22              | SCL              | Clock I2C                   |
| OLED Display     | 3.3V            | VCC              | Daya OLED                   |
| OLED Display     | GND             | GND              | Ground OLED                 |
| Push Button      | GPIO 5          | Terminal 1       | Input reset saldo           |
| Push Button      | GND             | Terminal 2       | Ground                      |
| Kapasitor 10uF   | EN              | + Terminal       | Stabilisasi daya            |
| Kapasitor 10uF   | GND             | - Terminal       | Ground                      |

## Cara Kerja

1. **Inisialisasi:**
   - ESP32 terhubung ke jaringan WiFi menggunakan WiFiManager.
   - Sistem menampilkan teks sambutan di layar OLED.

2. **Deteksi Nominal:**
   - Sensor warna membaca nilai RGB dari uang yang dimasukkan.
   - Berdasarkan nilai RGB, nominal uang diidentifikasi dan ditambahkan ke saldo.

3. **Tampilkan Saldo:**
   - Saldo terbaru ditampilkan di layar OLED.

4. **Notifikasi Telegram:**
   - Saldo terkini dikirimkan ke bot Telegram yang telah dikonfigurasi.

5. **Kalibrasi Uang:**
   - Nilai RGB untuk setiap nominal uang perlu dikalibrasi dengan menjalankan sistem dan mencatat nilai frekuensi warna untuk setiap nominal.

## Kalibrasi Uang

1. Jalankan sistem.
2. Masukkan setiap nominal uang ke sensor.
3. Catat nilai frekuensi RGB yang muncul di serial monitor.
4. Masukkan nilai frekuensi RGB ke dalam kode program sesuai parameter nominal.

Contoh:
```cpp
if (Red > 35 && Red < 40 && Green > 32 && Green < 37 && Blue > 25 && Blue < 30) {
    Uang += 2000;
}
```

## Cara Menggunakan

1. **Susun Rangkaian:**
   - Hubungkan semua komponen sesuai dengan tabel rangkaian di atas.
2. **Upload Kode:**
   - Gunakan Arduino IDE untuk mengunggah kode ke ESP32.
   - Pastikan semua library yang dibutuhkan telah diinstal:
     - WiFiManager
     - UniversalTelegramBot
     - Adafruit_SSD1306
   - Konfigurasikan token bot Telegram dan ID chat pada kode.
3. **Operasikan Sistem:**
   - Nyalakan ESP32.
   - Hubungkan ke WiFi melalui WiFiManager.
   - Masukkan uang ke sensor untuk mendeteksi nominal.
   - Lihat saldo pada layar OLED dan notifikasi Telegram.

## Kode Program

Berikut adalah kode program utama:
```cpp
#define S2 2       /*Define S2 Pin Number of ESP32*/
#define S3 4        /*Define S3 Pin Number of ESP32*/
#define sensorOut 15 /*Define Sensor Output Pin Number of ESP32*/
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

#define OLED_RESET -1        // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C  ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define WIFI_SSID "ichibot"
#define WIFI_PASSWORD "we@ichibot"
#define BOT_TOKEN "YOUR_BOT_TOKEN"
#define CHAT_ID "YOUR_CHAT_ID"

WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);

// (Tambahkan kode lengkap sesuai kebutuhan proyek)
```

## Lisensi
Proyek ini dilisensikan di bawah [MIT License](LICENSE).

