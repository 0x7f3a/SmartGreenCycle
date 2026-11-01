/*
  ==========================================================
  Smart GreenCycle - ប្រព័ន្ធដាំបន្លែវៃឆ្លាតប្រកបដោយចីរភាព
  គម្រោង STEM សិស្សថ្នាក់វិទ្យាល័យ - សាលាវិទ្យាល័យកំពង់ធំ
  ពិសោធលើការដាំត្រកួនក្នុងផើងបួនជ្រុង 60 x 40 cm
  Board: ESP32 Dev Board (ESP32-WROOM-32)
  ==========================================================
  គោលការណ៍ប្រព័ន្ធ (Water flow):

    ទឹកប្រើហើយពីផ្ទះបាយ -> ធុងច្រោះ -> ធុងស្តុកទឹកច្រោះ (មាន Sensor pH, EC)
                                              |
                                        [R4 Valve] ទឹកច្រោះ (ពេល pH/EC ល្អ)
                                              v
    ប្រព័ន្ធទឹកស្អាត --[R3 Valve]------> ធុងស្រោច (មាន Sensor Water Level)
                     (ពេលទឹកច្រោះមិនទាន់ល្អ)    |
                                     +--------+--------+
                                [R1 Pump]          [R2 Pump]
                              បាញ់តាមគល់ដំណាំ     បាញ់ចំហាយទឹក (ពេលក្តៅ)

  ==========================================================
  គ្រឿងឧបករណ៍ប្រើប្រាស់:
   - LCD 16x2 (I2C, address 0x27)
   - DHT11 (សីតុណ្ហភាព/សំណើមខ្យល់)
   - Capacitive Soil Moisture Sensor (សំណើមដី)
   - pH Sensor Module (ឧ. PH-4502C)         -> ធុងស្តុកទឹកច្រោះ
   - EC / TDS Sensor Module                 -> ធុងស្តុកទឹកច្រោះ
   - Water Level Sensor (Analog)            -> ធុងស្រោច
   - Relay 4-Channel
   - Push Button 5 គ្រាប់

  Library ត្រូវដំឡើង (Arduino IDE > Library Manager):
   1. "LiquidCrystal I2C" by Frank de Brabander
   2. "DHT sensor library" by Adafruit (និង "Adafruit Unified Sensor")
   3. "WiFiManager" by tzapu
   (WiFi.h, HTTPClient.h, WiFiClientSecure.h, Preferences.h ភ្ជាប់មកជាមួយ ESP32 core រួចហើយ)

  ចំណាំ: Arduino IDE តម្រូវឱ្យឈ្មោះ Folder ដូចឈ្មោះ .ino (SmartGreenCycle/SmartGreenCycle.ino)
         WebApp (index.html) ត្រូវ Host លើ Firebase Hosting (មិនត្រូវដាក់ក្នុង ESP32 ទៀតទេ)។

  ==========================================================
  ការតភ្ជាប់ខ្សែ (Wiring):
   - LCD 16x2:      SDA -> GPIO21, SCL -> GPIO22, VCC -> 5V, GND -> GND
   - DHT11:         DATA -> GPIO23
   - Soil Sensor:   AOUT -> GPIO34
   - pH Sensor:     Po -> [R1 20k] -> GPIO35 -> [R2 20k] -> GND
                    (Voltage Divider 1:2 ព្រោះ Po អាចឡើងដល់ 5V)
                    ដាក់ Capacitor 100nF រវាង GPIO35 និង GND
   - EC Sensor:     AOUT -> GPIO32   (!! តង់ស្យុងចេញមិនលើស 3.3V)
   - Water Level:   S    -> GPIO33
     (Sensor Analog ទាំងអស់ប្រើជើង ADC1 ព្រោះ ADC2 មិនដំណើរការពេលបើក WiFi)

   - Relay 1 (IN1) -> GPIO25 : ម៉ូទ័របាញ់តាមគល់ដំណាំ (Drip pump)
   - Relay 2 (IN2) -> GPIO26 : ម៉ូទ័របាញ់ចំហាយទឹក   (Mist pump)
   - Relay 3 (IN3) -> GPIO13 : Valve ទឹកស្អាត -> ធុងស្រោច
   - Relay 4 (IN4) -> GPIO19 : Valve ទឹកច្រោះ -> ធុងស្រោច

   - Button 1 -> GPIO16 : បើក/បិទ Relay 1   (ម្ខាងទៀតនៃប៊ូតុងភ្ជាប់ GND)
   - Button 2 -> GPIO17 : បើក/បិទ Relay 2
   - Button 3 -> GPIO4  : បើក/បិទ Relay 3
   - Button 4 -> GPIO27 : បើក/បិទ Relay 4
   - Button 5 -> GPIO18 : ប្តូរ Auto / Manual
     (ប្រើ INPUT_PULLUP ខាងក្នុង ដូច្នេះមិនចាំបាច់មាន Resistor ខាងក្រៅ)

  ==========================================================
  ការប្រើប្រាស់:
   - បើកលើកដំបូង (ឬមិនទាន់មាន WiFi ក្នុង Flash): ESP32 បង្កើត Hotspot ឈ្មោះ AP_SSID
     ភ្ជាប់ទូរស័ព្ទទៅ Hotspot នោះ ជ្រើស WiFi ហើយបញ្ចូល Password (Portal នៅ 192.168.4.1)
     ESP32 រក្សាទុកក្នុង Flash រួច Restart ហើយភ្ជាប់ WiFi នោះដោយស្វ័យប្រវត្តិ (WiFiManager)
   - ESP32 ផ្ញើទិន្នន័យ Sensor/Relay ទៅ Firebase Realtime Database
     ហើយ WebApp (Firebase Hosting) អានពី Database ដដែល និងផ្ញើពាក្យបញ្ជាត្រឡប់មកវិញ
   - Mode AUTO  : ESP32 សម្រេចចិត្តដោយខ្លួនឯងតាម Sensor
   - Mode MANUAL: បញ្ជា Relay ពីប៊ូតុង ឬ WebApp
   - ការការពារសុវត្ថិភាព ដំណើរការទាំង 2 Mode:
       * ទឹកក្នុងធុងស្រោចតិចពេក -> មិនឱ្យម៉ូទ័រ R1/R2 ដំណើរការ (ការពារម៉ូទ័ររលាក)
       * ទឹកក្នុងធុងស្រោចពេញពេក -> បិទ Valve R3/R4 (ការពារទឹកហៀរ)
  ==========================================================
*/

