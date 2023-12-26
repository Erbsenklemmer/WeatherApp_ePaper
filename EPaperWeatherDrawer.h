#pragma once

#define ENABLE_GxEPD2_GFX 1

#include "GxEPD2_3C.h"

#include "OneCallData.h"
#include "DailyData.h"
#include "HourlyData.h"
#include "CurrentData.h"

class EPaperWeatherDrawer
{
  public:
    static void setup();

    static void drawOneCallData(const OneCallData& oneCallData);

  protected:
    static void drawDailyData(int x, int y, const DailyData& dailyData);

private:
    static void DrawIcon(int x, int y, const String& crIcon);

    static void DrawSun(int x, int y);
    static void DrawMoon(int x, int y);

    static void DrawPolarRay(int middleX, int middleY, int length, int angle);
};