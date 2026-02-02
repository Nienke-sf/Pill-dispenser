
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Servo.h>


// ============================== LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);   // Tinkercad I2C LCD address

// ============================== 4x4 Keypad
const byte ROWS = 4;
const byte COLS = 4;

char Keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

Keypad customKeypad = Keypad(makeKeymap(Keys), rowPins, colPins, ROWS, COLS);

// ============================== Variables
String action;
long timein[6], countdown_time = 0, initialsecond = 0;
int i = 0;

// ============================== Buzzer / LED
int led = 11;
int sound = 10;

//========================================================================================servo
Servo myservo;  

void setup()
{
  //===================================================================================lcd
  lcd.init();                      // initialize the lcd 
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  lcd.print("Hello User");
  delay(3000);
  lcd.clear();
  lcd.print("Set timer = *");
  delay(3000);
  lcd.clear();
  lcd.print("Start = #");
  delay(3000);
  lcd.clear();

  lcd.setCursor(4,0);
  lcd.print("HH:MM:SS");

  lcd.setCursor(4, 1);
  lcd.print("00:00:00");
  delay(5000);
  
  //========================================================================================keypad
  Serial.begin(9600);
  //========================================================================================buzzer
  pinMode(led, OUTPUT);               // sets the LED as output
  pinMode(sound, OUTPUT);         // sets the buzzer alarm as output
  //===============================================================================servo
  myservo.attach(12);

}
  

void loop() {
  char key = customKeypad.getKey();
  if (key) {
    switch (key) {
      //If key is * set time.
      case '*'  :
        action = "set_time";
        lcd.setCursor(4, 1);
        lcd.blink();
        i = 0;
        break;
      //If key is # start countdown.
      case '#'  :
        action  = "start_countdown";
        lcd.noBlink();
        break;
      default :
        if (action == "set_time") {
          i++;
          int c = i - 1;
          timein[c] = key - 48;
          initialsecond = 0;
          long hour = (timein[0] * 10) + timein[1];
          long minute = (timein[2] * 10) + timein[3];
          long second = (timein[4] * 10) + timein[5]; //second
          countdown_time = (hour * 3600) + (minute * 60) + second;
          lcd.print(key);
          if (i % 2 == 0 && i < 6) {
            lcd.print(":");
          }
          break;
        }
    }
  }
  if (action  == "start_countdown") {
    if (initialsecond == 0) {
      initialsecond = millis() / 1000;
    }
    long countdowntime_seconds = countdown_time - (millis() / 1000) + initialsecond;
    if (countdowntime_seconds >= 0) {
      long countdown_hour = countdowntime_seconds / 3600;
      long countdown_minute = ((countdowntime_seconds / 60) % 60);
      long countdown_sec = countdowntime_seconds % 60;
      lcd.setCursor(4, 1);
      if (countdown_hour < 10) {
        lcd.print("0");
      }
      lcd.print(countdown_hour);
      lcd.print(":");
      if (countdown_minute < 10) {
        lcd.print("0");
      }
      lcd.print(countdown_minute);
      lcd.print(":");
      if (countdown_sec < 10) {
        lcd.print("0");
      }
      lcd.print(countdown_sec);
      if (countdowntime_seconds == 0) {
        tone(sound, 3000, 300);
        delay(1000);
        digitalWrite(led, HIGH);
        delay(2000); 
        tone(sound, 3000, 300);
        digitalWrite(led, LOW);
        delay(1000); 

        myservo.write(0);              // tell servo to go to position in variable 'pos'
        delay(1000);                       // waits 15 ms for the servo to reach the position
        myservo.write(100); 
        delay(1000);
      } 
    }
    delay(500);
  }
}
