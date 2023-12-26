#include "EPaperWeatherDrawer.h"

GxEPD2_3C<GxEPD2_420c, GxEPD2_420c::HEIGHT> display(GxEPD2_420c(D8, D3, D1, D2));
//GxEPD2_3C<GxEPD2_420c_Z21, GxEPD2_420c_Z21::HEIGHT> display(GxEPD2_420c_Z21(D8, D3, D1, D2));

const  int COLOR_FOREGROUND = GxEPD_BLACK;
const  int COLOR_BACKGROUND = GxEPD_WHITE;
const  int COLOR_RED = GxEPD_RED;

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
      display.fillScreen(COLOR_BACKGROUND);

      drawDailyData(0, 100, oneCallData.m_dailyData[0]);
      drawDailyData(0, 200, oneCallData.m_dailyData[1]);
      drawDailyData(0, 300, oneCallData.m_dailyData[2]);

      display.drawLine(100, 0, 100, 400, COLOR_FOREGROUND);
      
      display.drawLine(0, 100, 100, 100, COLOR_RED);
      display.drawLine(0, 200, 100, 200, COLOR_RED);
      display.drawLine(0, 300, 100, 300, COLOR_RED);
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
    Serial.println("drawing icon: " + crIcon + " at: " + x + ", " + y);

    if (crIcon == "01d")
      DrawSun(x, y);//01d;
    else if (crIcon == "01n")    
      DrawMoon(x, y);//01n;

    else if (crIcon == "02d")
      DrawMediumSunWithCloud(x, y);//02d
    else if (crIcon == "02n")
      DrawMediumMoonWithCloud(x, y);//02n

    else if (crIcon == "03d")
      DrawBlackAndWhiteCloudWithSun(x, y);//03d  
    else if (crIcon == "03n")
      DrawBlackAndWhiteCloudWithMoon(x, y);//03n
   }

  void EPaperWeatherDrawer::DrawSun(int offsetX, int offsetY) 
  {
    int middleX = 50 + offsetX;  
    int middleY = 50 + offsetY;

    int length = 70;

    display.drawLine(middleX-length/2, middleY, middleX+length/2, middleY, COLOR_FOREGROUND);//horz
    display.drawLine(middleX, middleY-length/2, middleX, middleY+length/2, COLOR_FOREGROUND);//vert

    for(int angle = 30; angle < 180; angle+=30) 
    {
      if (angle == 90)
        continue;
      
      DrawPolarRay(middleX, middleY, length, angle);
    }

  int radiusSun = 20;
  
  display.fillCircle(middleX, middleY, radiusSun, COLOR_RED);
  display.drawCircle(middleX, middleY, radiusSun, COLOR_FOREGROUND);
}

void EPaperWeatherDrawer::DrawMoon(int offsetX, int offsetY) 
{
  offsetX+=3;
  
  const int radiusMoon = 24;
  
  display.fillCircle(45+offsetX, 45+offsetY, radiusMoon, COLOR_RED);
  display.drawCircle(45+offsetX, 45+offsetY, radiusMoon, COLOR_FOREGROUND);
  
  const int offset = 12;
  display.fillCircle(45+offsetX-offset, 45+offsetY-offset, radiusMoon, COLOR_BACKGROUND);
  display.drawCircle(45+offsetX-offset, 45+offsetY-offset, radiusMoon, COLOR_FOREGROUND);
  
  // display.drawCircle(45+offsetX-offset-radiusMoon,        45+offsetY-offset-radiusMoon, 1, COLOR_RED);
  // display.drawCircle(45+offsetX-offset+radiusMoon+offset, 45+offsetY-offset-radiusMoon, 1, COLOR_RED);
  // display.drawCircle(45+offsetX-offset-radiusMoon,        45+offsetY-offset+radiusMoon+offset, 1, COLOR_RED);

  display.fillTriangle(45+offsetX-offset-radiusMoon,        45+offsetY-offset-radiusMoon, 
                       45+offsetX-offset+radiusMoon+offset, 45+offsetY-offset-radiusMoon, 
                       45+offsetX-offset-radiusMoon,        45+offsetY-offset+radiusMoon+offset, COLOR_BACKGROUND);
}

