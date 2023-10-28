#ifndef __LINE_H__
#define __LINE_H__

#include "includes/tgaimage.h"

class Line {
    private:
        int x0, y0, x1, y1;
        TGAImage* image;
        TGAColor color;
        bool steep = false;

        void draw(int x0, int y0, int x1, int y1, TGAImage* image, TGAColor &color);

    public:
        Line(int x0, int y0, int x1, int y1, TGAImage* image, TGAColor &color, bool draw = true);
        ~Line();
};

#endif