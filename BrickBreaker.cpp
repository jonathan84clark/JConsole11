/*******************************************************
* BRICK BREAKER
* DESC: Brick breaker is a fun game used on JConsole11
* Author: Jonathan L Clark
* Date: 5/31/2021
*******************************************************/
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "Solid.h"
#include "GameObject.h"
#include "Models.h"
#include "Bar.h"
#include "Menu.h"
#include "Config.h"

static uint16_t randomColors[] = {COLOR_BLUE, COLOR_RED, COLOR_MAGENTA, COLOR_ORANGE, COLOR_FORESTGREEN, COLOR_DRKGREY};

void BrickBreaker(Adafruit_ILI9341* tft)
{
   unsigned int debounceTimes[NUM_BTNS];
   uint16_t bgColor = COLOR_SKYBLUE;
   tft->fillScreen(bgColor);
   //tft->setRotation(1);

   // Clear all debounce times
   for (int i = 0; i < NUM_BTNS; i++)
   {
      debounceTimes[i] = millis() + i;
   }
   int numBlocks = 100;
   int numBlasters = 20;
   int blasterIndex = 0;
   int maxBalls = 6;

   int startId = 30;
   Solid playerPaddle(tft, 100, 220, 40, 5, COLOR_BLACK, bgColor, startId);
   startId++;
   //Solid playerPaddle(tft, 100, 220, 10, 10, COLOR_BLACK, bgColor);
   //Solid player2(tft, 100, 50, XWING_WIDTH, XWING_HEIGHT, xWing, bgColor);
   Bar healthBar(tft, 235, 5, 12, 80, COLOR_BLUE, COLOR_RED, bgColor);
   Menu menuSystem(tft, bgColor);
   
   //Solid blasters[numBlasters];
   Solid bricks[numBlocks];
   Solid balls[6]; //(tft, 10, 10, 20, 20, COLOR_BLUE, COLOR_RED, bgColor);
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

   int16_t startxBrickPos = 5;
   int16_t startYBrickPos = 20;
   int16_t brickSeperation = 3;
   int16_t brickWidth = 35;
   int16_t brickIndex = 0;

   /*
   for (int j = 0; j < 10; j++)
   {
      for (int i = 0; i < 8; i++)
      {
         bricks[brickIndex].Activate(tft, startxBrickPos, startYBrickPos, brickWidth, 10, COLOR_BLUE, bgColor, startId);
         startxBrickPos+= (brickWidth + brickSeperation);
         startId++;
         brickIndex++;
      }
      startxBrickPos = 5;
      startYBrickPos += (10 + brickSeperation);
      
   }
   */
   bricks[0].Activate(tft, 20, 200, 35, 10, COLOR_BLUE, bgColor, startId);
   balls[0].Activate(tft, 200, 200, 10, 10, COLOR_RED, bgColor, startId);
   balls[0].SetPhysics(200, 0.0, 0.0, 1.0, 0.0);
   balls[0].SetVelocity(0.0, 10.0);
   startId++;


   tft->setCursor(2, 2);
   tft->setTextColor(ILI9341_BLUE);  
   tft->setTextSize(2);
   tft->print("Score: ");
   tft->setCursor(75, 2);
   tft->print(score);
   
   while (true)
   {
       msTicks = millis();
       if (nextTime < msTicks)
       {
           playerPaddle.PhysicsMove(msTicks);
           int16_t intercept = 0;
           uint8_t collision = playerPaddle.CheckCollision(&balls[0], &intercept, true);
           if (collision)
           {
              
              float yVelocity = balls[0].getVelocityX();
              float xVelocity = balls[0].getVelocityY();

              if (intercept < 10)
              {
                  xVelocity = 15;
                  yVelocity = 8; 
                  balls[0].SetVelocity(yVelocity, xVelocity);
              }
              else if (intercept < 20)
              {
                 xVelocity = 10.0;
                 balls[0].SetVelocity(yVelocity * -1.0, xVelocity);
              }
              else if (intercept > 40)
              {
                  xVelocity = -15;
                  yVelocity = 8; 
                  balls[0].SetVelocity(yVelocity, xVelocity);
              }
              else if (intercept > 30)
              {
                 xVelocity = -10.0;
                 balls[0].SetVelocity(yVelocity * -1.0, xVelocity);
              }
              else
              {
                 balls[0].SetVelocity(yVelocity * -1.0, xVelocity);
              }
           }
           nextTime = msTicks + 50;
       }
       for (int i = 0; i < maxBalls; i++)
       {
           if (balls[i].getActive() && balls[i].getNextPhysicsTime() < msTicks)
           {
               balls[i].PhysicsMove(msTicks);
               int16_t intercept = 0;
               for (int j = 0; j < numBlocks; j++)
               {
                  if (bricks[j].getActive())
                  {
                      //uint8_t collision = balls[i].CheckCollision(&bricks[j], &intercept, false);
                      uint8_t collision = balls[i].CheckEdgeIntersect(&bricks[j]);
                      CheckEdgeIntersect
                      if (collision)
                      {
                        /*
                         bricks[j].Disable();
                         tft->setTextColor(bgColor);  
                         tft->setTextSize(2);
                         tft->setCursor(75, 2);
                         tft->print(score);
                         score++;
                         tft->setTextColor(ILI9341_BLUE);  
                         tft->setCursor(75, 2);
                         tft->print(score);
                         float yVelocity = balls[i].getVelocityX();
                         float xVelocity = balls[i].getVelocityY();
                         if (collision & 0x01)
                         {
                            xVelocity *= -1.0;  
                         }
                         if (collision & 0x02)
                         {
                            yVelocity *= -1.0;
                         }

                         balls[i].SetVelocity(yVelocity, xVelocity);
                         */
                      }
                  }
               }
           }
       }
       // Joystick/ Menu button handler
       if (debounceTimes[0] < msTicks && digitalRead(JOYSTICK_BTN))
       {
           // Handle joystick btn pressed
           int output = menuSystem.Pause();
           if (output == 2)
           {
               break;
           }
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
           
           fireDebounce = msTicks + 100; // Right now 200ms debounce time
       }
       // Top right button handler
       if (fireDebounce < msTicks && digitalRead(TOP_RIGHT) == 0)
       {
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
           /*
           if (xScaler > 0.2)
           {
              playerPaddle.RotateLeft();
           }
           else if (xScaler < -0.2)
           {
              playerPaddle.RotateRight();
           }
           */
           playerPaddle.SetVelocity(0.0, xScaler * 30.0);
           nextControlTime = msTicks + 50;
       }
   }
}
