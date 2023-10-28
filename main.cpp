#include "includes/tgaimage.h"
#include "includes/model.h"
#include "line.h"

TGAColor white = TGAColor(255, 255, 255, 255);
TGAColor red = TGAColor(255, 0, 0, 255);
Model *model = NULL;
const int width = 800;
const int height = 800;

int main(int argc, char **argv)
{
    void drawModel(TGAImage* image, Model* model);
    
    model = new Model("obj/head.obj");
    
    TGAImage image(width, height, TGAImage::RGB);

    drawModel(&image, model);

    image.flip_vertically(); // Origin should be left bottom corner
    image.write_tga_file("output.tga");
    delete model;
    return 0;
}

void drawModel(TGAImage* image, Model* model)
{
    for (int i = 0; i < model->nfaces(); i++)
        {
            std::vector<int> face = model->face(i);
            
            for (int j = 0; j < 3; j++)
            {
                // These two vertices make up a line
                Vec3f v0 = model->vert(face[j]);
                Vec3f v1 = model->vert(face[(j+1)%3]);

                // This converts the line into the image space
                int x0 = (v0.x + 1.0) * width/2.0;
                int y0 = (v0.y + 1.0) * height/2.0; 
                int x1 = (v1.x + 1.0) * width/2.0; 
                int y1 = (v1.y + 1.0) * height/2.0; 

                // Drawing the line
                Line line = Line(x0, y0, x1, y1, image, white);
            }
        }
}