#include <WiFi.h>
#include <WiFiManager.h>        // tzapu/WiFiManager
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <Preferences.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include "driver/gpio.h"

// ==========================================================
// ១. ការកំណត់ជើង (Pins)
// ==========================================================
#define DHT_PIN     23
#define DHT_TYPE    DHT11
#define SOIL_PIN    34
#define PH_PIN      35
#define EC_PIN      32
#define LEVEL_PIN   33

const int RELAY_PIN[4] = {25, 26, 13, 19};
const int BTN_PIN[4]   = {16, 17, 4, 27};
#define BTN_MODE_PIN 18

// លេខរៀង Relay (index 0-3 = Relay 1-4)
enum { R_DRIP = 0, R_MIST = 1, R_CLEAN = 2, R_FILTER = 3 };
const char* RELAY_NAME[4] = {"Drip", "Mist", "Clean", "Filter"};

// ---- ប្រភេទ Relay (Polarity) ----
// true  = Active-LOW  (LOW=ON)  : Module Relay 4-Channel ធម្មតា (មាន Optocoupler, IN1-IN4)
// false = Active-HIGH (HIGH=ON) : Module ខ្លះ (ឧ. មាន Jumper H/L ដាក់ទៅ H)
// បើ WebApp បង្ហាញ ON តែ Relay បិទ (ឬផ្ទុយមកវិញ) សូមប្តូរតម្លៃនេះ។
// true  = Active-LOW  (LOW energizes the relay) - typical opto-isolated 4-ch modules.
// false = Active-HIGH (HIGH energizes the relay). Flip this if ON/OFF is reversed.
// Only the physical pin level changes; relayOn[] and /status stay true = ON.
const bool RELAY_ACTIVE_LOW = true;

// ==========================================================
// ២. WiFi
// ==========================================================
// Hotspot សម្រាប់ Setup WiFi (បង្ហាញតែពេលមិនទាន់មាន WiFi ក្នុង Flash ឬភ្ជាប់មិនបាន)
const char* AP_SSID     = "GreenCycle-KTHS";
const char* AP_PASSWORD = "kths12345";      // យ៉ាងតិច 8 តួអក្សរ
const unsigned long PORTAL_TIMEOUT_S = 180; // Portal បិទបន្ទាប់ពី 3 នាទី ប្រសិនបើគ្មាននរណាកំណត់

// Firebase Realtime Database
//   greencycle/status          <- ESP32 សរសេរ (Sensor, Relay, ...)
//   greencycle/control         -> WebApp សរសេរ (mode, r1..r4, reset)
//   greencycle/control/config  -> ការកំណត់ Auto (ESP32 រក្សាទុកក្នុង Flash)
const char* FB_URL = "https://smartgreencycle-49d20-default-rtdb.asia-southeast1.firebasedatabase.app";
const unsigned long FB_CYCLE_MS  = 1000;   // អានពាក្យបញ្ជាពី Firebase រៀងរាល់ 1s
const unsigned long FB_STATUS_MS = 2000;   // ផ្ញើ Status ទៅ Firebase រៀងរាល់ 2s

// ==========================================================
// ៣. Calibration Sensor (ត្រូវ Calibrate ជាមួយ Sensor ពិត)
// ==========================================================
// ---- pH ----
// Po របស់ Module ឆ្លងកាត់ Divider (R1=20k, R2=20k) មុនចូល GPIO35
// ដូច្នេះ ESP32 ឃើញតែពាក់កណ្តាលនៃ Po។ កូដគុណ 2 វិញ ដើម្បីបានតង់ស្យុង Po ពិត។
// ប្រសិនបើអ្នកដកចេញ Divider សូមដាក់ PH_DIVIDER = 1.0
const float PH_DIVIDER  = 2.0;

// Po (Volt) ពេល pH = 7 -> វាស់ដោយ Short BNC (ដាក់ខ្សែទង់ដែងប៉ះទាំងចំណុចកណ្តាល និងចិញ្ចៀនក្រៅ)
// ក្នុងការតំឡើងពិត (12V -> LM2596 -> 5V, ដកUSB)។ តម្លៃដែលបានវាស់ = 1.04 V
// pH = 7 - (Po - PH_V_AT_7) / PH_V_PER_PH
const float PH_V_AT_7   = 1.04;
// Volt ប្រែប្រួលក្នុង 1 pH (PH-4502C ធម្មតា ~0.18)
// Calibrate ពិតប្រាកដ: PH_V_PER_PH = (Po_pH4 - Po_pH7) / 3.0 (ប្រើ Buffer pH 4 និង pH 7)
const float PH_V_PER_PH = 0.18;

// ជួរ Po ដែលចាត់ទុកថា Probe/Module ដំណើរការ។ ក្រៅជួរនេះ -> "pH ERR"
const float PH_V_MIN_OK = 0.30;
const float PH_V_MAX_OK = 4.50;

