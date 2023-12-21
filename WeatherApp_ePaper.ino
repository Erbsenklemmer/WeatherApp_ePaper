
#include "WifiParser.h"

#include <UnixTime.h>

#include <JsonStreamingParser.h>
#include "OneCallListener.h"

void setup() 
{
  Serial.begin(115200);

  WifiParser::setup();

}

void loop() 
{
  OneCallListener oneCallListener;

  oneCallListener.Reset();//needed if we want to loop
  JsonStreamingParser parser;
  parser.setListener(&oneCallListener);

  WifiParser::parse(&parser);

  UnixTime unixTime(oneCallListener.m_data.m_timezone_offset_in_half_hours / 2);
  //CurrentData *pCurrentData = &oneCallListener.m_data.m_currentData;
  
  //unixTime.getDateTime(pCurrentData->m_dateTime);
  unixTime.getDateTime(oneCallListener.m_data.m_currentData.m_dateTime);

  String dateTime = String(unixTime.hour) + ":" + String(unixTime.minute) + ":"  + String(unixTime.month) + "." + unixTime.month/*monthNames[unixTime.month-1]*/ + " " + String(unixTime.year);
  Serial.println(dateTime);

}
