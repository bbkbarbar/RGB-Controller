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

#define PWM_OUTPUT_CH0                      3
#define PWM_OUTPUT_CH1                      5
#define PWM_OUTPUT_CH2                      6
#define PWM_OUTPUT_CH3                      9
#define PWM_OUTPUT_CH4                     10
#define PWM_OUTPUT_CH5                     11

#define POT1                               A0
#define POT2                               A1
#define POT3                               A2
#define POT4                               A3
#define POT5                               A4
#define POT6                               A5

#define POT_RED                          POT1
#define POT_GREEN                        POT2
#define POT_BLUE                         POT3
#define POT_SPEED                        POT4
#define POT_MIN                          POT5
#define POT_MAX                          POT6

#define RED                                 0
#define GREEN                               1
#define BLUE                                2

#define SIDE_1_RED                          0
#define SIDE_1_GREEN                        1
#define SIDE_1_BLUE                         2
#define SIDE_2_RED                          3
#define SIDE_2_GREEN                        4
#define SIDE_2_BLUE                         5

#define OFF                                 0
#define DEFAULT_OUTPUT_VALUE              OFF  // can set any value in range [0..255] depending on planned useage

#define PWM_CHANNEL_COUNT                   6  // based on the physical limitations of Arduino Nano
#define ADC_MAX                          1023  // max value of 10bit adc intput
#define PWM_MAX                           255  // max value of 8bit pwm output
#define START_DELAY_TO_SHOW_COLOR_IN_MS  1000 


unsigned char color[3] = {
  DEFAULT_OUTPUT_VALUE,
  DEFAULT_OUTPUT_VALUE,
  DEFAULT_OUTPUT_VALUE
};

const char pwmChannelMap[PWM_CHANNEL_COUNT] = {
  PWM_OUTPUT_CH0,
  PWM_OUTPUT_CH1,
  PWM_OUTPUT_CH2,
  PWM_OUTPUT_CH3,
  PWM_OUTPUT_CH4,
  PWM_OUTPUT_CH5
};

unsigned char outputs[PWM_CHANNEL_COUNT];     // Array for storing values of pwm channels

#define TOP  100
int MAX = 100;
int MIN =  10;

//=================================================================================
//=================================================================================


void readColor(){
  int value = analogRead(POT_RED);
  color[RED] = map(value, 0, ADC_MAX, 0, PWM_MAX); //Map ADC value to PWM value
  
  value = analogRead(POT_GREEN);
  color[GREEN] = map(value, 0, ADC_MAX, 0, PWM_MAX); //Map ADC value to PWM value

  value = analogRead(POT_BLUE);
  color[BLUE] = map(value, 0, ADC_MAX, 0, PWM_MAX); //Map ADC value to PWM value
}

unsigned int readDelay(){
  int value = analogRead(POT_SPEED);
  if(value == 0){
    return 1;
  }
  return (value / 4); // delay can be between 1 .. 256
}

#if defined(POT_MIN) && defined(POT_MAX)
void readBoundaryValues(){
  int value = analogRead(POT_MIN);
  MIN = map(value, 0, ADC_MAX, 0, TOP);

  value = analogRead(POT_MAX);
  MAX = map(value, 0, ADC_MAX, 1, TOP);

  if(MIN > MAX){
    MIN = MAX;
  }
}
#endif


void updateOutputs(){
  for(unsigned char i=0; i<PWM_CHANNEL_COUNT; i++){
    analogWrite(pwmChannelMap[i], outputs[i]);
  }
}

//=================================================================================

void setup() {

  // sets the pin for PWM outputs
  for(unsigned char i=0; i<PWM_CHANNEL_COUNT; i++){
    pinMode(pwmChannelMap[i], OUTPUT);
    outputs[i] = DEFAULT_OUTPUT_VALUE;
  }

  readColor();
  outputs[SIDE_1_RED]   = color[RED];
  outputs[SIDE_1_GREEN] = color[GREEN];
  outputs[SIDE_1_BLUE]  = color[BLUE];
  outputs[SIDE_2_RED]   = OFF;
  outputs[SIDE_2_GREEN] = OFF;
  outputs[SIDE_2_BLUE]  = OFF;
  updateOutputs();
  delay(START_DELAY_TO_SHOW_COLOR_IN_MS);

  readColor();
  outputs[SIDE_1_RED]   = OFF;
  outputs[SIDE_1_GREEN] = OFF;
  outputs[SIDE_1_BLUE]  = OFF;
  outputs[SIDE_2_RED]   = color[RED];
  outputs[SIDE_2_GREEN] = color[GREEN];
  outputs[SIDE_2_BLUE]  = color[BLUE];
  updateOutputs();
  delay(START_DELAY_TO_SHOW_COLOR_IN_MS);

  #if defined(POT_MIN) && defined(POT_MAX)
    readBoundaryValues();
  #endif
  
}

//=================================================================================

int factor1 = 0;
int factor2 = 0;

//#define COLOR_FACTOR_OF_GREEN     0.3
//#define COLOR_FACTOR_OF_BLUE      0.4

//=================================================================================
void loop(){

  readColor();
  #if defined(POT_MIN) && defined(POT_MAX)
    readBoundaryValues();
  #endif

  for(factor1 = MIN, factor2 = MAX; factor1 <= MAX; factor1++, factor2--){
    outputs[SIDE_1_RED]   = (factor1 * color[RED]  ) / TOP;
    outputs[SIDE_1_GREEN] = (factor1 * color[GREEN]) / TOP;
    outputs[SIDE_1_BLUE]  = (factor1 * color[BLUE] ) / TOP;
    outputs[SIDE_2_RED]   = (factor2 * color[RED]  ) / TOP;
    outputs[SIDE_2_GREEN] = (factor2 * color[GREEN]) / TOP;
    outputs[SIDE_2_BLUE]  = (factor2 * color[BLUE] ) / TOP;
    updateOutputs();
    delay(readDelay());
  }

  for(factor1 = MAX, factor2 = MIN; factor1 >= MIN; factor1--, factor2++){
    outputs[SIDE_1_RED]   = (factor1 * color[RED]  ) / TOP;
    outputs[SIDE_1_GREEN] = (factor1 * color[GREEN]) / TOP;
    outputs[SIDE_1_BLUE]  = (factor1 * color[BLUE] ) / TOP;
    outputs[SIDE_2_RED]   = (factor2 * color[RED]  ) / TOP;
    outputs[SIDE_2_GREEN] = (factor2 * color[GREEN]) / TOP;
    outputs[SIDE_2_BLUE]  = (factor2 * color[BLUE] ) / TOP;
    updateOutputs();
    delay(readDelay());
  }

}



