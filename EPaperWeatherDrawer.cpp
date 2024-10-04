
//#include <Fonts/FreeMono9pt7b.h>
// #include <Fonts/FreeSans9pt7b.h>
// #include <Fonts/FreeSans12pt7b.h>
// #include <Fonts/FreeSans18pt7b.h>

// #include <Fonts/FreeSansBold9pt7b.h>
// #include <Fonts/FreeSansBold12pt7b.h>
// #include <Fonts/FreeSansBold18pt7b.h>

//more fonts: D:\Projekte\Arduino\libraries\Adafruit_GFX_Library\Fonts

#include "EPaperWeatherDrawer.h"

GxEPD2_3C<GxEPD2_420c, GxEPD2_420c::HEIGHT> display(GxEPD2_420c(D8, D3, D1, D2));
//GxEPD2_3C<GxEPD2_420c_Z21, GxEPD2_420c_Z21::HEIGHT> display(GxEPD2_420c_Z21(D8, D3, D1, D2));

U8G2_FOR_ADAFRUIT_GFX u8g2Fonts;

const  int COLOR_FOREGROUND = GxEPD_BLACK;
const  int COLOR_BACKGROUND = GxEPD_WHITE;
const  int COLOR_RED = GxEPD_RED;

const char* weekdays[] = { "Sonntag", "Montag", "Dienstag", "Mittwoch", "Donnerstag", "Freitag", "Samstag" };
const char* monthNames[] = { "Januar", "Februar", "März", "April", "Mai", "Juni", "Juli", "August", "September", "Oktober", "November", "Dezember" };

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

void EPaperWeatherDrawer::drawOneCallData(const OneCallData& oneCallData, UnixTime unixTime)
{
  // Serial.println("Drawing disabled!!!");
  // return;

  display.firstPage();
  Serial.println(String("GxEPD2_420c::WIDTH: ") + GxEPD2_420c::WIDTH + String(", GxEPD2_420c::HEIGHT: ") + GxEPD2_420c::HEIGHT);

  do 
  {
    display.fillScreen(COLOR_BACKGROUND);

    drawTodayData(0, 0, unixTime, oneCallData.m_currentData, oneCallData.m_dailyData[0]);

    drawDailyData(  0, 200, oneCallData.m_dailyData[1]);
    drawDailyData(100, 200, oneCallData.m_dailyData[2]);
    drawDailyData(200, 200, oneCallData.m_dailyData[3]);

    // display.drawLine(100, 100, 100, 400, COLOR_FOREGROUND);
    // display.drawLine(200, 100, 200, 400, COLOR_RED);
    
    // display.drawLine(0, 100, 300, 100, COLOR_RED);
    display.drawLine(0, 200, 300, 200, COLOR_RED);
    //display.drawLine(0, 300, 300, 300, COLOR_RED);
  } 
  while(display.nextPage());

  display.hibernate();
}

void EPaperWeatherDrawer::drawTodayData(int x, int y, UnixTime unixTime, const CurrentData& currentData, const DailyData& todayData)
{
  String textOut;
  int16_t textWidth, textHeight;
  int16_t startX = x; 
  int16_t startYNext = y; 

  u8g2Fonts.setFont(u8g2_font_helvR14_tf);
  textHeight = u8g2Fonts.getFontAscent() - u8g2Fonts.getFontDescent();
  startYNext += textHeight;

  unixTime.getDateTime(currentData.m_dateTime);

//day
  textOut = String(weekdays[todayData.m_dayOfWeek]) + ", " + String(unixTime.day) + ". " + String(monthNames[unixTime.month-1]);

  textWidth = u8g2Fonts.getUTF8Width(textOut.c_str());

  u8g2Fonts.setCursor(startX + ((display.width()-textWidth) / 2), startYNext);
  u8g2Fonts.print(textOut);

//time
  String fill = unixTime.minute < 10 ? ":0" : ":";
  textOut = String(unixTime.hour) + fill + String(unixTime.minute);

  textWidth = u8g2Fonts.getUTF8Width(textOut.c_str());

  u8g2Fonts.setCursor(startX + ((display.width()-textWidth)) - 5, startYNext);
  u8g2Fonts.print(textOut);

}

