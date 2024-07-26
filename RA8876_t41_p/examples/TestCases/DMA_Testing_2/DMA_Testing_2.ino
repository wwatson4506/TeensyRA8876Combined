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
#define BACKLITE 5                                   //External backlight control connected to this Arduino pin
RA8876_t3 tft = RA8876_t3(RA8876_CS, RA8876_RESET);  //Using standard SPI pins
#else
#if defined(ARDUINO_TEENSY_DEVBRD5)
#undef USE_MMOD_ATP_ADAPTER
//#define DB5_USE_CSI
//#define USE_DB5_SHIELD
//#define VSYNC_PIN 21

#ifdef USE_DB5_SHIELD
uint8_t dc = 55;
uint8_t cs = 53;
uint8_t rst = 54;
#else
uint8_t dc = 13;
uint8_t cs = 11;
uint8_t rst = 12;
#endif
#define BACKLITE 5  //External backlight control connected to this Arduino pin
//RA8876_t41_p lcd = RA8876_t41_p(dc,cs,rst); //(dc, cs, rst)
RA8876_t41_p lcd = RA8876_t41_p(dc, cs, rst);  //(dc, cs, rst)
#endif
#endif
uint32_t start = 0;
uint32_t end = 0;
uint8_t busSpeed = 12;

uint16_t *sdram_image;

void setup() {
    while (!Serial && millis() < 3000) {}  //wait for Serial Monitor
    Serial.printf("%c MicroMod Board and RA8876 parallel 8080 mode testing (8Bit/DMA)\n\n", 12);
    //  Serial.print(CrashReport);

    // Set 16bit mode
//      lcd.setBusWidth(16);
    // DB5.0 WR pin, RD pin, D0 pin.
      lcd.setFlexIOPins(53,52,40);

#if defined(use_spi)
    lcd.begin();
#else
#ifdef USE_DB5_SHIELD
    lcd.setBusWidth(16);
    lcd.setFlexIOPins(56, 52, 40);
#endif
    lcd.begin(busSpeed);  // 20 is working in 8bit and 16bit mode on T41
#endif
    delay(100);

    Serial.print("Bus speed: ");
    Serial.print(busSpeed, DEC);
    Serial.println(" MHZ");
    Serial.print("Bus Width: ");
    Serial.print(lcd.getBusWidth(), DEC);
    Serial.println("-bits");

    lcd.graphicMode(true);
    lcd.setRotation(0);

    sdram_image = (uint16_t*)sdram_malloc(1024*600*2);

    // lets fill it with RED, have a blue ring around edges. and Green one pixel around the edge.
    uint32_t x, y;
    uint16_t *pb = sdram_image;
    for (x = 0; x < (1024 * 600); x++) pb[x] = RED;

    for (x = 0; x < 1024; x++) pb[x] = GREEN;
   
    for (y = 1; y < 11; y++) {
        pb = &sdram_image[y * 1024];
        for (x = 1; x < 1023; x++) pb[x] = BLUE;
        pb[0] = GREEN;
        pb[1023] = GREEN;
    }
    for (; y < 589; y++) {
        pb = &sdram_image[y * 1024];
        pb[0] = GREEN;
        pb[1023] = GREEN;

        for (x = 1; x < 11; x++) pb[x] = BLUE;
        for (x = 1003; x < 1023; x++) pb[x] = BLUE;
    }
    for (; y < 599; y++) {
        pb = &sdram_image[y * 1024];
        for (x = 1; x < 1023; x++) pb[x] = BLUE;
        pb[0] = GREEN;
        pb[1023] = GREEN;
    }
    pb += 1024; // last row.
    for (x = 0; x < 1024; x++) pb[x] = GREEN;

}

void loop() {
    lcd.fillScreen(YELLOW);
    waitforInput();
    lcd.writeRect(0, 0, 1024, 600, sdram_image);
    waitforInput();
    lcd.fillScreen(MAGENTA);
    waitforInput();
    lcd.pushPixels16bitDMA(sdram_image, 0, 0, 1024, 600);  // FLASHMEM buffer
//    lcd.pushPixels16bitAsync(sdram_image, 0, 0, 1024, 600);  // FLASHMEM buffer
    waitforInput();
}

void waitforInput() {
    Serial.println("Press anykey to continue");
    while (Serial.read() == -1)
        ;
    while (Serial.read() != -1)
        ;
}
