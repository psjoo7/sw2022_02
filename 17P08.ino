#include <Servo.h>

// Arduino pin assignment

#define PIN_POTENTIOMETER 3 // Potentiometer at Pin A3
// Add IR Sensor Definition Here !!!
#define PIN_IR 0 // IR 센서 
#define PIN_SERVO 10
#define PIN_LED 9 // LED 출력 

#define _DUTY_MIN 553  // servo full clock-wise position (0 degree)
#define _DUTY_NEU 1476 // servo neutral position (90 degree)
#define _DUTY_MAX 2399 // servo full counter-clockwise position (180 degree)
#define _DIST_MIN 100.0 // min : 10cm
#define _DIST_MAX 250.0 // max : 25cm

#define LOOP_INTERVAL 50   // Loop Interval (unit: msec)
#define _EMA_ALPHA 0.5 // ema filter

Servo myservo;
unsigned long last_loop_time;   // unit: msec
float a_value_ema, dist_ema, dist_prev = _DIST_MAX;

void setup()
{
  myservo.attach(PIN_SERVO); 
  myservo.writeMicroseconds(_DUTY_NEU);
  pinMode(PIN_LED, OUTPUT);
  dist_prev = _DIST_MIN;
  digitalWrite(PIN_LED, LOW); //LED OFF
  Serial.begin(2000000);
}

void loop()
{
  unsigned long time_curr = millis();
  int a_value, duty;
  float dist;
  
  
  // wait until next event time
  if (time_curr < (last_loop_time + LOOP_INTERVAL))
    return;
  last_loop_time += LOOP_INTERVAL;

  // Remove Next line !!!
  a_value = analogRead(PIN_IR);
  a_value_ema = _EMA_ALPHA * a_value + (1 - _EMA_ALPHA) * a_value_ema;
  // Read IR Sensor value !!!
  // Convert IR sensor value into distance !!!
  // we need distance range filter here !!!
  // we need EMA filter here !!!
  dist = (6762.0 / (a_value - 9) - 4.0) * 10.0 - 60.0;

  if(dist < _DIST_MIN){
    dist = dist_prev;
    digitalWrite(PIN_LED, HIGH); // LED off
  }else if (dist > _DIST_MAX) {
    dist = dist_prev;
    digitalWrite(PIN_LED, HIGH); // LED off
  }else{
    digitalWrite(PIN_LED, LOW); // LED on
    dist_prev = dist;
  }
  
  // map distance into duty
  if(dist_ema < _DIST_MIN){
    myservo.writeMicroseconds(_DUTY_MIN);
  }
  else if(dist_ema > _DIST_MAX){
    myservo.writeMicroseconds(_DUTY_MAX);
  }
  else{
    duty = (_DUTY_MAX - _DUTY_MIN) / 1023.0 * a_value_ema + _DUTY_MIN;
    myservo.writeMicroseconds(duty);
  }
  
  dist_ema = _EMA_ALPHA * dist + (1 - _EMA_ALPHA) * dist_prev;
  
  // 시리얼 플로터 
  Serial.print("MIN:"); Serial.print(_DIST_MIN);
  Serial.print(",IR:"); Serial.print(a_value);
  Serial.print(",dist:"); Serial.print(dist);
  Serial.print(",ema:"); Serial.print(dist_ema);
  Serial.print(",servo:"); Serial.print(duty);
  Serial.print(",MAX:"); Serial.print(_DIST_MAX);
  Serial.println("");

}
