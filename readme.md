# Button Library

The Button library for Arduino makes common button tasks simple, filters
out mechanical noise, and adds some handy extra features. Solid, dependable,
and easy to use button handling is surprisingly difficult, and this 
library is here to help.

## Features

* De-bouncing. Mechanical buttons physically vibrate - bounce - when
  they are first pressed or released. This creates spurious state changes
  that need to be filtered or "de-bounced".
* Support for pull-up, pull-down, or internal-pull-up configurations. 
  * A pull-down button is tied to ground with a resistor (usually about
    10k), so the pin is normally LOW. When it closes, the button connects
    to Vcc and goes HIGH.
  * A pull-up button is normally tied to positive with a resistor (usually
    10k), so the pin is normally HIGH. When it closes, the button connects
    to ground and goes LOW.
  * An internal-pull-up button is just like the pull-up, but uses an internal
    resistor on the Arduino. These are very convenient to use, as the button
    just needs to close the connection to ground.

* Queries for 'press', 'down', and 'held'. Queries are checked after the `process()`
  method is called.

	* `press()` is true when a button is first pressed down.
	* `isDown()` returns true if the button is down. (When a button is initially
	   pressed both 'press' and 'isDown' will return true. Subsequent queries
	   to a down button result in 'press' false and `isDown` true.)
	* `held()` is true if the button has hit the holdThreshold in this loop.

* Callacks for press, hold, release, and click. Callbacks are called 
  from the `process()` method. Callbacks are generally easier for more
  complex button handling (doing something different on hold vs click, for
  example.)

	* 'press' is called when a button is first pressed.
	* 'click' is called when a button is released, if it wasn't held. 'click'
	  is called after 'release'.
	* 'hold' is called if a button is held down.
	* 'release' is called when the button is released.

## Example

### Basic Usage

```c++
	Button button(12);

	void setup() {
		Serial.begin(19200);
	}

	void loop() {
		button.process();
		if (button.uniquePress()) {
			Serial.println("Button pressed.");
		}
	}
```

### Callback Usage

```c++
	ButtonCB button(12);

	void onPress(const Button& b){
		Serial.println("Button pressed.");
	}

	void setup(){
	  Serial.begin(19200);
	  button.pressHandler(onPress);
	}

	void loop(){
	  button.process();
	}
```