// EC: EC (mS/cm) = V * EC_FACTOR  (ត្រូវ Calibrate ជាមួយ Solution ស្គាល់តម្លៃ៖ EC_FACTOR = EC_ស្គាល់ / V)
const float EC_FACTOR = 3.58;

// Soil: តម្លៃ raw ពេលដីស្ងួតខ្លាំង (ក្នុងខ្យល់) និងពេលសើមខ្លាំង (ក្នុងទឹក)
const int SOIL_RAW_DRY = 3000;
const int SOIL_RAW_WET = 1200;

// Water Level: តម្លៃ mV ពេលធុងទទេ និងពេលធុងពេញ (មើល "Level mV" លើ WebApp)
const int LEVEL_MV_EMPTY = 100;
const int LEVEL_MV_FULL  = 2000;

// ==========================================================
// ៤. ការកំណត់ Auto (អាចកែពី WebApp ហើយរក្សាទុកក្នុង Flash)
// ==========================================================
struct Settings {
  float phMin     = 6.0;   // ទឹកច្រោះអាចស្រោចបាន ពេល pH ក្នុងចន្លោះ phMin..phMax
  float phMax     = 7.5;
  float ecMax     = 2.0;   // និង EC មិនលើស ecMax (mS/cm) - ទឹកមានជាតិប្រៃ/សាប៊ូច្រើនពេក
  int   soilLow   = 40;    // សំណើមដី < soilLow  -> ចាប់ផ្តើមស្រោច (R1)
  int   soilHigh  = 65;    // សំណើមដី >= soilHigh -> ឈប់ស្រោច
  float tempHigh  = 33.0;  // សីតុណ្ហភាព > tempHigh -> បាញ់ចំហាយ (R2)
  int   humLow    = 50;    // ឬ សំណើមខ្យល់ < humLow -> បាញ់ចំហាយ
  int   levelLow  = 30;    // ធុងស្រោច < levelLow  -> បញ្ចូលទឹក (R3 ឬ R4)
  int   levelHigh = 85;    // ធុងស្រោច >= levelHigh -> ឈប់បញ្ចូលទឹក
} cfg;

// ---- តម្លៃថេរ (មិនបង្ហាញលើ WebApp) ----
const int  LEVEL_PUMP_MIN  = 10;   // ទឹកតិចជាងនេះ -> ហាមម៉ូទ័រ R1/R2 (ការពាររលាក)
const int  LEVEL_OVERFLOW  = 95;   // ទឹកលើសនេះ  -> បិទ Valve R3/R4 (ការពារហៀរ)
const bool ENABLE_SAFETY   = true; // false = បិទការការពារ (សម្រាប់តេស្តតែប៉ុណ្ណោះ)

const unsigned long DRIP_ON_MS      = 20000;   // ស្រោច 20s
const unsigned long DRIP_SOAK_MS    = 60000;   // ឈប់ 60s ឱ្យទឹកជ្រាបចូលដី រួចវាស់ម្តងទៀត
const unsigned long R1_MAX_ON_MS    = 120000;  // R1 (ម៉ូទ័រ) ដំណើរការជាប់គ្នាបានអតិបរមា 120s (ទាំង Auto និង Manual)
const unsigned long MIST_ON_MS      = 10000;   // បាញ់ចំហាយ 10s
const unsigned long MIST_OFF_MS     = 50000;   // ឈប់ 50s
const unsigned long FILL_TIMEOUT_MS = 10UL * 60 * 1000; // បញ្ចូលទឹកយូរជាង 10 នាទី -> មានបញ្ហា
const unsigned long WARMUP_MS       = 30000;   // រង់ចាំ Sensor pH/EC ស្ងប់ 30s ក្រោយបើក

const unsigned long SENSOR_INTERVAL = 2000;
const unsigned long LCD_INTERVAL    = 500;
const unsigned long LCD_PAGE_MS     = 3000;

// ==========================================================
// Objects និង អថេរ
// ==========================================================
LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(DHT_PIN, DHT_TYPE);
Preferences prefs;

// ---- តម្លៃ Sensor ----
float temperature = 0, humidity = 0;
bool  dhtOk = false;
unsigned long lastDhtOk = 0;
int   soilPercent = 0;
float phValue = 7.0, ecValue = 0.0;
float phVolt = 0;               // តង់ស្យុង Po ពិត (បន្ទាប់ពីគុណ Divider)
bool  phProbeOk = false;        // pH Module/Probe ដំណើរការ?
bool  phInit = false, ecInit = false;
int   levelPercent = 0;
int   phMv = 0, ecMv = 0, levelMv = 0;   // phMv = mV ត្រង់ GPIO35 (បន្ទាប់ Divider)
bool  waterOK = false;          // ទឹកច្រោះអាចស្រោចបាន?

// ---- ស្ថានភាព Relay ----
bool autoMode = true;
bool manualState[4] = {false, false, false, false};
bool relayOn[4]     = {false, false, false, false};
bool blocked[4]     = {false, false, false, false};

// ---- ការបញ្ចូលទឹកក្នុងធុងស្រោច ----
enum FillSource : uint8_t { SRC_NONE = 0, SRC_FILTERED = 1, SRC_CLEAN = 2 };
FillSource fillSrc = SRC_NONE;
unsigned long fillStart = 0;
bool fillFault = false;         // បញ្ចូលទឹកយូរពេក តែកម្រិតទឹកមិនឡើង

