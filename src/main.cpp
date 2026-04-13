#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

#define RELAY_PIN 26
#define SDA_PIN 21
#define SCL_PIN 22

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

bool relayState = false;
bool autoMode = true;

unsigned long previousMillis = 0;
unsigned long blinkInterval = 1000;

String inputBuffer = "";

void updateRelay(bool state) {
  relayState = state;
  digitalWrite(RELAY_PIN, relayState ? HIGH : LOW);
}

void printStatus() {
  Serial.println("===== STATUS SISTEM =====");
  Serial.print("Mode        : ");
  Serial.println(autoMode ? "AUTO" : "MANUAL");

  Serial.print("Relay       : ");
  Serial.println(relayState ? "ON" : "OFF");

  Serial.print("Interval    : ");
  Serial.print(blinkInterval);
  Serial.println(" ms");
  Serial.println("=========================");
}

void updateDisplay() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);
  display.println("ESP32 Control Panel");
  display.println("--------------------");

  display.setCursor(0, 18);
  display.print("Mode  : ");
  display.println(autoMode ? "AUTO" : "MANUAL");

  display.setCursor(0, 32);
  display.print("Relay : ");
  display.println(relayState ? "ON" : "OFF");

  display.setCursor(0, 46);
  display.print("Intvl : ");
  display.print(blinkInterval);
  display.println(" ms");

  display.display();
}

void printHelp() {
  Serial.println("\nPerintah yang tersedia:");
  Serial.println("on              -> Relay ON");
  Serial.println("off             -> Relay OFF");
  Serial.println("auto            -> Mode otomatis");
  Serial.println("manual          -> Mode manual");
  Serial.println("toggle          -> Toggle relay");
  Serial.println("status          -> Tampilkan status");
  Serial.println("interval 500    -> Ubah interval auto (ms)");
  Serial.println("help            -> Tampilkan bantuan\n");
}

void processCommand(String cmd) {
  cmd.trim();
  cmd.toLowerCase();

  if (cmd == "on") {
    autoMode = false;
    updateRelay(true);
    Serial.println("Relay diubah ke ON (MANUAL)");
  }
  else if (cmd == "off") {
    autoMode = false;
    updateRelay(false);
    Serial.println("Relay diubah ke OFF (MANUAL)");
  }
  else if (cmd == "toggle") {
    autoMode = false;
    updateRelay(!relayState);
    Serial.println("Relay di-toggle (MANUAL)");
  }
  else if (cmd == "auto") {
    autoMode = true;
    Serial.println("Mode diubah ke AUTO");
  }
  else if (cmd == "manual") {
    autoMode = false;
    Serial.println("Mode diubah ke MANUAL");
  }
  else if (cmd == "status") {
    printStatus();
  }
  else if (cmd.startsWith("interval")) {
    int spaceIndex = cmd.indexOf(' ');
    if (spaceIndex > 0) {
      String valueStr = cmd.substring(spaceIndex + 1);
      unsigned long newInterval = valueStr.toInt();

      if (newInterval >= 100) {
        blinkInterval = newInterval;
        Serial.print("Interval auto diubah menjadi ");
        Serial.print(blinkInterval);
        Serial.println(" ms");
      } else {
        Serial.println("Interval terlalu kecil. Gunakan >= 100 ms");
      }
    } else {
      Serial.println("Format salah. Contoh: interval 1000");
    }
  }
  else if (cmd == "help") {
    printHelp();
  }
  else if (cmd.length() > 0) {
    Serial.println("Perintah tidak dikenali. Ketik: help");
  }

  updateDisplay();
}

void readSerialCommand() {
  while (Serial.available()) {
    char c = Serial.read();

    if (c == '\n' || c == '\r') {
      if (inputBuffer.length() > 0) {
        processCommand(inputBuffer);
        inputBuffer = "";
      }
    } else {
      inputBuffer += c;
    }
  }
}

void handleAutoMode() {
  if (!autoMode) return;

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= blinkInterval) {
    previousMillis = currentMillis;
    updateRelay(!relayState);

    Serial.print("[AUTO] Relay: ");
    Serial.println(relayState ? "ON" : "OFF");

    updateDisplay();
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  updateRelay(false);

  Wire.begin(SDA_PIN, SCL_PIN);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED SSD1306 gagal diinisialisasi");
    while (true) {
      delay(10);
    }
  }

  display.clearDisplay();
  display.display();

  Serial.println("Sistem mulai...");
  Serial.println("ESP32 + Relay + OLED + Serial Command");
  printHelp();
  printStatus();
  updateDisplay();
}

void loop() {
  readSerialCommand();
  handleAutoMode();
}