# SentryLock 🔒

An Arduino-based proximity security system that uses an ultrasonic sensor mounted on a servo motor to detect nearby objects, trigger a buzzer alarm, and lock on to the object's location.

---

## 🛠 Hardware Components

| Component | Description |
|---|---|
| Arduino (Uno/Nano) | Main microcontroller |
| HC-SR04 | Ultrasonic distance sensor |
| SG90 Servo Motor | Rotates the ultrasonic sensor |
| Active Buzzer | Sounds alarm on detection |

---

## ⚙️ How It Works

1. The servo motor continuously sweeps the ultrasonic sensor across a set angle range.
2. The HC-SR04 measures the distance to any object in its path.
3. Detections are confirmed over 3 consecutive readings to avoid false triggers.
4. Once confirmed, the buzzer sounds an alarm and the servo locks on to the threat angle.

---

## 📌 Pin Configuration

| Component | Pin |
|---|---|
| Servo Signal | D9 |
| Ultrasonic TRIG | D10 |
| Ultrasonic ECHO | D11 |
| Buzzer | D3 |

> Pin numbers can be changed at the top of the source code.

---

## 🚀 Getting Started

### Prerequisites

- Arduino IDE (https://www.arduino.cc/en/software)
- Servo.h library (built-in with Arduino IDE)

### Steps

1. Clone this repository:

   git clone https://github.com/rubiks001/SentryLock.git

2. Open SentryLock.ino in the Arduino IDE.
3. Connect the hardware as shown in the pin configuration table.
4. Upload the sketch to your Arduino board.
5. Open Serial Monitor at 9600 baud to watch live readings.

---

## 💻 Code

    #include <Servo.h>

    // ── Pin definitions ──────────────────────────────────────────
    const int TRIG_PIN   = 10;
    const int ECHO_PIN   = 11;
    const int BUZZER_PIN = 3;
    const int SERVO_PIN  = 9;

    // ── Tunable parameters ───────────────────────────────────────
    const int THRESHOLD_CM = 30;
    const int STEP_DEGREES = 2;
    const int STEP_DELAY   = 20;
    const int CONFIRM_HITS = 3;

    // ── State ────────────────────────────────────────────────────
    Servo servo;
    int currentAngle   = 0;
    int sweepDir       = 1;
    int detectionCount = 0;

    void setup() {
      Serial.begin(9600);
      pinMode(TRIG_PIN,   OUTPUT);
      pinMode(ECHO_PIN,   INPUT);
      pinMode(BUZZER_PIN, OUTPUT);
      servo.attach(SERVO_PIN);
      servo.write(currentAngle);
      delay(500);
      Serial.println("SentryLock ready. Sweeping...");
    }

    void loop() {
      long dist = measureDistance();

      Serial.print("Angle: "); Serial.print(currentAngle);
      Serial.print("  |  Dist: "); Serial.print(dist);
      Serial.println(" cm");

      if (dist > 0 && dist <= THRESHOLD_CM) {
        detectionCount++;
        if (detectionCount >= CONFIRM_HITS) {
          Serial.print(">>> LOCKED ON at ");
          Serial.print(currentAngle); Serial.print("°, ");
          Serial.print(dist); Serial.println(" cm — FROZEN");
          beep(BUZZER_PIN, 1000, STEP_DELAY);
        } else {
          digitalWrite(BUZZER_PIN, LOW);
          advanceSweep();
        }
      } else {
        detectionCount = 0;
        digitalWrite(BUZZER_PIN, LOW);
        advanceSweep();
      }
    }

    void advanceSweep() {
      currentAngle += sweepDir * STEP_DEGREES;
      if (currentAngle >= 180) { currentAngle = 180; sweepDir = -1; }
      if (currentAngle <= 0)   { currentAngle = 0;   sweepDir =  1; }
      servo.write(currentAngle);
      delay(STEP_DELAY);
    }

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

---

## 📐 Circuit Diagram

*(Coming soon)*

---

## 🔭 Planned Features

- [ ] ESP32-CAM integration for visual capture on detection
- [ ] Wireless alert notifications
- [ ] Physical lock mechanism triggered on detection
- [ ] Web dashboard for real-time monitoring

---

## 📄 License

This project is licensed under the MIT License.

---

## 👤 Author

-Olatunde David Durojaiye
-Electrical & Electronics Engineering, University of Lagos
-GitHub: https://github.com/rubiks001
-LinkedIn: https://www.linkedin.com/in/olatunde-david-durojaiye-bb5985207/
