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
