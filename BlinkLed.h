#include "arduino.h"

class BlinkLED
{
    public:

        BlinkLED(int pin, int interval, int blink)
        {
            LED_PIN = pin;
            INTERVAL = interval;
            BLINK = blink;
            lastBlink = 0;
            state = LOW;

            pinMode(LED_PIN, OUTPUT);
            digitalWrite(LED_PIN, state);
        }

        ~BlinkLED();

        void update()
        {
            unsigned long currentMillis = millis();
            
            int test = (state == HIGH) ? BLINK : INTERVAL;
            if (currentMillis - lastBlink >= test)
            {
                lastBlink = currentMillis;
                state = !state;
                digitalWrite(LED_PIN, state);
            }
        }

        void reset() 
        {
            lastBlink = millis();
            state = LOW;
            digitalWrite(LED_PIN, state);
        }

    private:
        int LED_PIN;
        int INTERVAL;
        int BLINK;
        unsigned long lastBlink;
        int state;   
};