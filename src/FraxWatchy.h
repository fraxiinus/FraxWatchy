#ifndef FRAXWATCHY_H
#define FRAXWATCHY_H

#include <Arduino.h>
#include <WiFiManager.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include <DS3232RTC.h>
#include <GxEPD2_BW.h>
#include <Wire.h>
#include "DSEG7_Classic_Bold_53.h"
#include "BLE.h"
#include "bma.h"
#include "config.h"
#include "keys.h"
#include "FraxMenu.h"
#include "FraxModels.h"

class FraxWatchy
{
    public:
        static DS3232RTC RTC;
        static GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT> display;
        static FraxMenu menu;
        tmElements_t currentTime;

    public:
        FraxWatchy();
        void init(String datetime = "");
        void deepSleep();
        float getBatteryVoltage();
        void vibMotor(uint8_t intervalMs = 100, uint8_t length = 20);

        void handleButtonPress();
        void showMenu();
        void showBattery();
        void showBuzz();
        void showAccelerometer();
        void showUpdateFW();
        void setTime();
        void setTimeFormat();
        void setBluetoothLE();
        void setupWifi();
        bool connectWiFi();
        weatherData getWeatherData();
        void updateFWBegin();

        void showWatchFace(bool partialRefresh);
        virtual void drawWatchFace();                           //override this method for different watch faces
        virtual void handleWatchFaceButton(uint8_t button);     //override this method to handle button presses on the watch face

    private:
        void _rtcConfig(String datetime);    
        void _bmaConfig();
        static void _configModeCallback(WiFiManager *myWiFiManager);
        static uint16_t _readRegister(uint8_t address, uint8_t reg, uint8_t *data, uint16_t len);
        static uint16_t _writeRegister(uint8_t address, uint8_t reg, uint8_t *data, uint16_t len);
};

extern RTC_DATA_ATTR BMA423 sensor;
extern RTC_DATA_ATTR watchState state;

#endif