// ---- Pulse (បើក-ឈប់-បើក) សម្រាប់ម៉ូទ័រ ----
struct Pulser { bool on = false; unsigned long t0 = 0; bool started = false; };
Pulser dripPulse, mistPulse;
bool needWater = false;         // ដីស្ងួត (hysteresis)
unsigned long dripOnSince = 0;  // R1 ចាប់ផ្តើមដំណើរការនៅពេលណា
bool dripLimit = false;         // R1 ដំណើរការគ្រប់ R1_MAX_ON_MS -> បិទ រហូតដល់មានការបញ្ជាថ្មី
bool tooHot = false;            // ក្តៅ/ស្ងួត (hysteresis)

// ---- ប៊ូតុង ----
struct Button { int pin; bool stable; bool lastRead; unsigned long t; };
Button btns[5];

// ---- LCD ----
unsigned long lastSensor = 0, lastLcd = 0, lastPage = 0, bootMillis = 0;
int lcdPage = 0;
char lcdMsg[17] = "";
unsigned long lcdMsgUntil = 0;

// ==========================================================
// Helpers
// ==========================================================
int relaySignal(bool on) {
  if (RELAY_ACTIVE_LOW) return on ? LOW : HIGH;
  return on ? HIGH : LOW;
}

// The only place that writes a relay pin (applies RELAY_ACTIVE_LOW).
void writeRelay(int i, bool on) {
  digitalWrite(RELAY_PIN[i], relaySignal(on));
}

// Preload the OFF level before enabling the output so the relay doesn't click on at boot.
void initRelayPin(int i) {
  gpio_set_level((gpio_num_t)RELAY_PIN[i], relaySignal(false));
  pinMode(RELAY_PIN[i], OUTPUT);
  writeRelay(i, false);
}

// អាន ADC 15 ដង ហើយយកតម្លៃកណ្តាល (Median) ដើម្បីកាត់បន្ថយ Noise
int readMedian(int pin, bool milliVolts) {
  const int N = 15;
  int v[N];
  for (int i = 0; i < N; i++) {
    v[i] = milliVolts ? analogReadMilliVolts(pin) : analogRead(pin);
    delayMicroseconds(300);
  }
  for (int i = 1; i < N; i++) {          // insertion sort
    int x = v[i], j = i - 1;
    while (j >= 0 && v[j] > x) { v[j + 1] = v[j]; j--; }
    v[j + 1] = x;
  }
  return v[N / 2];
}

// Pulse: active=true -> ON onMs, OFF offMs, ON ... ; active=false -> OFF
bool runPulse(Pulser &p, bool active, unsigned long onMs, unsigned long offMs, unsigned long now) {
  if (p.on) {
    if (!active || now - p.t0 >= onMs) { p.on = false; p.t0 = now; }
  } else if (active && (!p.started || now - p.t0 >= offMs)) {
    p.on = true; p.t0 = now; p.started = true;
  }
  return p.on;
}

void showMessage(const char* msg) {
  snprintf(lcdMsg, sizeof(lcdMsg), "%-16s", msg);
  lcdMsgUntil = millis() + 1500;
}

// ==========================================================
// SETUP
// ==========================================================
void setup() {
  Serial.begin(115200);

  for (int i = 0; i < 4; i++) initRelayPin(i);

  for (int i = 0; i < 4; i++) btns[i] = {BTN_PIN[i], HIGH, HIGH, 0};
  btns[4] = {BTN_MODE_PIN, HIGH, HIGH, 0};
  for (int i = 0; i < 5; i++) pinMode(btns[i].pin, INPUT_PULLUP);

  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);   // អានបាន 0 - ~3.1V

  dht.begin();
  loadSettings();

  Wire.begin(21, 22);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0); lcd.print("Smart GreenCycle");
  lcd.setCursor(0, 1); lcd.print("Booting...      ");
  delay(800);

  setupWiFi();
  startFirebaseTask();

  lcd.clear();
  bootMillis = millis();
}

// ==========================================================
// LOOP
// ==========================================================
void loop() {
  unsigned long now = millis();

  handleButtons();

  if (now - lastSensor >= SENSOR_INTERVAL) {
    lastSensor = now;
    readSensors();
    logSerial();
  }

  updateRelays();

  if (now - lastPage >= LCD_PAGE_MS) { lastPage = now; lcdPage = (lcdPage + 1) % 3; }
  if (now - lastLcd >= LCD_INTERVAL) { lastLcd = now; updateDisplay(); }
}

// ==========================================================
// Sensors
// ==========================================================
void readSensors() {
  // ---- DHT11 ----
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  if (!isnan(t) && !isnan(h)) {
    temperature = t; humidity = h;
    lastDhtOk = millis();
  }
  dhtOk = (lastDhtOk != 0) && (millis() - lastDhtOk < 10000);

  // ---- Soil ----
  int raw = readMedian(SOIL_PIN, false);
  soilPercent = constrain(map(raw, SOIL_RAW_DRY, SOIL_RAW_WET, 0, 100), 0, 100);

  // ---- pH (ធុងស្តុកទឹកច្រោះ) ----
  phMv   = readMedian(PH_PIN, true);                 // mV ត្រង់ GPIO35 (បន្ទាប់ Divider)
  phVolt = (phMv / 1000.0f) * PH_DIVIDER;            // តង់ស្យុង Po ពិត (V)
  phProbeOk = (phVolt > PH_V_MIN_OK && phVolt < PH_V_MAX_OK);
  if (phProbeOk) {
    float ph = 7.0f - ((phVolt - PH_V_AT_7) / PH_V_PER_PH);
    ph = constrain(ph, 0.0f, 14.0f);
    if (!phInit) { phValue = ph; phInit = true; }
    else         { phValue = phValue * 0.7f + ph * 0.3f; }   // Smoothing
  }

  // ---- EC (ធុងស្តុកទឹកច្រោះ) ----
  ecMv = readMedian(EC_PIN, true);
  float ec = constrain((ecMv / 1000.0f) * EC_FACTOR, 0.0f, 5.0f);
  if (!ecInit) { ecValue = ec; ecInit = true; }
  else         { ecValue = ecValue * 0.7f + ec * 0.3f; }

  // ---- Water Level (ធុងស្រោច) ----
  levelMv = readMedian(LEVEL_PIN, true);
  levelPercent = constrain(map(levelMv, LEVEL_MV_EMPTY, LEVEL_MV_FULL, 0, 100), 0, 100);

  // ---- គុណភាពទឹកច្រោះ ----
  // Probe មិនដំណើរការ ឬ pH ក្រៅ 2..12 -> ចាត់ទុកថាមិនល្អ (ប្រើទឹកស្អាតជំនួស)
  bool phPlausible = phProbeOk && phValue > 2.0f && phValue < 12.0f;
  waterOK = phPlausible && phValue >= cfg.phMin && phValue <= cfg.phMax && ecValue <= cfg.ecMax;
}

