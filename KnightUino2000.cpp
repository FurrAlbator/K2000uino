#include "esp32-hal.h"
/**
 * KnightUino2000.cpp
 * 
 * KnightUino, a library to hack the cheap controller for the popular chinese K2000 hood scanner
 * 
 * @Author RJE
 * @version 1.0.0
 */

#include "KnightUino2000.h"

int boot1 = 24;
int boot2 = 60;
int boot3 = 126;
int boot4 = 255;
int boot5 = 231;  
int boot6 = 195;
int boot7 = 129;
int boot8 = 0;

int ScannerFull[8][2] = {
  {64, 128},
  {160, 64},
  {80, 32},
  {40, 16},
  {20, 8},
  {10, 4},
  {5, 2},
  {2, 1}
};


KnightUino2000::KnightUino2000(int latchPin, int clockPin, int dataPin, int noePin, int redPin, int greenPin, int bluePin)
  : _output(1, dataPin, clockPin, latchPin, noePin) {
  _redPin = redPin;
  _greenPin = greenPin;
  _bluePin = bluePin;
  _segments[0][0]=2;
  _segments[0][1]=100;
  _segments[0][2]=1.0;
  _segments[1][0]=100;
  _segments[1][1]=100;
  _segments[1][2]=1.0;
  _segments[2][0]=2;
  _segments[2][1]=100;
  _segments[2][2]=1.0;
  _segments[3][0]=200;
  _segments[3][1]=100;
  _segments[3][2]=1.0;

  initializeOutputs(_redPin, _greenPin, _bluePin);
}

void KnightUino2000::initializeOutputs(int redPin, int greenPin, int bluePin) {
  ledcSetup(PWM_RED, PWM_FREQ_HZ, PWM_RESOLUTION);
  ledcSetup(PWM_GREEN, PWM_FREQ_HZ, PWM_RESOLUTION);
  ledcSetup(PWM_BLUE, PWM_FREQ_HZ, PWM_RESOLUTION);

  ledcAttachPin(_redPin, PWM_RED);
  ledcAttachPin(_greenPin, PWM_GREEN);
  ledcAttachPin(_bluePin, PWM_BLUE);
}

void KnightUino2000::setAllRGB(float h, float s, float v) {
  _output.setAllHigh();
  unsigned int r, g, b;
  hsvToRgb(h,s,v,r,g,b);
  ledcWrite(PWM_RED, r);
  ledcWrite(PWM_GREEN, g);
  ledcWrite(PWM_BLUE, b);
}

void KnightUino2000::setOneRGB(int segment, float h, float s, float v) {
  _output.setPin(segment);
  unsigned int r, g, b;
  hsvToRgb(h,s,v,r,g,b);
  ledcWrite(PWM_RED, r);
  ledcWrite(PWM_GREEN, g);
  ledcWrite(PWM_BLUE, b);
}

void KnightUino2000::setOneAndOtherRGB(int segment, float one_h, float one_s, float one_v, float other_h, float other_s, float other_v) {
  unsigned int one_r, one_g, one_b;
  unsigned int other_r, other_g, other_b;
  hsvToRgb(one_h,one_s,one_v,one_r,one_g,one_b);
  hsvToRgb(other_h,other_s,other_v,other_r,other_g,other_b);
  int mask = ~(1 << segment - 1);
  //_output.setMultipleHigh(mask);
  ledcWrite(PWM_RED, one_r);
  ledcWrite(PWM_GREEN, one_g);
  ledcWrite(PWM_BLUE, one_b);
  delay(1);
  //_output.setMultipleHigh(255-mask);
  ledcWrite(PWM_RED, other_r);
  ledcWrite(PWM_GREEN, other_g);
  ledcWrite(PWM_BLUE, other_b);
  delay(2);
}

void KnightUino2000::hsvToRgb(float h, float s, float v, unsigned int& r, unsigned int& g, unsigned int& b) {
  float hh, p, q, t, ff;
  long i;

  if(s <= 0.0) { // < is bogus, just shuts up warnings
    r = v;
    g = v;
    b = v;
  }

  hh = h;
  if(hh >= 360.0) hh = 0.0;
  hh /= 60.0;
  i = (long)hh;
  ff = hh - i;
  p = v * (1.0 - s);
  q = v * (1.0 - (s * ff));
  t = v * (1.0 - (s * (1.0 - ff)));

  switch(i){
    case 0:
      r = v * 255; g = t * 255; b = p * 255; 
      break;
    case 1: 
      r = q * 255; g = v * 255; b = p * 255; 
      break;
    case 2: 
      r = p * 255; g = v * 255; b = t * 255; 
      break;
    case 3: 
      r = p * 255; g = q * 255; b = v * 255; 
      break;
    case 4: 
      r = t * 255; g = p * 255; b = v * 255; 
      break;
    case 5: 
      r = v * 255; g = p * 255; b = q * 255;
      break;
  }
}

void KnightUino2000::boot()
{
  unsigned int r, g, b;
  hsvToRgb(2,100,1.0,r,g,b);
  ledcWrite(PWM_RED, r);
  ledcWrite(PWM_GREEN, g);
  ledcWrite(PWM_BLUE, b);
  _output.setAll(convertToInt8(boot1));
  delay(250);
  _output.setAll(convertToInt8(boot2));
  delay(250);
  _output.setAll(convertToInt8(boot3));
  delay(250);
  _output.setAll(convertToInt8(boot4));
  delay(250);
  _output.setAll(convertToInt8(boot5));
  delay(250);
  _output.setAll(convertToInt8(boot6));
  delay(250);
  _output.setAll(convertToInt8(boot7));
  delay(250);
  _output.setAll(convertToInt8(boot8));
  delay(500);

}

void KnightUino2000::scanner(float h, float s, float v)
{
  setOneRGB(_segment, h, s, v);
  if(_increasing) 
  {
    _segment++;
    if (_segment >= 8) 
    {
      _increasing = false;
    }
  } 
  else 
  {
    _segment--;
    if (_segment <= 1) 
    {
      _increasing = true;
    }
  }
  delay(160);
}

void KnightUino2000::scannerComplete(float h, float s, float v)
{
  unsigned int r, g, b;
  hsvToRgb(2,100,1.0,r,g,b);
  ledcWrite(PWM_RED, r);
  ledcWrite(PWM_GREEN, g);
  ledcWrite(PWM_BLUE, b);
  _output.setAll(convertToInt8(ScannerFull[_segment][0])); //Line to do the faded ones
  _output.setAll(convertToInt8(ScannerFull[_segment][1])); //Line to do the main one
  
  delay(20);
  delay(140);

  if(_increasing) 
  {
    _segment++;
    if (_segment >= 7) 
    {
      _increasing = false;
    }
  } 
  else 
  {
    _segment--;
    if (_segment <= 0) 
    {
      _increasing = true;
    }
  }
  
}

uint8_t* KnightUino2000::convertToInt8(int number) {
  static uint8_t result;
  
  // Ensure number is within valid range
  if (number < 0)
    number = 0;
  else if (number > 255)
    number = 255;
  
  result = static_cast<uint8_t>(number);
  
  return &result;
}