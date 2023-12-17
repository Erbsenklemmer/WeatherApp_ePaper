#include "core_esp8266_features.h"
#include "GlobalSettings.h"

#include "WifiParser.h"

uint lastTime = millis();
unsigned long timerDelay = 10000;
unsigned int availableRemainingCounter = 0;

bool WifiParser::setup() 
{
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  return true;
}

bool WifiParser::parse()
{
  // String oneCallWheaterRequest = String("http://api.openweathermap.org/data/3.0/onecall")
  //         + "?lat=" + lat + "&lon=" + lon + "&appid=" + appid + "&units=" + units + "&lang=" + lang;

  String oneCallWheaterRequest = String("http://api.openweathermap.org/data/2.5/weather")
          + "?lat=" + lat + "&lon=" + lon + "&appid=" + appid + "&units=" + units + "&lang=" + lang;

  if ((millis() - lastTime) > timerDelay)
  {
    Serial.println(oneCallWheaterRequest);

    // Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED)
    {    
      httpGETRequest(oneCallWheaterRequest.c_str());
      //Serial.println(jsonBuffer);
    }
    else 
    {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }

  return true;
}

bool WifiParser::httpGETRequest(const char* serverName) 
{
  bool ok(true);

  WiFiClient client;
  HTTPClient http;
    
  // Your Domain name with URL path or IP address with path
  http.begin(client, serverName);
  
  // Send HTTP POST request
  int httpResponseCode = http.GET();
  
  String payload; 
  WiFiClient* pClientStream;

  if (httpResponseCode > 0) 
  {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);

    pClientStream = http.getStreamPtr();

    Serial.printf("length: %d\n", http.getSize());

    while (pClientStream && pClientStream->available())
    {
      char c = pClientStream->read();
      Serial.print(c);

      yield();//give the stream a chance to read the next character
    }
    unsigned int availableRemaining = pClientStream->available();
    if (availableRemaining > 0)
    {
      availableRemainingCounter++;
    }
    Serial.printf("\nStream is available: %d, is null %d times\n", availableRemaining, availableRemainingCounter);

    Serial.flush();
    Serial.println("done read");
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);

    ok = false;
  }
  // Free resources
  http.end();

  Serial.println("waiting for next call\n-------------");

  return ok;
}