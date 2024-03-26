
#include "UnicodeDrawer.h"

UnicodeDrawer::UnicodeDrawer(GxEPD2_3C<GxEPD2_420c, GxEPD2_420c::HEIGHT>* gxDisplay) {
  display = gxDisplay;
}

String UnicodeDrawer::preUnicode(const String& text) {
  String ret = text;
  ret.replace("Ä", "A");
  ret.replace("Ö", "O");
  ret.replace("Ü", "U");

  ret.replace("ä", "a");
  ret.replace("ö", "o");
  ret.replace("ü", "u");

  ret.replace("°", " ");
  return ret;
}

void UnicodeDrawer::printUnicode(const String& text, uint16_t textColor, uint16_t backColor, bool isBold) {
  Serial.println(text + ": Hello from printUnicode ************************");
  Serial.println("Unicode length: " + String(text.length()));

  std::map<int, String> umls;

  findUmls(text, "Ä", umls);
  findUmls(text, "Ö", umls);
  findUmls(text, "Ü", umls);

  findUmls(text, "ä", umls);
  findUmls(text, "ö", umls);
  findUmls(text, "ü", umls);

  findUmls(text, "°", umls);

  // for (std::map<int, String>::iterator iter = umls.begin(); iter != umls.end(); iter++) {
  //   Serial.println(iter->second + " at map: " + String(iter->first));
  // }

  int16_t cursorX = display->getCursorX();
  int16_t cursorY = display->getCursorY();

  int16_t tbx, tby; 
  uint16_t tbw, tbh, heightUpper, heightLower;

  display->getTextBounds("U", 0, 0, &tbx, &tby, &tbw, &heightUpper);
  display->getTextBounds("a", 0, 0, &tbx, &tby, &tbw, &heightLower);

  const String preUni = preUnicode(text);
  display->print(preUni);

  int umlOffset = 0;
  for (std::map<int, String>::iterator iter = umls.begin(); iter != umls.end(); iter++) {
    String start = preUni.substring(0, umlOffset + iter->first);
    String end = preUni.substring(0, umlOffset + iter->first + 1);
    umlOffset--;

    Serial.println(iter->second + ": Start: " + start + ", end: " + end);

    uint16_t widthStart, widthEnd;
    if (start.length() == 0) {
      widthStart = 0;
    }
    else {
      display->getTextBounds(start, 0, 0, &tbx, &tby, &widthStart, &tbh);
    }
    display->getTextBounds(end, 0, 0, &tbx, &tby, &widthEnd, &tbh);

    Serial.println("widthStart: " + String(widthStart) + " widthEnd: " + String(widthEnd));

    if (iter->second != "°")
    {
      //reduce some pixels at start
      uint16_t marginLeft;
      if (start.length() == 0) {
        marginLeft = (widthEnd - widthStart) / 4;
      }
      else {
        marginLeft = (widthEnd - widthStart) / 3;
      }
      widthStart += marginLeft;

      uint16_t marginRight = (widthEnd - widthStart) / 4 + 1;
      widthEnd -= marginRight;
    }

    uint16_t x1 = cursorX + widthStart;
    uint16_t x2 = cursorX + widthEnd;
    uint16_t y = -1;
    bool drawDegree = false;
    const String uml = iter->second;
    if (uml == "ä" || uml == "ö" || uml == "ü") {
      y = cursorY - heightLower-1;
      if (uml == "ä")
        x1+=2;
    }
    else if (uml == "Ä" || uml == "Ö" || uml == "Ü") {
      y = cursorY - heightUpper-1;
    }
    else if (uml == "°") {
      y = cursorY - heightLower;
      drawDegree = true;
    }
    uint16_t color = textColor;//COLOR_RED;
    if (drawDegree == true) {
      int dist = (widthEnd -  widthStart);
      // display->fillCircle(x1 + 2*dist, y, dist-2, color);
      // display->fillCircle(x1 + 2*dist, y, dist-3, backColor);
      display->drawCircle(x1 + 2*dist, y, dist/2+.5, color);
      Serial.println("draw degree x: " + String(x1 + dist) + ", y: " + String(y) + ", radius: " + dist);
      Serial.println("degree widthStart: " + String(widthStart) + " widthEnd: " + String(widthEnd));
    }
    else {
      display->drawPixel(x1  , y, color);
      display->drawPixel(x1+1, y, color);
      display->drawPixel(x1  , y-1, color);
      display->drawPixel(x1+1, y-1, color);
      
      if (isBold){
        display->drawPixel(x1+2, y, color);
        display->drawPixel(x1+2, y-1, color);
      }

      display->drawPixel(x2  , y, color);
      display->drawPixel(x2-1, y, color);
      display->drawPixel(x2  , y-1, color);
      display->drawPixel(x2-1, y-1, color);

      if (isBold) {
        display->drawPixel(x2+1, y, color);
        display->drawPixel(x2+1, y-1, color);
      }

      Serial.println(uml + " x: " + String(x1) + ", y: " + String(y));
      Serial.println(uml + " x: " + String(x2) + ", y: " + String(y));
    }
  }
}

void UnicodeDrawer::findUmls(const String& text, const String& toFind, std::map<int, String>& umls) {
  int pos = text.indexOf(toFind);
  while (pos > -1) {
    //Serial.println(toFind + " at: " + String(pos));
    umls[pos] = toFind;
    pos = text.indexOf(toFind, pos + 1);
  }
}
