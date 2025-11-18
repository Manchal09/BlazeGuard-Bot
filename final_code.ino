#include <Servo.h>            
#include <SoftwareSerial.h>

// Create servo object
Servo myservo;
int pos = 90;    
boolean fire = false;

// Communication settings
const String PHONE = "+91****";  // Replace with your phone number
#define rxPin 2
#define txPin 3
SoftwareSerial sim800L(rxPin, txPin);

// Pin definitions
#define Left 4          // Left flame sensor
#define Right 5         // Right flame sensor
#define Forward 6       // Forward flame sensor
#define GAS_SENSOR 7    // Flame sensor (digital)
#define LM1 8           // Left motor pin 1
#define LM2 9           // Left motor pin 2
#define RM1 10          // Right motor pin 1
#define RM2 11          // Right motor pin 2
#define pump 12         // Water pump
#define SERVO_PIN 13    // Servo motor
#define GAS_ANALOG A0   // MQ2 sensor analog output

// Navigation variables
bool leftFire = false;
bool rightFire = false;
bool frontFire = false;
int flameThreshold = 150;  // Adjust based on your sensor

void setup() {
  Serial.begin(115200);
  sim800L.begin(9600);
  delay(1000);
  
  // Initialize SIM800L for SMS
  sim800L.println("AT");
  delay(1000);
  sim800L.println("AT+CMGF=1");
  delay(1000);
 
  // Set pin modes
  pinMode(Left, INPUT);
  pinMode(Right, INPUT);
  pinMode(Forward, INPUT);
  pinMode(GAS_SENSOR, INPUT);
  pinMode(GAS_ANALOG, INPUT);
  pinMode(LM1, OUTPUT);
  pinMode(LM2, OUTPUT);
  pinMode(RM1, OUTPUT);
  pinMode(RM2, OUTPUT);
  pinMode(pump, OUTPUT);
 
  myservo.attach(SERVO_PIN);
  myservo.write(90);  // Center position

  // Clear any incoming serial data
  while (sim800L.available()) sim800L.read();

  stop_motors();
  digitalWrite(pump, LOW);
  
  Serial.println("Fire fighting robot ready. Waiting for sensors to warm up...");
  delay(3000);  // MQ2 warm-up time
}

void loop() {
  // Read sensor values
  int gasAnalog = analogRead(GAS_ANALOG);        // MQ2 sensor reading
  int flameDetected = digitalRead(GAS_SENSOR);   // Main flame sensor (LOW = fire)
  leftFire = digitalRead(Left) == LOW;           // Left flame sensor (LOW = fire)
  rightFire = digitalRead(Right) == LOW;         // Right flame sensor (LOW = fire)
  frontFire = digitalRead(Forward) == LOW;       // Front flame sensor (LOW = fire)
  
  // Debug output
  Serial.print("MQ2: ");
  Serial.print(gasAnalog);
  Serial.print(" | Flame: ");
  Serial.print(flameDetected);
  Serial.print(" | Left: ");
  Serial.print(leftFire);
  Serial.print(" | Right: ");
  Serial.print(rightFire);
  Serial.print(" | Front: ");
  Serial.println(frontFire);

  // Fire or smoke detected by any sensor
  if ((gasAnalog >= flameThreshold || flameDetected == LOW || leftFire || rightFire || frontFire) && !fire) {
    Serial.println("ALERT: Fire detected!");
    fire = true;
    
    // Send alert before trying to extinguish
    send_sms();
    make_call();
    
    // Navigate towards the fire and extinguish it
    navigate_to_fire();
  } 
  else if (gasAnalog < flameThreshold && flameDetected == HIGH && !leftFire && !rightFire && !frontFire) {
    if (fire) {
      Serial.println("Clear: No fire detected");
      fire = false;
    }
    stop_motors();
    digitalWrite(pump, LOW);
  }
  
  delay(100);  // Reduced delay for more responsive movements
}

