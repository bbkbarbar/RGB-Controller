/*
 * RGB Controller
 * version 1.00
 * 
 * Device: 
 *     Arduino Pro Mini
 * Number of output channels: 
 *     6 (8bit pwm outputs)
 *
 * 
 * Created by: Andras Boor
 * 2017.04.
 */
#define VERSION  "v1.00"

#include <EEPROM.h>


//===============================================================================================

#define PWM_OUTPUT_CH0               3
#define PWM_OUTPUT_CH1               5
#define PWM_OUTPUT_CH2               6
#define PWM_OUTPUT_CH3               9
#define PWM_OUTPUT_CH4              10
#define PWM_OUTPUT_CH5              11

#define DELAY_BETWEEN_BRIGHTNESS_CHANGES 10

#define DEFAULT_OUTPUT_VALUE         0  // can set any value in range [0..255] depending on planned useage

#define PWM_CHANNEL_COUNT            6  // based on the physical limitations of Arduino Nano
#define CHANNEL_UNDEFINED           -1
#define PWM_MAX                    255  // max value of 8bit pwm output

const char pwmChannelMap[PWM_CHANNEL_COUNT] = {
  PWM_OUTPUT_CH0,
  PWM_OUTPUT_CH1,
  PWM_OUTPUT_CH2,
  PWM_OUTPUT_CH3,
  PWM_OUTPUT_CH4,
  PWM_OUTPUT_CH5
};

unsigned char outputs[PWM_CHANNEL_COUNT];     // Array for storing values of pwm channels

//=================================================================================

void setup() {

  // sets the pin for PWM outputs
  unsigned char i;
  for(i=0; i<PWM_CHANNEL_COUNT; i++){
    pinMode(pwmChannelMap[i], OUTPUT);
    outputs[i] = DEFAULT_OUTPUT_VALUE;
  }

  outputs[1] = 255;
  outputs[2] = 1;
  outputs[3] = 255;
  setOutputs();
  
}


void setOutputs(){
  unsigned char i;
  for(i=0; i<PWM_CHANNEL_COUNT; i++){
    analogWrite(pwmChannelMap[i], outputs[i]);
  }
}

//=================================================================================

int brightness = 0;

void loop(){
  // int val = analogRead(analogPin);

  for(brightness = 0; brightness <= 256; brightness++){
    outputs[0] = brightness;
    outputs[5] = 255 - brightness;
    setOutputs();
    delay(DELAY_BETWEEN_BRIGHTNESS_CHANGES);
  }

  for(brightness = 255; brightness >= 0; brightness--){
    outputs[0] = brightness;
    outputs[5] = 255 - brightness;
    setOutputs();
    delay(DELAY_BETWEEN_BRIGHTNESS_CHANGES);
  }

}
