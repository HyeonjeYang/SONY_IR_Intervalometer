// Present_0206 & Penta0308

// Sony Alpha/NEX IR Intervalometer
// Compatible: NEX-5T, a6000/a6300/a6500, a7/a7R/a7S series (same RMT-DSLR2 code)

#include <IRremote.hpp>

#define IR_SEND_PIN 9   // Signal pin of IR transmitter module → Arduino pin 9

// ==================== Camera IR Codes ====================
// All RMT-DSLR2-compatible cameras share the same code.
const uint16_t SONY_ADDR = 0x1E3A;
const uint8_t  SONY_CMD  = 0x2D;

// ==================== Shooting Settings ==================
enum Mode { BULB, SINGLE };
Mode     shootMode    = BULB;
uint32_t bulbTime     = 11;   // Exposure time (s) — set 1s longer than target (offset correction) ~ but not necessary
uint32_t intervalTime = 5;    // Wait time after shutter close (s)
// Older cameras may need longer intervals for image processing...
int      totalShots   = 0;    // Total shots (0 = infinite)

int      shotCount    = 0;    // Cumulative shots
uint32_t nextShotTime = 0;

void triggerShutter() {
  for (int i = 0; i < 3; i++) {
    IrSender.sendSony(SONY_ADDR, SONY_CMD, 0, 20);
    delay(30);
  }
}
// Requires 3 times of signaling

void printShot() {
  Serial.print("[Shot ");
  Serial.print(shotCount);
  if (totalShots > 0) { Serial.print("/"); Serial.print(totalShots); }
  Serial.print("] ");
}

void setup() {
  IrSender.begin(IR_SEND_PIN);
  Serial.begin(9600);
  Serial.println("=== Sony IR Intervalometer ===");
  Serial.print("Mode     : "); Serial.println(shootMode == BULB ? "BULB" : "SINGLE");
  if (shootMode == BULB) {
    Serial.print("Exposure : "); Serial.print(bulbTime); Serial.println("s");
  }
  Serial.print("Interval : "); Serial.print(intervalTime); Serial.println("s");
  Serial.print("Shots    : "); Serial.println(totalShots == 0 ? "inf" : String(totalShots));
  if (shootMode == BULB) Serial.println("[!] Set camera to BULB mode");
  Serial.println("------------------------------");
  for (int i = 3; i > 0; i--) {
    Serial.print("Start in "); Serial.print(i); Serial.println("s...");
    delay(1000);
  }
  nextShotTime = millis();
}

void loop() {
  if (totalShots > 0 && shotCount >= totalShots) {
    Serial.println("=== Done! ===");
    while (true) {}
  }

  if ((int32_t)(millis() - nextShotTime) < 0) return;

  shotCount++;
  printShot();

  if (shootMode == BULB) {
    Serial.println("Shutter open");
    triggerShutter();
    for (uint32_t t = bulbTime; t > 0; t--) {
      Serial.print("  Exposing: "); Serial.print(t); Serial.println("s");
      delay(1000);
    }
    Serial.println("  Shutter close");
    triggerShutter();
  } else {
    Serial.println("Trigger");
    triggerShutter();
  }

  nextShotTime = millis() + intervalTime * 1000UL;

  // Interval countdown
  for (uint32_t t = intervalTime; t > 0; t--) {
    Serial.print("  Next shot in: "); Serial.print(t); Serial.println("s");
    delay(1000);
  }
  Serial.println();
}
