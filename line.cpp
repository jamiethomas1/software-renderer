#include "line.h"
#include <iostream>

Line::Line(int x0, int y0, int x1, int y1, TGAImage* image, TGAColor &color, bool draw)
{
    this->x0 = x0;
    this->y0 = y0;
    this->x1 = x1;
    this->y1 = y1;
    this->image = image;
    this->color = color;

    if (std::abs(this->x0 - this->x1) < std::abs(this->y0 - this->y1)) // Transpose line if height greater than width
    {
        std::swap(this->x0, this->y0);
        std::swap(this->x1, this->y1);
        this->steep = true;
    }

    if (this->x0 > this->x1) // Swap co-ordinates if line would be right-to-left
    {
        std::swap(this->x0, this->x1);
        std::swap(this->y0, this->y1);
    }

    if (draw) {
        this->draw(this->x0, this->y0, this->x1, this->y1, this->image, this->color);
    }
}

Line::~Line()
{
}

void Line::draw(int x0, int y0, int x1, int y1, TGAImage* image, TGAColor &color)
{
    const int dx = x1 - x0;
    const int dy = y1 - y0;
    const int derror = std::abs(dy) * 2;
    int error = 0;
    int y = y0;

    for (int x = x0; x <= x1; x++)
    {
        if (this->steep)
        {
            image->set(y, x, color); // De-transpose if transposed
        }
        else
        {
            image->set(x, y, color);
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