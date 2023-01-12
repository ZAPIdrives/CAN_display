// Scott's test code
// CAN communication with Feather M4 CAN
//incorporating display

#include <SPI.h>
#include "Adafruit_GFX.h"
#include "Adafruit_HX8357.h"
#include <limits.h>
#include <CAN.h>
#include <OBD2.h>

// defining the pins for the display
#define STMPE_CS 6
#define TFT_CS   9
#define TFT_DC   10
#define SD_CS    5
#define TFT_RST -1

// Use hardware SPI and the above for CS/DC
Adafruit_HX8357 tft = Adafruit_HX8357(TFT_CS, TFT_DC, TFT_RST);

//variables
int oil_temp = 0;

//set dimensions of the canvas (width, height)
//GFXcanvas1 canvas(30,20);

void setup() {
  Serial.begin(115200);
  
  Serial.println("HX8357D Test!");
  Serial.println("CAN receiver");

  //configure M4 CAN for CAN communication
  pinMode(PIN_CAN_STANDBY, OUTPUT);
  digitalWrite(PIN_CAN_STANDBY, false); // turn off STANDBY
  pinMode(PIN_CAN_BOOSTEN, OUTPUT);
  digitalWrite(PIN_CAN_BOOSTEN, true); // turn on booster
 
  // start the CAN bus at 250 kbps
  if (!CAN.begin(250000)) {
    Serial.println("Starting CAN failed!");
    while (1);
  }
  
  //start the display
  tft.begin(); 
  tft.fillScreen(0x0000);            //clear display
  tft.setRotation(3);                //set the displays rotation

  tft.setTextColor(0xFFFF, 0x0000); //white text, black background
  tft.setTextSize(2);           //doubles the text size
  delay(500);
}


void loop(void) {
  int i = 8;
  while(i>0){
    i--;
    oil_temp = CANread(1349,4);
    if (i == 1){
      testText();
      Serial.print("value of byte 4 in the code is: ");
      Serial.println(oil_temp);
    }
  }
  delay(2000);
}

unsigned long testText() {
  
 // tft.fillScreen(HX8357_BLACK);
  unsigned long start = micros();
  tft.setCursor(0, 0);
//  tft.setTextColor(HX8357_WHITE);  
 // tft.setTextSize(1);
  tft.println("Hello World!");

  tft.setCursor(50,100);
 // tft.setTextColor(HX8357_GREEN);
  //tft.setTextSize(3);
  tft.println("CAN ID: 0x545 ");

  //message within CAN_Id
  tft.setCursor(50,150);
  //tft.setTextColor(HX8357_WHITE);
  //tft.setTextSize(2);
  tft.print("Message contains: ");
  tft.println(oil_temp);
  Serial.print("byte 4 message on display is: 0x");
  Serial.println(oil_temp);

  return micros() - start;
}

int CANread(int CAN_Id, int CAN_byte) {
  byte myArray[8];
  byte CAN_msg;
  
  // try to parse packet
  int packetSize = CAN.parsePacket();
  if (packetSize) {
    // received a packet
    if (CAN.packetId() == CAN_Id) {
      for (int i=0; i<8; i++) {
      myArray[i] = (byte)CAN.read(); //fill an array with the bytes from CAN_Id
      }
      }
    } else{
      return CAN_msg = 0;
    }
    CAN_msg = myArray[CAN_byte];
    return CAN_msg;
}
