#include "KnightUino2000.h"

KnightUino2000 KnightUino2000(33, 32, 26, 25, 16, 4, 17);

int segment=1;

void setup() {
  KnightUino2000.boot();

  Serial.begin(115200);
  delay(2000);
}

void loop() {
  //cycleThroughColors();
  KnightUino2000.scanner(2, 100, 1.0);
  //KnightUino2000.setOneAndOtherRGB(2, 100, 100, 0.1, 360, 100, 1.0);

}

void cycleThroughColors(){
    //define your saturation and brightness level here. (0.0 - 1.0)
    float saturation = 1.0;
    float brightness = 0.1;
    
    for(int hue = 0; hue < 360; hue++){ 
        //convert hue to float
        float hue_float = hue; 
        KnightUino2000.setAllRGB(7, 100, brightness);
        delay(5);
        KnightUino2000.setOneRGB(segment, hue_float, saturation, 1.0);
        delay(3); // delay between color change. Decrease delay to speed up, increase to slow down.
    }
    segment++;
    if(segment>8)
    {segment=1;}
}
