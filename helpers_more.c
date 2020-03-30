#include "helpers.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// Use #define to create macro that replaces any value over 255 with 255
#define cap(channel) ((channel) > 255.0 ? 255.0 : (channel))

// Function prototype
RGBTRIPLE sobelValue(int i, int j, int height, int width, RGBTRIPLE[height][width]);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Set int avg to BYTE color values cast to floats and find average value for all colors
            int avg = round(((float)image[i][j].rgbtRed + (float)image[i][j].rgbtGreen + (float)image[i][j].rgbtBlue) / 3);

            // Set the value for each color in pixel to the average of all color values
            image[i][j].rgbtRed = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtBlue = avg;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Create a temporary array to swap values
    RGBTRIPLE temp;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width * 0.5; j++)
        {
            // Swap value horizonatally for each row of pixels
            temp = image[i][j];
            image[i][j] = image[i][width - j - 1];
            image[i][width - j - 1] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Declare an array to store a copy of the original pixel values
    RGBTRIPLE copy[height][width];

    // Declare three temp variables to store averages for each color value
    int avg_r;
    int avg_g;
    int avg_b;

    // Declare variable to track total number of pixels iterated through in nested loops
    float count;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            avg_r = 0;
            avg_g = 0;
            avg_b = 0;
            count = 0.00;

            // Nested loops to iterate through neighboring pixels
            for (int a = -1; a < 2; a++)
            {
                for (int b = -1; b < 2; b++)
                {
                    // Skip any neighboring pixels that don't exist
                    if ((i + a) < 0 || (i + a) > height - 1 || (j + b) < 0 || (j + b) > width - 1)
                    {
                        continue;
                    }

                    // Set temp variables to the sum of the color values
                    else
                    {
                        avg_r += image[i + a][j + b].rgbtRed;
                        avg_g += image[i + a][j + b].rgbtGreen;
                        avg_b += image[i + a][j + b].rgbtBlue;
                        count++;
                    }
                }
            }

            // Set copy to average color values
            copy[i][j].rgbtRed = round(avg_r / count);
            copy[i][j].rgbtGreen = round(avg_g / count);
            copy[i][j].rgbtBlue = round(avg_b / count);
        }
    }

    // Replace pixel colors values with those stored in global copy array
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            image[j][i].rgbtRed = copy[j][i].rgbtRed;
            image[j][i].rgbtGreen = copy[j][i].rgbtGreen;
            image[j][i].rgbtBlue = copy[j][i].rgbtBlue;
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Declare temp array to store channel values
    RGBTRIPLE *temp = (RGBTRIPLE *) malloc(height * width * sizeof(RGBTRIPLE));
    
    // Declare temp array to store Sobel channel values
    RGBTRIPLE sobelPixel;
    
    // Loop through each pixel in 3x3 grid
    // Assign return of sobelValues function that has been passed original pixel values to sobelPixel array
    // Assign sobelPixel values for each pixel index to temp array
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            sobelPixel = sobelValue(j, i, height, width, image);
            temp[width * i + j] = sobelPixel;
        }
    }
    
    // Initialize variable to track index value
    int count = 0;
    
    // Loop through each pixel in 3x3 grid
    // Assign sobelPixel values to original pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = temp[count++];
        }
    }
    free(temp);
    return;
}

// Function to calculate Sobel values for each pixel
RGBTRIPLE sobelValue(int j, int i, int height, int width, RGBTRIPLE image[height][width])
{
    // Initialize arrays to store Gx and Gy kernel values
    int gx[] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};
    int gy[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
   
    // Declare temp array to store index values for 3x3 grid of pixels
    RGBTRIPLE *pixelIndex = (RGBTRIPLE *) malloc(9 * sizeof(RGBTRIPLE));
    
    // Declare temp array to store Gx and Gy values
    RGBTRIPLE kernel;
    
    // Declare temp array to store Sobel values
    RGBTRIPLE sobel;
    
    // Declare temp variables to be used in nested loops iterating through each pixel in 3X3 grid
    int si;
    int sj;
    
    // Declare variable to be used to track index position in 3x3 grid
    int count = 0;
    
    // Loop through each pixel in 3x3 grid
    // Assign channel values to temp array for each pixel
    // Keep track of index value with count
    for (int b = -1; b <= 1; b++)
    {
        for (int a = -1; a <= 1; a++)
        {
            // Initialize temp variables to index positions
            sj = j + b;
            si = i + a;
            
            // Set channel values to black for pixels beyond border of image
            if (sj < 0 || si < 0 || sj == width || si == height)
            {
                kernel = (RGBTRIPLE)
                {
                    0, 0, 0
                };
            }
            
            // Assign channel values for each pixel to temp array
            else
            {
                kernel = image[i + a][j + b];
            }
            pixelIndex[count++] = kernel;
        }
    }
    
    // Initialize variables to store Gx and Gy values
    int gxr = 0;
    int gyr = 0;
    int gxg = 0;
    int gyg = 0;
    int gxb = 0;
    int gyb = 0;
    
    // Declare temp array to store index values for 3x3 grid
    RGBTRIPLE index;
    
    // Loop through each pixel in 3x3 grid and calculate Gx and Gy values
    for (int x = 0; x < 9; x++)
    {
        index = pixelIndex[x];
        gxr += gx[x] * index.rgbtRed;
        gxg += gx[x] * index.rgbtGreen;
        gxb += gx[x] * index.rgbtBlue;
        gyr += gy[x] * index.rgbtRed;
        gyg += gy[x] * index.rgbtGreen;
        gyb += gy[x] * index.rgbtBlue;
    }
    free(pixelIndex);
    
    // Calculate Sobel values cast as BYTE capped to 255
    sobel.rgbtRed = (BYTE) round(cap(sqrt((gxr * gxr) + (gyr * gyr))));
    sobel.rgbtGreen = (BYTE) round(cap(sqrt((gxg * gxg) + (gyg * gyg))));
    sobel.rgbtBlue = (BYTE) round(cap(sqrt((gxb * gxb) + (gyb * gyb))));

    return sobel;
}

