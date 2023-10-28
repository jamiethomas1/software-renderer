#include "canvas.h"
#include "includes/model.h"

TGAColor white = TGAColor(255, 255, 255, 255);
TGAColor red = TGAColor(255, 0, 0, 255);
TGAColor green = TGAColor(0, 255, 0, 255);
Model *model = NULL;
const int width = 200;
const int height = 200;

int main(int argc, char **argv)
{
    void drawModel(Canvas * canvas, Model * model);
    void triangle(Vec2i t0, Vec2i t1, Vec2i t2, Canvas & canvas, TGAColor & color);

    model = new Model("obj/head.obj");

    Canvas canvas(width, height, Canvas::RGB);

    Vec2i t0[3] = {Vec2i(10, 70), Vec2i(50, 160), Vec2i(70, 80)};
    Vec2i t1[3] = {Vec2i(180, 50), Vec2i(150, 1), Vec2i(70, 180)};
    Vec2i t2[3] = {Vec2i(180, 150), Vec2i(120, 160), Vec2i(130, 180)};
    canvas.drawTriangle(t0[0], t0[1], t0[2], red);
    canvas.drawTriangle(t1[0], t1[1], t1[2], white);
    canvas.drawTriangle(t2[0], t2[1], t2[2], green);

    canvas.flip_vertically(); // Origin should be left bottom corner
    canvas.write_tga_file("output.tga");
    delete model;
    return 0;
}

void drawModel(Canvas *canvas, Model *model)
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
            canvas->drawLine(x0, y0, x1, y1, white);
        }
    }
}