// ==========================================================
// Relay logic
// ==========================================================
void runAuto(unsigned long now, bool want[4]) {
  for (int i = 0; i < 4; i++) want[i] = false;
  if (now - bootMillis < WARMUP_MS) return;   // រង់ចាំ Sensor ស្ងប់

  // ---- 1) បញ្ចូលទឹកក្នុងធុងស្រោច (R3 / R4) ----
  if (fillFault && levelPercent >= cfg.levelHigh) fillFault = false;   // មាននរណាម្នាក់បញ្ចូលទឹករួច

  if (fillSrc == SRC_NONE) {
    if (!fillFault && levelPercent < cfg.levelLow) {
      // សម្រេចប្រភពទឹកនៅពេលចាប់ផ្តើម៖ ទឹកច្រោះជាអាទិភាព (សន្សំទឹក)
      fillSrc = waterOK ? SRC_FILTERED : SRC_CLEAN;
      fillStart = now;
    }
  } else if (levelPercent >= cfg.levelHigh) {
    fillSrc = SRC_NONE;                          // ពេញហើយ
  } else if (fillSrc == SRC_FILTERED && !waterOK) {
    fillSrc = SRC_CLEAN; fillStart = now;        // ទឹកច្រោះប្រែជាមិនល្អពាក់កណ្តាលទី
  } else if (now - fillStart > FILL_TIMEOUT_MS) {
    if (fillSrc == SRC_FILTERED) {
      fillSrc = SRC_CLEAN; fillStart = now;      // ប្រហែលធុងស្តុកទឹកច្រោះអស់ទឹក -> ប្តូរទៅទឹកស្អាត
    } else {
      fillSrc = SRC_NONE; fillFault = true;      // ទឹកស្អាតក៏មិនចូល -> មានបញ្ហា
    }
  }
  want[R_FILTER] = (fillSrc == SRC_FILTERED);
  want[R_CLEAN]  = (fillSrc == SRC_CLEAN);

  // ---- 2) ស្រោចតាមគល់ (R1) ----
  if (soilPercent < cfg.soilLow) needWater = true;
  else if (soilPercent >= cfg.soilHigh) needWater = false;
  want[R_DRIP] = runPulse(dripPulse, needWater, DRIP_ON_MS, DRIP_SOAK_MS, now);

  // ---- 3) បាញ់ចំហាយពេលក្តៅ (R2) ----
  if (!dhtOk) tooHot = false;
  else if (temperature > cfg.tempHigh || humidity < cfg.humLow) tooHot = true;
  else if (temperature < cfg.tempHigh - 1.5f && humidity > cfg.humLow + 5) tooHot = false;
  want[R_MIST] = runPulse(mistPulse, tooHot, MIST_ON_MS, MIST_OFF_MS, now);
}

void updateRelays() {
  unsigned long now = millis();
  bool want[4];

  if (autoMode) runAuto(now, want);
  else for (int i = 0; i < 4; i++) want[i] = manualState[i];

  // R1 motor: never run longer than R1_MAX_ON_MS in a row, whatever the sensors or the mode say.
  // It stays off until the request ends (Auto: soil/pulse, Manual: switch OFF) and a new one starts.
  if (!want[R_DRIP]) {
    dripLimit = false;
  } else if (relayOn[R_DRIP] && now - dripOnSince >= R1_MAX_ON_MS) {
    dripLimit = true;
    if (!autoMode) manualState[R_DRIP] = false;
    showMessage("R1 120s limit");
    Serial.println("R1 reached max run time -> OFF");
  }
  if (dripLimit) want[R_DRIP] = false;

  bool pumpBlock  = ENABLE_SAFETY && levelPercent < LEVEL_PUMP_MIN;
  bool valveBlock = ENABLE_SAFETY && levelPercent >= LEVEL_OVERFLOW;
  blocked[R_DRIP] = blocked[R_MIST] = pumpBlock;
  blocked[R_CLEAN] = blocked[R_FILTER] = valveBlock;

  for (int i = 0; i < 4; i++) {
    bool on = want[i] && !blocked[i];
    if (on != relayOn[i]) {
      relayOn[i] = on;
      if (i == R_DRIP && on) dripOnSince = now;
      writeRelay(i, on);
      Serial.printf("Relay %d (%s) -> %s\n", i + 1, RELAY_NAME[i], on ? "ON" : "OFF");
    }
  }
}

