#include "canvas.h"

Canvas::Canvas(int width, int height, int bpp) : TGAImage(width, height, bpp)
{
}

Canvas::~Canvas()
{
}

void Canvas::drawLine(int x0, int y0, int x1, int y1, TGAColor &color)
{
    // Temporary transformations for steep/backwards lines
    bool steep = false;
    if (std::abs(x0 - x1) < std::abs(y0 - y1)) // Transpose line if height greater than width
    {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }

    if (x0 > x1) // Swap co-ordinates if line would be right-to-left
    {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    // Drawing the line
    const int dx = x1 - x0;
    const int dy = y1 - y0;
    const int derror = std::abs(dy) * 2;
    int error = 0;
    int y = y0;

    for (int x = x0; x <= x1; x++)
    {
        if (steep)
        {
            this->set(y, x, color); // De-transpose if transposed
        }
        else
        {
            this->set(x, y, color);
        }

        /**
         * derror (2 * dy) is added to error each loop. If error then becomes larger than dx,
         * increase/decrease y value by 1 (depending on which direction the line is in),
         * and account for that increase/decrease in the error variable.
        */
        error += derror;
        if (error > dx)
        {
            y += (y1 > y0 ? 1 : -1);
            error -= dx * 2;
        }
    }
}

void Canvas::drawLine(Vec2i v0, Vec2i v1, TGAColor &color)
{
    this->drawLine(v0.x, v0.y, v1.x, v1.y, color);
}

void Canvas::drawTriangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAColor &color)
{
    this->drawLine(t0, t1, color);
    this->drawLine(t1, t2, color);
    this->drawLine(t2, t0, color);
}

void Canvas::drawModel(Model *model, TGAColor &color)
{
    for (int i = 0; i < model->nfaces(); i++)
    {
        std::vector<int> face = model->face(i);

        for (int j = 0; j < 3; j++)
        {
            // These two vertices make up a line
            Vec3f v0 = model->vert(face[j]);
            Vec3f v1 = model->vert(face[(j + 1) % 3]);

            // This converts the line into the canvas space
            int x0 = (v0.x + 1.0) * width / 2.0;
            int y0 = (v0.y + 1.0) * height / 2.0;
            int x1 = (v1.x + 1.0) * width / 2.0;
            int y1 = (v1.y + 1.0) * height / 2.0;

            // Drawing the line
            this->drawLine(x0, y0, x1, y1, color);
        }
    }
}