// Navigate towards the fire source
void navigate_to_fire() {
  int attempts = 0;
  const int MAX_ATTEMPTS = 20;  // Safety limit
  
  while (attempts < MAX_ATTEMPTS) {
    // Read flame sensors
    leftFire = digitalRead(Left) == LOW;
    rightFire = digitalRead(Right) == LOW;
    frontFire = digitalRead(Forward) == LOW;
    
    Serial.print("Navigation: L:");
    Serial.print(leftFire);
    Serial.print(" R:");
    Serial.print(rightFire);
    Serial.print(" F:");
    Serial.println(frontFire);
    
    // Decision making for navigation
    if (frontFire) {
      // Fire detected in front - move forward
      Serial.println("Fire detected ahead, moving forward");
      move_forward();
      delay(500);
      
      // When close enough, stop and put out the fire
      if (attempts > MAX_ATTEMPTS / 2) {
        stop_motors();
        put_off_fire();
        return;
      }
    }
    else if (leftFire && !rightFire) {
      // Fire detected on left - turn left
      Serial.println("Fire detected on left, turning left");
      turn_left();
      delay(300);
    }
    else if (rightFire && !leftFire) {
      // Fire detected on right - turn right
      Serial.println("Fire detected on right, turning right");
      turn_right();
      delay(300);
    }
    else if (leftFire && rightFire) {
      // Fire detected on both sides - move forward
      Serial.println("Fire detected on both sides, moving forward");
      move_forward();
      delay(500);
    }
    else {
      // No direct detection, search for fire
      Serial.println("Searching for fire...");
      search_for_fire();
    }
    
    stop_motors();
    delay(100);
    attempts++;
  }
  
  // If exceeded attempts, stop and try to put out fire anyway
  stop_motors();
  put_off_fire();
}

// Search pattern to find fire
void search_for_fire() {
  // Rotate servo to scan environment
  for (pos = 30; pos <= 150; pos += 10) {
    myservo.write(pos);
    delay(100);
    
    // If fire detected during scan, break
    if (digitalRead(GAS_SENSOR) == LOW) {
      break;
    }
  }
  
  // Rotate robot to do a complete scan
  turn_right();
  delay(200);
  stop_motors();
  delay(100);
}

void put_off_fire() {
  Serial.println("Extinguishing fire...");
  stop_motors();
  
  digitalWrite(pump, HIGH);
  Serial.println("Spraying water...");

  // Sweep servo to spray water
  for (int i = 0; i < 3; i++) {
    for (pos = 30; pos <= 150; pos += 2) {
      myservo.write(pos);
      delay(15);
    }
    for (pos = 150; pos >= 30; pos -= 2) {
      myservo.write(pos);
      delay(15);
    }
  }

  digitalWrite(pump, LOW);
  myservo.write(90);
  Serial.println("Suppression complete");
}

void move_forward() {
  digitalWrite(LM1, HIGH);
  digitalWrite(LM2, LOW);
  digitalWrite(RM1, HIGH);
  digitalWrite(RM2, LOW);
}

void turn_left() {
  digitalWrite(LM1, LOW);
  digitalWrite(LM2, HIGH);
  digitalWrite(RM1, HIGH);
  digitalWrite(RM2, LOW);
}

void turn_right() {
  digitalWrite(LM1, HIGH);
  digitalWrite(LM2, LOW);
  digitalWrite(RM1, LOW);
  digitalWrite(RM2, HIGH);
}

void stop_motors() {
  digitalWrite(LM1, LOW);
  digitalWrite(LM2, LOW);
  digitalWrite(RM1, LOW);
  digitalWrite(RM2, LOW);
}

void make_call() {
  Serial.println("Making emergency call...");
  sim800L.println("AT");
  delay(1000);
  sim800L.print("ATD");
  sim800L.print(PHONE);
  sim800L.println(";");
  delay(15000); // Let it ring
  sim800L.println("ATH");
  delay(1000);
  Serial.println("Call ended");
}

void send_sms() {
  Serial.println("Sending SMS alert...");
  sim800L.println("AT+CMGF=1");
  delay(1000);
  sim800L.print("AT+CMGS=\"");
  sim800L.print(PHONE);
  sim800L.println("\"");
  delay(1000);
  sim800L.print("EMERGENCY: Fire detected! Robot responding and tracking fire source.");
  delay(500);
  sim800L.write(26); // Send SMS
  delay(5000);
  Serial.println("SMS sent");
}
