#include "Watchy_Menu.h"

GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT>* Menu::display;
int* Menu::guiState;
menuState* Menu::state;

const menuItem setupMenuItems[3] = 
{
    {"Set Time", MENU_ACTION_APP, 4},
    {"Setup WiFi", MENU_ACTION_APP, 5},
    {"Update Firmware", MENU_ACTION_APP, 6}
};

const menuItem mainMenuItems[6] = 
{
    {"Check Battery", MENU_ACTION_APP,  1}, // opens app with id=1
    {"Vibrate Motor", MENU_ACTION_APP, 2},
    {"Show Accelerometer", MENU_ACTION_APP, 3},
    {"Advanced Setup", MENU_ACITON_SUB, 0}    // opens submenu with id=0 (entry point)
};

const menuList mainMenu =
{
    NULL,               // main menu is not a submenu so had no parent
    mainMenuItems,      // link to the array of items
    4,                  // how many items are in the array
    0                   // main menu has no parent, so the parent entry point isn't used
};

const menuList setupMenu =
{
    &mainMenu,          // second menu is a submenu under main menu
    setupMenuItems,
    3,
    3                   // the 3rd entry in main menu is the entry point to the second menu
};

const menuList* Menu::getSubMenu(uint8_t id)
{
    // Add a case for your menu here so your menu can be accesseed
    switch(id)
    {
        case 0:
            return &setupMenu;
        default:
            return &mainMenu;
    }
}

void Menu::goToMenu(bool goToMain)
{
    *guiState = MAIN_MENU_STATE;

    if (goToMain)
    {
        state->currentMenu = &mainMenu;
        showMenu(&mainMenu, true);
    } else
    {
        showMenu(state->currentMenu, true);
    }
}

void Menu::showMenu(const menuList* menu, bool partialRefresh)
{
    display->init(0, false); //_initial_refresh to false to prevent full update on init
    display->setFullWindow();
    display->fillScreen(GxEPD_BLACK);
    display->setFont(&FreeMonoBold9pt7b);

    int16_t x1, y1;
    uint16_t w, h;
    int16_t yPos;

    for (int i = 0; i < menu->length; i++)
    {
        yPos = 30 + (MENU_HEIGHT * i);
        display->setCursor(0, yPos);
        if (i == state->menuIndex)
        {
            display->getTextBounds(menu->items[i].name, 0, yPos, &x1, &y1, &w, &h);
            display->fillRect(x1 - 1, y1 - 10, 200, h + 15, GxEPD_WHITE);
            display->setTextColor(GxEPD_BLACK);
            display->println(menu->items[i].name);
        }
        else
        {
            display->setTextColor(GxEPD_WHITE);
            display->println(menu->items[i].name);
        }
    }

    display->display(partialRefresh);
    display->hibernate();
}

uint8_t Menu::clickMenuItem()
{
    // get the selected item
    menuItem menuSelection = (state->currentMenu)->items[state->menuIndex];
    
    // handle submenu actions internally
    if (menuSelection.action == MENU_ACITON_SUB)
    {
        state->currentMenu = getSubMenu(menuSelection.id);
        state->menuIndex = 0;

        showMenu(getSubMenu(menuSelection.id), true);

        return 0;   // 0 is reserved value for doing nothing
    }

    // otherwise return the id of the menu item
    return menuSelection.id;
}

void Menu::navigate(int movement)
{

    int desiredIndex = state->menuIndex + movement;
    // if the index is negative, we need to loop around
    if (desiredIndex == -1)
    {
        desiredIndex = (state->currentMenu)->length - 1;
    }

    state->menuIndex = desiredIndex % (state->currentMenu)->length;

    showMenu(state->currentMenu, true);
}

// return true if there is a parent menu to go back to
bool Menu::goToPreviousMenu()
{
    if ((state->currentMenu)->parentIndex != NULL)
    {
        // Update our state
        state->menuIndex = (state->currentMenu)->parentIndex;
        state->currentMenu = (state->currentMenu)->parent;

        showMenu(state->currentMenu, true);
        return true;
    }

    // tell the caller there is no more menu left (go to watchface)
    return false;
}