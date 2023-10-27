#include "tgaimage.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);

/**
 * First attempt
 * Changing T value causes line to become dotted
 */
void line1(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color)
{
    for (float t = 0.0; t < 1.0; t += 0.1) // Setting t from 0.01 to 0.1 causes line to become dotted
    {
        int x = x0 + (x1 - x0) * t;
        int y = y0 + (y1 - y0) * t;
        image.set(x, y, color);
    }
}

/**
 * Second attempt
 * If line height is greater than width, the line is dotted
 * Right-to-left lines are not rendered
 */
void line2(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color)
{
    // Loop through all values of x on the line
    for (int x = x0; x <= x1; x++)
    {
        float t = (x - x0) / (float)(x1 - x0); // t is a normalised float representing how far along the line on the x-axis x is

        /**
         * y is calculated as the sum of y0 * the normalised remaining pixels on x-axis, and y1 * the normalised pixels on x-axis already done
         * I am still trying to completely understand this part.
         */
        int y = y0 * (1.0 - t) + y1 * t;
        image.set(x, y, color);
    }
}

/**
 * Third attempt
 * Inefficient
 */
void line3(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color)
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

    for (int x = x0; x <= x1; x++)
    {
        float t = (x - x0) / (float)(x1 - x0);
        int y = y0 * (1.0 - t) + y1 * t;
        if (steep)
        {
            image.set(y, x, color); // De-transpose if transposed
        }
        else
        {
            image.set(x, y, color);
        }
    }
}

/**
 * Fourth attempt
 */
void line4(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color)
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
    float derror = std::abs(dy / float(dx));
    float error = 0;
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
         * If the next point in the line is more than half a pixel above the current x value,
         * increase/decrease the y value by 1 (depending on which direction the line is in)
         * and account for that increase/decrease in the error variable.
        */
        error += derror;
        if (error > 0.5)
        {
            y += (y1 > y0 ? 1 : -1);
            error -= 1.0;
        }
    }
}

/**
 * Fifth and final attempt
 */
void line5(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor &color)
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
    TGAImage image(100, 100, TGAImage::RGB);
    line4(13, 20, 80, 40, image, white);
    line4(20, 13, 40, 80, image, red);
    line4(80, 40, 13, 20, image, red);
    image.flip_vertically(); // Origin should be left bottom corner
    image.write_tga_file("output.tga");
    return 0;
}