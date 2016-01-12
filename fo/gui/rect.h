#ifndef RECT_H_
#define RECT_H_

#include "define.h"

struct Rect
{
    int x;
    int y;
    int w;
    int h;

    Rect()
    {
        set(0, 0, 1, 1);
    }

    Rect(int x, int y, int w, int h)
    {
        set(x, y, w, h);
    }

    void set(int x, int y, int w, int h)
    {
        this->x = x;
        this->y = y;
        this->w = w;
        this->h = h;
    }
};

#endif // EG_RECT_H_
