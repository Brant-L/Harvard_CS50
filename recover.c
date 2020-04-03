#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

// Create new type to store a byte of data
typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // Ensure only 1 command line argument
    if (argc != 2)
    {
        printf("File name may not contain spaces\n");
        return 1;
    }

    // Open file and generate error if file can't be opened for read access
    FILE *file = fopen(argv[1], "r");
    if (!file)
    {
        printf("Unable to open file for read access\n");
        return 1;
    }
    
    // Allocate memory and declare buffer for fread function
    BYTE *block = malloc(512);
    
    // Initialize variable for output image files
    FILE *jpg = NULL;

    // Declare variable to keep track of JPEG count
    int jpgCount = 0;

    // Declare variable for storing output file names
    char fileName[8];
    
    // Loop to read through each 512 BYTE block until end of card is reached
    while (fread(block, 512, 1, file))
    {
        // If new JPEG header is found
        if (block[0] == 0xff && block[1] == 0xd8 && block[2] == 0xff && (block[3] & 0xf0) == 0xe0)
        {
            // Close any previously opened output files
            if (jpgCount > 0)
            {
                fclose(jpg);
            }
            
            // Open new output file
            sprintf(fileName, "%03i.jpg", jpgCount);
            jpg = fopen(fileName, "w");
            jpgCount++;
            
            if (!jpg)
            {
                return 1;
            }
        }
        
        // If first JPEG hasn't been found skip block of 512 bytes
        if (!jpgCount)
        {
            continue;
        }
        
        // If output file is open write 512 bytes to it
        fwrite(block, 512, 1, jpg);
    }
    
    // Close the last open output file and input file
    fclose(jpg);
    fclose(file);
    
    // Free allocated memory
    free(block);
    
    return 0;
}
