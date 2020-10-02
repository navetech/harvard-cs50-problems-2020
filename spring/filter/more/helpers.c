#include <stdlib.h>
#include <math.h>

#include "helpers.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Repeat for each pixel in the image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Set gray scale to the average of red, green and blue values of the pixel
            RGBTRIPLE *pixel = &image[i][j];
            BYTE  gray;
            gray = round((pixel->rgbtRed + pixel->rgbtGreen + pixel->rgbtBlue) / 3.);
            pixel->rgbtRed = gray;
            pixel->rgbtGreen = gray;
            pixel->rgbtBlue = gray;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Repeat for each row in the image
    for (int i = 0; i < height; i++)
    {
        // Swap pixels on the letf with pixels on the right of the row
        for (int j = 0; j < width / 2; j++)
        {
            RGBTRIPLE tmp;
            tmp = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = tmp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Allocate memory for filtered image
    RGBTRIPLE(*filtered)[width] = calloc(2, width * sizeof(RGBTRIPLE));
    if (filtered == NULL)
    {
        return;
    }

    // Repeat for each pixel in the image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Iterate from the row above to the row bellow of the pixel
            int npixels = 0;
            int sumRed = 0;
            int sumGreen = 0;
            int sumBlue = 0;
            int ra = fmax(i - 1, 0);
            int rb = fmin(i + 1, height - 1);
            for (int ii = ra; ii <= rb; ii++)
            {
                // Iterate from the left column to the row right column of the pixel
                int cl = fmaxf(j - 1, 0);
                int cr = fminf(j + 1, width - 1);
                for (int jj = cl; jj <= cr; jj++)
                {
                    /// Sum the RGB values of the pixels in the box
                    RGBTRIPLE *pixel = &image[ii][jj];
                    sumRed += pixel->rgbtRed;
                    sumGreen += pixel->rgbtGreen;
                    sumBlue += pixel->rgbtBlue;
                    npixels++;
                }
            }

            // Set RGB value of pixel image to average value of the box
            RGBTRIPLE *pixel = &filtered[i % 2][j];
            pixel->rgbtRed = round(sumRed / (float)npixels);
            pixel->rgbtGreen = round(sumGreen / (float)npixels);
            pixel->rgbtBlue = round(sumBlue / (float)npixels);
        }

        // Copy line from filtered image to original image
        if (i > 0)
        {
            for (int j = 0; j < width; j++)
            {
                image[i - 1][j] = filtered[(i - 1) % 2][j];
            }
        }
    }

    // Copy last line from filtered image to original image
    if (height > 0)
    {
        for (int j = 0; j < width; j++)
        {
            image[height - 1][j] = filtered[(height - 1) % 2][j];
        }
    }

    free(filtered);

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Define weights kernels
    int Gx_kernel[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy_kernel[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    // Allocate memory for filtered image
    RGBTRIPLE(*filtered)[width] = calloc(2, width * sizeof(RGBTRIPLE));
    if (filtered == NULL)
    {
        return;
    }

    // Repeat for each pixel in the image
    for (int i = 0; i < height; i++)
    {
        int ra = fmax(i - 1, 0);
        int rb = fmin(i + 1, height - 1);
        for (int j = 0; j < width; j++)
        {
            int cl = fmaxf(j - 1, 0);
            int cr = fminf(j + 1, width - 1);

            int GyRed = 0;
            int GyGreen = 0;
            int GyBlue = 0;

            int GxRed = 0;
            int GxGreen = 0;
            int GxBlue = 0;

            // Iterate from the row above to the row bellow of the pixel
            for (int ii = ra; ii <= rb; ii++)
            {
                int ki = ii - i + 1;

                // Iterate from the left column to the row right column of the pixel
                for (int jj = cl; jj <= cr; jj++)
                {
                    int kj = jj - j + 1;
                    RGBTRIPLE *pixel = &image[ii][jj];

                    /// Sum the RGB Gy values of the pixel
                    int y_weight = Gy_kernel[ki][kj];
                    GyRed += pixel->rgbtRed * y_weight;
                    GyGreen += pixel->rgbtGreen * y_weight;
                    GyBlue += pixel->rgbtBlue * y_weight;

                    /// Sum the RGB Gx values of the pixel
                    int x_weight = Gx_kernel[ki][kj];
                    GxRed += pixel->rgbtRed * x_weight;
                    GxGreen += pixel->rgbtGreen * x_weight;
                    GxBlue += pixel->rgbtBlue * x_weight;
                }
            }

            // Set RGB values of filtered pixel
            RGBTRIPLE *pixel = &filtered[i % 2][j];
            pixel->rgbtRed = round(fminf(255, sqrt(GxRed * GxRed + GyRed * GyRed)));
            pixel->rgbtGreen = round(fminf(255, sqrt(GxGreen * GxGreen + GyGreen * GyGreen)));
            pixel->rgbtBlue = round(fminf(255, sqrt(GxBlue * GxBlue + GyBlue * GyBlue)));
        }

        // Copy line from filtered image to original image
        if (i > 0)
        {
            for (int j = 0; j < width; j++)
            {
                image[i - 1][j] = filtered[(i - 1) % 2][j];
            }
        }
    }

    // Copy last line from filtered image to original image
    if (height > 0)
    {
        for (int j = 0; j < width; j++)
        {
            image[height - 1][j] = filtered[(height - 1) % 2][j];
        }
    }

    free(filtered);

    return;
}
