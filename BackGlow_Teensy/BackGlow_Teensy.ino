#include <Adafruit_NeoPixel.h>

#define PIN 27
#define MAXPIXELS 30

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(MAXPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
    pixels.begin();
}

void loop() {
    if(Serial.available()){
        uint8_t len = Serial.read();
        if(len > MAXPIXELS) len = MAXPIXELS;
        for(uint8_t i = 0; i < len; ++i) {
            uint8_t r, g, b;
            r = Serial.read();
            g = Serial.read();
            b = Serial.read();
            pixels.setPixelColor(i, r, g, b); 
        } 
    }

    pixels.show();
}