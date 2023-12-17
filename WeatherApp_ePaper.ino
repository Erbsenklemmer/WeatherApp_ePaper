
#include "WifiParser.h"

void setup() 
{
  // put your setup code here, to run once:

  Serial.begin(115200);

  WifiParser::setup();

}

void loop() {
  // put your main code here, to run repeatedly:
 WifiParser::parse();
}
