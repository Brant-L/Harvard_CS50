#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>


int main(int argc, string argv[])
{
    // ensure only a single command line argument is accepted
    if (argc != 2)
    {
        printf("Error: key may not contain spaces\n");
        return 1;
    }
    
    string s = argv[1];
    
    for (int i = 0, n = strlen(s); i < n; i++)
    {
        int count = 1;
        
        // ensure key only contains distinct letters
        for (int j = i + 1; j < n; j++)
        {
            if (s[i] == s[j])
            {
                count++;
            }
        
            if (count > 1)
            {
                printf("Error: key must contain only distinct letters\n");
                return 1;
            }
        }
        
        // ensure key is at least 26 characters
        if (n < 26)
        {
            printf("Error: key must be at least 26 characters\n");
            return 1;
        }
        
        // ensure key only contains letters
        else if (!isalpha(s[i]))
        {
            printf("Error: key may only contain letters\n");
            return 1;
        }
    }
    // apply cipher to plaintext
    string plaintext = get_string("plaintext: ");
        
    printf("ciphertext: ");
        
    int pt = strlen(plaintext);
        
    for (int z = 0, n = strlen(plaintext); z < n; z++)
    {
        for (int c = 0; c < 26; c++)
        {
            if (plaintext[z] == 'a' + c)
            {
                printf("%c", tolower(argv[1][c]));
            }
            else if (plaintext[z] == 'A' + c)
            {
                printf("%c", toupper(argv[1][c]));
            }
        }
            
        if (!isalpha(plaintext[z]))
        {
            printf("%c", plaintext[z]);
        }
    }
    printf("\n");
    return 0;
}