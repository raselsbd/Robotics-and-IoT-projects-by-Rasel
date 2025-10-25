#include <AFMotor.h>  
#include <NewPing.h>
#include <Servo.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h> 

// OLED display settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define TRIG_PIN A0 
#define ECHO_PIN A1 
#define MAX_DISTANCE 400 
#define MAX_SPEED 190
#define MAX_SPEED_OFFSET 20

#define SOUND_SENSOR_PIN A3
#define LED_PIN 13  

#define DHT_PIN 2
#define DHT_TYPE DHT11 

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);
DHT dht(DHT_PIN, DHT_TYPE);

AF_DCMotor motor1(1, MOTOR12_1KHZ); 
AF_DCMotor motor2(2, MOTOR12_1KHZ);
AF_DCMotor motor3(3, MOTOR34_1KHZ);
AF_DCMotor motor4(4, MOTOR34_1KHZ);
Servo myservo;   

boolean goesForward = false;
int distance = 250;
int speedSet = 0;

void setup() {
    Serial.begin(9600);

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println("SSD1306 initialization failed!");
        for (;;);
    }
    display.clearDisplay();
    dht.begin();

    pinMode(LED_PIN, OUTPUT);

    myservo.attach(10);
    myservo.write(115); 
    delay(2000);

    distance = readPing();
    delay(100);
}

void loop() {
    int distanceR = 0;
    int distanceL = 0;
    delay(40);

    if (distance <= 15) {
        moveStop();
        delay(100);
        moveBackward();
        delay(300);
        moveStop();
        delay(200);

        distanceR = lookRight();
        delay(200);
        distanceL = lookLeft();
        delay(200);

        if (distanceR >= distanceL) {
            turnRight();
        } else {
            turnLeft();
        }
        delay(200);
    } else {
        moveForward();
    }

    distance = readPing();
    
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    if (isnan(temperature) || isnan(humidity)) {
        temperature = 0.0;
        humidity = 0.0;
    }

    int soundValue = analogRead(SOUND_SENSOR_PIN);
    bool soundDetected = soundValue > 544;
    digitalWrite(LED_PIN, soundDetected ? HIGH : LOW);

    updateDisplay(distance, temperature, humidity, soundValue, soundDetected);
}

int readPing() {
    delay(70);
    int cm = sonar.ping_cm();
    return cm == 0 ? 250 : cm;
}

int lookRight() {
    myservo.write(50);
    delay(500);
    int distance = readPing();
    delay(100);
    myservo.write(115);
    return distance;
}

int lookLeft() {
    myservo.write(170);
    delay(500);
    int distance = readPing();
    delay(100);
    myservo.write(115);
    return distance;
}

void moveStop() {
    motor1.run(RELEASE);
    motor2.run(RELEASE);
    motor3.run(RELEASE);
    motor4.run(RELEASE);
} 

void moveForward() {
    if (!goesForward) {
        goesForward = true;
        motor1.run(FORWARD);
        motor2.run(FORWARD);
        motor3.run(FORWARD);
        motor4.run(FORWARD);

        for (speedSet = 0; speedSet < MAX_SPEED; speedSet += 2) {
            motor1.setSpeed(speedSet);
            motor2.setSpeed(speedSet);
            motor3.setSpeed(speedSet);
            motor4.setSpeed(speedSet);
            delay(5);
        }
    }
}

void moveBackward() {
    goesForward = false;
    motor1.run(BACKWARD);
    motor2.run(BACKWARD);
    motor3.run(BACKWARD);
    motor4.run(BACKWARD);

    for (speedSet = 0; speedSet < MAX_SPEED; speedSet += 2) {
        motor1.setSpeed(speedSet);
        motor2.setSpeed(speedSet);
        motor3.setSpeed(speedSet);
        motor4.setSpeed(speedSet);
        delay(5);
    }
}  

void turnRight() {
    motor1.run(FORWARD);
    motor2.run(FORWARD);
    motor3.run(BACKWARD);
    motor4.run(BACKWARD);     
    delay(500);
    moveForward();
} 

void turnLeft() {
    motor1.run(BACKWARD);
    motor2.run(BACKWARD);
    motor3.run(FORWARD);
    motor4.run(FORWARD);
    delay(500);
    moveForward();
}

void updateDisplay(float distance, float temperature, float humidity, int soundValue, bool soundDetected) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    display.setCursor(0, 0);
    display.print("Dist: "); display.print(distance); display.println(" cm");

    display.setCursor(0, 10);
    display.print("Temp: "); display.print(temperature); display.println(" C");

    display.setCursor(0, 20);
    display.print("Humidity: "); display.print(humidity); display.println(" %");

    display.setCursor(0, 30);
    display.print("Sound: "); display.println(soundDetected ? "Yes" : "No");

    display.setCursor(0, 40);
    display.print("Sound Wave: "); display.println(soundValue);

    display.display();
}
  
