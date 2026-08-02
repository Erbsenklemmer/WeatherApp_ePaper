#pragma once

#define ENABLE_GxEPD2_GFX 1
#define ENABLE_GxEPD2_display 0

#include "GxEPD2_3C.h"
#include <U8g2_for_Adafruit_GFX.h>

#include <UnixTime.h>

#include "OneCallData.h"
#include "DailyData.h"
#include "HourlyData.h"
#include "CurrentData.h"

enum alignment {LEFT, RIGHT, CENTER};

class EPaperWeatherDrawer2
{
  public:
    static void setup();

    static void drawOneCallData(const OneCallData& oneCallData, UnixTime unixTime);
  private:
    static void DrawHeadingSection(); 
    static void DrawBattery(int x, int y);

    static void drawString(int x, int y, String text, alignment align);
};
