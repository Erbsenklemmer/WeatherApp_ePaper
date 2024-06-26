#pragma once

#define ENABLE_GxEPD2_GFX 1
#define ENABLE_GxEPD2_display 0

#include "GxEPD2_3C.h"
#include <U8g2_for_Adafruit_GFX.h>

#include <UnixTime.h>

//#define __Test_Paint_DailyData__

#include "OneCallData.h"
#include "DailyData.h"
#include "HourlyData.h"
#include "CurrentData.h"

#include "PointData.h"

class EPaperWeatherDrawer
{
  public:
    static void setup();

    static void drawOneCallData(const OneCallData& oneCallData, UnixTime unixTime);

  protected:
    static void drawTodayData(int x, int y, UnixTime unixTime, const CurrentData& currentData, const DailyData& todayData);
    static void drawDailyData(int x, int y, const DailyData& dailyData);

  private:
    static void DrawIcon(int x, int y, const String& crIcon);

    static void DrawSun(int x, int y);//01d
    static void DrawMoon(int x, int y);//01n

    static void DrawMediumSunWithCloud(int x, int y);//02d
    static void DrawMediumMoonWithCloud(int x, int y);//02n

    static void DrawBlackAndWhiteCloudWithSun(int x, int y);//03d
    static void DrawBlackAndWhiteCloudWithMoon(int x, int y);//03n

    static void DrawBlackAndWhiteCloud(int x, int y);//04d/n

    static void DrawMediumSunWithCloudAndRain(int x, int y);//09d
    static void DrawMediumMoonWithCloudAndRain(int x, int y);//09n

    static void DrawThunderstorm(int offsetX, int offsetY);//11d

    static void DrawRain(int x, int y);//10d
    static void DrawSnow(int x, int y);//013d

    static void DrawFogg(int x, int y);//50d

    static void DrawMediumSun(int x, int y);
    static void DrawMediumMoon(int x, int y);

    static void DrawSmallSun(int x, int y);
    static void DrawSmallMoon(int x, int y);

    static void DrawWhiteCloud(int x, int y);
    static void DrawBlackCloud(int x, int y);
    

    static void DrawPolarRay(int middleX, int middleY, int length, int angle);
    static void DrawLine(PointData p1, PointData p2, uint16_t color);
    static void FillTriangle(PointData p1, PointData p2, PointData p3, uint16_t color);
};