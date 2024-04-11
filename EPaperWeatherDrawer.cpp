
//#include <Fonts/FreeMono9pt7b.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSans18pt7b.h>

#include <Fonts/FreeSansBold9pt7b.h>
#include <Fonts/FreeSansBold12pt7b.h>
#include <Fonts/FreeSansBold18pt7b.h>

//more fonts: D:\Projekte\Arduino\libraries\Adafruit_GFX_Library\Fonts

#include "EPaperWeatherDrawer.h"
#include "UnicodeDrawer.h"

GxEPD2_3C<GxEPD2_420c, GxEPD2_420c::HEIGHT> display(GxEPD2_420c(D8, D3, D1, D2));
//GxEPD2_3C<GxEPD2_420c_Z21, GxEPD2_420c_Z21::HEIGHT> display(GxEPD2_420c_Z21(D8, D3, D1, D2));

U8G2_FOR_ADAFRUIT_GFX u8g2Fonts;

const  int COLOR_FOREGROUND = GxEPD_BLACK;
const  int COLOR_BACKGROUND = GxEPD_WHITE;
const  int COLOR_RED = GxEPD_RED;

void EPaperWeatherDrawer::setup()
{
  Serial.println("EPaperWeatherDrawer::setup() enter");

  display.init();

  u8g2Fonts.begin(display);  
  u8g2Fonts.setForegroundColor(COLOR_FOREGROUND);
  u8g2Fonts.setBackgroundColor(COLOR_BACKGROUND);

  display.setRotation(1);
  display.setFullWindow();

  //display.setUTF8Print(true);

  Serial.println("EPaperWeatherDrawer::setup() leaving");
}

void EPaperWeatherDrawer::drawOneCallData(const OneCallData& oneCallData)
{
  // Serial.println("Drawing disabled!!!");
  // return;

  display.firstPage();
  Serial.println(String("GxEPD2_420c::WIDTH: ") + GxEPD2_420c::WIDTH + String(", GxEPD2_420c::HEIGHT: ") + GxEPD2_420c::HEIGHT);

  do 
  {
    display.fillScreen(COLOR_BACKGROUND);

    drawDailyData(  0, 200, oneCallData.m_dailyData[1]);
    drawDailyData(100, 200, oneCallData.m_dailyData[2]);
    drawDailyData(200, 200, oneCallData.m_dailyData[3]);

    display.drawLine(100, 100, 100, 400, COLOR_FOREGROUND);
    display.drawLine(200, 100, 200, 400, COLOR_RED);
    
    display.drawLine(0, 100, 300, 100, COLOR_RED);
    display.drawLine(0, 200, 300, 200, COLOR_RED);
    //display.drawLine(0, 300, 300, 300, COLOR_RED);
  } 
  while(display.nextPage());

  display.hibernate();
}

void EPaperWeatherDrawer::drawDailyData(int x, int y, const DailyData& dailyData)
{
  int startX = x;
  int startY = y;

  String textOut;
  int16_t textWidth, textHeight;
  int16_t startYNext = startY + 10; 

  u8g2Fonts.setFont(u8g2_font_helvR14_tf);
  u8g2Fonts.setCursor(startX, startY);
  u8g2Fonts.print(String(dailyData.m_dayOfWeek));

  startY += 70;
  DrawIcon(startX, startY, dailyData.m_weatherIcon);

// weather description
  // u8g2Fonts.setFont(u8g2_font_helvB14_tf);

  // textHeight = u8g2Fonts.getFontAscent() - u8g2Fonts.getFontDescent();
  // startYNext += textHeight;

  // u8g2Fonts.setCursor(startX, startYNext);
  // u8g2Fonts.print(dailyData.m_weatherDescription);

// main temp
  u8g2Fonts.setFont(u8g2_font_helvB24_tf);

  textHeight = u8g2Fonts.getFontAscent() - u8g2Fonts.getFontDescent();
  startYNext += textHeight;

  textOut = String(dailyData.m_tempDay, 1) + "°";
  textWidth = u8g2Fonts.getUTF8Width(textOut.c_str());
  Serial.println(textOut + " length: " + String(textWidth));
  
  u8g2Fonts.setCursor(startX + ((100-textWidth) / 2), startYNext);
  u8g2Fonts.print(textOut);

// min / max temp
  u8g2Fonts.setFont(u8g2_font_helvR14_tf);

  textHeight = u8g2Fonts.getFontAscent() - u8g2Fonts.getFontDescent();
  startYNext += textHeight;

  textOut = String(dailyData.m_tempMin, 0) + "° | " + String(dailyData.m_tempMax, 0) + "°";
  textWidth = u8g2Fonts.getUTF8Width(textOut.c_str());
  Serial.println(textOut + " length: " + String(textWidth));

  u8g2Fonts.setCursor(startX + ((100-textWidth) / 2), startYNext);
  u8g2Fonts.print(textOut);
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

  else if (crIcon == "04d")
    DrawBlackAndWhiteCloud(x, y);//04d/n

  else if (crIcon == "09d")
    DrawMediumSunWithCloudAndRain(x, y);//09d  
  else if (crIcon == "09n")
    DrawMediumMoonWithCloudAndRain(x, y);//09n

  else if (crIcon == "10d")
    DrawRain(x, y);//10d  
  else if (crIcon == "13d")
    DrawSnow(x, y);//13d

  else if (crIcon == "50d")
    DrawFogg(x, y);//50d/n
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
  DrawBlackAndWhiteCloud(offsetX, offsetY);
}

