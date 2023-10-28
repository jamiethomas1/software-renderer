#ifndef __CANVAS_H__
#define __CANVAS_H__

#include "includes/tgaimage.h"
#include "includes/geometry.h"
#include "includes/model.h"

class Canvas : public TGAImage {
    private:

    public:
        Canvas(int width, int height, int bpp = Canvas::RGB);
        ~Canvas();

        void drawLine(int x0, int y0, int x1, int y1, TGAColor &color);
        void drawLine(Vec2i v0, Vec2i v1, TGAColor &color);
        void drawTriangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAColor &color);
        void drawModel(Model *model, TGAColor &color);
};

#endif