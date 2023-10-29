#include "canvas.h"

TGAColor tempred = TGAColor(255, 0, 0, 255);
TGAColor tempgreen = TGAColor(0, 255, 0, 255);

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

void Canvas::drawTriangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAColor &color, bool fill)
{
    /**
     * Sort vertices by increasing y-value. This is so that the line that goes top-to-bottom of the
     * whole triangle is always the line t2-t0. This makes line-by-line filling easier.
     */
    if (t0.y > t1.y)
        std::swap(t0, t1);
    if (t0.y > t2.y)
        std::swap(t0, t2);
    if (t1.y > t2.y)
        std::swap(t1, t2);

    this->drawLine(t0, t1, color);
    this->drawLine(t1, t2, color);
    this->drawLine(t2, t0, color);

    if (fill)
    {
        this->fillTriangle(t0, t1, t2, color);
    }
}

void Canvas::fillTriangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAColor &color)
{
    // Total triangle height, for calculating points along the total height of the triangle
    int totalHeight = t2.y - t0.y;

    // Bottom segment height for calculating points along the height of the bottom segment
    int bottomSegmentHeight = t1.y - t0.y + 1;
    for (int y = t0.y; y < t1.y; y++)
    {
        /**
         * Normalised representation of how far along the triangle/segment height current y-value is
         * Naming assumes left side is bottom-to-top of triangle, but can be different
         */
        float leftFraction = (float)(y - t0.y) / totalHeight;
        float rightFraction = (float)(y - t0.y) / bottomSegmentHeight;

        /**
         * Get difference between top and bottom vertices of triangle/segment, multiply by relevant
         * normalised fraction to find position along that line, then add to t0 to position
         * that point on the image relative to the triangle
         */
        Vec2i leftPoint = t0 + (t2 - t0) * leftFraction;
        Vec2i rightPoint = t0 + (t1 - t0) * rightFraction;

        this->drawLine(leftPoint.x, y, rightPoint.x, y, color);
    }

    // Top segment height for calculating points along the height of the top segment
    int topSegmentHeight = t2.y - t1.y;
    for (int y = t1.y; y < t2.y; y++)
    {
        /**
         * Normalised representation of how far along the triangle/segment height current y-value is
         * Naming assumes left side is bottom-to-top of triangle, but can be different
         */
        float leftFraction = (float)(y - t0.y) / totalHeight;
        float rightFraction = (float)(y - t1.y) / topSegmentHeight;

        /**
         * Get difference between top and bottom vertices of triangle/segment, multiply by relevant
         * normalised fraction to find position along that line, then add to t0 to position
         * that point on the image relative to the triangle
         */
        Vec2i leftPoint = t0 + (t2 - t0) * leftFraction;
        Vec2i rightPoint = t1 + (t2 - t1) * rightFraction;

        this->drawLine(leftPoint.x, y, rightPoint.x, y, color);
    }
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
