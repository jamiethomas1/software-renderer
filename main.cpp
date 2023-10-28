#include "includes/tgaimage.h"
#include "includes/model.h"
#include "line.h"

TGAColor white = TGAColor(255, 255, 255, 255);
TGAColor red = TGAColor(255, 0, 0, 255);
TGAColor green = TGAColor(0, 255, 0, 255);
Model *model = NULL;
const int width = 200;
const int height = 200;

int main(int argc, char **argv)
{
    void drawModel(TGAImage * image, Model * model);
    void triangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage & image, TGAColor & color);

    model = new Model("obj/head.obj");

    TGAImage image(width, height, TGAImage::RGB);

    Vec2i t0[3] = {Vec2i(10, 70), Vec2i(50, 160), Vec2i(70, 80)};
    Vec2i t1[3] = {Vec2i(180, 50), Vec2i(150, 1), Vec2i(70, 180)};
    Vec2i t2[3] = {Vec2i(180, 150), Vec2i(120, 160), Vec2i(130, 180)};
    triangle(t0[0], t0[1], t0[2], image, red);
    triangle(t1[0], t1[1], t1[2], image, white);
    triangle(t2[0], t2[1], t2[2], image, green);

    image.flip_vertically(); // Origin should be left bottom corner
    image.write_tga_file("output.tga");
    delete model;
    return 0;
}

void drawModel(TGAImage *image, Model *model)
{
    for (int i = 0; i < model->nfaces(); i++)
    {
        std::vector<int> face = model->face(i);

        for (int j = 0; j < 3; j++)
        {
            // These two vertices make up a line
            Vec3f v0 = model->vert(face[j]);
            Vec3f v1 = model->vert(face[(j + 1) % 3]);

            // This converts the line into the image space
            int x0 = (v0.x + 1.0) * width / 2.0;
            int y0 = (v0.y + 1.0) * height / 2.0;
            int x1 = (v1.x + 1.0) * width / 2.0;
            int y1 = (v1.y + 1.0) * height / 2.0;

            // Drawing the line
            Line line = Line(x0, y0, x1, y1, image, white);
        }
    }
}

void triangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage &image, TGAColor &color)
{
    Line line1 = Line(t0, t1, &image, color);
    Line line2 = Line(t1, t2, &image, color);
    Line line3 = Line(t2, t0, &image, color);
}