void EPaperWeatherDrawer::DrawMediumSunWithCloud(int offsetX, int offsetY) 
{
    Serial.println("DrawMediumSunWithCloud at: " + String(offsetX) + ", " + offsetY);
    DrawMediumSun(8 + offsetX, 9 + offsetY);
    DrawWhiteCloud(12 + offsetX, 37 + offsetY);
}

void EPaperWeatherDrawer::DrawMediumMoonWithCloud(int offsetX, int offsetY) 
{
  DrawMediumMoon(-10 + offsetX, -10 + offsetY);
  DrawWhiteCloud(12  + offsetX, 37 + offsetY);
}  

void EPaperWeatherDrawer::DrawBlackAndWhiteCloudWithSun(int offsetX, int offsetY) 
{
  Serial.println("DrawBlackAndWhiteCloudWithSun at: " + String(offsetX) + ", " + offsetY);
  DrawSmallSun(offsetX+10, offsetY+9);
  DrawBlackAndWhiteCloud(offsetX+7, offsetY+32);
}

void EPaperWeatherDrawer::DrawBlackAndWhiteCloudWithMoon(int offsetX, int offsetY) 
{
    Serial.println("DrawBlackAndWhiteCloudWithMoon at: " + String(offsetX) + ", " + offsetY);
    DrawSmallMoon(offsetX-15, offsetY-10);
    DrawBlackAndWhiteCloud(offsetX+7, offsetY+32);
}  

void EPaperWeatherDrawer::DrawMediumSun(int offsetX, int offsetY) 
{
  int middleX = 30 + offsetX;  
  int middleY = 30 + offsetY;

  int length = 60;

  display.drawLine(middleX-length/2, middleY, middleX+length/2, middleY, COLOR_FOREGROUND);//horz
  display.drawLine(middleX, middleY-length/2, middleX, middleY+length/2, COLOR_FOREGROUND);//vert

  for(int angle = 30; angle < 180; angle+=30) {
    if (angle == 90)
      continue;
      
    DrawPolarRay(middleX, middleY, length, angle);
  }

  int radiusSun = 17;
  
  display.fillCircle(middleX, middleY, radiusSun, COLOR_RED);
  display.drawCircle(middleX, middleY, radiusSun, COLOR_FOREGROUND);
}

void EPaperWeatherDrawer::DrawMediumMoon(int offsetX, int offsetY) 
{
  offsetX+=3;
  
  const int radiusMoon = 20;
  
  display.fillCircle(45+offsetX, 45+offsetY, radiusMoon, COLOR_RED);
  display.drawCircle(45+offsetX, 45+offsetY, radiusMoon, COLOR_FOREGROUND);
  
  const int offset = 10;
  display.fillCircle(45+offsetX-offset, 45+offsetY-offset, radiusMoon, COLOR_BACKGROUND);
  display.drawCircle(45+offsetX-offset, 45+offsetY-offset, radiusMoon, COLOR_FOREGROUND);
  
  // display.drawCircle(45+offsetX-offset-radiusMoon,        45+offsetY-offset-radiusMoon, 1, COLOR_RED);
  // display.drawCircle(45+offsetX-offset+radiusMoon+offset, 45+offsetY-offset-radiusMoon, 1, COLOR_RED);
  // display.drawCircle(45+offsetX-offset-radiusMoon,        45+offsetY-offset+radiusMoon+offset, 1, COLOR_RED);

  display.fillTriangle(45+offsetX-offset-radiusMoon,        45+offsetY-offset-radiusMoon, 
                       45+offsetX-offset+radiusMoon+offset, 45+offsetY-offset-radiusMoon, 
                       45+offsetX-offset-radiusMoon,        45+offsetY-offset+radiusMoon+offset, COLOR_BACKGROUND);
}
void EPaperWeatherDrawer::DrawSmallSun(int offsetX, int offsetY) 
{
  int middleX = 26 + offsetX;  
  int middleY = 26 + offsetY;

  int length = 53;

  display.drawLine(middleX-length/2, middleY, middleX+length/2, middleY, COLOR_FOREGROUND);//horz
  display.drawLine(middleX, middleY-length/2, middleX, middleY+length/2, COLOR_FOREGROUND);//vert

  for(int angle = 30; angle < 180; angle+=30) {
    if (angle == 90)
      continue;
      
    DrawPolarRay(middleX, middleY, length, angle);
  }

  int radiusSun = 15;
  
  display.fillCircle(middleX, middleY, radiusSun, COLOR_RED);
  display.drawCircle(middleX, middleY, radiusSun, COLOR_FOREGROUND);
}

