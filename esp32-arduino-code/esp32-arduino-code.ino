
#include "FS.h"
#include "SD.h"

#include "FastLED.h"

#define file_count 1170

#define LED_BUILTIN 22

#define LED_PIN 21
#define NUM_LEDS 900

CRGB leds[NUM_LEDS];

#define bufflen 2700
char file_buff[bufflen];

void blink3times(){
  for(int i=0; i<3; i++){
        digitalWrite(LED_BUILTIN, 1);
        delay(300);
        digitalWrite(LED_BUILTIN, 0);
        delay(300);
  }
}

void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    
    Serial.printf("Listing directory: %s\n", dirname);
    File root = fs.open(dirname);
    if(!root){
        Serial.println("Failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println("Not a directory");
        return;
    }
    File file = root.openNextFile();
    while(file){
        digitalWrite(LED_BUILTIN, HIGH);
        file = root.openNextFile();
        digitalWrite(LED_BUILTIN, LOW);    
    }
}

void load_frame(fs::FS &fs, int f_no){
  
  char filename_base[]="/00000001";
  int index = 1;
  while (f_no>0){
    filename_base[9-index++] = '0' + (f_no%10);
    f_no/=10;
  }
  
  File file = fs.open(filename_base, "rb");
  file.readBytes((char *)leds, bufflen);
  //file.readBytes(file_buff, bufflen); 
  //memcpy(leds, file_buff, bufflen);  
  file.close();  
  //for(int i=0; i<10; i++)
    //Serial.printf("%d,%d,%d ", file_buff[i*3], file_buff[i*3+1], file_buff[i*3+2]);
}

void setup(){
    pinMode(LED_BUILTIN,OUTPUT);
    
    Serial.begin(115200);
    if(!SD.begin()){//
        Serial.println("Card Mount Failed");  
        blink3times();
        return;
    }
    uint8_t cardType = SD.cardType();

    if(cardType == CARD_NONE){
        Serial.println("No SD card attached");
        return;
    }
    Serial.print("SD Card Type: ");
    if(cardType == CARD_MMC){
        Serial.println("MMC");
    } else if(cardType == CARD_SD){
        Serial.println("SDSC");
    } else if(cardType == CARD_SDHC){
        Serial.println("SDHC");
    } else {
        Serial.println("UNKNOWN");
    }

    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    Serial.printf("SD Card Size: %lluMB\n", cardSize); 
    
    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
    
}

void loop(){
  long t1 = millis();
  for(int i=1; i<=file_count; i++){
  load_frame(SD, i);
  FastLED.show();
  //delay(2000);
  }
  t1 = millis() - t1;
  Serial.println(t1);
  //while(1);
}