void EPaperWeatherDrawer::drawDailyData(int x, int y, const DailyData& dailyData)
{
  int startX = x;
  int startY = y;

  String textOut;
  int16_t textWidth, textHeight;
  int16_t startYNext = startY; 

//weekday
  u8g2Fonts.setFont(u8g2_font_helvR14_tf);

  textHeight = u8g2Fonts.getFontAscent() - u8g2Fonts.getFontDescent();
  startYNext += textHeight;

  textOut = weekdays[dailyData.m_dayOfWeek];

  textWidth = u8g2Fonts.getUTF8Width(textOut.c_str());

  u8g2Fonts.setCursor(startX + ((100-textWidth) / 2), startYNext);
  u8g2Fonts.print(textOut);

//draw Icon
  //startY += 70;
  DrawIcon(startX, startYNext, dailyData.m_weatherIcon);

  startYNext += 100;

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

  textOut = String(dailyData.m_tempMax, 0) + "° | " + String(dailyData.m_tempMin, 0) + "°";
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
    DrawRain(x, y);//10d  

  else if (crIcon == "10d")
    DrawMediumSunWithCloudAndRain(x, y);//10d  
  else if (crIcon == "10n")
    DrawMediumMoonWithCloudAndRain(x, y);//10n

  else if (crIcon == "11d")
    DrawThunderstorm(x, y);//11d

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

void EPaperWeatherDrawer::DrawSun_Small(int offsetX, int offsetY) {
  int middleX = 25 + offsetX;  
  int middleY = 25 + offsetY;

  int length = 35;

  display.drawLine(middleX-length/2, middleY, middleX+length/2, middleY, COLOR_FOREGROUND);//horz
  display.drawLine(middleX, middleY-length/2, middleX, middleY+length/2, COLOR_FOREGROUND);//vert

  for(int angle = 30; angle < 180; angle+=30) {
    if (angle == 90)
      continue;
      
    DrawPolarRay(middleX, middleY, length, angle);
  }

  int radiusSun = 10;
  
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

void DrawMoon_Small(int offsetX, int offsetY) {
  offsetX+=1;

  offsetX+=22;
  offsetY+=22;
  
  const int radiusMoon = 12;
  
  display.fillCircle(offsetX, offsetY, radiusMoon, COLOR_RED);
  display.drawCircle(offsetX, offsetY, radiusMoon, COLOR_FOREGROUND);
  
  const int offset = 6;
  display.fillCircle(offsetX-offset, offsetY-offset, radiusMoon, COLOR_BACKGROUND);
  display.drawCircle(offsetX-offset, offsetY-offset, radiusMoon, COLOR_FOREGROUND);
  
  // display.drawCircle(offsetX-offset-radiusMoon,        offsetY-offset-radiusMoon, 1, COLOR_RED);
  // display.drawCircle(offsetX-offset+radiusMoon+offset, offsetY-offset-radiusMoon, 1, COLOR_RED);
  // display.drawCircle(offsetX-offset-radiusMoon,        offsetY-offset+radiusMoon+offset, 1, COLOR_RED);

  display.fillTriangle(offsetX-offset-radiusMoon,        offsetY-offset-radiusMoon, 
                       offsetX-offset+radiusMoon+offset, offsetY-offset-radiusMoon, 
                       offsetX-offset-radiusMoon,        offsetY-offset+radiusMoon+offset, COLOR_BACKGROUND);
}

void EPaperWeatherDrawer::DrawMediumSunWithCloud(int offsetX, int offsetY) 
{
    Serial.println("DrawMediumSunWithCloud at: " + String(offsetX) + ", " + offsetY);
    DrawMediumSun(8 + offsetX, 9 + offsetY);
    DrawWhiteCloud(12 + offsetX, 37 + offsetY);
}
void EPaperWeatherDrawer::DrawMediumSunWithCloud_Small(int offsetX, int offsetY) 
{
    DrawMediumSun_Small(offsetX+8, offsetY+7);
    DrawWhiteCloud_Small(offsetX+10, offsetY+24);
}

void EPaperWeatherDrawer::DrawMediumMoonWithCloud(int offsetX, int offsetY) 
{
  DrawMediumMoon(-10 + offsetX, -10 + offsetY);
  DrawWhiteCloud(12  + offsetX, 37 + offsetY);
}  
void EPaperWeatherDrawer::DrawMediumMoonWithCloud_Small(int offsetX, int offsetY) 
{
  DrawMediumMoon_Small(offsetX-10, offsetY-11);
  DrawWhiteCloud_Small(offsetX+10, offsetY+19);
}  

void EPaperWeatherDrawer::DrawBlackAndWhiteCloudWithSun(int offsetX, int offsetY) 
{
  Serial.println("DrawBlackAndWhiteCloudWithSun at: " + String(offsetX) + ", " + offsetY);
  DrawSmallSun(offsetX+10, offsetY+9);
  DrawBlackAndWhiteCloud(offsetX, offsetY);
}
void EPaperWeatherDrawer::DrawBlackAndWhiteCloudWithSun_Small(int offsetX, int offsetY) 
{
  DrawSmallSun_Small(offsetX, offsetY);
  DrawBlackAndWhiteCloud_Small(offsetX, offsetY+10);
}

void EPaperWeatherDrawer::DrawBlackAndWhiteCloudWithMoon(int offsetX, int offsetY) 
{
    Serial.println("DrawBlackAndWhiteCloudWithMoon at: " + String(offsetX) + ", " + offsetY);
    DrawSmallMoon(offsetX-15, offsetY-7);
    DrawBlackAndWhiteCloud(offsetX, offsetY+7);
}
void EPaperWeatherDrawer::DrawBlackAndWhiteCloudWithMoon_Small(int offsetX, int offsetY) 
{
    DrawSmallMoon_Small(offsetX-29, offsetY-29);
    DrawBlackAndWhiteCloud_Small(offsetX+3, offsetY+16);
}

void EPaperWeatherDrawer::DrawBlackAndWhiteCloud(int offsetX, int offsetY) 
{
  offsetX += 7;
  offsetY += 25;
  DrawWhiteCloud(offsetX+10, offsetY);
  DrawBlackCloud(offsetX   , offsetY+10);
}
void EPaperWeatherDrawer::DrawBlackAndWhiteCloud_Small(int offsetX, int offsetY) 
{
  DrawWhiteCloud_Small(offsetX+10, offsetY);
  DrawBlackCloud_Small(offsetX+3 , offsetY+5);
}

void EPaperWeatherDrawer::DrawMediumSunWithCloudAndRain(int offsetX, int offsetY)//09d
{
  DrawMediumSunWithCloud(offsetX, offsetY);
    
  for(int i = 0; i <= 50; i+=8) 
  {
    display.drawLine(i+24+offsetX, 70+offsetY, i+27+offsetX, 77+offsetY, COLOR_FOREGROUND);
  }
}
void EPaperWeatherDrawer::DrawMediumSunWithCloudAndRain_Small(int offsetX, int offsetY) 
{
  DrawMediumSunWithCloud_Small(offsetX, offsetY);

  for(int i = 0; i <= 20; i+=4) {
    display.drawLine(i+16+offsetX, 41+offsetY, i+18+offsetX, 43+offsetY, COLOR_FOREGROUND);
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
void EPaperWeatherDrawer::DrawMediumMoonWithCloudAndRain_Small(int offsetX, int offsetY) 
{
  DrawMediumMoonWithCloud_Small(offsetX, offsetY);
    
  for(int i = 0; i <= 20; i+=4) {
    display.drawLine(i+15+offsetX, 36+offsetY, i+17+offsetX, 39+offsetY, COLOR_FOREGROUND);
  } 
}

void EPaperWeatherDrawer::DrawLine(PointData p1, PointData p2, uint16_t color) {
  display.drawLine(p1.x, p1.y, p2.x, p2.y, color);
}

void EPaperWeatherDrawer::FillTriangle(PointData p1, PointData p2, PointData p3, uint16_t color) {
  display.fillTriangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, color);
}

void EPaperWeatherDrawer::DrawThunderstorm(int offsetX, int offsetY) {
  DrawRain(offsetX, offsetY);

  int x = offsetX+5;
  int y = offsetY+46;

  PointData p1(44+x, 24+y);
  PointData p2(35+x, 24+y);
  PointData p3(24+x, 40+y);
  PointData p4(30+x, 40+y);
  PointData p5(22+x, 55+y);
  PointData p6(40+x, 35+y);
  PointData p7(36+x, 35+y);

  FillTriangle(p1, p2, p3, COLOR_RED);
  FillTriangle(p3, p1, p7, COLOR_RED);
  FillTriangle(p3, p4, p7, COLOR_RED);
  FillTriangle(p4, p6, p7, COLOR_RED);
  FillTriangle(p4, p6, p5, COLOR_RED);

  DrawLine(p1, p2, COLOR_FOREGROUND);
  DrawLine(p2, p3, COLOR_FOREGROUND);
  DrawLine(p3, p4, COLOR_FOREGROUND);
  DrawLine(p4, p5, COLOR_FOREGROUND);
  DrawLine(p5, p6, COLOR_FOREGROUND);
  DrawLine(p6, p7, COLOR_FOREGROUND);
  DrawLine(p7, p1, COLOR_FOREGROUND);
}
void EPaperWeatherDrawer::DrawThunderstorm_Small(int offsetX, int offsetY) 
{
  DrawRain_Small(offsetX+2, offsetY);

  int x = offsetX+9;
  int y = offsetY+136;

//  1 ----------7
//  /          /
// 2 -- 3  5--6
//   /    /
//  /  /
//  4

  PointData p1((44+x)/2, (24+y)/2);
  PointData p2((/*36*/32+x)/2, (24+y)/2);
  PointData p3((24+x)/2, (40+y)/2);
  PointData p4((30+x)/2, (40+y)/2);
  PointData p5((22+x)/2, (55+y)/2);
  PointData p6((40+x)/2, (35+y)/2);
  PointData p7((36+x)/2, (35+y)/2);

  FillTriangle(p1, p2, p3, COLOR_RED);
  FillTriangle(p3, p1, p7, COLOR_RED);
  FillTriangle(p3, p4, p7, COLOR_RED);
  FillTriangle(p4, p6, p7, COLOR_RED);
  FillTriangle(p4, p6, p5, COLOR_RED);

  DrawLine(p1, p2, COLOR_FOREGROUND);
  DrawLine(p2, p3, COLOR_FOREGROUND);
  DrawLine(p3, p4, COLOR_FOREGROUND);
  DrawLine(p4, p5, COLOR_FOREGROUND);
  DrawLine(p5, p6, COLOR_FOREGROUND);
  DrawLine(p6, p7, COLOR_FOREGROUND);
  DrawLine(p7, p1, COLOR_FOREGROUND);
  //DrawLine(p8, p1, COLOR_FOREGROUND);
}

void EPaperWeatherDrawer::DrawRain(int offsetX, int offsetY)//10d
{
  offsetX += 4;
  offsetY += 20;

  DrawWhiteCloud(offsetX+20, offsetY);
  DrawBlackCloud(offsetX   , offsetY+24);

  for(int i = 40; i <= 60; i+=8) {
    display.drawLine(i+22+offsetX, 33+offsetY, i+25+offsetX, 40+offsetY, COLOR_FOREGROUND);
  }
  
  for(int i = 0; i <= 50; i+=8) {
      display.drawLine(i+12+offsetX, 57+offsetY, i+15+offsetX, 64+offsetY, COLOR_FOREGROUND);
  }
} 
void EPaperWeatherDrawer::DrawRain_Small(int offsetX, int offsetY) 
{
  DrawWhiteCloud_Small(offsetX+10, offsetY+1);
  DrawBlackCloud_Small(offsetX+ 2, offsetY+9);

  for(int i = 28; i <= 36; i+=4) {
    display.drawLine(i+offsetX, 18+offsetY, i+2+offsetX, 20+offsetY, COLOR_FOREGROUND);
  }
  
  for(int i = 0; i <= 16; i+=4) {
      display.drawLine(i+9+offsetX, 27+offsetY, i+11+offsetX, 29+offsetY, COLOR_FOREGROUND);
  }
}  
void EPaperWeatherDrawer::DrawSnow(int offsetX, int offsetY) //13d
{
  offsetX += 4;
  offsetY += 15;

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
void EPaperWeatherDrawer::DrawSnow_Small(int offsetX, int offsetY) 
{
  offsetY+=10;

  DrawBlackAndWhiteCloud_Small(offsetX, offsetY);
  
  offsetY+=3;
  for(int i = 0; i <= 20; i+=6) {
    display.drawCircle(i+8+offsetX, 22+offsetY, 1, COLOR_FOREGROUND);
    display.drawCircle(i+11+offsetX, 25+offsetY, 1, COLOR_FOREGROUND);
    // display.drawCircle(i+8+offsetX, 29+offsetY, 1, COLOR_FOREGROUND);
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
void EPaperWeatherDrawer::DrawFogg_Small(int offsetX, int offsetY) 
{
  display.drawLine(15+offsetX, 18+offsetY, 33+offsetX, 18+offsetY, COLOR_FOREGROUND);
  display.drawLine(7+offsetX, 22+offsetY, 35+offsetX, 22+offsetY, COLOR_FOREGROUND);
  display.drawLine(11+offsetX, 26+offsetY, 30+offsetX, 26+offsetY, COLOR_FOREGROUND);
  display.drawLine(12+offsetX, 30+offsetY, 40+offsetX, 30+offsetY, COLOR_FOREGROUND);
  display.drawLine(19+offsetX, 34+offsetY, 33+offsetX, 34+offsetY, COLOR_FOREGROUND);
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
void EPaperWeatherDrawer::DrawMediumSun_Small(int offsetX, int offsetY) 
{
  int middleX = 15 + offsetX;  
  int middleY = 15 + offsetY;

  int length = 30;

  display.drawLine(middleX-length/2, middleY, middleX+length/2, middleY, COLOR_FOREGROUND);//horz
  display.drawLine(middleX, middleY-length/2, middleX, middleY+length/2, COLOR_FOREGROUND);//vert

  for(int angle = 30; angle < 180; angle+=30) {
    if (angle == 90)
      continue;
      
    DrawPolarRay(middleX, middleY, length, angle);
  }

  int radiusSun = 8;
  
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
void EPaperWeatherDrawer::DrawMediumMoon_Small(int offsetX, int offsetY) 
{
  offsetX+=32;
  offsetY+=29;
  
  const int radiusMoon = 10;
  
  display.fillCircle(offsetX, offsetY, radiusMoon, COLOR_RED);
  display.drawCircle(offsetX, offsetY, radiusMoon, COLOR_FOREGROUND);
  
  const int offset = 6;
  display.fillCircle(offsetX-offset, offsetY-offset, radiusMoon, COLOR_BACKGROUND);
  display.drawCircle(offsetX-offset, offsetY-offset, radiusMoon, COLOR_FOREGROUND);
  
  // display.drawCircle(offsetX-offset-radiusMoon,        offsetY-offset-radiusMoon, 1, COLOR_RED);
  // display.drawCircle(offsetX-offset+radiusMoon+offset, offsetY-offset-radiusMoon, 1, COLOR_RED);
  // display.drawCircle(offsetX-offset-radiusMoon,        offsetY-offset+radiusMoon+offset, 1, COLOR_RED);

  display.fillTriangle(offsetX-offset-radiusMoon,        offsetY-offset-radiusMoon, 
                       offsetX-offset+radiusMoon+offset, offsetY-offset-radiusMoon, 
                       offsetX-offset-radiusMoon,        offsetY-offset+radiusMoon+offset, COLOR_BACKGROUND);
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
void EPaperWeatherDrawer::DrawSmallSun_Small(int offsetX, int offsetY) 
{
  int middleX = 14 + offsetX;  
  int middleY = 14 + offsetY;

  int length = 26;

  display.drawLine(middleX-length/2, middleY, middleX+length/2, middleY, COLOR_FOREGROUND);//horz
  display.drawLine(middleX, middleY-length/2, middleX, middleY+length/2, COLOR_FOREGROUND);//vert

  for(int angle = 30; angle < 180; angle+=30) {
    if (angle == 90)
      continue;
      
    DrawPolarRay(middleX, middleY, length, angle);
  }

  int radiusSun = 8;
  
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

  display.fillTriangle(45+offsetX-offset-radiusMoon,        45+offsetY-offset-radiusMoon, 
                       45+offsetX-offset+radiusMoon+offset, 45+offsetY-offset-radiusMoon, 
                       45+offsetX-offset-radiusMoon,        45+offsetY-offset+radiusMoon+offset, COLOR_BACKGROUND);

}
void EPaperWeatherDrawer::DrawSmallMoon_Small(int offsetX, int offsetY) 
{
  offsetX+=48;
  offsetY+=45;
  
  const int radiusMoon = 9;
  
  display.fillCircle(offsetX, offsetY, radiusMoon, COLOR_RED);
  display.drawCircle(offsetX, offsetY, radiusMoon, COLOR_FOREGROUND);
  
  const int offset = 4;
  display.fillCircle(offsetX-offset, offsetY-offset, radiusMoon, COLOR_BACKGROUND);
  display.drawCircle(offsetX-offset, offsetY-offset, radiusMoon, COLOR_FOREGROUND);

  display.fillTriangle(offsetX-offset-radiusMoon,        offsetY-offset-radiusMoon, 
                       offsetX-offset+radiusMoon+offset, offsetY-offset-radiusMoon, 
                       offsetX-offset-radiusMoon,        offsetY-offset+radiusMoon+offset, COLOR_BACKGROUND);
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
void EPaperWeatherDrawer::DrawWhiteCloud_Small(int offsetX, int offsetY) 
{

  display.drawCircle(5+offsetX, 12+offsetY, 4, COLOR_FOREGROUND);
  display.drawCircle(12+offsetX, 10+offsetY, 7, COLOR_FOREGROUND);
  display.drawCircle(18+offsetX,  9+offsetY, 8, COLOR_FOREGROUND);
  display.drawCircle(26+offsetX, 12+offsetY, 4, COLOR_FOREGROUND);

  display.fillCircle(5+offsetX, 12+offsetY, 3, COLOR_BACKGROUND);
  display.fillCircle(12+offsetX, 10+offsetY, 6, COLOR_BACKGROUND);
  display.fillCircle(18+offsetX,  9+offsetY, 7, COLOR_BACKGROUND);
  display.fillCircle(26+offsetX, 12+offsetY, 3, COLOR_BACKGROUND);
  
  display.fillRect(6+offsetX, 14+offsetY, 21, 4, COLOR_BACKGROUND);
  display.drawLine(5+offsetX, 16+offsetY, 26+offsetX, 16+offsetY, COLOR_FOREGROUND);
}

void EPaperWeatherDrawer::DrawBlackCloud(int offsetX, int offsetY) 
{
  display.fillCircle(10+offsetX, 25+offsetY, 8, COLOR_FOREGROUND);
  display.fillCircle(25+offsetX, 19+offsetY, 14, COLOR_FOREGROUND);
  display.fillCircle(43+offsetX, 17+offsetY, 16, COLOR_FOREGROUND);
  display.fillCircle(60+offsetX, 23+offsetY, 10, COLOR_FOREGROUND);

  display.fillRect(10+offsetX, 28+offsetY, 50, 6, COLOR_FOREGROUND);
}
void EPaperWeatherDrawer::DrawBlackCloud_Small(int offsetX, int offsetY) {

  display.fillCircle(5+offsetX, 12+offsetY, 4, COLOR_FOREGROUND);
  display.fillCircle(12+offsetX, 10+offsetY, 7, COLOR_FOREGROUND);
  display.fillCircle(18+offsetX,  9+offsetY, 8, COLOR_FOREGROUND);
  display.fillCircle(26+offsetX, 12+offsetY, 4, COLOR_FOREGROUND);

  display.fillRect(6+offsetX, 14+offsetY, 21, 4, COLOR_FOREGROUND);
}

void EPaperWeatherDrawer::DrawPolarRay(int middleX, int middleY, int length, int angle) 
{
  int dX = length/2 * cos(radians(angle));
  int dY = length/2 * sin(radians(angle));  

  display.drawLine(middleX + dX, middleY + dY, middleX - dX, middleY - dY, COLOR_FOREGROUND);
}