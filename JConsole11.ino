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
#include "BrickBreaker.h"

// For the Adafruit shield, these are the default.
#define TFT_DC 4
#define TFT_CS 1
#define TFT_MOSI 3
#define TFT_CLK 2
#define TFT_RST 5
#define TFT_MISO 0

// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
//Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
// If using the breakout, change pins as desired
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);
char mainMenuOptions[][15] = {"Debris", "Breaker", "Tanks", "Options"};

void setup() 
{
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

  
  tft.begin(20000000);
  tft.setRotation(1);
  Menu menu(&tft, COLOR_DARKGREEN);

  menu.MultiOption("JConsole 11", mainMenuOptions, 4);
  //BrickBreaker(&tft);
}

void loop(void) 
{

}
