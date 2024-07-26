#include <Adafruit_GFX.h>
#include <_font_ComicSansMS.h>
#include "Fonts/FreeSansOblique12pt7b.h"
#include "RA8876_Config.h"

#include "Arduino.h"
#if defined(USE_SPI)
#include <SPI.h>
#include <RA8876_t3.h>
#else
#include <RA8876_t41_p.h>
#endif

#if defined(USE_SPI)
#define RA8876_CS 10
#define RA8876_RESET 9
#define BACKLITE 7 //External backlight control connected to this Arduino pin
RA8876_t3 tft = RA8876_t3(RA8876_CS, RA8876_RESET); //Using standard SPI pins
#else
uint8_t dc = 13;
uint8_t cs = 11;
uint8_t rst = 12;
RA8876_t41_p tft = RA8876_t41_p(dc,cs,rst); //(dc, cs, rst)
#endif

void setup() {
#ifdef BACKLITE
  pinMode(BACKLITE, OUTPUT);
  digitalWrite(BACKLITE, HIGH);
#endif
  Serial.begin(38400);
  long unsigned debug_start = millis ();
  while (!Serial && ((millis () - debug_start) <= 5000)) ;
  Serial.println("Setup");

#if !defined(USE_SPI)
  // Set 16bit mode
//  tft.setBusWidth(16);
  // DB5.0 WR pin, RD pin, D0 pin.
//  tft.setFlexIOPins(53,52,40);
#endif

#if defined(USE_SPI)
  tft.begin(); // default SPI clock speed is 30000000 MHz 
//  tft.begin(47000000); // Max is 47000000 MHz (using short 3" wires)
#else
  tft.begin(20);// 20 is working in 8bit and 16bit mode on T41
#endif

#ifndef BACKLITE
  tft.backlight(true);
  tft.fillScreen(RED);
  delay(2000);
  tft.pwm0_Duty(0x0000);
  delay(2000);
  tft.pwm0_Duty(0x0ff0);
  delay(2000);
  tft.pwm0_Duty(0xffff);
  delay(2000);
#endif

  //tft.setRotation(1);
  tft.fillScreen(BLACK);

  tft.setTextColor(WHITE);
  tft.println("Test of the default font");
  tft.println();
//  wait_for_keyboard();

  tft.setTextColor(WHITE, BLUE);
  tft.setFont(ComicSansMS_18);
  tft.println("Opaque ILI font ComicSansMS_18");
  tft.println();

//  wait_for_keyboard();
  int cursor_x = tft.getCursorX();
  int cursor_y = tft.getCursorY();
  tft.fillRect(cursor_x, cursor_y, tft.width(), 20, RED);
//  wait_for_keyboard();
  tft.setTextColor(GREEN);
  tft.println("Transparent ILI ComicSansMS_18");
  tft.println();
//  wait_for_keyboard();

  tft.setTextColor(WHITE, RED);
  tft.setFont(&FreeSansOblique12pt7b);
  tft.println("Opaque GFX FreeSansOblique12pt");
//  wait_for_keyboard();

  cursor_x = tft.getCursorX();
  cursor_y = tft.getCursorY();
  tft.fillRect(cursor_x, cursor_y, tft.width(), 20, YELLOW);
//  wait_for_keyboard();
  tft.setTextColor(BLUE);
  tft.println("Transparent GFX");

//  wait_for_keyboard();
  tft.setFontDef();
  tft.setTextColor(GREEN);
  tft.setFontSize(1, true);
  tft.println("This is default font:");
  //tft.setFontSpacing(1);//now give 5 pix extra spacing between chars
  tft.println("ABCDEF 1 2 3 4 5 6 7");

}

void wait_for_keyboard() {
  Serial.println("Wait for any key to continue");
  while (Serial.read() == -1);
  while (Serial.read() != -1);
}

void loop()
{  }
