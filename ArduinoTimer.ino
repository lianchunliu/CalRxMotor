#include <EEPROM.h>
#include <DigitShield.h>


const int INCR_BUTTON_PIN = 8;
const int DECR_BUTTON_PIN = 7;
const int MOTOR_PIN  = 12;

const int MIN_SPEED = 1;
const int MAX_SPEED = 99;

const int SPEED_ADDR = 0;

volatile int my_speed = 0;
volatile int my_counter = 0;
volatile int toggle = 0;

void setup()
{
  // put your setup code here, to run once:

  pinMode(MOTOR_PIN, OUTPUT);
  pinMode(INCR_BUTTON_PIN, INPUT);
  pinMode(DECR_BUTTON_PIN, INPUT);

  Serial.begin(9600);
  while (!Serial);

  loadSpeed();

 
//  attachInterrupt(digitalPinToInterrupt(INCR_BUTTON_PIN), incrSpeed, RISING);
 // attachInterrupt(digitalPinToInterrupt(DECR_BUTTON_PIN), decrSpeed, RISING);

  DigitShield.begin();
  //DigitShield.setPrecision(2);
  DigitShield.setValue(my_speed);
}




void onTimer()
{

  my_counter++;
  if (my_counter >= (MAX_SPEED - my_speed)) {
    my_counter = 0;                                                                                                                

    if (toggle) {
      digitalWrite(MOTOR_PIN, HIGH);
      toggle = 0;
    } else {
      digitalWrite(MOTOR_PIN, LOW);
      toggle = 1;
    }
  }


}

int cnt = 0;
int cnt2 = 0;

void loop() {
  // put your main code here, to run repeatedly:
  delayMicroseconds(1);
  
  onTimer();

  cnt2++;
  if (cnt2 > 10) {
    cnt2 = 0;
    cnt++;
  
    if (cnt > 5000) {
      cnt = 0; 
      onTimer200();
   
    }
  }
}

void onTimer200()
{
  displaySpeed();

  if (digitalRead(INCR_BUTTON_PIN)) {
    incrSpeed();
  }

  if (digitalRead(DECR_BUTTON_PIN)) {
    decrSpeed();
  }
  
}

void displaySpeed()
{

  DigitShield.setValue(my_speed);

  Serial.print("my_speed=");
  Serial.println(my_speed);
  Serial.print("my_counter=");
  Serial.println(my_counter);

}

void loadSpeed()
{

  int val = 0;
  EEPROM.get(SPEED_ADDR, val);
  my_speed = val;

  if (my_speed > MAX_SPEED ) {
    my_speed = MAX_SPEED;
  }

  if (my_speed < MIN_SPEED) {
    my_speed = MIN_SPEED;
  }

}

void saveSpeed()
{
  int val = my_speed;

  EEPROM.put(SPEED_ADDR, val);
}

void incrSpeed()
{
  my_speed = my_speed + 1;
  if (my_speed > MAX_SPEED) {
    my_speed = MAX_SPEED;
  }
  saveSpeed();
}

void decrSpeed()
{

  my_speed = my_speed - 1;
  if (my_speed < MIN_SPEED) {
    my_speed = MIN_SPEED;
  }
  saveSpeed();
}
