#pragma once

#define ENABLE_GxEPD2_GFX 1

#include "GxEPD2_3C.h"


//#define __Test_Paint_DailyData__

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

    static void DrawSun(int x, int y);//01d
    static void DrawMoon(int x, int y);//01n

    static void DrawMediumSunWithCloud(int x, int y);//02d
    static void DrawMediumMoonWithCloud(int x, int y);//02n

    static void DrawBlackAndWhiteCloudWithSun(int x, int y);//03d
    static void DrawBlackAndWhiteCloudWithMoon(int x, int y);//03n

    static void DrawBlackAndWhiteCloud(int x, int y);//04d/n

    static void DrawMediumSunWithCloudAndRain(int x, int y);//09d
    static void DrawMediumMoonWithCloudAndRain(int x, int y);//09n

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
};