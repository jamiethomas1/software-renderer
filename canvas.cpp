#include "canvas.h"
#include <vector>

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

Vec3f Canvas::barycentric(Vec2i *pts, Vec2i P)
{
    /**
     * ^ is being used as a cross-product operator.
     * This formula is "looking for a vector (u,v,1) that is orthogonal to (ABx,ACx,PAx) and (ABy,ACy,PAy) at the same time"
     */
    Vec3f u = Vec3f(pts[2].x - pts[0].x, pts[1].x - pts[0].x, pts[0].x - P.x) ^ Vec3f(pts[2].y - pts[0].y, pts[1].y - pts[0].y, pts[0].y - P.y);
    if (std::abs(u.z) < 1)
        return Vec3f(-1, 1, 1);
    return Vec3f(1.0 - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
}

void Canvas::drawTriangle(Vec2i *pts, TGAColor &color)
{
    Vec2i bboxmin(this->width - 1, this->height - 1);
    Vec2i bboxmax(0, 0);
    Vec2i clamp(this->width - 1, this->height - 1);

    /**
     * Loop through all vertices of triangle, generating two sets of co-ordinates which make up a
     * bounding box for the triangle.
     */
    for (int i = 0; i < 3; i++)
    {
        bboxmin.x = std::max(0, std::min(bboxmin.x, pts[i].x));
        bboxmin.y = std::max(0, std::min(bboxmin.y, pts[i].y));

        bboxmax.x = std::min(clamp.x, std::max(bboxmax.x, pts[i].x));
        bboxmax.y = std::min(clamp.y, std::max(bboxmax.y, pts[i].y));
    }

    Vec2i P; // This will be used to test each pixel inside the bounding box for whether it is inside or outside the triangle
    for (P.x = bboxmin.x; P.x <= bboxmax.x; P.x++)
    {
        for (P.y = bboxmin.y; P.y <= bboxmax.y; P.y++)
        {
            Vec3f bc_screen = barycentric(pts, P);
            if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0)
                continue;
            this->set(P.x, P.y, color);
        }
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