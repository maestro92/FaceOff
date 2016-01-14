#include "utility.h"


float Utility::DEGREE_TO_RADIAN = 0.0174;    /// pi/180
float Utility::RADIAN_TO_DEGREE = 57.32;     /// 180/pi

string Utility::floatToStr(float value)
{
    ostringstream buff;
    buff << std::setprecision(4) << value;
    return buff.str();
}

float Utility::randFloat(float min, float max)
{
    float num = (float)rand() / (float) RAND_MAX;
    return min + (max - min) * num;
}
