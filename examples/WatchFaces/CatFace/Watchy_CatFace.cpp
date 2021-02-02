#include "Watchy_CatFace.h"

WatchyCatFace::WatchyCatFace() { } //constructor

void WatchyCatFace::drawWatchFace()
{
    display.fillScreen(GxEPD_WHITE);
    display.setTextColor(GxEPD_BLACK);

    drawTime();
    drawBattery();
}

void WatchyCatFace::drawTime()
{
    display.setFont(&FreeMonoBold24pt7b);
    display.setCursor(0, 50);
    
    uint8_t hour = currentTime.Hour;
    if (!settings.timeFormat)
    {
        if (hour == 0) hour = 12;
        else if(hour > 12) hour = hour - 12;
    }

    if (hour < 10) display.print("0");
    display.print(hour);

    display.print(":");
    if (currentTime.Minute < 10) display.print("0");
    display.println(currentTime.Minute);
}

void WatchyCatFace::drawBattery()
{
    display.fillRect(20, 100, 50, 20, GxEPD_BLACK); //clear battery segments
    display.setTextColor(GxEPD_WHITE);
    display.setFont(&FreeMonoBold9pt7b);
    display.setCursor(20, 110);
    display.print(getBatteryVoltage());
    float VBAT = getBatteryVoltage();
}

void WatchyCatFace::handleWatchFaceButton(uint8_t button)
{
    if (button == 1)
    {
        settings.timeFormat = !settings.timeFormat;
        RTC.alarm(ALARM_2); //resets the alarm flag in the RTC
        RTC.read(currentTime);
        showWatchFace(true);
    }
}