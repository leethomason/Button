# Button Library

The Button library for Arduino makes common button tasks simple, filters
out mechanical noise, and adds some handy extra features. Solid, dependable,
and easy to use button handling is surprisingly difficult, and this
library is here to help.

## Features

* Debouncing. Mechanical buttons physically vibrate (bounce) when
  they are first pressed or released. This creates spurious state changes
  that need to be filtered or "debounced".
* Support for pull-up, pull-down, or internal pull-up configurations:
  * A pull-down button is tied to ground with a resistor (typically 10kΩ),
    so the pin reads LOW normally. When pressed, the button connects to VCC
    and the pin reads HIGH.
  * A pull-up button is tied to VCC with a resistor (typically 10kΩ),
    so the pin reads HIGH normally. When pressed, the button connects to
    ground and the pin reads LOW.
  * An internal pull-up button works like a pull-up configuration but uses
    the Arduino's internal resistor. These are convenient as the button only
    needs to connect to ground when pressed.

* Query methods for 'press', 'down', and 'held' states. Queries are checked after
  the `process()` method is called:

	* `press()` returns true when a button is first pressed down (edge detection).
	* `isDown()` returns true while the button is currently pressed. (When a button
	  is initially pressed, both `press()` and `isDown()` will return true. On
	  subsequent calls while the button remains down, `press()` returns false and
	  `isDown()` returns true.)
	* `held()` returns true if the button has been held longer than the hold threshold.

* Callbacks for press, hold, release, and click events. Callbacks are invoked
  from the `process()` method. Callbacks are generally easier to use for more
  complex button handling (such as doing different actions on hold vs click):

	* 'press' is called when a button is first pressed down.
	* 'click' is called when a button is released after a short press (not held).
	  The 'click' callback is called after the 'release' callback.
	* 'hold' is called when a button has been held down longer than the threshold.
	* 'release' is called when the button is released.

## Example

### Basic Usage

```cpp
Button button(12);

void setup() {
	Serial.begin(19200);
}

void loop() {
	button.process();
	if (button.press()) {
		Serial.println("Button pressed.");
	}
}
```

### Callback Usage

```cpp
ButtonCB button(12);

void onPress(const Button& b) {
	Serial.println("Button pressed.");
}

void setup() {
	Serial.begin(19200);
	button.setPressHandler(onPress);
}

void loop() {
	button.process();
}
```