void EPaperWeatherDrawer::DrawSmallMoon(int offsetX, int offsetY) 
{
  offsetX+=3;
  
  const int radiusMoon = 18;
  
  display.fillCircle(45+offsetX, 45+offsetY, radiusMoon, COLOR_RED);
  display.drawCircle(45+offsetX, 45+offsetY, radiusMoon, COLOR_FOREGROUND);
  
  const int offset = 9;
  display.fillCircle(45+offsetX-offset, 45+offsetY-offset, radiusMoon, COLOR_BACKGROUND);
  display.drawCircle(45+offsetX-offset, 45+offsetY-offset, radiusMoon, COLOR_FOREGROUND);
  
  // display.drawCircle(45+offsetX-offset-radiusMoon,        45+offsetY-offset-radiusMoon, 1, COLOR_RED);
  // display.drawCircle(45+offsetX-offset+radiusMoon+offset, 45+offsetY-offset-radiusMoon, 1, COLOR_RED);
  // display.drawCircle(45+offsetX-offset-radiusMoon,        45+offsetY-offset+radiusMoon+offset, 1, COLOR_RED);

  display.fillTriangle(45+offsetX-offset-radiusMoon,        45+offsetY-offset-radiusMoon, 
                       45+offsetX-offset+radiusMoon+offset, 45+offsetY-offset-radiusMoon, 
                       45+offsetX-offset-radiusMoon,        45+offsetY-offset+radiusMoon+offset, COLOR_BACKGROUND);

}

void EPaperWeatherDrawer::DrawBlackAndWhiteCloud(int offsetX, int offsetY) 
{
  DrawWhiteCloud(offsetX+10, offsetY);
  DrawBlackCloud(offsetX   , offsetY+10);
}

void EPaperWeatherDrawer::DrawWhiteCloud(int offsetX, int offsetY) 
{
  display.drawCircle(10+offsetX, 25+offsetY, 8, COLOR_FOREGROUND);
  display.drawCircle(25+offsetX, 19+offsetY, 14, COLOR_FOREGROUND);
  display.drawCircle(43+offsetX, 17+offsetY, 16, COLOR_FOREGROUND);
  display.drawCircle(60+offsetX, 23+offsetY, 10, COLOR_FOREGROUND);

  display.fillCircle(10+offsetX, 25+offsetY, 7, COLOR_BACKGROUND);
  display.fillCircle(25+offsetX, 19+offsetY, 13, COLOR_BACKGROUND);
  display.fillCircle(43+offsetX, 17+offsetY, 15, COLOR_BACKGROUND);
  display.fillCircle(60+offsetX, 23+offsetY, 9, COLOR_BACKGROUND);
  
  display.fillRect(10+offsetX, 28+offsetY, 50, 5, COLOR_BACKGROUND);
  display.drawLine(10+offsetX, 33+offsetY, 60+offsetX, 33+offsetY, COLOR_FOREGROUND);
}

void EPaperWeatherDrawer::DrawBlackCloud(int offsetX, int offsetY) 
{
  display.fillCircle(10+offsetX, 25+offsetY, 8, COLOR_FOREGROUND);
  display.fillCircle(25+offsetX, 19+offsetY, 14, COLOR_FOREGROUND);
  display.fillCircle(43+offsetX, 17+offsetY, 16, COLOR_FOREGROUND);
  display.fillCircle(60+offsetX, 23+offsetY, 10, COLOR_FOREGROUND);

  display.fillRect(10+offsetX, 28+offsetY, 50, 6, COLOR_FOREGROUND);
}

void EPaperWeatherDrawer::DrawPolarRay(int middleX, int middleY, int length, int angle) 
{
  int dX = length/2 * cos(radians(angle));
  int dY = length/2 * sin(radians(angle));  

  display.drawLine(middleX + dX, middleY + dY, middleX - dX, middleY - dY, COLOR_FOREGROUND);
}