void EPaperWeatherDrawer::DrawBlackAndWhiteCloudWithMoon(int offsetX, int offsetY) 
{
    Serial.println("DrawBlackAndWhiteCloudWithMoon at: " + String(offsetX) + ", " + offsetY);
    DrawSmallMoon(offsetX-15, offsetY-7);
    DrawBlackAndWhiteCloud(offsetX, offsetY+7);
}

void EPaperWeatherDrawer::DrawBlackAndWhiteCloud(int offsetX, int offsetY) 
{
  offsetX += 7;
  offsetY += 25;
  DrawWhiteCloud(offsetX+10, offsetY);
  DrawBlackCloud(offsetX   , offsetY+10);
}

void EPaperWeatherDrawer::DrawMediumSunWithCloudAndRain(int offsetX, int offsetY)//09d
{
  DrawMediumSunWithCloud(offsetX, offsetY);
    
  for(int i = 0; i <= 50; i+=8) 
  {
    display.drawLine(i+24+offsetX, 70+offsetY, i+27+offsetX, 77+offsetY, COLOR_FOREGROUND);
  }
}

void EPaperWeatherDrawer::DrawMediumMoonWithCloudAndRain(int offsetX, int offsetY)//09n
{
  DrawMediumMoonWithCloud(offsetX, offsetY);
    
  for(int i = 0; i <= 50; i+=8) 
  {
    display.drawLine(i+24+offsetX, 70+offsetY, i+27+offsetX, 77+offsetY, COLOR_FOREGROUND);
  }
}

void EPaperWeatherDrawer::DrawRain(int offsetX, int offsetY)//10d
{
  offsetY+=20;

  DrawWhiteCloud(offsetX+20, offsetY);
  DrawBlackCloud(offsetX   , offsetY+24);

  for(int i = 40; i <= 60; i+=8) {
    display.drawLine(i+22+offsetX, 33+offsetY, i+25+offsetX, 40+offsetY, COLOR_FOREGROUND);
  }
  
  for(int i = 0; i <= 50; i+=8) {
      display.drawLine(i+12+offsetX, 57+offsetY, i+15+offsetX, 64+offsetY, COLOR_FOREGROUND);
  }
} 

void EPaperWeatherDrawer::DrawSnow(int offsetX, int offsetY) //13d
{
  offsetY+=15;

  DrawWhiteCloud(offsetX+20, offsetY);
  DrawBlackCloud(offsetX   , offsetY+24);
    
  offsetY+=15;

  for (int j = 0; j < 2; j++)
  {
    int i = 0;
    for (; i <= 40; i+=8) 
    {
      display.drawCircle(i+16+offsetX, 47+offsetY, 1, COLOR_FOREGROUND);
      display.drawCircle(i+20+offsetX, 52+offsetY, 1, COLOR_FOREGROUND);
      display.drawCircle(i+16+offsetX, 57+offsetY, 1, COLOR_FOREGROUND);
    }
    display.drawCircle(i+16+offsetX, 57+offsetY, 1, COLOR_FOREGROUND);

    offsetX+=20;
    offsetY-=24;
  }
}

void EPaperWeatherDrawer::DrawFogg(int offsetX, int offsetY) //50d
{
  Serial.println("Draw Fog: " + String(offsetX) + ", " + offsetY);
  // display.drawLine(30, 29, 67, 29, COLOR_FOREGROUND);
  // display.drawLine(25, 36, 75, 36, COLOR_FOREGROUND);
  // display.drawLine(15, 43, 70, 43, COLOR_FOREGROUND);
  // display.drawLine(20, 50, 67, 50, COLOR_FOREGROUND);
  // display.drawLine(23, 57, 73, 57, COLOR_FOREGROUND);
  // display.drawLine(24, 64, 72, 64, COLOR_FOREGROUND);
  // display.drawLine(30, 71, 67, 71, COLOR_FOREGROUND);


  display.drawLine(30+offsetX, 36+offsetY, 67+offsetX, 36+offsetY, COLOR_FOREGROUND);
  //display.drawLine(25+offsetX, 36+offsetY, 75+offsetX, 36+offsetY, COLOR_FOREGROUND);
  display.drawLine(15+offsetX, 43+offsetY, 70+offsetX, 43+offsetY, COLOR_FOREGROUND);
  //display.drawLine(20+offsetX, 50+offsetY, 67+offsetX, 50+offsetY, COLOR_FOREGROUND);
  display.drawLine(23+offsetX, 50+offsetY, 60+offsetX, 50+offsetY, COLOR_FOREGROUND);
  display.drawLine(24+offsetX, 57+offsetY, 80+offsetX, 57+offsetY, COLOR_FOREGROUND);
  display.drawLine(37+offsetX, 64+offsetY, 67+offsetX, 64+offsetY, COLOR_FOREGROUND);
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