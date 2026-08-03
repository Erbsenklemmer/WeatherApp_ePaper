#include "EPaperWeatherDrawer2.h"

#include "lang_gr.h"

const char* City = "Endersbach";
String Hemisphere  = "north";                         // or "south"  
String UnitsShort  = "M";                             // Use 'M' for Metric or I for Imperial 

GxEPD2_3C<GxEPD2_420c, GxEPD2_420c::HEIGHT> display(GxEPD2_420c(D8, D3, D1, D2));
//GxEPD2_3C<GxEPD2_420c_Z21, GxEPD2_420c_Z21::HEIGHT> display(GxEPD2_420c_Z21(D8, D3, D1, D2));

U8G2_FOR_ADAFRUIT_GFX u8g2Fonts;

const  int COLOR_FOREGROUND = GxEPD_BLACK;
const  int COLOR_BACKGROUND = GxEPD_WHITE;
const  int COLOR_RED = GxEPD_RED;

const int SCREEN_WIDTH = GxEPD2_420c::WIDTH;
const int SCREEN_HEIGHT = GxEPD2_420c::HEIGHT;

boolean LargeIcon = true, SmallIcon = false;
#define Large  11           // For icon drawing, needs to be odd number for best effect
#define Small  5            // For icon drawing, needs to be odd number for best effect
String  time_str, date_str; // strings to hold time and received weather data

const char* weekdays[] = { "Sonntag", "Montag", "Dienstag", "Mittwoch", "Donnerstag", "Freitag", "Samstag" };
const char* monthNames[] = { "Januar", "Februar", "März", "April", "Mai", "Juni", "Juli", "August", "September", "Oktober", "November", "Dezember" };

void EPaperWeatherDrawer2::setup()
{
  Serial.println("EPaperWeatherDrawer::setup() enter");

  display.init();

  u8g2Fonts.begin(display);  
  u8g2Fonts.setForegroundColor(COLOR_FOREGROUND);
  u8g2Fonts.setBackgroundColor(COLOR_BACKGROUND);

  display.setRotation(1);
  display.setFullWindow();

  Serial.println("EPaperWeatherDrawer::setup() leaving");
}

void EPaperWeatherDrawer2::drawOneCallData(const OneCallData& oneCallData, UnixTime unixTime)
{
  // Serial.println("Drawing disabled!!!");
  // return;

  display.firstPage();
  Serial.println(String("GxEPD2_420c::WIDTH: ") + GxEPD2_420c::WIDTH + String(", GxEPD2_420c::HEIGHT: ") + GxEPD2_420c::HEIGHT);

  do 
  {
    display.fillScreen(COLOR_BACKGROUND);
    
    //here starts the code
    DrawHeadingSection();                 // Top line of the display
    DrawMainWeatherSection(172, 70, oneCallData);      // Centre section of display for Location, temperature, Weather report, current Wx Symbol and wind direction
    
  } 
  while(display.nextPage());

  display.hibernate();
}

void EPaperWeatherDrawer2::DrawHeadingSection()
{
  u8g2Fonts.setFont(u8g2_font_helvB08_tf);
  drawString(SCREEN_WIDTH / 2, 0, City, CENTER);
  drawString(SCREEN_WIDTH, 0, date_str, RIGHT);
  drawString(4, 0, time_str, LEFT);
  DrawBattery(65, 12);
  display.drawLine(0, 12, SCREEN_WIDTH, 12, COLOR_FOREGROUND);
}

void EPaperWeatherDrawer2::DrawBattery(int x, int y) {
  uint8_t percentage = 100;
  float voltage = analogRead(A0) / 4096.0 * 7.46;
  if (voltage > 1 ) { // Only display if there is a valid reading
    Serial.println("Voltage = " + String(voltage));
    percentage = 2836.9625 * pow(voltage, 4) - 43987.4889 * pow(voltage, 3) + 255233.8134 * pow(voltage, 2) - 656689.7123 * voltage + 632041.7303;
    if (voltage >= 4.20) percentage = 100;
    if (voltage <= 3.50) percentage = 0;
    display.drawRect(x + 15, y - 12, 19, 10, COLOR_FOREGROUND);
    display.fillRect(x + 34, y - 10, 2, 5, COLOR_FOREGROUND);
    display.fillRect(x + 17, y - 10, 15 * percentage / 100.0, 6, COLOR_FOREGROUND);
    drawString(x + 65, y - 11, String(percentage) + "%", RIGHT);
    //drawString(x + 13, y + 5,  String(voltage, 2) + "v", CENTER);
  }
}

