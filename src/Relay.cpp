#include "Relay.h"
#include <Arduino.h>

bool smartHomeDidTrigger = false;
long debounctime = xTaskGetTickCount();
portMUX_TYPE smartHomeMux = portMUX_INITIALIZER_UNLOCKED;

void SmartHomeSwitchUp(){
  portENTER_CRITICAL_ISR(&smartHomeMux);
  long now = xTaskGetTickCount();
  if (now - debounctime > SMART_HOME_SWITCH_DEBOUNCE_TIME_MS) {
    if (digitalRead(SMART_HOME_SWITCH_PIN)){
      Serial.println(F("UP"));  
      smartHomeDidTrigger = true;          
      debounctime = now;
    }
  }
  portEXIT_CRITICAL_ISR(&smartHomeMux);
}

Relay::Relay(int rPin) : relayPin(rPin) {
    pinMode(relayPin, OUTPUT);
    off();    
}

void Relay::on(){
    _state = true;
    digitalWrite(relayPin, HIGH);
}

void Relay::off(){
    _state = false;
    digitalWrite(relayPin, LOW);
}

SmartHomeRelay::SmartHomeRelay(std::function<void(SmartHomeRelay*)> cbk, int rPin, int gPn, int rPn) : Relay(rPin), switchUp(cbk), greenPin(gPn), redPin(rPn){
  pinMode(greenPin, OUTPUT); //LED
  pinMode(redPin, OUTPUT); //LED2

  pinMode(SMART_HOME_SWITCH_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(SMART_HOME_SWITCH_PIN), SmartHomeSwitchUp, RISING);
}

void SmartHomeRelay::loop(){
    portENTER_CRITICAL_ISR(&smartHomeMux);
    bool hadTrigger = smartHomeDidTrigger;
    smartHomeDidTrigger = false;
    portEXIT_CRITICAL_ISR(&smartHomeMux);

    if (hadTrigger){
        switchUp(this);
    }
}

void SmartHomeRelay::on(){
    Relay::on();
    red();
}

void SmartHomeRelay::off(){
    Relay::off();
    none();
}

//blau == green == LED
//orange == rot == LED2
void SmartHomeRelay::green(){
    digitalWrite(greenPin, LOW);
    digitalWrite(redPin, HIGH);
}

void SmartHomeRelay::red(){
    digitalWrite(greenPin, HIGH);
    digitalWrite(redPin, LOW);
}

void SmartHomeRelay::none(){
    digitalWrite(greenPin, HIGH);
    digitalWrite(redPin, HIGH);
}