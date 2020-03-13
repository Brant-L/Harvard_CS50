#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>



int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./ceasar key\n");
        return 1;
    }
    
    string s = argv[1];

    for (int i = 0, n = strlen(s); i < n; i++)
    {
        if (!isdigit(s[i]))
        {
            printf("Usage: ./ceasar key\n");
            return 1;
        }
        else
        {
            int val_int = atoi(s);
        }
    }
    
    string plaintext = get_string("plaintext: ");
    
    string pt = argv[1];
    int k = atoi(pt);
    
    printf("ciphertext: ");
    
    for (int j = 0, p = strlen(plaintext); j < p; j++)
    {
        int uc_ai = ((plaintext[j]) - 65);
        int uc_cipher = (uc_ai + k) % 26;
        int uc_encrypted = uc_cipher + 65;

        int lc_ai = ((plaintext[j]) - 97);
        int lc_cipher = (lc_ai + k) % 26;
        int lc_encrypted = lc_cipher + 97;
        
        if (isupper(plaintext[j]) != 0 && isalpha(plaintext[j]) != 0)
        {
            printf("%c", uc_encrypted);
        }
        else if (isalpha(plaintext[j]) != 0)
        {
            printf("%c", lc_encrypted);
        }
        else
        {
            printf("%c", plaintext[j]);
        }
    }
    printf("\n");
    return 0;
}
