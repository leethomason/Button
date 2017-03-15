/** 
 *  Demonstration of how to use the button in query mode.
 */

#include <Button.h>

#define PIN 2
#define PULL Button::PULL_UP

Button button(PIN, PULL);

void setup() {
  Serial.begin(19200);
  Serial.println("Button Event Demo");
  pinMode(13, OUTPUT);
}

void loop() {
  button.process();

  if (button.press()) {
    Serial.println("LOG: Press");
    digitalWrite(13, HIGH);
  }
  else 
  {
        digitalWrite(13, LOW);
  }
  if (button.held()) {
    Serial.println("LOG: Held");
  }
  //digitalWrite(13, button.isDown());
}
 
