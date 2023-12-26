#include "EPaperWeatherDrawer.h"

GxEPD2_3C<GxEPD2_420c, GxEPD2_420c::HEIGHT> display(GxEPD2_420c(D8, D3, D1, D2));
//GxEPD2_3C<GxEPD2_420c_Z21, GxEPD2_420c_Z21::HEIGHT> display(GxEPD2_420c_Z21(D8, D3, D1, D2));

void EPaperWeatherDrawer::setup()
{
  display.init(115200);

  display.setRotation(1);
  display.setFullWindow();
}

  void EPaperWeatherDrawer::drawOneCallData(const OneCallData& oneCallData)
  {
    display.firstPage();

    do 
    {
      display.fillScreen(GxEPD_WHITE);
     
      drawDailyData(0, 100, oneCallData.m_dailyData[0]);
    } 
    while(display.nextPage());

    display.hibernate();
  }

  void EPaperWeatherDrawer::drawDailyData(int x, int y, const DailyData& dailyData)
  {
    DrawIcon(x, y, dailyData.m_weatherIcon);
  }

  void EPaperWeatherDrawer::DrawIcon(int x, int y, const String& crIcon)
  {
    if (crIcon == "01d")
      DrawSun(x, y);//01d;
  // else if (crIcon == "01n")  
     DrawMoon(x, y);//01n
   }

  void EPaperWeatherDrawer::DrawSun(int offsetX, int offsetY) 
  {
    int middleX = 50 + offsetX;  
    int middleY = 50 + offsetY;

    int length = 70;

    display.drawLine(middleX-length/2, middleY, middleX+length/2, middleY, GxEPD_BLACK);//horz
    display.drawLine(middleX, middleY-length/2, middleX, middleY+length/2, GxEPD_BLACK);//vert

    for(int angle = 30; angle < 180; angle+=30) 
    {
      if (angle == 90)
        continue;
      
      DrawPolarRay(middleX, middleY, length, angle);
    }

  int radiusSun = 20;
  
  display.fillCircle(middleX, middleY, radiusSun, GxEPD_RED);
  display.drawCircle(middleX, middleY, radiusSun, GxEPD_BLACK);
}

void EPaperWeatherDrawer::DrawMoon(int offsetX, int offsetY) 
{
  offsetX+=3;
  
  const int radiusMoon = 24;
  
  display.fillCircle(45+offsetX, 45+offsetY, radiusMoon, GxEPD_RED);
  display.drawCircle(45+offsetX, 45+offsetY, radiusMoon, GxEPD_BLACK);
  
  const int offset = 12;
  display.fillCircle(45+offsetX-offset, 45+offsetY-offset, radiusMoon, GxEPD_WHITE);
  display.drawCircle(45+offsetX-offset, 45+offsetY-offset, radiusMoon, GxEPD_BLACK);
  
  // display.drawCircle(45+offsetX-offset-radiusMoon,        45+offsetY-offset-radiusMoon, 1, ST7735_RED);
  // display.drawCircle(45+offsetX-offset+radiusMoon+offset, 45+offsetY-offset-radiusMoon, 1, ST7735_RED);
  // display.drawCircle(45+offsetX-offset-radiusMoon,        45+offsetY-offset+radiusMoon+offset, 1, ST7735_RED);

  display.fillTriangle(45+offsetX-offset-radiusMoon,        45+offsetY-offset-radiusMoon, 
                       45+offsetX-offset+radiusMoon+offset, 45+offsetY-offset-radiusMoon, 
                       45+offsetX-offset-radiusMoon,        45+offsetY-offset+radiusMoon+offset, GxEPD_WHITE);
}


void EPaperWeatherDrawer::DrawPolarRay(int middleX, int middleY, int length, int angle) 
{
  int dX = length/2 * cos(radians(angle));
  int dY = length/2 * sin(radians(angle));  

  display.drawLine(middleX + dX, middleY + dY, middleX - dX, middleY - dY, GxEPD_BLACK);
}