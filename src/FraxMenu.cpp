#include "FraxMenu.h"

GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT>* FraxMenu::display;
menuState* FraxMenu::state;

const menuItem setupMenuItems[4] = 
{
    // Title        Icon    Action              Action Id
    {"Set Time",    NULL,   MENU_ACTION_APP,    3},
    {"Time Format", NULL,   MENU_ACTION_APP,    6},
    {"Setup WiFi",  NULL,   MENU_ACTION_APP,    4},
    {"OTA Update",  NULL,   MENU_ACTION_APP,    5}
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
    4,
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
FraxMenu::FraxMenu(GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT>* displayPtr, menuState* statePtr)
{
    // Save required pointers
    display = displayPtr;
    state = statePtr;

    // Set default menu state
    state->currentMenu = &mainMenu;
    state->menuIndex = 0;
}

// Show the main menu structure at the saved state. Returns when user makes a selection, or closes the menu
uint8_t FraxMenu::startMenu()
{
    while (1)
    {
        Serial.print("startMenu index: ");
        Serial.println(state->menuIndex);

        // Get the user's selection
        int8_t selection = displayMenu((state->currentMenu)->items, (state->currentMenu)->length, state->menuIndex, true);

        // If the user pressed the exit button
        if (selection == MENU_EXIT_CODE)
        {
            // Check if we are in a submenu, if we are not we exit
            if ((state->currentMenu)->parent == NULL)
            {
                Serial.println("Return exit code");
                return MENU_EXIT_CODE;
            }
            else // Otherwise move into parent menu
            {
                Serial.println("Go to parent menu");
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
                Serial.println("Go to submenu");
                // Open the submenu
                state->currentMenu = getSubMenu(menuSelection.id);
                state->menuIndex = 0;
                continue; // return to start of the loop
            }
            else if (menuSelection.action == MENU_ACTION_APP)
            {
                Serial.println("Go to app");
                state->menuIndex = selection;
                // Return the App Id for the main library to resolve
                return menuSelection.id;
            }
        }
    }
}

// uint8_t Menu::enterMenu(const menuList* menu, uint8_t initialSelection)
// {
//     int8_t selection = displayMenu(menu->items, menu->length, uint8_t initialSelection, true);
    
//     menuItem menuSelection = menu->items[selection];

//     // handle submenu actions internally
//     if (menuSelection.action == MENU_ACTION_SUB)
//     {
//         const menuList* subMenu = getSubMenu(menuSelection.id);

//         enterMenu(subMenu);
//     }
//     else if (menuSelection.action == MENU_ACTION_APP)
//     {
//         return menuSelection.id;
//     }
// }

// Show the given menu items, Returns when user makes a selection, or closes the menu
uint8_t FraxMenu::displayMenu(const menuItem* items, uint8_t length, uint8_t initialSelection, bool partialRefresh)
{
    Serial.println("displayMenu...");

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

        if (digitalRead(BACK_BTN_PIN) == 1) // back button press
        {
            Serial.println("Exit displayMenu");
            return MENU_EXIT_CODE;
        }
        else if (digitalRead(MENU_BTN_PIN) == 1) // menu (confirm) button press
        {
            Serial.print("Selected ");
            Serial.print(initialSelection);
            Serial.println(" index");
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

        delay(200); // wait 50ms to stop loop from going insane
    }
}

// void Menu::goToMenu(bool goToMain)
// {
//     *guiState = MAIN_MENU_STATE;

//     if (goToMain)
//     {
//         state->currentMenu = &mainMenu;
//         drawMenu(&mainMenu, true);
//     } else
//     {
//         drawMenu(state->currentMenu, true);
//     }
// }

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

// uint8_t Menu::clickMenuItem()
// {
//     // get the selected item
//     menuItem menuSelection = (state->currentMenu)->items[state->menuIndex];
    
//     // handle submenu actions internally
//     if (menuSelection.action == MENU_ACTION_SUB)
//     {
//         state->currentMenu = getSubMenu(menuSelection.id);
//         state->menuIndex = 0;

//         drawMenu(getSubMenu(menuSelection.id), true);

//         return 0;   // 0 is reserved value for doing nothing
//     }

//     // otherwise return the id of the menu item
//     return menuSelection.id;
// }

// void Menu::navigate(int movement)
// {

//     int desiredIndex = state->menuIndex + movement;
//     // if the index is negative, we need to loop around
//     if (desiredIndex == -1)
//     {
//         desiredIndex = (state->currentMenu)->length - 1;
//     }

//     state->menuIndex = desiredIndex % (state->currentMenu)->length;

//     drawMenu(state->currentMenu, true);
// }

// return true if there is a parent menu to go back to
// bool Menu::goToPreviousMenu()
// {
//     if ((state->currentMenu)->parentIndex != NULL)
//     {
//         // Update our state
//         state->menuIndex = (state->currentMenu)->parentIndex;
//         state->currentMenu = (state->currentMenu)->parent;

//         drawMenu(state->currentMenu, true);
//         return true;
//     }

//     // tell the caller there is no more menu left (go to watchface)
//     return false;
// }
