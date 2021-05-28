/*******************************************************
* MENU CLASS
* DESC: The menu class handles menu interactions on the UI
* Author: Jonathan L Clark
* Date: 5/27/2021
*******************************************************/
#ifndef _MENU_
#define _MENU_

#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "Config.h"

class Menu
{
   public:
      Menu(Adafruit_ILI9341 *inTft);
      uint8_t Pause();
   
   private:
      Adafruit_ILI9341 *tft;
      
};

#endif
