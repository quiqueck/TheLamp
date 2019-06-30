#include <Arduino.h>
#include "Relay.h"
#include "LEDHardware.h"
#include "LEDGrid.h"
#include "VirtualGrid.h"
#include "Timeline.h"
#include "Animation.h"

uint8_t RANDOM = 0;
#define  TOUCH_PIN 4 

LEDHardware leds;
LEDGrid* panels[6];
VirtualGrid* master;
std::shared_ptr<Animation> anim;

void mainButtonPressed(SmartHomeRelay* r){
  if (r->isOn()){
    //turn off relay
    r->off();
  } else {
    r->on();    
  }
}

Relay* rly = nullptr;

std::shared_ptr<Timeline> t1;

void setup(){
    Serial.begin(115200);
    delay(200);  
    rly = new SmartHomeRelay(mainButtonPressed);
    leds.begin();

    int offset = 0;
    panels[0] = new LEDGridRowMajor(    leds.storeAt(offset),   9,  6, LEDGrid::oTL);
    offset += panels[0]->count;

    panels[1] = new LEDGridColumnMajor( leds.storeAt(offset),   7, 10, LEDGrid::oTL);
    offset += panels[1]->count;

    panels[2] = new LEDGridRowMajor(    leds.storeAt(offset),   6,  6, LEDGrid::oBR);
    offset += panels[2]->count;

    panels[3] = new LEDGridColumnMajor( leds.storeAt(offset),  7,  10, LEDGrid::oBR);
    offset += panels[3]->count;

    panels[4] = new LEDGridColumnMajor( leds.storeAt(offset),   5,  7, LEDGrid::oTL);
    offset += panels[4]->count;

    panels[5] = new LEDGridRowMajor(  leds.storeAt(offset),     7,  5, LEDGrid::oBL);
    offset += panels[5]->count;

    panels[0]->fill(CRGB::Red);
    panels[1]->fill(CRGB::Green);
    panels[2]->fill(CRGB::Blue);
    panels[3]->fill(CRGB::Magenta);
    panels[4]->fill(CRGB::Cyan);
    panels[5]->fill(CRGB::Yellow);
    master = new VirtualGrid(27, 23);

    master->addPanel(panels[0],  9,  9);
    master->addPanel(panels[1], 20, 10);
    master->addPanel(panels[2],  0, 17);
    master->addPanel(panels[3],  0,  4);
    master->addPanel(panels[4], 11,  0);
    master->addPanel(panels[5], 20,  2);

    //master->pixel(10, 11) = CRGB(0xff, 0xff, 0xff);
    
    rly->on();
    delay(500);
    leds.show();

    t1 = std::make_shared<Timeline>();

    auto a1 = std::make_shared<TestAnimation>("A1", 1.00, timeRepeat<Animation>);
    auto a2 = std::make_shared<TestAnimation>("A2");
    auto a3 = std::make_shared<TestAnimation>("A3", 5.00);
    auto a4 = std::make_shared<TestAnimation>("A4", 1.00, timeBounce<Animation>);

    auto c1 = std::make_shared<TestCompositor>("C0");

    //auto e1 = std::make_shared<TestAction>("PAUSE");
    auto timeline = t1;
    auto e1 = std::make_shared<DefaultAction>([timeline](const double time, const Action* a){
      Serial.println("---- PAUSE ----");
      timeline->pause();
    });

    //auto e2 = std::make_shared<TestAction>("GOTO");
     auto e2 = std::make_shared<RepeatableAction>([timeline](const double time, const Action* a){
      Serial.printf("---- GO BACK ----\n");      
      timeline->goToTime(1.00);
    }, 3);

    t1->addTrack(a1, 0, LEDState::LayerTypes::SolidBackground);
    t1->addTrack(a2, 1.00, LEDState::LayerTypes::SolidBackground);
    t1->addTrack(a1, 2.00, 2.00, LEDState::LayerTypes::BottomOverlay);
    t1->addTrack(a4, 2.00, 2.00, LEDState::LayerTypes::TopOverlay);
    t1->addTrack(a3, 0.50, LEDState::LayerTypes::MiddleOverlay);

    t1->addCompositor(c1, 0, LEDState::LayerTypes::FinalComposit);

    t1->addAction(e2, 5.20);
    t1->addAction(e1, 9.00);

    t1->begin();
    // t1->runAt(0);
    // t1->runAt(.25);
    // t1->runAt(.50);
    // t1->runAt(.75);
    // t1->runAt(.99);
    // t1->runAt(1.00);
    // t1->runAt(1.01);
    // t1->runAt(1.99);
    // t1->runAt(2.00);
    // t1->runAt(2.01);
    // t1->runAt(2.50);
    // t1->runAt(2.99);
    // t1->runAt(3.00);
    // t1->runAt(3.01);
    // t1->runAt(3.99);
    // t1->runAt(4.00);
    // t1->runAt(4.01);
    // t1->runAt(5.49);
    // t1->runAt(5.50);
    // t1->runAt(5.51);
    // t1->runAt(10.00);
}
Timeline t;
void loop(){
  t1->tick();
}

