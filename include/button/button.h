#ifndef BUTTON_LIBRARY_INCLUDED
#define BUTTON_LIBRARY_INCLUDED

#include <stdint.h>

class Button;
typedef void (*ButtonEventHandler)(const Button&);

struct ButtonCBHandlers {
    ButtonCBHandlers() : cb_onPress(0), cb_onRelease(0), cb_onClick(0), cb_onHold(0) {}
    ButtonEventHandler  cb_onPress;
    ButtonEventHandler  cb_onRelease;
    ButtonEventHandler  cb_onClick;
    ButtonEventHandler  cb_onHold;
};

/**
 * A simple button class that configures the Arduino, filters bounces, and
 * detects clicks and holds. Use `ButtonCB` if you would like to add callback handlers.
 */
class Button {
public:

    enum class Wiring : uint8_t {
        pullDown,
        pullUp,
        internalPullUp
    };

    static constexpr uint16_t kDefaultHoldTime = 500;        ///< Default hold time in milliseconds
    static constexpr uint16_t kDefaultBounceDuration = 20;   ///< Default debounce time in milliseconds

    /**
     * Construct a button.
     * 
     * @param buttonPin The Arduino pin number for this button (default: 255 = uninitialized)
     * @param resistor The wiring configuration of the button:
     *   - pullDown: External pull-down resistor, button connects to HIGH when pressed
     *   - pullUp: External pull-up resistor, button connects to LOW when pressed  
     *   - internalPullUp: Uses Arduino's internal pull-up, button connects to LOW when pressed
     * @param debounceDuration Time in milliseconds for button to settle mechanically (default: 20ms)
     */
    Button(uint8_t buttonPin=255, Wiring resistor = Wiring::internalPullUp, uint16_t debounceDuration = kDefaultBounceDuration);

    /**
     * Initialize or re-initialize the button with new parameters.
     * Can be called after default construction to configure the button.
     */
    void init(uint8_t buttonPin, Wiring resistor = Wiring::internalPullUp, uint16_t debounceDuration = kDefaultBounceDuration);
    /**
     * Get the Arduino pin number this button is configured for.
     * @return The pin number, or 255 if not initialized
     */
    const int pin() const {
        return m_myPin;
    }

    /**
     * Process button state changes and detect events.
     * Should be called once per loop() iteration.
     * Will trigger callbacks if configured and events are detected.
     */
    void process();

    /// Returns true if the button was just pressed (edge detection)
    bool press() const;
    /// Returns true if the button is currently down/pressed
    bool isDown() const;
    /// Returns true if the button has been held longer than the hold threshold
    bool held() const;
    /// Returns the duration in milliseconds the button has been held down
    uint32_t holdTime() const;
    /// Returns the timestamp when the button was last pressed down
    uint32_t pressedTime() const {
        return m_pressedStartTime;
    }

    /**
     * Set the minimum time in milliseconds for a press to be considered a "hold".
     * @param holdTime Duration in milliseconds (default is 500ms)
     */
    void setHoldThreshold(uint32_t holdTime);
    /**
     * Get the current hold threshold time.
     * @return Hold threshold in milliseconds
     */
    const uint16_t holdThreshold() const {
        return m_holdEventThreshold;
    }

    /**
     * Configure whether hold events repeat while the button remains pressed.
     * @param holdRepeats If false (default), only one hold event is triggered.
     *                    If true, a hold event is triggered repeatedly at holdThreshold intervals.
     */
    void setHoldRepeats(bool holdRepeats) { m_holdRepeats = holdRepeats; }

    /// Returns true if hold events repeat while button is held down
    const bool holdRepeats() const {
        return m_holdRepeats;
    }

    /**
     * Returns the number of hold events that have occurred for the current press.
     * Only meaningful when holdRepeats() is true. The first hold event returns 1.
     * @return Number of hold events, starting from 1
     */
    int nHolds() const {
        return m_nHolds;
    }

    /**
     * Get the current cycle number for repeating holds with on/off pattern.
     * Useful for creating blinking or alternating effects during long holds.
     * 
     * @param on Optional pointer to receive the current on/off state (true = on phase)
     * @return The cycle number (1, 2, 3...) or 0 if not held
     */
    int cycle(bool* on = 0) const {
        if (on) *on = false;            

        if (held()) {
            if (on) *on = (m_nHolds & 1) ? true : false;
            return (m_nHolds + 1) / 2;
        }
        return 0;
    }

    /**
     * @name Testing Methods
     * For unit testing and debugging - do not call during normal operation.
     * @{
     */
    
    /// Get pointer to callback handlers (for testing)
    const ButtonCBHandlers* queryHandlers() const {
        return m_handlers;
    }
    
    /// Enable/disable test mode for simulated button presses
    void enableTestMode(bool testMode);
    
    /// Simulate a button press (test mode only)
    void testPress();
    
    /// Simulate a button release (test mode only)
    void testRelease();
    
    /// @}

private:
    bool stateChanged() const;
    bool queryButtonDown() const;

    bool                m_holdRepeats;
    uint8_t             m_myPin;
    Wiring              m_wiring;
    uint8_t             m_state;
    uint16_t            m_nHolds;
    uint16_t            m_holdEventThreshold;
    uint16_t            m_debounceDuration;
    uint32_t            m_pressedStartTime;
    uint32_t            m_debounceStartTime;

protected:
    const ButtonCBHandlers* m_handlers;
};


/**
 * Button class with callback support for Press, Release, Click, and Hold events.
 * Extends the base Button class with event handler functionality.
 * Uses additional memory (~24 bytes) so callback functionality is opt-in.
 */
class ButtonCB : public Button {
public:
    /**
     * Construct a button with callback support.
     */
    ButtonCB(uint8_t buttonPin=255, Button::Wiring resistor = Button::Wiring::internalPullUp, uint16_t debounceDuration = kDefaultBounceDuration) :
        Button(buttonPin, resistor, debounceDuration)
    {
        m_handlers = &m_handlerData;
    }

    /**
     * @name Event Handler Registration
     * Set callback functions for button events.
     * 
     * Event sequence: Every button interaction generates press and release events.
     * Additionally, a click event (short press) or hold event (long press) may be generated.
     * @{
     */
    
    /// Set handler for button press events (called when button goes down)
    void setPressHandler(ButtonEventHandler handler);
    
    /// Set handler for button release events (called when button comes up)
    void setReleaseHandler(ButtonEventHandler handler);
    
    /// Set handler for click events (called on short press/release)
    void setClickHandler(ButtonEventHandler handler);
    
    /// Set handler for hold events (called when button held beyond threshold)
    void setHoldHandler(ButtonEventHandler handler);
    
    /// @}

private:
    ButtonCBHandlers m_handlerData;
};

#endif // BUTTON_LIBRARY_INCLUDED
