/** 
 *  Demonstration of how to use the button in callback mode.
 */

#include <button/button.h>
#include <Arduino.h>

/* Pin, Mode
   - A PULL_DOWN resistor means the button is tied to ground, and the button connects to HIGH on close.
   - A PULL_UP resistor is tied to Vcc, and the button connects to LOW on close.
   - An INTERNAL_PULLUP uses the internal resistor, and the button connects to LOW on close.
*/
ButtonCB button(12, Button::INTERNAL_PULLUP);

void doPressHandler(const Button& button) {
  Serial.print("LOG: doPressHandler ");
  Serial.println(button.pin());
}

void doReleaseHandler(const Button& button) {
  Serial.print("LOG: doReleaseHandler ");
  Serial.println(button.pin());
}

void doClickHandler(const Button& button) {
  Serial.print("LOG: doClickHandler ");
  Serial.println(button.pin());
}

void doHoldHandler(const Button& button) {
  Serial.print("LOG: doHoldHandler ");
  Serial.println(button.pin());
}

void setup() {
  Serial.begin(19200);
  Serial.println("Button Callback Demo");
  pinMode(13, OUTPUT);

  button.setPressHandler(doPressHandler);
  button.setReleaseHandler(doReleaseHandler);
  button.setClickHandler(doClickHandler);
  button.setHoldHandler(doHoldHandler);
}

void loop() {
  button.process();
  digitalWrite(13, button.isDown());
}
 
