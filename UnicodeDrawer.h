
#include <Arduino.h>
#include "GxEPD2_3C.h"

#include <map>

class UnicodeDrawer {

public:
  UnicodeDrawer(GxEPD2_3C<GxEPD2_420c, GxEPD2_420c::HEIGHT>* gxDisplay);
  ~UnicodeDrawer() {};
  String preUnicode(const String& text);
  void printUnicode(const String& text, uint16_t textColor, uint16_t backColor, bool isBold);

private:
  void findUmls(const String& text, const String& toFind, std::map<int, String>& umls);

  GxEPD2_3C<GxEPD2_420c, GxEPD2_420c::HEIGHT>* display;
};