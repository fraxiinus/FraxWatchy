# FraxWatchy - Custom Watchy Library

This is where I keep my changes to the Watchy library. [See new features](#new-features)

Visit [**https://watchy.sqfmi.com**](https://watchy.sqfmi.com) for original Watchy documentation, hardware design files, and more.

## New Features
- [FraxWatchy - Custom Watchy Library](#fraxwatchy---custom-watchy-library)
  - [New Features](#new-features)
    - [Watch Face Button Handling](#watch-face-button-handling)
    - [New Menu Implementation](#new-menu-implementation)

### Watch Face Button Handling
Watch faces are now able to react to when the two right side buttons are pressed. Override the `void handleWatchFaceButton(uint64_t buttonBit);` function in your watch face:
```C
void Watchy7SEG::handleWatchFaceButton(uint64_t buttonBit){
    if(buttonBit & UP_BTN_MASK){
        // Do something when up button is pressed
    } else if (buttonBit & DOWN_BTN_MASK){
        // Do something when down button is pressed
    }
}
```
This method will only be called when the watch is in "watch face" state.

### New Menu Implementation
Make edits to the menu with minimal code and logic changes! Submenus are now possible! 

Menu items are defined using the following struct:
```C
typedef struct menuItem
{
    char* name;      // the name to display
    uint8_t action;  // what type of action to take
    uint8_t id;      // unique within action type
} menuItem;
```
The following struct defines a menu item with text `Check Battery`, and opens an `app` with `id = 1`.
Right now, apps are defined under `Watchy::handleButtonPress()`
```C
{"Check Battery", MENU_ACTION_APP,  1}
```

This struct defines a menu item that will open a `submenu` with `id = 0`
```C
{"Advanced Setup", MENU_ACTION_SUB, 0}
```

In order for a menu to show up, they need to be put into a `menuList` struct:
```C
typedef struct menuList
{
    const menuList* parent;     // reference to parent menu
    const menuItem* items;      // array of menu items
    uint8_t length;             // number of items
    uint8_t parentIndex;        // the menu item this sub menu is under
} menuList;
```

and then add the menu id to the `Menu::getSubMenu(uint8_t id)` method:
```C
const menuList* Menu::getSubMenu(uint8_t id)
{
    // Add a case for your menu here so your menu can be accessed
    switch(id)
    {
        case 0:
            return &setupMenu;
        default:
            return &mainMenu;
    }
}
```