void setMode(bool toAuto) {
  if (toAuto == autoMode) return;
  autoMode = toAuto;
  if (!autoMode) {
    // ចូល Manual: ចាប់ផ្តើមពីស្ថានភាពបច្ចុប្បន្ន ដើម្បីកុំឱ្យ Relay ប្តូរភ្លាមៗ
    for (int i = 0; i < 4; i++) manualState[i] = relayOn[i];
  } else {
    fillSrc = SRC_NONE; fillFault = false;
    dripPulse = Pulser(); mistPulse = Pulser();
  }
  showMessage(autoMode ? "Mode: AUTO" : "Mode: MANUAL");
  Serial.println(autoMode ? "Mode -> AUTO" : "Mode -> MANUAL");
}

// ==========================================================
// ប៊ូតុង (Debounce 30ms, ចុចទៅ GND)
// ==========================================================
bool buttonPressed(Button &b) {
  bool r = digitalRead(b.pin);
  if (r != b.lastRead) { b.lastRead = r; b.t = millis(); }
  if (millis() - b.t > 30 && r != b.stable) {
    b.stable = r;
    if (r == LOW) return true;
  }
  return false;
}

void handleButtons() {
  if (buttonPressed(btns[4])) setMode(!autoMode);

  for (int i = 0; i < 4; i++) {
    if (!buttonPressed(btns[i])) continue;
    if (autoMode) {
      showMessage("Press MODE first");
      continue;
    }
    manualState[i] = !manualState[i];
    char msg[17];
    snprintf(msg, sizeof(msg), "R%d %s %s", i + 1, RELAY_NAME[i], manualState[i] ? "ON" : "OFF");
    showMessage(msg);
  }
}

// ==========================================================
// LCD 16x2 (ប្តូរទំព័ររៀងរាល់ 3 វិនាទី)
// ==========================================================
void lcdLine(int row, const char* text) {
  char buf[17];
  snprintf(buf, sizeof(buf), "%-16s", text);
  lcd.setCursor(0, row);
  lcd.print(buf);
}

void updateDisplay() {
  char l0[17], l1[17];
  const char* modeTxt = autoMode ? "AUTO" : "MAN";

  if (millis() < lcdMsgUntil) {
    lcdLine(0, lcdMsg);
    snprintf(l1, sizeof(l1), "Mode: %s", autoMode ? "AUTO" : "MANUAL");
    lcdLine(1, l1);
    return;
  }

  if (lcdPage == 0) {
    if (dhtOk) snprintf(l0, sizeof(l0), "T:%.1fC H:%d%%", temperature, (int)humidity);
    else       snprintf(l0, sizeof(l0), "DHT11 error");
    snprintf(l1, sizeof(l1), "Soil:%d%%  %s", soilPercent, modeTxt);
  } else if (lcdPage == 1) {
    if (phProbeOk) snprintf(l0, sizeof(l0), "pH:%.2f EC:%.2f", phValue, ecValue);
    else           snprintf(l0, sizeof(l0), "pH:ERR EC:%.2f", ecValue);
    if (fillFault) snprintf(l1, sizeof(l1), "Tank:%d%% FAULT!", levelPercent);
    else           snprintf(l1, sizeof(l1), "Tank:%d%% %s", levelPercent, waterOK ? "Wtr:OK" : "Wtr:BAD");
  } else {
    snprintf(l0, sizeof(l0), "R1:%-3s R2:%-3s", relayOn[0] ? "ON" : "OFF", relayOn[1] ? "ON" : "OFF");
    snprintf(l1, sizeof(l1), "R3:%-3s R4:%-3s %c", relayOn[2] ? "ON" : "OFF", relayOn[3] ? "ON" : "OFF", autoMode ? 'A' : 'M');
  }
  lcdLine(0, l0);
  lcdLine(1, l1);
}

void logSerial() {
  Serial.printf("T=%.1f H=%.0f Soil=%d pH=%.2f(gpio %dmV, Po %.2fV, %s) EC=%.2f(%dmV) Lv=%d%%(%dmV) WaterOK=%d Src=%d R=%d%d%d%d %s\n",
                temperature, humidity, soilPercent, phValue, phMv, phVolt, phProbeOk ? "ok" : "ERR",
                ecValue, ecMv, levelPercent, levelMv, waterOK, fillSrc,
                relayOn[0], relayOn[1], relayOn[2], relayOn[3], autoMode ? "AUTO" : "MANUAL");
}

// ==========================================================
// Settings (រក្សាទុកក្នុង Flash ទោះបិទភ្លើងក៏មិនបាត់)
// ==========================================================
void loadSettings() {
  Settings d;
  prefs.begin("greencycle", true);
  cfg.phMin     = prefs.getFloat("phMin", d.phMin);
  cfg.phMax     = prefs.getFloat("phMax", d.phMax);
  cfg.ecMax     = prefs.getFloat("ecMax", d.ecMax);
  cfg.soilLow   = prefs.getInt("soilLow", d.soilLow);
  cfg.soilHigh  = prefs.getInt("soilHigh", d.soilHigh);
  cfg.tempHigh  = prefs.getFloat("tempHigh", d.tempHigh);
  cfg.humLow    = prefs.getInt("humLow", d.humLow);
  cfg.levelLow  = prefs.getInt("levelLow", d.levelLow);
  cfg.levelHigh = prefs.getInt("levelHigh", d.levelHigh);
  prefs.end();
}

void saveSettings() {
  prefs.begin("greencycle", false);
  prefs.putFloat("phMin", cfg.phMin);
  prefs.putFloat("phMax", cfg.phMax);
  prefs.putFloat("ecMax", cfg.ecMax);
  prefs.putInt("soilLow", cfg.soilLow);
  prefs.putInt("soilHigh", cfg.soilHigh);
  prefs.putFloat("tempHigh", cfg.tempHigh);
  prefs.putInt("humLow", cfg.humLow);
  prefs.putInt("levelLow", cfg.levelLow);
  prefs.putInt("levelHigh", cfg.levelHigh);
  prefs.end();
}

