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
#include "Solid.h"
#include "GameObject.h"
#include "Models.h"
#include "Bar.h"
#include "Menu.h"
#include "Config.h"

// For the Adafruit shield, these are the default.
#define TFT_DC 4
#define TFT_CS 1
#define TFT_MOSI 3
#define TFT_CLK 2
#define TFT_RST 5
#define TFT_MISO 0

static uint16_t randomColors[] = {COLOR_BLUE, COLOR_RED, COLOR_MAGENTA, COLOR_ORANGE, COLOR_FORESTGREEN, COLOR_DRKGREY};

Solid blasters[20];
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
  randomSeed(analogRead(0));
  pinMode(LEDS, OUTPUT);
  Serial.begin(115200);
  Serial.println("ILI9341 Test!"); 
 
  tft.begin(20000000);

  uint16_t bgColor = COLOR_SKYBLUE;
  tft.fillScreen(bgColor);
  tft.setRotation(1);

   // Clear all debounce times
   for (int i = 0; i < NUM_BTNS; i++)
   {
      debounceTimes[i] = millis() + i;
   }
   int numBlocks = 10;
   int numBlasters = 20;
   int blasterIndex = 0;

   GameObject player2(&tft, 100, 50, XWING_WIDTH, XWING_HEIGHT, xWing, bgColor);
   Bar healthBar(&tft, 235, 5, 12, 80, COLOR_BLUE, COLOR_RED, bgColor);
   Menu menuSystem(&tft);
   
   Solid blasters[numBlasters];
   Solid blocks[numBlocks];
   float originalHealth = 100;
   float currentHealth = originalHealth;

   unsigned long msTicks = 0;
   unsigned long nextTime = 0;
   unsigned long nextControlTime = 0;
   int16_t firePoint = 0;
   unsigned int fireDebounce = 0;
   unsigned int nextBlockTime = 0;
   int colorIndex = 0;
   int16_t startYPos = 10;
   unsigned int score = 0;

   tft.setCursor(2, 2);
   tft.setTextColor(ILI9341_BLUE);  
   tft.setTextSize(2);
   tft.print("Score: ");
   tft.setCursor(75, 2);
   tft.print(score);
   
   while (true)
   {
       msTicks = millis();
       if (nextTime < msTicks)
       {
           player2.PhysicsMove();
           for (int k = 0; k < numBlocks; k++)
           {
               uint8_t collision = player2.CheckCollision(&blocks[k]);
               if (collision)
               {
                  currentHealth -= 3;
                  if (currentHealth < 0.0)
                  {
                     currentHealth = 0.0;
                  }
                  healthBar.Update(currentHealth / originalHealth);
                  blocks[k].Disable();
               }
           }
           nextTime = msTicks + 50;
       }
       // Load balancing, each gameobject only moves at certain times
       for (int i = 0; i < numBlocks; i++)
       {
           if (blocks[i].getActive() && blocks[i].getNextPhysicsTime() < msTicks)
           {
               blocks[i].PhysicsMove(msTicks);
           }
       }
       for (int i = 0; i < numBlasters; i++)
       {
           if (blasters[i].getActive() && blasters[i].getNextPhysicsTime() < msTicks)
           {
               blasters[i].PhysicsMove(msTicks);
               // See if this blaster has hit any blocks
               for (int j = 0; j < numBlocks; j++)
               {
                   uint8_t collision = blasters[i].CheckCollision(&blocks[j]);
                   if (collision)
                   {
                      blasters[i].Disable();
                      blocks[j].Disable();
                      tft.setTextColor(bgColor);  
                      tft.setTextSize(2);
                      tft.setCursor(75, 2);
                      tft.print(score);
                      score++;
                      tft.setTextColor(ILI9341_BLUE);  
                      tft.setCursor(75, 2);
                      tft.print(score);
                   }
               }
           }
       }
       if (nextBlockTime < msTicks)
       {
           for (int i = 0; i < numBlocks; i++)
           {
               if (!blocks[i].getActive())
               {
                  int16_t randomY = random(20, 230);
                  colorIndex++;
                  if (colorIndex > 5)
                  {
                     colorIndex = 0;
                  }
                  uint16_t selColor = randomColors[colorIndex];
                  blocks[i].Activate(&tft, 300, randomY, 12, 12, selColor, bgColor);
                  blocks[i].SetVelocity(0.0, 10.0);
                  blocks[i].SetBehavior(true, 0);
                  
                  break;
               }
           }
            nextBlockTime = msTicks + 600;
       }
       // Joystick/ Menu button handler
       if (debounceTimes[0] < msTicks && digitalRead(JOYSTICK_BTN))
       {
           // Handle joystick btn pressed
           menuSystem.Pause();
           debounceTimes[0] = msTicks + 200; // Right now 200ms debounce time
       }
       // Start button handler
       if (debounceTimes[1] < msTicks && digitalRead(START_BTN) == 0)
       {
           // Handle joystick btn pressed
           debounceTimes[1] = msTicks + 200; // Right now 200ms debounce time
       }
       // Select button handler
       if (debounceTimes[2] < msTicks && digitalRead(SELECT_BTN) == 0)
       {
           // Handle joystick btn pressed
           debounceTimes[2] = msTicks + 200; // Right now 200ms debounce time
       }
       // Fire button
       if (fireDebounce < msTicks && digitalRead(TOP_LEFT) == 0)
       {
           // User fired
           int16_t xFirePos = player2.getYPos() + 5.0;
           int16_t yFirePos = player2.getXPos();
           float shotVelocity = -40.0;

           if (firePoint == 1)
           {
               yFirePos = yFirePos + player2.getHeight();
           }
           firePoint++;
           if (firePoint == 2)
           {
              firePoint = 0;
           }

           if (player2.getRotation() == LEFT)
           {
               shotVelocity *= -1.0;
               xFirePos = player2.getYPos() - 5.0;
           }
           blasters[blasterIndex].Activate(&tft, xFirePos, yFirePos, 10, 2, COLOR_RED, bgColor);
           blasters[blasterIndex].SetVelocity(0, shotVelocity);
           blasters[blasterIndex].SetBehavior(true, 0);
           blasterIndex++;
           if (blasterIndex == numBlasters)
           {
              blasterIndex = 0;
           }
           fireDebounce = msTicks + 100; // Right now 200ms debounce time
       }
       // Top right button handler
       if (fireDebounce < msTicks && digitalRead(TOP_RIGHT) == 0)
       {
           // User fired
           int16_t xFirePos = player2.getYPos() + 5.0;
           int16_t yFirePos = player2.getXPos();
           float shotVelocity = -40.0;

           if (player2.getRotation() == LEFT)
           {
               shotVelocity *= -1.0;
               xFirePos = player2.getYPos() - 5.0;
           }
           blasters[blasterIndex].Activate(&tft, xFirePos, yFirePos, 10, 2, COLOR_RED, bgColor);
           blasters[blasterIndex].SetVelocity(-4.0, shotVelocity);
           blasters[blasterIndex].SetBehavior(true, 0);
           blasterIndex++;
           if (blasterIndex == numBlasters)
           {
              blasterIndex = 0;
           }
           blasters[blasterIndex].Activate(&tft, xFirePos, yFirePos + player2.getHeight(), 10, 2, COLOR_RED, bgColor);
           blasters[blasterIndex].SetVelocity(4.0, shotVelocity);
           blasters[blasterIndex].SetBehavior(true, 0);
           blasterIndex++;
           if (blasterIndex == numBlasters)
           {
              blasterIndex = 0;
           }
           //blasters[blasterIndex](&tft, 50, 150, 10, 2, bgColor, COLOR_RED);
           fireDebounce = msTicks + 300; // Right now 200ms debounce time
           //debounceTimes[4] = msTicks + 200; // Right now 200ms debounce time
       }
       // Bottom left button handler
       if (debounceTimes[5] < msTicks && digitalRead(BOTTOM_LEFT) == 0)
       {
           debounceTimes[5] = msTicks + 200; // Right now 200ms debounce time
       }
       // Bottom right button handler
       if (debounceTimes[6] < msTicks && digitalRead(BOTTOM_RIGHT) == 0)
       {
           debounceTimes[6] = msTicks + 200; // Right now 200ms debounce time
       }
       if (nextControlTime < msTicks)
       {
           float xScaler = (float)analogRead(JOYSTICK_X) - (float)(ADC_MAX / 2);
           xScaler = xScaler / (float)(ADC_MAX / 2);
           float yScaler = (float)analogRead(JOYSTICK_Y) - (float)(ADC_MAX / 2);
           yScaler = yScaler / (float)(ADC_MAX / 2);
           if (xScaler > 0.2)
           {
              player2.RotateLeft();
           }
           else if (xScaler < -0.2)
           {
              player2.RotateRight();
           }
           player2.SetVelocity(xScaler * 30.0, yScaler * -30.0);
           nextControlTime = msTicks + 50;
       }
   }
}


void loop(void) 
{

}
