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

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library.
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET -1        // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C  ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES 10  // Number of snowflakes in the animation example

#define LOGO_HEIGHT 16
#define LOGO_WIDTH 16
static const unsigned char PROGMEM logo_bmp[] = { 0b00000000, 0b11000000,
                                                  0b00000001, 0b11000000,
                                                  0b00000001, 0b11000000,
                                                  0b00000011, 0b11100000,
                                                  0b11110011, 0b11100000,
                                                  0b11111110, 0b11111000,
                                                  0b01111110, 0b11111111,
                                                  0b00110011, 0b10011111,
                                                  0b00011111, 0b11111100,
                                                  0b00001101, 0b01110000,
                                                  0b00011011, 0b10100000,
                                                  0b00111111, 0b11100000,
                                                  0b00111111, 0b11110000,
                                                  0b01111100, 0b11110000,
                                                  0b01110000, 0b01110000,
                                                  0b00000000, 0b00110000 };

// Wifi network station credentials
#define WIFI_SSID "ichibot"
#define WIFI_PASSWORD "we@ichibot"
// Telegram BOT Token (Get from Botfather)
#define BOT_TOKEN "6527652038:AAEy5hAaSS72aX-NnJnYdQBFqp_9wtkX6QQ"

// Use @myidbot (IDBot) to find out the chat ID of an individual or a group
// Also note that you need to click "start" on a bot before it can
// message you
#define CHAT_ID "6320889521"

WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);

/*Define int variables*/
int Red = 0;
int Green = 0;
int Blue = 0;
int Frequency = 0;
bool statusUang = 0;
bool msg = 0;
int Uang = 0;

void setup() {
  pinMode(S2, OUTPUT);       /*Define S2 Pin as a OUTPUT*/
  pinMode(S3, OUTPUT);       /*Define S3 Pin as a OUTPUT*/
  pinMode(sensorOut, INPUT); /*Define Sensor Output Pin as a INPUT*/
  Serial.begin(115200);      /*Set the baudrate to 115200*/
  Serial.print("This is TCS3200 Calibration Code");

  Serial.println();

  // attempt to connect to Wifi network:
  Serial.print("Connecting to Wifi SSID ");
  Serial.print(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT);  // Add root certificate for api.telegram.org
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nWiFi connected. IP address: ");
  Serial.println(WiFi.localIP());

  Serial.print("Retrieving time: ");
  configTime(0, 0, "pool.ntp.org");  // get UTC time via NTP
  time_t now = time(nullptr);
  while (now < 24 * 3600) {
    Serial.print(".");
    delay(100);
    now = time(nullptr);
  }
  Serial.println(now);

  bot.sendMessage(CHAT_ID, "Bot started up", "");

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;  // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000);  // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();

  // Draw a single pixel in white
  display.drawPixel(10, 10, SSD1306_WHITE);

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();
  delay(2000);
}

void loop() {
  display.clearDisplay();
  display.setTextSize(2);  // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(5, 0);
  display.println(F("Infaq Yuk!"));
  display.display();  // Show initial text
  delay(100);

  Red = getRed();
  delay(200); /*wait a 200mS*/
  Green = getGreen();
  delay(200); /*wait a 200mS*/
  Blue = getBlue();
  delay(200); /*wait a 200mS*/
  Serial.print("Red Freq = ");
  Serial.print(Red); /*Print Red Color Value on Serial Monitor*/
  Serial.print("   ");
  Serial.print("Green Freq = ");
  Serial.print(Green); /*Print Green Color Value on Serial Monitor*/
  Serial.print("   ");
  Serial.print("Blue Freq = ");
  Serial.println(Blue); /*Print Blue Color Value on Serial Monitor*/

  if (Red > 35 && Red < 40 && Green > 32 && Green < 37 && Blue > 25 && Blue < 30 && statusUang == 0) {  //parameter masing-masing uang harus dikalibrasi manual
    Serial.println("2rb");
    Uang = Uang + 2000;
    statusUang = !statusUang;
  } else if (Red > 20 && Red < 28 && Green > 25 && Green < 35 && Blue > 25 && Blue < 35 && statusUang == 0) {  //parameter masing-masing uang harus dikalibrasi manual
    Serial.println("5rb");
    Uang = Uang + 5000;
    statusUang = !statusUang;
  } else if (Red > 30 && Red < 37 && Green > 35 && Green < 40 && Blue > 25 && Blue < 30 && statusUang == 0) {  //parameter masing-masing uang harus dikalibrasi manual
    Serial.println("10rb");
    Uang = Uang + 10000;
    statusUang = !statusUang;
  } else if (Red > 22 && Red < 27 && Green > 18 && Green < 23 && Blue > 17 && Blue < 22 && statusUang == 0) {  //parameter masing-masing uang harus dikalibrasi manual
    Serial.println("20rb");
    Uang = Uang + 20000;
    statusUang = !statusUang;
  } else if (Red > 46 && Red < 55 && Green > 33 && Green < 38 && Blue > 25 && Blue < 30 && statusUang == 0) {  //parameter masing-masing uang harus dikalibrasi manual
    Serial.println("50rb");
    Uang = Uang + 50000;
    statusUang = !statusUang;
  } else if (Red > 17 && Red < 22 && Green > 23 && Green < 29 && Blue > 19 && Blue < 25 && statusUang == 0) {  //parameter masing-masing uang harus dikalibrasi manual
    Serial.println("100rb");
    Uang = Uang + 100000;
    statusUang = !statusUang;
  } else if (Red > 100 && Green > 100 && Blue > 100) {  //reset
    statusUang = 0;
    msg = 0;
  }
  if (statusUang == 1 && msg == 0) {
    Serial.println(Uang);
    display.clearDisplay();
    display.setTextSize(2);  // Draw 2X-scale text
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(5, 0);
    display.println(F("Jazakllahu"));
    display.setCursor(10, 15);
    display.println(F("Khairan!"));
    display.display();  // Show initial text
    delay(2000);

    bot.sendMessage(CHAT_ID, "Saldo Infaq Hari Ini :");
    String nominal = String(Uang);
    bot.sendMessage(CHAT_ID, nominal);

    msg = !msg;
  }
}

int getRed() {
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  Frequency = pulseIn(sensorOut, LOW); /*Get the Red Color Frequency*/
  return Frequency;
}

int getGreen() {
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  Frequency = pulseIn(sensorOut, LOW); /*Get the Green Color Frequency*/
  return Frequency;
}

int getBlue() {
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  Frequency = pulseIn(sensorOut, LOW); /*Get the Blue Color Frequency*/
  return Frequency;
}