// ==========================================================
// WiFi (WiFiManager)
// ==========================================================
bool portalSaved = false;

void onPortalStart(WiFiManager* wm) {
  Serial.print("Setup hotspot: "); Serial.print(AP_SSID);
  Serial.print("  IP: "); Serial.println(WiFi.softAPIP());
  lcd.clear();
  lcdLine(0, "WiFi Setup:");
  lcdLine(1, AP_SSID);
}

void onPortalSaved() { portalSaved = true; }

void setupWiFi() {
  WiFi.mode(WIFI_STA);
  lcd.clear();
  lcdLine(0, "WiFi connecting");

  WiFiManager wm;
  wm.setConfigPortalTimeout(PORTAL_TIMEOUT_S);
  wm.setAPCallback(onPortalStart);
  wm.setSaveConfigCallback(onPortalSaved);

  // Saved WiFi found in flash -> connects by itself.
  // Nothing saved (or it fails) -> starts the AP_SSID hotspot so the user can enter SSID + password.
  bool ok = wm.autoConnect(AP_SSID, AP_PASSWORD);

  if (ok && portalSaved) {
    // WiFi was just entered in the hotspot page and saved to flash -> reboot and autoconnect
    lcd.clear();
    lcdLine(0, "WiFi saved");
    lcdLine(1, "Restarting...");
    delay(1500);
    ESP.restart();
  }

  if (ok) {
    Serial.print("WiFi: "); Serial.print(WiFi.SSID());
    Serial.print("  IP: "); Serial.println(WiFi.localIP());
    lcd.clear();
    lcdLine(0, "WiFi connected");
    lcdLine(1, WiFi.SSID().c_str());
  } else {
    WiFi.mode(WIFI_STA);
    if (WiFi.SSID().length() == 0) ESP.restart();   // nothing saved yet -> open the setup hotspot again
    WiFi.begin();                                   // saved WiFi is only unreachable: keep running and keep retrying
    lcd.clear();
    lcdLine(0, "No WiFi");
    lcdLine(1, "Retrying...");
  }
  delay(2000);
}

// ==========================================================
// Firebase Realtime Database (REST) - runs in its own task so the
// buttons / relays / LCD in loop() never wait for the network
// ==========================================================
WiFiClientSecure fbClient;
HTTPClient fbHttp;
String lastSeq[6];                 // last handled command: mode, r1, r2, r3, r4, reset
bool   seqInit   = false;
bool   cfgSeeded = false;
bool   pushNow   = false;          // send Status again right after a command

// Finds  "key":value  in the flat JSON Firebase returns. Strings are returned without quotes.
bool jsonFind(const String& body, const char* key, String& out) {
  String k = String("\"") + key + "\":";
  int i = body.indexOf(k);
  if (i < 0) return false;
  i += k.length();
  if (i >= (int)body.length()) return false;
  if (body[i] == '"') {
    i++;
    int e = body.indexOf('"', i);
    if (e < 0) return false;
    out = body.substring(i, e);
    return true;
  }
  int e = i;
  while (e < (int)body.length() && body[e] != ',' && body[e] != '}') e++;
  out = body.substring(i, e);
  return true;
}

bool fbPut(const char* path, const String& body) {
  fbHttp.begin(fbClient, String(FB_URL) + path + ".json?print=silent");
  fbHttp.addHeader("Content-Type", "application/json");
  int code = fbHttp.PUT(body);
  fbHttp.end();
  return code == 200 || code == 204;
}

bool fbGet(const char* path, String& out) {
  fbHttp.begin(fbClient, String(FB_URL) + path + ".json");
  int code = fbHttp.GET();
  if (code == 200) out = fbHttp.getString();
  fbHttp.end();
  return code == 200;
}

const char* validateSettings(const Settings& n) {
  if (n.phMin < 0 || n.phMax > 14 || n.phMin >= n.phMax)           return "pH min ត្រូវតូចជាង pH max (0-14)";
  if (n.ecMax <= 0 || n.ecMax > 5)                                  return "EC max ត្រូវនៅចន្លោះ 0-5";
  if (n.soilLow < 0 || n.soilHigh > 100 || n.soilLow >= n.soilHigh) return "សំណើមដី ចាប់ផ្តើម ត្រូវតូចជាង ឈប់";
  if (n.tempHigh < 20 || n.tempHigh > 50)                           return "សីតុណ្ហភាពត្រូវនៅចន្លោះ 20-50°C";
  if (n.humLow < 0 || n.humLow > 100)                               return "សំណើមខ្យល់ត្រូវនៅចន្លោះ 0-100%";
  if (n.levelLow <= LEVEL_PUMP_MIN || n.levelHigh >= LEVEL_OVERFLOW || n.levelLow >= n.levelHigh)
                                                                    return "កម្រិតទឹក៖ 10 < ចាប់ផ្តើម < ឈប់ < 95";
  return nullptr;
}

bool sameSettings(const Settings& a, const Settings& b) {
  return fabsf(a.phMin - b.phMin) < 0.001f && fabsf(a.phMax - b.phMax) < 0.001f &&
         fabsf(a.ecMax - b.ecMax) < 0.001f && fabsf(a.tempHigh - b.tempHigh) < 0.001f &&
         a.soilLow == b.soilLow && a.soilHigh == b.soilHigh && a.humLow == b.humLow &&
         a.levelLow == b.levelLow && a.levelHigh == b.levelHigh;
}

