#ifndef __RELAY_H__
#define __RELAY_H__

#define SMART_HOME_SWITCH_DEBOUNCE_TIME_MS 200
#define SMART_HOME_SWITCH_PIN 27
#define SMART_HOME_LED_GREEN 18 
#define SMART_HOME_LED_RED 5
#define SMART_HOME_RLY 25 

#include <functional>

class Relay {
    public:
        Relay(int rPin);

        virtual void on();
        virtual void off();
        virtual void loop() {};

        inline bool isOn() const { return _state; }
    protected:
        const int relayPin;
        bool _state;
};

class SmartHomeRelay : public Relay{
    public:
        SmartHomeRelay(std::function<void(SmartHomeRelay*)> cbk, int rPin=SMART_HOME_RLY, int greenPin=SMART_HOME_LED_GREEN, int redPin=SMART_HOME_LED_RED);
        virtual void on();
        virtual void off();
        virtual void loop();

        
    
    protected:
        void red();
        void green();
        void none();

        std::function<void(SmartHomeRelay*)> switchUp;
        const int greenPin;
        const int redPin;
};

#endif