void loop3(){
  //rly->loop();
  static int x = 0;
  static int y = -1;

  y += 1;
  if (y>=master->height){
    x++;
    y=0;

    if (x>=master->width) {
      x = 0;
      panels[0]->fill(CRGB::Red);
      panels[1]->fill(CRGB::Green);
      panels[2]->fill(CRGB::Blue);
      panels[3]->fill(CRGB::Magenta);
      panels[4]->fill(CRGB::Cyan);
      panels[5]->fill(CRGB::Yellow);
    }
  }

  master->pixel(x, y) = CRGB(0xff, 0xff, 0xff);
  leds.show();
  delay(20);
}

void loops(){
  bool ison[NUMPIXELS];
  
    static int delayRamp = 70;
    int delayTime = 30;
    static int nextRandom = 98;
    static uint8_t state = 0;
    static uint8_t steps = 10;
    static uint8_t r = 0;   

    static bool down = false;
    const uint8_t samples = 16;
    int touchVal = 0;
    for(int i=0; i< samples; i++){
      touchVal += touchRead(T0);    
    }
    touchVal /= samples;
    //Serial.printf("touch: %d @ %d\n", touchVal, T0);
    if (touchVal < 30 ) {
      if (!down){
        down = true;
        RANDOM = RANDOM + 1;
        if (RANDOM > 13) RANDOM = 1;
        Serial.printf("Mode: %d\n", RANDOM);
        
        
        nextRandom = 98;
        state = 0;
        r = 160;
      
        if (RANDOM==1) {
          steps = 6;
          delayTime = 30;
          delayRamp = 70;        
        } else if (RANDOM == 2) {
          steps = 1;
          delayTime = 30;
          delayRamp = 70;  
          state = 2;      
        } else if (RANDOM == 3) {
          steps = 1; 
          delayTime = 30;
          delayRamp = 70;
          state = 2;         
        }
        for (int i=0; i<NUMPIXELS; i++){
          #ifdef ADAFRUIT 
          #else
          leds[i] = CRGB::Black; 
          #endif
        }
        #ifdef ADAFRUIT 
        #else
        FastLED.show();
        #endif
      }
    }  else {
      down = false;
    }

    if (RANDOM == 4){
      
      #ifdef ADAFRUIT 
          #else
          leds[10] = CRGB::Green; 
          leds[11] = CRGB::Green; 
          leds[12] = CRGB::Green; 
          leds[13] = CRGB::Green; 
          leds[14] = CRGB::Green; 
          leds[15] = CRGB::Green; 
          leds[16] = CRGB::Green; 
          leds[17] = CRGB::Green; 
          leds[22] = CRGB::Green; 
          leds[26] = CRGB::Green; 
          leds[27] = CRGB::Green; 
          leds[31] = CRGB::Green; 
          leds[40] = CRGB::Green; 
          leds[43] = CRGB::Green; 
          leds[47] = CRGB::Green; 
          leds[48] = CRGB::Green; 
          FastLED.show();
          #endif
      delay(100);
      return;
    } else if (RANDOM == 5){      
      #ifdef ADAFRUIT 
          #else
          for (int i=0; i<NUMPIXELS; i++){
            leds[i] = CHSV(0xff, 0, 0x80);
          }
          FastLED.show();
          #endif
          delay(100);
      return;
    } else if (RANDOM == 6){      
      #ifdef ADAFRUIT 
          #else
          for (int i=0; i<NUMPIXELS; i++){
            leds[i] = CHSV(0xff, 0, 0xa0);
          }
          FastLED.show();
          #endif
          delay(100);
      return;
    } else if (RANDOM == 7){      
      #ifdef ADAFRUIT 
          #else
          for (int i=0; i<NUMPIXELS; i++){
            leds[i] = CHSV(0xff, 0, 0xd0);
          }
          FastLED.show();
          #endif
          delay(100);
      return;
    }else if (RANDOM == 8){      
      #ifdef ADAFRUIT 
          #else
          for (int i=0; i<NUMPIXELS; i++){
            leds[i] = CHSV(0xff, 0, 0xff);
          }
          FastLED.show();
          #endif
          delay(100);
      return;
    } else if (RANDOM == 9){      
      #ifdef ADAFRUIT 
          #else
          for (int i=0; i<NUMPIXELS; i++){
            if (i%2==0){
              leds[i] = CRGB(0xff, 0xe0, 0x60);
            } else {
              leds[i] = CRGB::Black;
            }
          }
          FastLED.show();
          #endif
          delay(100);
      return;
    } else if (RANDOM == 10){      
      #ifdef ADAFRUIT 
          #else
          for (int i=0; i<NUMPIXELS; i++){
            leds[i] = CRGB(0xff, 0xe0, 0x60);            
          }
          FastLED.show();
          #endif
          delay(100);
      return;
    } else if (RANDOM == 12){      
      #ifdef ADAFRUIT 
          #else
          for (int i=0; i<NUMPIXELS; i++){
            leds[i] = CRGB(0xff, 0xc0, 0xc0);            
          }
          FastLED.show();
          #endif
          delay(100);
      return;
    } else if (RANDOM == 11){      
      #ifdef ADAFRUIT 
          #else
          for (int i=0; i<NUMPIXELS; i++){
            leds[i] = CRGB(0xff, 0xff, 0xff);            
          }
          FastLED.show();
          #endif
          delay(100);
      return;
    } else if (RANDOM == 13){      
      #ifdef ADAFRUIT 
          #else
          for (int i=0; i<NUMPIXELS; i++){
            leds[i] = CRGB(0xc0, 0xff, 0xff);            
          }
          FastLED.show();
          #endif
          delay(100);
      return;
    }

    
    r += steps;
    
    if (state == 0){
      for (int i=0; i<NUMPIXELS; i++){
        #ifdef ADAFRUIT
          pixels.setPixelColor(i, 0, 0, 0);
        #else
          leds[i] = CRGB::Black;          
        #endif
        ison[i] = false;
      }
      state = 1;
    } else if (state == 1){      
      int setCount = 0;
      for (int i=0; i<NUMPIXELS; i++){
        if (ison[i]) continue;
        setCount++;
        long nr = random(100);
        if (nr<nextRandom) continue;
        ison[i]=true;

        #ifdef ADAFRUIT
          pixels.setPixelColor(i, random(0xff), random(0xff), random(0xff));
        #else
          if (RANDOM)
            leds[i] = CHSV(random(0xff), 0xff, 0xff);          
          else
            leds[i] = CHSV(0xff, 0, 0xff);          
          
        #endif
      }
      
      
      nextRandom = 50 + (setCount * 48) / NUMPIXELS;
      //delayTime = (setCount * 300) / NUMPIXELS;
      if (setCount==0) {
        state = 2;
        
        for (int i=1; i<NUMPIXELS; i++){
          ison[i] = false;
        }
      }
      
      //Serial.printf("%d, %d\n", setCount, delayTime);
    } else if (state==2){
      //White
      // 10,  br=50: 0.288A 
      // 10,  br=0xff: 0.660A => 44mA 
      // 20,  br=0xff: 1.073A => 43mA
      // 10,  br=0xe0: 0.587A => 37mA 
      // 100, br=0xA0: 1.897A => 17mA
      // 100, br=0xB0: 1.953A => 18mA
      // 20,  br=0xD0: 0.924A => 35mA
      // 100, br=0xD0: 2.079A => 19mA
      // 150, br=0xD0: 2.231A => 13mA
      // 10,  br=0x7d: 0.424A => 20mA
      // 20,  br=0x7d: 0.601A => 19mA
    
      //Black
      //r = 0x00; b= 0x00; g = 0x00; //10, br=50: 0.225A; 100, br=50: 0.225A
      for(int i=NUMPIXELS-1;i>0;i--){
#ifdef ADAFRUIT      
        pixels.setPixelColor(i, pixels.getPixelColor(i-1));
        // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
        // Moderately bright green color.
      
        //pixels.setPixelColor(i, i*(0xff/10), i*(0xff/10), i*(0xff/10));
        //pixels.setPixelColor(i, i*(0xff/10), i*(0xff/10), i*(0xff/10));
        //Serial.printf("%d=%x ", i, i*(0xff/10)); 
#else
        if (RANDOM==3){
leds[i] = CHSV( r, 0xff, 0xff);
        } else {
          if (ison[i-1]){
            leds[i] = leds[i-1];
            ison[i] = true;
          }
        }
#endif      
      }
      //Serial.println();

#ifdef ADAFRUIT   
      pixels.setPixelColor(0, r, r, r); 
#else
      //leds[0] = CRGB::White;
      if (RANDOM)
        leds[0] = CHSV( r, 0xff, 0xff);    
      else
        leds[0] = CHSV( 0xff, 0, 0xff);          
#endif

      if (delayRamp>30) delayRamp = max(30, delayRamp  - 1);
      delayTime = delayRamp;
    }

#ifdef ADAFRUIT    
    pixels.show(); // This sends the updated pixel color to the hardware.
#else
    FastLED.show();
#endif

  delay(delayTime); // Delay for a period of time (in milliseconds).
}