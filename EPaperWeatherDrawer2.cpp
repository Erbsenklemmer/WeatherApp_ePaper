#include "EPaperWeatherDrawer2.h"

const char* City = "Endersbach";

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
  display.drawLine(0, 12, SCREEN_WIDTH, 12, GxEPD_BLACK);
}

void EPaperWeatherDrawer2::DrawBattery(int x, int y) {
  uint8_t percentage = 100;
  float voltage = analogRead(A0) / 4096.0 * 7.46;
  if (voltage > 1 ) { // Only display if there is a valid reading
    Serial.println("Voltage = " + String(voltage));
    percentage = 2836.9625 * pow(voltage, 4) - 43987.4889 * pow(voltage, 3) + 255233.8134 * pow(voltage, 2) - 656689.7123 * voltage + 632041.7303;
    if (voltage >= 4.20) percentage = 100;
    if (voltage <= 3.50) percentage = 0;
    display.drawRect(x + 15, y - 12, 19, 10, GxEPD_BLACK);
    display.fillRect(x + 34, y - 10, 2, 5, GxEPD_BLACK);
    display.fillRect(x + 17, y - 10, 15 * percentage / 100.0, 6, GxEPD_BLACK);
    drawString(x + 65, y - 11, String(percentage) + "%", RIGHT);
    //drawString(x + 13, y + 5,  String(voltage, 2) + "v", CENTER);
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
