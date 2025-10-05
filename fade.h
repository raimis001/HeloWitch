
#include "arduino.h"

class FadeLED
{
    public:

        FadeLED(int pin, float spd)
        {
            LED_PIN = pin;
            progress = 0.0;

            speed = spd / 1000.0;

            pinMode(LED_PIN, OUTPUT);
            analogWrite(LED_PIN, 0);

        }

	    ~FadeLED();

        void update(int direction)
        {
            if (direction == 1)
            {
                if (progress >= 1.0) return;
                if (progress < 1.0) progress += speed;
                if (progress > 1.0) progress = 1.0;
            }

            if (direction == 2) 
            {
                if (progress <= 0.0) return;
                if (progress > 0.0) progress -= speed;
                if (progress < 0.0) progress = 0.0;
            }
        
            int brightness = (int)(255.0 * (progress * progress * progress));
            analogWrite(LED_PIN, brightness);            
        }

    private:
        int LED_PIN;
        float speed = 1;
        float progress = 0.0;   
};