// Handles what the WebApp wrote to greencycle/control (same actions the old /mode /relay /reset /save did)
void applyControl(const String& body) {
  const char* keys[6] = {"mode", "r1", "r2", "r3", "r4", "reset"};
  String seq[6];
  bool   has[6];
  for (int i = 0; i < 6; i++) has[i] = jsonFind(body, (String(keys[i]) + "Seq").c_str(), seq[i]);

  if (!seqInit) {
    // Commands that were already in Firebase before this boot are old -> do not run them
    for (int i = 0; i < 6; i++) lastSeq[i] = has[i] ? seq[i] : "";
    seqInit = true;
  } else {
    bool changed[6];
    for (int i = 0; i < 6; i++) {
      changed[i] = has[i] && seq[i] != lastSeq[i];
      if (changed[i]) lastSeq[i] = seq[i];
    }
    String v;
    if (changed[0] && jsonFind(body, "mode", v)) {
      if (v == "auto") setMode(true);
      else if (v == "manual") setMode(false);
      pushNow = true;
    }
    for (int r = 0; r < 4; r++) {
      if (changed[1 + r] && jsonFind(body, keys[1 + r], v)) {
        if (!autoMode) manualState[r] = (v == "1");   // Relay can only be controlled in Manual
        pushNow = true;
      }
    }
    if (changed[5]) {
      fillFault = false;
      fillSrc = SRC_NONE;
      pushNow = true;
    }
  }

  // ---- Auto settings ----
  Settings n = cfg;
  bool all = true;
  String v;
  auto F = [&](const char* k, float& d) { if (jsonFind(body, k, v)) d = v.toFloat(); else all = false; };
  auto I = [&](const char* k, int& d)   { if (jsonFind(body, k, v)) d = v.toInt();   else all = false; };
  F("phMin", n.phMin);   F("phMax", n.phMax);   F("ecMax", n.ecMax);
  I("soilLow", n.soilLow); I("soilHigh", n.soilHigh);
  F("tempHigh", n.tempHigh); I("humLow", n.humLow);
  I("levelLow", n.levelLow); I("levelHigh", n.levelHigh);

  if (all && !sameSettings(n, cfg)) {
    const char* err = validateSettings(n);
    if (err) {
      Serial.printf("Settings rejected: %s\n", err);
      fbPut("/greencycle/control/config", configJson());   // put the WebApp back to the real values
    } else {
      cfg = n;
      saveSettings();
      showMessage("Settings saved");
      Serial.println("Settings saved");
    }
  }
}

void firebaseSync() {
  static unsigned long lastPut = 0;
  unsigned long now = millis();

  // Publish the settings stored in flash once after every boot, so the WebApp shows the real values
  if (!cfgSeeded) {
    if (!fbPut("/greencycle/control/config", configJson())) return;
    cfgSeeded = true;
  }

  if (pushNow || now - lastPut >= FB_STATUS_MS) {
    if (fbPut("/greencycle/status", statusJson())) { lastPut = now; pushNow = false; }
  }

  String body;
  if (fbGet("/greencycle/control", body)) applyControl(body);
}

void firebaseTask(void*) {
  fbClient.setInsecure();          // no certificate check (simple); data is still encrypted
  fbHttp.setReuse(true);           // keep the TLS connection open between requests
  fbHttp.setTimeout(5000);
  for (;;) {
    if (WiFi.status() == WL_CONNECTED) firebaseSync();
    vTaskDelay(FB_CYCLE_MS / portTICK_PERIOD_MS);
  }
}

void startFirebaseTask() {
  xTaskCreatePinnedToCore(firebaseTask, "firebase", 16384, NULL, 1, NULL, 0);
}

String uptimeString() {
  unsigned long s = (millis() - bootMillis) / 1000;
  char buf[12];
  snprintf(buf, sizeof(buf), "%02lu:%02lu:%02lu", s / 3600, (s / 60) % 60, s % 60);
  return String(buf);
}

String statusJson() {
  unsigned long up = millis() - bootMillis;
  int warm = (autoMode && up < WARMUP_MS) ? (int)((WARMUP_MS - up) / 1000) + 1 : 0;
  char buf[560];
  snprintf(buf, sizeof(buf),
    "{\"t\":%.1f,\"h\":%.0f,\"dht\":%s,\"soil\":%d,\"ph\":%.2f,\"phok\":%s,\"ec\":%.2f,\"lvl\":%d,"
    "\"wq\":%s,\"src\":%d,\"auto\":%s,\"warm\":%d,\"fault\":%s,"
    "\"r\":[%d,%d,%d,%d],\"blk\":[%d,%d,%d,%d],\"mv\":[%d,%d,%d],\"up\":\"%s\"}",
    temperature, humidity, dhtOk ? "true" : "false", soilPercent, phValue,
    phProbeOk ? "true" : "false", ecValue, levelPercent,
    waterOK ? "true" : "false", (int)fillSrc, autoMode ? "true" : "false", warm, fillFault ? "true" : "false",
    relayOn[0], relayOn[1], relayOn[2], relayOn[3],
    blocked[0], blocked[1], blocked[2], blocked[3],
    phMv, ecMv, levelMv, uptimeString().c_str());
  return String(buf);
}

String configJson() {
  char buf[256];
  snprintf(buf, sizeof(buf),
    "{\"phMin\":%.1f,\"phMax\":%.1f,\"ecMax\":%.2f,\"soilLow\":%d,\"soilHigh\":%d,"
    "\"tempHigh\":%.1f,\"humLow\":%d,\"levelLow\":%d,\"levelHigh\":%d}",
    cfg.phMin, cfg.phMax, cfg.ecMax, cfg.soilLow, cfg.soilHigh,
    cfg.tempHigh, cfg.humLow, cfg.levelLow, cfg.levelHigh);
  return String(buf);
}
