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
    static void DrawMainWeatherSection(int x, int y, const OneCallData& oneCallData);

    static void DisplayDisplayWindSection(int x, int y, float angle, float windspeed, int Cradius);
    static void DrawPressureAndTrend(int x, int y, int pressure, String slope);
    static void DrawMainWx(int x, int y, const OneCallData& oneCallData);

    static String WindDegToDirection(float winddirection);
    static String TitleCase(const String& text);

    static void drawString(int x, int y, String text, alignment align);
    static void drawStringMaxWidth(int x, int y, unsigned int text_width, String text, alignment align);
    static void arrow(int x, int y, int asize, float aangle, int pwidth, int plength);
};
