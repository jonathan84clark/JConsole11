/*******************************************************
* SPRITE CLASS
* DESC: The sprite class handles the behavior of a 2D sprite
* Gameobject. The class is desinged to be inherited and subclasses
* can be used to emulate different behaviors.
* This class was re-written to work with JConsole11
* Author: Jonathan L Clark
* Date: 5/2/2021
*******************************************************/
#include <stdio.h>
#include "SPI.h"
#include "GameObject.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

GameObject::GameObject()
{

}

GameObject::GameObject(Adafruit_ILI9341 *inTft, uint16_t* pcolors)
{
   //uint16_t pcolors[25];
   //for (int i = 0; i < 25; i++)
   //{
   //   pcolors[i] = 0x8282;
   //}
   uint16_t startx = 100;
   inTft->drawRGBBitmap(startx, 100, pcolors, 5, 5);
}
