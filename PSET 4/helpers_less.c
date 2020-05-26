#include "helpers.h"
#include <math.h>
#include <stdio.h>

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

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Declare variables to hold new sepia color values
            int r;
            int g;
            int b;
            
            r = round((float)(image[i][j].rgbtRed * .393 + image[i][j].rgbtGreen * .769 + image[i][j].rgbtBlue * .189));
            g = round((float)(image[i][j].rgbtRed * .349 + image[i][j].rgbtGreen * .686 + image[i][j].rgbtBlue * .168));
            b = round((float)(image[i][j].rgbtRed * .272 + image[i][j].rgbtGreen * .534 + image[i][j].rgbtBlue * .131));

            // If sepia value > 255 set the color value to 255 else set color value to calculated variable
            if (r > 255)
            {
                image[i][j].rgbtRed = 255;
            }
            
            else
            {
                image[i][j].rgbtRed = r;
            }
            
            if (g > 255)
            {
                image[i][j].rgbtGreen = 255;
            }
            else
            {
                image[i][j].rgbtGreen = g;
            }
            
            if (b > 255)
            {
                image[i][j].rgbtBlue = 255;
            }
            else
            {
                image[i][j].rgbtBlue = b;
            }
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
