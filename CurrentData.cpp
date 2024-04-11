#include "CurrentData.h"

CurrentData::CurrentData()
{
    clear();
}

CurrentData::~CurrentData()
{
}

void CurrentData::clear()
{
    m_dateTime = -1;
    m_dayOfWeek = -1;
    
    m_temp = 0;
    m_tempFeelsLike = 0;

    m_windSpeed = -1;
    m_windDeg = -1;
    m_windGust = -1;
}