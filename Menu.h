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
      Menu(Adafruit_ILI9341 *inTft, uint16_t inBg_color);
      int Pause();
      int MultiOption(char* title, char options[][15], int numOptions);
      void DrawMenu(char* title,char options[][15], int numOptions, int selectedOption);
   
   private:
      Adafruit_ILI9341 *tft;
      uint16_t bg_color;
      
};

#endif
