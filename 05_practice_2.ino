#define PIN_LED 7
unsigned int count;
void setup() {
  // put your setup code here, to run once:
  count = 0;
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);
  delay(1000);
  digitalWrite(PIN_LED, HIGH);
  delay(200);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(PIN_LED, LOW);
  delay(200);
  digitalWrite(PIN_LED, HIGH);
  delay(200);
  count++;
  if(count == 5) while(1){}
}
