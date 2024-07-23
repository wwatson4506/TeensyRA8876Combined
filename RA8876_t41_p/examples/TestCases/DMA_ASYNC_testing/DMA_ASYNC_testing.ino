#include "flexio_teensy_mm.c"
#include "teensy41.c"

//#define use_spi
#if defined(use_spi)
#include <SPI.h>
#include <RA8876_t3.h>
#else
//#include <RA8876_t3.h>
#include <RA8876_t41_p.h>
#endif
//#include <math.h>

#if defined(use_spi)
#define RA8876_CS 10
#define RA8876_RESET 9
#define BACKLITE 5 //External backlight control connected to this Arduino pin
RA8876_t3 tft = RA8876_t3(RA8876_CS, RA8876_RESET); //Using standard SPI pins
#else
uint8_t dc = 13;
uint8_t cs = 11;
uint8_t rst = 12;
#define BACKLITE 5 //External backlight control connected to this Arduino pin
RA8876_t41_p lcd = RA8876_t41_p(dc,cs,rst); //(dc, cs, rst)
#endif

uint32_t start = 0;
uint32_t end =  0;
uint8_t busSpeed = 12; // 12MHz max in 16-bit mode. 20 or greater in 8-bit mode.

void setup() {
  while (!Serial && millis() < 3000) {} //wait for Serial Monitor
  Serial.printf("%c DB5 board and RA8876 parallel 8080 mode testing (8-Bit/16-bit,DMA/ASYNC)\n\n",12);
//  Serial.print(CrashReport);

  // Un-comment to set 16bit mode
  lcd.setBusWidth(16);
  // DB5.0 WR pin, RD pin, D0 pin.
  // Set for DB5 board.
  lcd.setFlexIOPins(53,52,40);  //Set this to match your board configuration.

#if defined(use_spi)
  lcd.begin(); 
#else
  lcd.begin(busSpeed); // 20 is working in 8bit and 16bit mode on T41
#endif
//  if(!lcd.begin(busSpeed)) Serial.printf("lcd.begin(busSpeed) FAILED!!!\n");
  delay(100);

  Serial.print("Bus speed: ");
  Serial.print(busSpeed,DEC);
  Serial.println(" MHZ");
  Serial.print("Bus Width: ");
  Serial.print(lcd.getBusWidth(),DEC);
  Serial.println("-bits");

  lcd.graphicMode(true);
  lcd.fillScreen(0x0000);
  lcd.setRotation(0);
}

void loop() {
  start = micros();

  lcd.pushPixels16bitAsync(flexio_teensy_mm,530,260,480,320); // 480x320
  lcd.pushPixels16bitDMA(teensy41,1,1,480,320);    // FLASHMEM buffer

  end = micros() - start;
  Serial.printf("Wrote %d bytes in %dus\n\n",(575*424), end);
  waitforInput();
}

void waitforInput()
{
  Serial.println("Press anykey to continue");
  while (Serial.read() == -1) ;
  while (Serial.read() != -1) ;
}
