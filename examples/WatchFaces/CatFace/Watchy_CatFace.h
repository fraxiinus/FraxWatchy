#ifndef WATCHY_CATFACE
#define WATCHY_CATFACE

#include <FraxWatchy.h>
#include <Fonts/FreeMonoBold24pt7b.h>

class WatchyCatFace : public FraxWatchy
{
    public:
        WatchyCatFace();
        void drawWatchFace(); // override
        void handleWatchFaceButton(uint8_t button); // override
        void drawTime();
        // void drawDate();
        // void drawSteps();
        // void drawWeather();
        void drawBattery();
};

#endif
