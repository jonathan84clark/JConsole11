/***************************************************
* JCONSOLE11
* DESC: JConsole11 is a handleheld video game system. The framework here
* is designed to work with the Adafruit ILI9341 driver. 
* 
* Author: Jonathan L Clark
* Date: 5/13/2021
*****************************************************/
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "GameObject.h"
#include "Models.h"

// For the Adafruit shield, these are the default.
#define TFT_DC 4
#define TFT_CS 1
#define TFT_MOSI 3
#define TFT_CLK 2
#define TFT_RST 5
#define TFT_MISO 0

#define NUM_BTNS 7
#define JOYSTICK_X A2
#define JOYSTICK_Y A1
#define JOYSTICK_BTN 22
#define SELECT_BTN 15
#define START_BTN 14
#define TOP_RIGHT 11
#define TOP_LEFT 8
#define BOTTOM_RIGHT 10
#define BOTTOM_LEFT 9
#define LEDS 21

#define ADC_MAX 4095

// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
//Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
// If using the breakout, change pins as desired
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);
unsigned int debounceTimes[NUM_BTNS];

void setup() {
  pinMode(7, OUTPUT);
  digitalWrite(7, HIGH);
  pinMode(JOYSTICK_X, INPUT);
  pinMode(JOYSTICK_Y, INPUT);
  pinMode(JOYSTICK_BTN, INPUT);
  pinMode(SELECT_BTN, INPUT_PULLUP);
  pinMode(START_BTN, INPUT_PULLUP);
  pinMode(TOP_RIGHT, INPUT_PULLUP);
  pinMode(TOP_LEFT, INPUT_PULLUP);
  pinMode(BOTTOM_RIGHT, INPUT_PULLUP);
  pinMode(BOTTOM_LEFT, INPUT_PULLUP);
  pinMode(LEDS, OUTPUT);
  Serial.begin(115200);
  Serial.println("ILI9341 Test!"); 
 
  tft.begin(200000000);

  // read diagnostics (optional but can help debug problems)
  uint8_t x = tft.readcommand8(ILI9341_RDMODE);
  Serial.print("Display Power Mode: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDMADCTL);
  Serial.print("MADCTL Mode: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDPIXFMT);
  Serial.print("Pixel Format: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDIMGFMT);
  Serial.print("Image Format: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDSELFDIAG);
  Serial.print("Self Diagnostic: 0x"); Serial.println(x, HEX); 
  uint16_t bgColor = COLOR_LTGREY;
  tft.fillScreen(bgColor);

   // Clear all debounce times
   for (int i = 0; i < NUM_BTNS; i++)
   {
      debounceTimes[i] = millis() + i;
   }
   //int16_t height = 21;
   //int16_t width = 24;
   int blasterIndex = 0;
   //GameObject gameObject(&tft, 100, 100, XWING_WIDTH, XWING_HEIGHT, xWing, bgColor);
   GameObject gameObject(&tft, 100, 100, TIE_WIDTH, TIE_HEIGHT, tieFighter, bgColor);
   GameObject blasters[5];
   Serial.println(sizeof(GameObject));
   //GameObject blaster(&tft, 50, 150, 10, 2, bgColor, COLOR_RED);
   gameObject.RotateRight();
   //gameObject.RotateDown();
   //gameObject.RotateDefault();
   //GameObject gameObject(&tft, 21, 24, 21, 24, newImage, COLOR_WHITE);
   unsigned long msTicks = 0;
   unsigned long nextTime = 0;
   unsigned long nextControlTime = 0;
   //gameObject.SetPhysics(10.0, 0.3, 9.81, 0.0, 0.003);
   //gameObject.SetVelocity(10.0, -30.0);
   //blaster.SetVelocity(-20, 0);
   while (true)
   {
       msTicks = millis();
       if (nextTime < msTicks)
       {
           gameObject.PhysicsMove();
           //blaster.PhysicsMove();
           nextTime = msTicks + 50;
       }
       // Joystick/ Menu button handler
       if (debounceTimes[0] < msTicks && digitalRead(JOYSTICK_BTN))
       {
           // Handle joystick btn pressed
           debounceTimes[0] = msTicks + 200; // Right now 200ms debounce time
       }
       // Start button handler
       if (debounceTimes[1] < msTicks && digitalRead(START_BTN))
       {
           // Handle joystick btn pressed
           debounceTimes[1] = msTicks + 200; // Right now 200ms debounce time
       }
       // Select button handler
       if (debounceTimes[2] < msTicks && digitalRead(SELECT_BTN))
       {
           // Handle joystick btn pressed
           debounceTimes[2] = msTicks + 200; // Right now 200ms debounce time
       }
       // Fire button
       if (debounceTimes[3] < msTicks && digitalRead(TOP_LEFT))
       {
           // User fired
           //blasters[blasterIndex++].ActivateSolid(&tft, 50, 150, 10, 2, bgColor, COLOR_RED);
           if (blasterIndex == 20)
           {
              blasterIndex = 0;
           }
           //blasters[blasterIndex](&tft, 50, 150, 10, 2, bgColor, COLOR_RED);
           debounceTimes[3] = msTicks + 200; // Right now 200ms debounce time
       }
       // Top right button handler
       if (debounceTimes[4] < msTicks && digitalRead(TOP_RIGHT))
       {
           debounceTimes[4] = msTicks + 200; // Right now 200ms debounce time
       }
       // Bottom left button handler
       if (debounceTimes[5] < msTicks && digitalRead(BOTTOM_LEFT))
       {
           debounceTimes[5] = msTicks + 200; // Right now 200ms debounce time
       }
       // Bottom right button handler
       if (debounceTimes[6] < msTicks && digitalRead(BOTTOM_RIGHT))
       {
           debounceTimes[6] = msTicks + 200; // Right now 200ms debounce time
       }
       if (nextControlTime < msTicks)
       {
           float xScaler = (float)analogRead(JOYSTICK_X) - (float)(ADC_MAX / 2);
           xScaler = xScaler / (float)(ADC_MAX / 2);
           float yScaler = (float)analogRead(JOYSTICK_Y) - (float)(ADC_MAX / 2);
           yScaler = yScaler / (float)(ADC_MAX / 2);

           gameObject.SetVelocity(xScaler * 20.0, yScaler * 20.0);
           nextControlTime = msTicks + 50;
       }
   }

}


void loop(void) 
{
  /*
  for(uint8_t rotation=0; rotation<4; rotation++) {
    tft.setRotation(rotation);
    testText();
    delay(1000);
  }
  */
}

unsigned long testFillScreen() {
  unsigned long start = micros();
  tft.fillScreen(ILI9341_BLACK);
  yield();
  tft.fillScreen(ILI9341_RED);
  yield();
  tft.fillScreen(ILI9341_GREEN);
  yield();
  tft.fillScreen(ILI9341_BLUE);
  yield();
  tft.fillScreen(ILI9341_BLACK);
  yield();
  return micros() - start;
}

unsigned long testText() {
  tft.fillScreen(ILI9341_BLACK);
  unsigned long start = micros();
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(1);
  tft.println("Hello World!");
  tft.setTextColor(ILI9341_YELLOW); tft.setTextSize(2);
  tft.println(1234.56);
  tft.setTextColor(ILI9341_RED);    tft.setTextSize(3);
  tft.println(0xDEADBEEF, HEX);
  tft.println();
  tft.setTextColor(ILI9341_GREEN);
  tft.setTextSize(5);
  tft.println("Groop");
  tft.setTextSize(2);
  tft.println("I implore thee,");
  tft.setTextSize(1);
  tft.println("my foonting turlingdromes.");
  tft.println("And hooptiously drangle me");
  tft.println("with crinkly bindlewurdles,");
  tft.println("Or I will rend thee");
  tft.println("in the gobberwarts");
  tft.println("with my blurglecruncheon,");
  tft.println("see if I don't!");
  return micros() - start;
}

unsigned long testLines(uint16_t color) {
  unsigned long start, t;
  int           x1, y1, x2, y2,
                w = tft.width(),
                h = tft.height();

  tft.fillScreen(ILI9341_BLACK);
  yield();
  
  x1 = y1 = 0;
  y2    = h - 1;
  start = micros();
  for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, color);
  x2    = w - 1;
  for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, color);
  t     = micros() - start; // fillScreen doesn't count against timing

  yield();
  tft.fillScreen(ILI9341_BLACK);
  yield();

  x1    = w - 1;
  y1    = 0;
  y2    = h - 1;
  start = micros();
  for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, color);
  x2    = 0;
  for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, color);
  t    += micros() - start;

  yield();
  tft.fillScreen(ILI9341_BLACK);
  yield();

  x1    = 0;
  y1    = h - 1;
  y2    = 0;
  start = micros();
  for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, color);
  x2    = w - 1;
  for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, color);
  t    += micros() - start;

  yield();
  tft.fillScreen(ILI9341_BLACK);
  yield();

  x1    = w - 1;
  y1    = h - 1;
  y2    = 0;
  start = micros();
  for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, color);
  x2    = 0;
  for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, color);

  yield();
  return micros() - start;
}

