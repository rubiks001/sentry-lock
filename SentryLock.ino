#include <Servo.h>

// ── Pin definitions ──────────────────────────────────────────
const int TRIG_PIN   = 10;
const int ECHO_PIN   = 11;
const int BUZZER_PIN = 3;
const int SERVO_PIN  = 9;

// ── Tunable parameters ───────────────────────────────────────
const int THRESHOLD_CM = 30;   // detection distance in cm
const int STEP_DEGREES = 2;    // degrees per sweep step
const int STEP_DELAY   = 20;   // ms between each step

// ── State ────────────────────────────────────────────────────
Servo servo;
int currentAngle = 0;
int sweepDir     = 1;   // +1 = sweeping up, -1 = sweeping down

// ────────────────────────────────────────────────────────────
void setup() {
  Serial.begin(9600);
  pinMode(TRIG_PIN,   OUTPUT);
  pinMode(ECHO_PIN,   INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  servo.attach(SERVO_PIN);
  servo.write(currentAngle);
  delay(500);

  Serial.println("Radar Guard ready. Sweeping...");
}

// ────────────────────────────────────────────────────────────
void loop() {
  long dist = measureDistance();

  Serial.print("Angle: "); Serial.print(currentAngle);
  Serial.print("  |  Dist: "); Serial.print(dist);
  Serial.println(" cm");

  if (dist > 0 && dist <= THRESHOLD_CM) {
    // ── OBJECT PRESENT: freeze servo, beep ──────────────────
    Serial.print(">>> OBJECT at ");
    Serial.print(currentAngle); Serial.print("°, ");
    Serial.print(dist); Serial.println(" cm — FROZEN");

    beep(BUZZER_PIN, 1000, STEP_DELAY);

  } else {
    // ── CLEAR: silence, advance sweep ───────────────────────
    digitalWrite(BUZZER_PIN, LOW);

    currentAngle += sweepDir * STEP_DEGREES;

    if (currentAngle >= 180) { currentAngle = 180; sweepDir = -1; }
    if (currentAngle <= 0)   { currentAngle = 0;   sweepDir =  1; }

    servo.write(currentAngle);
    delay(STEP_DELAY);
  }
}

// ── Manual beep — no timer conflict ─────────────────────────
void beep(int pin, int freqHz, int durationMs) {
  long halfPeriodUs = 1000000L / (freqHz * 2);
  long endTime      = millis() + durationMs;

  while (millis() < endTime) {
    digitalWrite(pin, HIGH);
    delayMicroseconds(halfPeriodUs);
    digitalWrite(pin, LOW);
    delayMicroseconds(halfPeriodUs);
  }
}

// ── Measure distance in cm ───────────────────────────────────
long measureDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000UL);
  if (duration == 0) return 0;
  return duration / 58L;
}
