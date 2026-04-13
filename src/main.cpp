#define BLYNK_TEMPLATE_ID "TMPL6CNsifER0"
#define BLYNK_TEMPLATE_NAME "control LED"
#define BLYNK_AUTH_TOKEN "XEE4RQGoC3Wjlx2bAbFi0NhD_mDa4P9C"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

#define RELAY_PIN 26
#define SDA_PIN 21
#define SCL_PIN 22

char ssid[] = "Wokwi-GUEST";
char pass[] = "";

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
BlynkTimer timer;

bool relayState = false;

void updateDisplay() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);
  display.println("ESP32 + Blynk");
  display.println("----------------");

  display.setCursor(0, 18);
  display.print("WiFi  : ");
  display.println(WiFi.status() == WL_CONNECTED ? "Connected" : "Disconnected");

  display.setCursor(0, 32);
  display.print("Blynk : ");
  display.println(Blynk.connected() ? "Connected" : "Disconnected");

  display.setCursor(0, 46);
  display.print("Relay : ");
  display.println(relayState ? "ON" : "OFF");

  display.display();
}

void publishStateToBlynk() {
  Blynk.virtualWrite(V0, relayState ? 1 : 0); // sinkronkan switch
  Blynk.virtualWrite(V1, relayState ? 255 : 0); // LED widget indikator
}

void setRelay(bool state) {
  relayState = state;
  digitalWrite(RELAY_PIN, relayState ? HIGH : LOW);

  Serial.print("Relay changed to: ");
  Serial.println(relayState ? "ON" : "OFF");

  updateDisplay();
  publishStateToBlynk();
}

BLYNK_CONNECTED() {
  Serial.println("Blynk connected");
  Blynk.syncVirtual(V0);
  publishStateToBlynk();
  updateDisplay();
}

BLYNK_WRITE(V0) {
  int value = param.asInt();
  setRelay(value == 1);
}

void checkConnection() {
  Serial.print("WiFi: ");
  Serial.print(WiFi.status() == WL_CONNECTED ? "OK" : "NO");
  Serial.print(" | Blynk: ");
  Serial.println(Blynk.connected() ? "OK" : "NO");

  updateDisplay();
}

void setup() {
  Serial.begin(115200);

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);

  Wire.begin(SDA_PIN, SCL_PIN);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED SSD1306 gagal diinisialisasi");
    while (true) {
      delay(10);
    }
  }

  display.clearDisplay();
  display.display();

  Serial.println("Booting ESP32...");
  Serial.println("Connecting to WiFi and Blynk...");

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  timer.setInterval(2000L, checkConnection);

  updateDisplay();
  publishStateToBlynk();
}

void loop() {
  Blynk.run();
  timer.run();
}