#ifndef FRAXMENU_H
#define FRAXMENU_H

#include "Arduino.h"
#include <GxEPD2_BW.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include "config.h"
#include "FraxModels.h"

// unique action codes
#define MENU_EXIT_CODE 127 // must not be a menu index value
#define MENU_ACTION_APP 1 // open app action
#define MENU_ACTION_SUB 2 // open submenu action

class FraxMenu
{
    public: // public members
        
    public: // public methods
        FraxMenu(GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT>* displayPtr, watchState* statePtr);
        uint8_t startMenu();
        uint8_t displayMenu(const menuItem* items, uint8_t length, uint8_t initialSelection, bool partialRefresh);
    
    private:    // private members
        static GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT>* display;
        static watchState* state;

    private:    // private methods
        const menuList* getSubMenu(uint8_t id);
        void drawMenu(const menuItem* items, uint8_t length, uint8_t selectedIndex, bool partialRefresh);
        void drawMenu(const menuList* menu, bool partialRefresh);
};

const uint8_t batteryIcon [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 
	0x01, 0xff, 0xe0, 0x00, 0x01, 0xff, 0xe0, 0x00, 0x01, 0xff, 0xe0, 0x00, 0x01, 0xff, 0xe0, 0x00, 
	0x01, 0xff, 0xe0, 0x00, 0x01, 0xff, 0xe0, 0x00, 0x01, 0xff, 0xe0, 0x00, 0x01, 0xff, 0xe0, 0x00, 
	0x01, 0xff, 0xe0, 0x00, 0x01, 0xff, 0xe0, 0x00, 0x01, 0xff, 0xe0, 0x00, 0x01, 0xff, 0xe0, 0x00, 
	0x01, 0xff, 0xe0, 0x00, 0x01, 0xff, 0xe0, 0x00, 0x01, 0xff, 0xe0, 0x00, 0x01, 0xff, 0xe0, 0x00, 
	0x01, 0xff, 0xe0, 0x00, 0x01, 0xff, 0xe0, 0x00, 0x01, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const uint8_t vibrateIcon [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xf8, 0x00, 
	0x07, 0xff, 0xf8, 0x00, 0x07, 0x00, 0x38, 0x00, 0x07, 0x00, 0x38, 0x00, 0x1f, 0x00, 0x3e, 0x00, 
	0x1f, 0x00, 0x3e, 0x00, 0x1f, 0x00, 0x3e, 0x00, 0xdf, 0x00, 0x3e, 0xc0, 0xdf, 0x00, 0x3e, 0xc0, 
	0xdf, 0x00, 0x3e, 0xc0, 0xdf, 0x00, 0x3e, 0xc0, 0xdf, 0x00, 0x3e, 0xc0, 0xdf, 0x00, 0x3e, 0xc0, 
	0x1f, 0x00, 0x3e, 0x00, 0x1f, 0x00, 0x3e, 0x00, 0x1f, 0x00, 0x3e, 0x00, 0x07, 0x00, 0x38, 0x00, 
	0x07, 0x00, 0x38, 0x00, 0x07, 0xff, 0xf8, 0x00, 0x07, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const uint8_t rotationIcon [] PROGMEM = {
	0x00, 0x0f, 0xe0, 0x00, 0x00, 0xff, 0xf8, 0x00, 0x01, 0xff, 0xf8, 0x00, 0x03, 0xff, 0xfe, 0x00, 
	0x07, 0xff, 0xde, 0x00, 0x0f, 0x1f, 0x07, 0x80, 0x1f, 0x0f, 0x87, 0x80, 0x3e, 0x07, 0xc3, 0xc0, 
	0x78, 0x03, 0xe1, 0xc0, 0x78, 0x01, 0xf1, 0xc0, 0x78, 0x00, 0xf8, 0xc0, 0x7c, 0x00, 0x7c, 0xc0, 
	0x3e, 0x00, 0x3e, 0xc0, 0xdf, 0x00, 0x1f, 0x00, 0xef, 0x80, 0x0f, 0x80, 0xe7, 0xc0, 0x07, 0x80, 
	0xe3, 0xe0, 0x07, 0x80, 0xe1, 0xf0, 0x07, 0x80, 0xf0, 0xf8, 0x1f, 0x00, 0x78, 0x7c, 0x3e, 0x00, 
	0x78, 0x3e, 0x3c, 0x00, 0x3e, 0xdf, 0xf8, 0x00, 0x1f, 0xef, 0xf0, 0x00, 0x0f, 0xf7, 0xe0, 0x00, 
	0x07, 0xfb, 0xc0, 0x00, 0x01, 0xfc, 0x00, 0x00
};

const uint8_t settingsIcon [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 
	0x00, 0x3f, 0x00, 0x00, 0x0e, 0x7f, 0x9c, 0x00, 0x0f, 0xff, 0xfc, 0x00, 0x1f, 0xff, 0xfe, 0x00, 
	0x1f, 0xff, 0xfe, 0x00, 0x1f, 0xff, 0xfe, 0x00, 0x1f, 0xe1, 0xfe, 0x00, 0x1f, 0xc0, 0xfe, 0x00, 
	0x0f, 0xc0, 0xfc, 0x00, 0x0f, 0xc0, 0xfc, 0x00, 0x1f, 0xc0, 0xfe, 0x00, 0x1f, 0xe1, 0xfe, 0x00, 
	0x1f, 0xff, 0xfe, 0x00, 0x1f, 0xff, 0xfe, 0x00, 0x1f, 0xff, 0xfe, 0x00, 0x0f, 0xff, 0xfc, 0x00, 
	0x0f, 0xff, 0xfc, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

#endif