void EPaperWeatherDrawer2::DrawMainWeatherSection(int x, int y, const OneCallData& oneCallData) {
  DisplayDisplayWindSection(x - 115, y - 3, oneCallData.m_dailyData[0].m_windDeg, oneCallData.m_dailyData[0].m_windSpeed, 40);
  //DisplayWXicon(x + 5, y - 5, WxConditions[0].Icon, LargeIcon);
  u8g2Fonts.setFont(u8g2_font_helvB10_tf);

  double trend = oneCallData.m_dailyData[0].m_pressure - oneCallData.m_dailyData[2].m_pressure;
  String trendTxt;
  if (trend > 0)    trendTxt = "+";
  if (trend > 0)    trendTxt = "-";
  if (fabs(trend) < 0.1)    trendTxt = "0";

  DrawPressureAndTrend(x - 120, y + 58, oneCallData.m_dailyData[0].m_pressure, trendTxt);
  u8g2Fonts.setFont(u8g2_font_helvB12_tf);
  // String Wx_Description = WxConditions[0].Forecast0;
  // if (WxConditions[0].Forecast1 != "") Wx_Description += " & " +  WxConditions[0].Forecast1;
  // if (WxConditions[0].Forecast2 != "" && WxConditions[0].Forecast1 != WxConditions[0].Forecast2) Wx_Description += " & " +  WxConditions[0].Forecast2;
  drawStringMaxWidth(x - 170, y + 83, 28, TitleCase(oneCallData.m_dailyData[0].m_weatherDescription), LEFT);
  DrawMainWx(x, y + 60, oneCallData);
  display.drawRect(0, y + 68, 232, 48, COLOR_FOREGROUND);
}

void EPaperWeatherDrawer2::DrawMainWx(int x, int y, const OneCallData& oneCallData) {
  u8g2Fonts.setFont(u8g2_font_helvB14_tf);
  drawString(x - 25, y - 22, String(oneCallData.m_dailyData[0].m_tempDay, 1) + "°" + (UnitsShort == "M" ? "C" : "F"), CENTER); // Show current Temperature
  u8g2Fonts.setFont(u8g2_font_helvB12_tf);
  drawString(x - 15, y - 3, String(oneCallData.m_dailyData[0].m_tempMax, 0) + "° | " + String(oneCallData.m_dailyData[0].m_tempMin, 0) + "°", CENTER); // Show forecast high and Low
  drawString(x + 30, y - 22, String(oneCallData.m_dailyData[0].m_humidity, 0) + "%", CENTER);
  // u8g2Fonts.setFont(u8g2_font_helvB10_tf);
  // drawString(x + 32, y - 3, "RH", CENTER);
}

void EPaperWeatherDrawer2::DisplayDisplayWindSection(int x, int y, float angle, float windspeed, int Cradius) {
  arrow(x, y, Cradius - 7, angle, 12, 18); // Show wind direction on outer circle of width and length
  u8g2Fonts.setFont(u8g2_font_helvB08_tf);
  int dxo, dyo, dxi, dyi;
  display.drawLine(0, 15, 0, y + Cradius + 30, COLOR_FOREGROUND);
  display.drawCircle(x, y, Cradius, COLOR_FOREGROUND);     // Draw compass circle
  display.drawCircle(x, y, Cradius + 1, COLOR_FOREGROUND); // Draw compass circle
  display.drawCircle(x, y, Cradius * 0.7, COLOR_FOREGROUND); // Draw compass inner circle
  for (float a = 0; a < 360; a = a + 22.5) {
    dxo = Cradius * cos((a - 90) * PI / 180);
    dyo = Cradius * sin((a - 90) * PI / 180);
    if (a == 45)  drawString(dxo + x + 10, dyo + y - 10, TXT_NE, CENTER);
    if (a == 135) drawString(dxo + x + 7,  dyo + y + 5,  TXT_SE, CENTER);
    if (a == 225) drawString(dxo + x - 15, dyo + y,      TXT_SW, CENTER);
    if (a == 315) drawString(dxo + x - 15, dyo + y - 10, TXT_NW, CENTER);
    dxi = dxo * 0.9;
    dyi = dyo * 0.9;
    display.drawLine(dxo + x, dyo + y, dxi + x, dyi + y, COLOR_FOREGROUND);
    dxo = dxo * 0.7;
    dyo = dyo * 0.7;
    dxi = dxo * 0.9;
    dyi = dyo * 0.9;
    display.drawLine(dxo + x, dyo + y, dxi + x, dyi + y, COLOR_FOREGROUND);
  }
  drawString(x, y - Cradius - 10,     TXT_N, CENTER);
  drawString(x, y + Cradius + 5,      TXT_S, CENTER);
  drawString(x - Cradius - 10, y - 3, TXT_W, CENTER);
  drawString(x + Cradius + 8,  y - 3, TXT_E, CENTER);
  drawString(x - 2, y - 20, WindDegToDirection(angle), CENTER);
  drawString(x + 3, y + 12, String(angle, 0) + "°", CENTER);
  drawString(x + 3, y - 3, String(windspeed, 1) + (UnitsShort == "M" ? "m/s" : "mph"), CENTER);
}