unsigned long testFastLines(uint16_t color1, uint16_t color2) {
  unsigned long start;
  int           x, y, w = tft.width(), h = tft.height();

  tft.fillScreen(ILI9341_BLACK);
  start = micros();
  for(y=0; y<h; y+=5) tft.drawFastHLine(0, y, w, color1);
  for(x=0; x<w; x+=5) tft.drawFastVLine(x, 0, h, color2);

  return micros() - start;
}

unsigned long testRects(uint16_t color) {
  unsigned long start;
  int           n, i, i2,
                cx = tft.width()  / 2,
                cy = tft.height() / 2;

  tft.fillScreen(ILI9341_BLACK);
  n     = min(tft.width(), tft.height());
  start = micros();
  for(i=2; i<n; i+=6) {
    i2 = i / 2;
    tft.drawRect(cx-i2, cy-i2, i, i, color);
  }

  return micros() - start;
}

unsigned long testFilledRects(uint16_t color1, uint16_t color2) {
  unsigned long start, t = 0;
  int           n, i, i2,
                cx = tft.width()  / 2 - 1,
                cy = tft.height() / 2 - 1;

  tft.fillScreen(ILI9341_BLACK);
  n = min(tft.width(), tft.height());
  for(i=n; i>0; i-=6) {
    i2    = i / 2;
    start = micros();
    tft.fillRect(cx-i2, cy-i2, i, i, color1);
    t    += micros() - start;
    // Outlines are not included in timing results
    tft.drawRect(cx-i2, cy-i2, i, i, color2);
    yield();
  }

  return t;
}

