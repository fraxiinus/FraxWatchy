#include "FraxMenu.h"

GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT>* FraxMenu::display;
watchState* FraxMenu::state;

const menuItem setupMenuItems[5] = 
{
    // Title            Icon    Action              Action Id
    {"Set Time",        NULL,   MENU_ACTION_APP,    3},
    {"Time Format",     NULL,   MENU_ACTION_APP,    6},
    {"Setup WiFi",      NULL,   MENU_ACTION_APP,    4},
    {"Bluetooth LE",    NULL,   MENU_ACTION_APP,    7},
    {"OTA Update",      NULL,   MENU_ACTION_APP,    5}
};

const menuItem mainMenuItems[6] = 
{
    // Title            Icon            Action              Action Id
    {"Check Battery",   batteryIcon,    MENU_ACTION_APP,    0},  // opens app with id=1
    {"Vibrate Motor",   vibrateIcon,    MENU_ACTION_APP,    1},
    {"Show BMA423",     rotationIcon,   MENU_ACTION_APP,    2},
    {"Settings",        settingsIcon,   MENU_ACTION_SUB,    0}   // opens submenu with id=0 (entry point)
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
    5,
    3                   // the 4th entry in main menu is the entry point to the second menu
};

const menuList* FraxMenu::getSubMenu(uint8_t id)
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

// Constructor
FraxMenu::FraxMenu(GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT>* displayPtr, watchState* statePtr)
{
    // Save required pointers
    display = displayPtr;
    state = statePtr;

    // Set default menu state
    if (state->menuSaved != true)
    {
        state->currentMenu = &mainMenu;
        state->menuIndex = 0;
    }
}

// Show the main menu structure at the saved state. Returns when user makes a selection, or closes the menu
uint8_t FraxMenu::startMenu()
{
    while (1)
    {
        // Get the user's selection
        int8_t selection = displayMenu((state->currentMenu)->items, (state->currentMenu)->length, state->menuIndex, true);

        // If the user pressed the exit button
        if (selection == MENU_EXIT_CODE)
        {
            // Check if we are in a submenu, if we are not we exit
            if ((state->currentMenu)->parent == NULL)
            {
                return MENU_EXIT_CODE;
            }
            else // Otherwise move into parent menu
            {
                state->menuIndex = (state->currentMenu)->parentIndex;
                state->currentMenu = (state->currentMenu)->parent;
                continue; // return to start of the loop
            }
        }
        else // user made a menu selection
        {
            // Get the selection item
            menuItem menuSelection = (state->currentMenu)->items[selection];

            if (menuSelection.action == MENU_ACTION_SUB)
            {
                // Open the submenu
                state->currentMenu = getSubMenu(menuSelection.id);
                state->menuIndex = 0;

                continue; // return to start of the loop
            }
            else if (menuSelection.action == MENU_ACTION_APP)
            {
                state->menuIndex = selection;
                state->menuSaved = true;

                // Return the App Id for the main library to resolve
                return menuSelection.id;
            }
        }
    }
}

// Show the given menu items, Returns when user makes a selection, or closes the menu
uint8_t FraxMenu::displayMenu(const menuItem* items, uint8_t length, uint8_t initialSelection, bool partialRefresh)
{
    // Enable reading input from button pins
    pinMode(DOWN_BTN_PIN, INPUT);
    pinMode(UP_BTN_PIN, INPUT);
    pinMode(MENU_BTN_PIN, INPUT);
    pinMode(BACK_BTN_PIN, INPUT);

    uint8_t drawNew = true; // Indicates when to update screen
    while (1)
    {   
        if (drawNew)
        {
            drawNew = false; // Reset the flag
            drawMenu(items, length, initialSelection, partialRefresh);
        }
        else 
        {
            delay(200); // wait to stop loop from going insane
        }

        if (digitalRead(BACK_BTN_PIN) == 1) // back button press
        {
            return MENU_EXIT_CODE;
        }
        else if (digitalRead(MENU_BTN_PIN) == 1) // menu (confirm) button press
        {
            return initialSelection;
        }
        else if (digitalRead(UP_BTN_PIN) == 1) // up
        {
            int desiredIndex = initialSelection - 1;
            // if the index is negative, we need to loop around
            if (desiredIndex == -1) desiredIndex = length - 1;
            else if (desiredIndex == length) desiredIndex = 0;

            // Set the selection value and raise the draw flag
            initialSelection = desiredIndex;
            drawNew = true;
        }
        else if (digitalRead(DOWN_BTN_PIN) == 1) // down
        {
            int desiredIndex = initialSelection + 1;
            // if the index is negative, we need to loop around
            if (desiredIndex == -1) desiredIndex = length - 1;
            else if (desiredIndex == length) desiredIndex = 0;

            initialSelection = desiredIndex;
            drawNew = true;
        }
    }
}

// Draw the given menu items to the screen
void FraxMenu::drawMenu(const menuItem* items, uint8_t length, uint8_t selectedIndex, bool partialRefresh)
{
    display->init(0, false); //_initial_refresh to false to prevent full update on init
    display->setFullWindow();
    display->fillScreen(GxEPD_BLACK);
    display->setFont(&FreeMonoBold9pt7b);

    int16_t x1, y1;
    uint16_t w, h;
    int16_t yPos;

    for (int i = 0; i < length; i++)
    {
        yPos = 30 + (MENU_HEIGHT * i);
        display->setCursor(40, yPos);
        if (i == selectedIndex)
        {
            display->getTextBounds(items[i].name, 0, yPos, &x1, &y1, &w, &h);
            display->fillRect(x1 - 1, y1 - 10, 200, 30, GxEPD_WHITE);

            //display->fillRect(2, yPos - 18, 26, 26, GxEPD_BLACK);

            if (items[i].icon)
            {
                display->drawBitmap(5, yPos - 18, items[i].icon, 26, 26, GxEPD_BLACK);
            }

            display->setTextColor(GxEPD_BLACK);
            display->println(items[i].name);
        }
        else
        {
            //display->fillRect(2, yPos - 18, 26, 26, GxEPD_WHITE);

            if (items[i].icon)
            {
                display->drawBitmap(5, yPos - 18, items[i].icon, 26, 26, GxEPD_WHITE);
            }

            display->setTextColor(GxEPD_WHITE);
            display->println(items[i].name);
        }
    }

    display->display(partialRefresh);
    display->hibernate();
}

// Draw the given menuList to the screen
void FraxMenu::drawMenu(const menuList* menu, bool partialRefresh)
{
    drawMenu(menu->items, menu->length, state->menuIndex, partialRefresh);
}