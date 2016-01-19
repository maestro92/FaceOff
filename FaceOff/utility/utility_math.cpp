#include "utility.h"




string utl::floatToStr(float value)
{
    ostringstream buff;
    buff << std::setprecision(4) << value;
    return buff.str();
}

float utl::randFloat(float min, float max)
{
    float num = (float)rand() / (float) RAND_MAX;
    return min + (max - min) * num;
}