unsigned long testFilledCircles(uint8_t radius, uint16_t color) {
  unsigned long start;
  int x, y, w = tft.width(), h = tft.height(), r2 = radius * 2;

  tft.fillScreen(ILI9341_BLACK);
  start = micros();
  for(x=radius; x<w; x+=r2) {
    for(y=radius; y<h; y+=r2) {
      tft.fillCircle(x, y, radius, color);
    }
  }

  return micros() - start;
}

unsigned long testCircles(uint8_t radius, uint16_t color) {
  unsigned long start;
  int           x, y, r2 = radius * 2,
                w = tft.width()  + radius,
                h = tft.height() + radius;

  // Screen is not cleared for this one -- this is
  // intentional and does not affect the reported time.
  start = micros();
  for(x=0; x<w; x+=r2) {
    for(y=0; y<h; y+=r2) {
      tft.drawCircle(x, y, radius, color);
    }
  }

  return micros() - start;
}

unsigned long testTriangles() {
  unsigned long start;
  int           n, i, cx = tft.width()  / 2 - 1,
                      cy = tft.height() / 2 - 1;

  tft.fillScreen(ILI9341_BLACK);
  n     = min(cx, cy);
  start = micros();
  for(i=0; i<n; i+=5) {
    tft.drawTriangle(
      cx    , cy - i, // peak
      cx - i, cy + i, // bottom left
      cx + i, cy + i, // bottom right
      tft.color565(i, i, i));
  }

  return micros() - start;
}

unsigned long testFilledTriangles() {
  unsigned long start, t = 0;
  int           i, cx = tft.width()  / 2 - 1,
                   cy = tft.height() / 2 - 1;

  tft.fillScreen(ILI9341_BLACK);
  start = micros();
  for(i=min(cx,cy); i>10; i-=5) {
    start = micros();
    tft.fillTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
      tft.color565(0, i*10, i*10));
    t += micros() - start;
    tft.drawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
      tft.color565(i*10, i*10, 0));
    yield();
  }

  return t;
}

unsigned long testRoundRects() {
  unsigned long start;
  int           w, i, i2,
                cx = tft.width()  / 2 - 1,
                cy = tft.height() / 2 - 1;

  tft.fillScreen(ILI9341_BLACK);
  w     = min(tft.width(), tft.height());
  start = micros();
  for(i=0; i<w; i+=6) {
    i2 = i / 2;
    tft.drawRoundRect(cx-i2, cy-i2, i, i, i/8, tft.color565(i, 0, 0));
  }

  return micros() - start;
}

unsigned long testFilledRoundRects() {
  unsigned long start;
  int           i, i2,
                cx = tft.width()  / 2 - 1,
                cy = tft.height() / 2 - 1;

  tft.fillScreen(ILI9341_BLACK);
  start = micros();
  for(i=min(tft.width(), tft.height()); i>20; i-=6) {
    i2 = i / 2;
    tft.fillRoundRect(cx-i2, cy-i2, i, i, i/8, tft.color565(0, i, 0));
    yield();
  }

  return micros() - start;
}
