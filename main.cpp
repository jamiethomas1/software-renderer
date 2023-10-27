#include "tgaimage.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);

/**
 * First attempt
*/
void line1(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color)
{
    for (float t = 0.0; t < 1.0; t += 0.1) // Setting t from 0.01 to 0.1 causes line to become dotted
    {
        int x = x0 + (x1-x0)*t;
        int y = y0 + (y1-y0)*t;
        image.set(x, y, color);
    }
}

/**
 * Second attempt
*/
void line2(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color)
{
    // Loop through all values of x on the line
    for (int x = x0; x <= x1; x++)
    {
        float t = (x-x0)/(float)(x1-x0); // t is a normalised float representing how far along the line on the x-axis x is
        
        /**
         * y is calculated as the sum of y0 * the normalised remaining pixels on x-axis, and y1 * the normalised pixels on x-axis already done
         * I am still trying to completely understand this part. Not moving on until I do.
        */
        int y = y0 * (1.0 - t) + y1 * t;
        image.set(x, y, color);
    }
}

int main(int argc, char **argv)
{
    TGAImage image(100, 100, TGAImage::RGB);
    line2(13, 20, 80, 40, image, white); 
    line2(20, 13, 40, 80, image, red); 
    line2(80, 40, 13, 20, image, red);
    image.flip_vertically(); // Origin should be left bottom corner
    image.write_tga_file("output.tga");
    return 0;
}