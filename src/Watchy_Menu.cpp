#include "Watchy_Menu.h"

GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT>* Menu::display;
int* Menu::guiState;
menuState* Menu::state;

const menuItem thirdMenuItems[2] = 
{
    {"2 - One", MENU_ACTION_APP, 9},
    {"2 - Two", MENU_ACTION_APP, 10}
};

const menuItem secondMenuItems[4] = 
{
    {"1 - One", MENU_ACTION_APP, 6},
    {"1 - Two", MENU_ACTION_APP, 7},
    {"1 - Three", MENU_ACTION_APP, 8},
    {"1 - Four", MENU_ACITON_SUB, 1}
};

const menuItem mainMenuItems[6] = 
{
    {"Check Battery", MENU_ACTION_APP,  1},
    {"Vibrate Motor", MENU_ACTION_APP, 2},
    {"Show Accelerometer", MENU_ACTION_APP, 3},
    {"Set Time", MENU_ACTION_APP, 4},
    {"Setup WiFi", MENU_ACTION_APP, 5},
    {"Big Menu", MENU_ACITON_SUB, 0}
};

const menuList mainMenu =
{
    NULL,
    mainMenuItems,
    6,
    0
};

const menuList secondMenu =
{
    &mainMenu,
    secondMenuItems,
    4,
    5
};

const menuList thirdMenu =
{
    &secondMenu,
    thirdMenuItems,
    2,
    3
};

const menuList* Menu::getSubMenu(uint8_t id)
{
    switch(id)
    {
        case 0:
            return &secondMenu;
        case 1:
            return &thirdMenu;
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
    menuItem menuSelection = (state->currentMenu)->items[state->menuIndex];
    // 0x00 is the exclusive action id for folders
    if (menuSelection.action == MENU_ACITON_SUB)
    {
        state->currentMenu = getSubMenu(menuSelection.id);
        state->menuIndex = 0;

        showMenu(getSubMenu(menuSelection.id), true);

        return 0;
    }

    return menuSelection.id;
}

void Menu::navigate(int movement)
{
    int desiredIndex = state->menuIndex + movement;

    // if the index is negative, we need to loop around
    // you would think -1 % 6 = 5, but it equals -1 in C
    if (desiredIndex == -1)
    {
        desiredIndex = (state->currentMenu)->length - 1;
    }

    state->menuIndex = desiredIndex % (state->currentMenu)->length;

    showMenu(state->currentMenu, true);
}

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

    return false;
}

int Menu::getIndex()
{
    return state->menuIndex;
}