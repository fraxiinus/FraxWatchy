# FraxiinusWatchy - Peronsal Watchy Library

This is where I keep my changes to the Watchy library. [See new features](#new-features)

Visit [**https://watchy.sqfmi.com**](https://watchy.sqfmi.com) for original Watchy documentation, hardware design files, and more!

## Setup
1. In the Arduino IDE Boards Manager, install support for the ESP32. You can find instructions here: https://github.com/espressif/arduino-esp32/blob/master/docs/arduino-ide/boards_manager.md
2. Install this library (search for **Watchy** in the library manager), and any other depdencies when prompted
3. Check out the examples under ```Examples``` -> ```Watchy```
4. Compile & Upload with these board settings:
    * Board: "ESP32 Dev Module"
    * Partition Scheme: "Minimal SPIFFS"
    * All Other Settings: leave to default

You may also have to install the [CP2104 USB to Serial drivers](https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers) if the port is not automatically detected

## New Features
- [Watch faces can react to right button presses](#watch-face-button-handling)
- [New menu implementation makes it easier to change](#new-menu-implementation)

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
{"Advanced Setup", MENU_ACITON_SUB, 0}
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
    // Add a case for your menu here so your menu can be accesseed
    switch(id)
    {
        case 0:
            return &setupMenu;
        default:
            return &mainMenu;
    }
}
```