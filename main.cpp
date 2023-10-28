#include "includes/tgaimage.h"
#include "includes/model.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
Model *model = NULL;
const int width = 800;
const int height = 800;

/**
 * Line-drawing function
 */
void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color)
{
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

    int dx = x1 - x0;
    int dy = y1 - y0;
    int derror = std::abs(dy) * 2;
    int error = 0;
    int y = y0;

    for (int x = x0; x <= x1; x++)
    {
        if (steep)
        {
            image.set(y, x, color); // De-transpose if transposed
        }
        else
        {
            image.set(x, y, color);
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

int main(int argc, char **argv)
{
    if (2 == argc)
    {
        model = new Model(argv[1]);
    }
    else
    {
        model = new Model("obj/head.obj");
    }
    
    TGAImage image(width, height, TGAImage::RGB);

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
            line(x0, y0, x1, y1, image, white);
        }
    }

    image.flip_vertically(); // Origin should be left bottom corner
    image.write_tga_file("output.tga");
    delete model;
    return 0;
}