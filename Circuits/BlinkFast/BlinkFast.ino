/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
*/

int curr = 9;
int wait = 200/8;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
//  pinMode(curr, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(curr, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(wait);                       // wait for a second
  digitalWrite(curr, LOW);    // turn the LED off by making the voltage LOW
  delay(wait);  
  --curr;
  if (curr < 2) curr = 9;
}
