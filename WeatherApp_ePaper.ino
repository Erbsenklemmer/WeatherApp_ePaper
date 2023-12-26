
#include "WifiParser.h"

#include <UnixTime.h>

#include <JsonStreamingParser.h>
#include "OneCallListener.h"

#include "EPaperWeatherDrawer.h"

void setup() 
{
  Serial.begin(115200);

  WifiParser::setup();

  EPaperWeatherDrawer::setup();
}

void loop() 
{
  Serial.println("Starting LOOP");
  delay(2*1000);
  Serial.println("------ LOOP ------");

  OneCallListener oneCallListener;
  oneCallListener.Reset();//needed if we want to loop

  JsonStreamingParser parser;
  parser.setListener(&oneCallListener);

  Serial.println("Start parsing");
  WifiParser::parse(&parser);

  EPaperWeatherDrawer::drawOneCallData(oneCallListener.m_data);

  UnixTime unixTime(oneCallListener.m_data.m_timezone_offset_in_half_hours / 2);
  //CurrentData *pCurrentData = &oneCallListener.m_data.m_currentData;
  
  //unixTime.getDateTime(pCurrentData->m_dateTime);
  unixTime.getDateTime(oneCallListener.m_data.m_currentData.m_dateTime);

  String dateTimeString = 
    String(unixTime.hour) + ":" + String(unixTime.minute) + ":" + String(unixTime.second) + " " + 
    String(unixTime.day) + "." + unixTime.month/*monthNames[unixTime.month-1]*/ + "." + String(unixTime.year);
  Serial.println(dateTimeString);

  Serial.println("RETURN");
  delay(60 * 1000);
}
