/**
 * KnightUino2000.h
 * 
 * KnightUino, a library to hack the cheap controller for the popular chinese K2000 hood scanner
 * 
 * @Author RJE
 * @version 1.0.0
 */

#ifndef KnightUino2000_h
#define KnightUino2000_h

#include <output_expand74XX595.h>
#include <math.h>
#include <ESP32Time.h>


class KnightUino2000
{
  private:

    const int PWM_FREQ_HZ = 5000;  // PWM frequency
    const int PWM_RED = 1;   // Channel for redPin
    const int PWM_GREEN = 2;   // Channel for greenPin
    const int PWM_BLUE = 3;   // Channel for bluePin
    const int PWM_RESOLUTION = 8;  // Resolution 8 bits
    int _redPin;
    int _greenPin;
    int _bluePin;
    int _segment = 1;
    bool _increasing = true;
    uint8_t * _pos;
    float _segments[8][3];
    output_expand74XX595 _output;

    void initializeOutputs(int redPin, int greenPin, int bluePin);
    void hsvToRgb(float h, float s, float v, unsigned int& r, unsigned int& g, unsigned int& b);
    uint8_t * convertToInt8(int number);
  
  public:
    KnightUino2000(int latchPin, int clockPin, int dataPin, int OE, int Red, int Green, int Blue);
    void setAllRGB(float h, float s, float v);
    void setOneRGB(int segment, float h, float s, float v);
    void setOneAndOtherRGB(int segment, float one_h, float one_s, float one_v, float other_h, float other_s, float other_v);
    void scanner(float h, float s, float v);
    void scannerComplete(float h, float s, float v);
    void updateSegments();
    void boot();

};

#endif