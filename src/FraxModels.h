#ifndef FRAXMODELS_H
#define FRAXMODELS_H

typedef struct weatherData
{
    int8_t temperature;
    int16_t weatherConditionCode;
} weatherData;

// data for the individual menu choices
typedef struct menuItem
{
    char* name;                 // the name to display
    const uint8_t* icon;              // pointer to bitmap
    uint8_t action;             // define how an entry acts
    uint8_t id;                 // unique within action type, used to differentiate things in the same type
} menuItem;

// data for list of menu choices
typedef struct menuList
{
    const menuList* parent;     // reference to parent menu
    const menuItem* items;
    uint8_t length;             // number of items
    uint8_t parentIndex;        // the menu item this sub menu is under
} menuList;

typedef struct watchState
{
    bool timeFormat; // false = 12hour time, true = 24hour time
    bool wifiConfigured;
    bool bleConfigured;
    int guiMode;
    uint8_t menuIndex;
    const menuList* currentMenu;
    bool menuSaved;
} watchState;

#endif