String EPaperWeatherDrawer2::WindDegToDirection(float winddirection) {
  int dir = int((winddirection / 22.5) + 0.5);
  String Ord_direction[16] = {TXT_N, TXT_NNE, TXT_NE, TXT_ENE, TXT_E, TXT_ESE, TXT_SE, TXT_SSE, TXT_S, TXT_SSW, TXT_SW, TXT_WSW, TXT_W, TXT_WNW, TXT_NW, TXT_NNW};
  return Ord_direction[(dir % 16)];
}

String EPaperWeatherDrawer2::TitleCase(const String& text){
  if (text.length() > 0) {
    String temp_text = text.substring(0,1);
    temp_text.toUpperCase();
    return temp_text + text.substring(1); // Title-case the string
  }
  else return text;
}

void EPaperWeatherDrawer2::DrawPressureAndTrend(int x, int y, int pressure, String slope) {
  drawString(x, y, String(pressure, (UnitsShort == "M" ? 0 : 1)) + (UnitsShort == "M" ? "mb" : "in"), CENTER);
  x = x + 40; y = y + 2;
  if      (slope == "+") {
    display.drawLine(x,  y, x + 4, y - 4, COLOR_FOREGROUND);
    display.drawLine(x + 4, y - 4, x + 8, y, COLOR_FOREGROUND);
  }
  else if (slope == "0") {
    display.drawLine(x + 4, y - 4, x + 8, y, COLOR_FOREGROUND);
    display.drawLine(x + 4, y + 4, x + 8, y, COLOR_FOREGROUND);
  }
  else if (slope == "-") {
    display.drawLine(x,  y, x + 4, y + 4, COLOR_FOREGROUND);
    display.drawLine(x + 4, y + 4, x + 8, y, COLOR_FOREGROUND);
  }
}

void EPaperWeatherDrawer2::drawString(int x, int y, String text, alignment align) {
  int16_t  x1, y1; //the bounds of x,y and w and h of the variable 'text' in pixels.
  uint16_t w, h;
  display.setTextWrap(false);
  display.getTextBounds(text, x, y, &x1, &y1, &w, &h);
  if (align == RIGHT)  x = x - w;
  if (align == CENTER) x = x - w / 2;
  u8g2Fonts.setCursor(x, y + h);
  u8g2Fonts.print(text);
}

void EPaperWeatherDrawer2::drawStringMaxWidth(int x, int y, unsigned int text_width, String text, alignment align) {
  int16_t  x1, y1; //the bounds of x,y and w and h of the variable 'text' in pixels.
  uint16_t w, h;
  display.getTextBounds(text, x, y, &x1, &y1, &w, &h);
  if (align == RIGHT)  x = x - w;
  if (align == CENTER) x = x - w / 2;
  u8g2Fonts.setCursor(x, y);
  if (text.length() > text_width * 2) {
    u8g2Fonts.setFont(u8g2_font_helvB10_tf);
    text_width = 42;
    y = y - 3;
  }
  u8g2Fonts.println(text.substring(0, text_width));
  if (text.length() > text_width) {
    u8g2Fonts.setCursor(x, y + h + 15);
    String secondLine = text.substring(text_width);
    secondLine.trim(); // Remove any leading spaces
    u8g2Fonts.println(secondLine);
  }
}

void EPaperWeatherDrawer2::arrow(int x, int y, int asize, float aangle, int pwidth, int plength) {
  float dx = (asize + 28) * cos((aangle - 90) * PI / 180) + x; // calculate X position
  float dy = (asize + 28) * sin((aangle - 90) * PI / 180) + y; // calculate Y position
  float x1 = 0;           float y1 = plength;
  float x2 = pwidth / 2;  float y2 = pwidth / 2;
  float x3 = -pwidth / 2; float y3 = pwidth / 2;
  float angle = aangle * PI / 180;
  float xx1 = x1 * cos(angle) - y1 * sin(angle) + dx;
  float yy1 = y1 * cos(angle) + x1 * sin(angle) + dy;
  float xx2 = x2 * cos(angle) - y2 * sin(angle) + dx;
  float yy2 = y2 * cos(angle) + x2 * sin(angle) + dy;
  float xx3 = x3 * cos(angle) - y3 * sin(angle) + dx;
  float yy3 = y3 * cos(angle) + x3 * sin(angle) + dy;
  display.fillTriangle(xx1, yy1, xx3, yy3, xx2, yy2, GxEPD_BLACK);
}
