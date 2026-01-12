/** 
 *  Demonstration of how to use the button in query mode.
 */

#include <button/button.h>
#include <Arduino.h>


ButtonCB button(12, Button::INTERNAL_PULLUP);

void onPress(const Button&) {
  Serial.println("Press.");
}

void onHold(const Button& button) {
  Serial.print("Hold count=");
  Serial.println(button.nHolds());
}

void setup() {
  Serial.begin(19200);
  Serial.println("Button Hold Demo");
  pinMode(13, OUTPUT);  

  button.setPressHandler(onPress);
  button.setHoldHandler(onHold);
  button.setHoldRepeats(true);
}

void loop() 
{
  button.process();  
}
 
