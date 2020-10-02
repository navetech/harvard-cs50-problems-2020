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

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    // Repeat for each pixel in the image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Set new red, green and blue values to the pixel according to sepia transformation
            RGBTRIPLE *pixel = &image[i][j];
            BYTE sepiaRed;
            BYTE sepiaGreen;
            BYTE sepiaBlue;
            sepiaRed = round(fminf(255, .393 * pixel->rgbtRed + .769 * pixel->rgbtGreen + .189 * pixel->rgbtBlue));
            sepiaGreen = round(fminf(255, .349 * pixel->rgbtRed + .686 * pixel->rgbtGreen + .168 * pixel->rgbtBlue));
            sepiaBlue = round(fminf(255, .272 * pixel->rgbtRed + .534 * pixel->rgbtGreen + .131 * pixel->rgbtBlue));
            pixel->rgbtRed = sepiaRed;
            pixel->rgbtGreen = sepiaGreen;
            pixel->rgbtBlue = sepiaBlue;
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
    // Allocate memory for blurred image
    RGBTRIPLE(*blurred)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    if (blurred == NULL)
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
            RGBTRIPLE *pixel = &blurred[i][j];
            pixel->rgbtRed = round(sumRed / (float)npixels);
            pixel->rgbtGreen = round(sumGreen / (float)npixels);
            pixel->rgbtBlue = round(sumBlue / (float)npixels);
        }
    }

    // Copy blurred image to original image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = blurred[i][j];
        }
    }
    free(blurred);

    return;
}
