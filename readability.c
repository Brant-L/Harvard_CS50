#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>


int main(void)
{
    // Prompt user for string of text
    string s = get_string("Text: ");
    int letter_count = 0;
    int word_count = 1;
    int sentence_count = 0;

    // Identify and count parts of the string via else if statements
    for (int i = 0, n = strlen(s); i < n; i++)
    {
        if ((s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z'))
        {
            letter_count++;
        }
        else if (s[i] == ' ')
        {
            word_count++;
        }
        else if (s[i] == '.' || s[i] == '?' || s[i] == '!')
        {
            sentence_count++;
        }
    }


    // Apply Coleman-Liau index
    float grade = 0.0588 * (100 * (float) letter_count / (float) word_count) - 0.296 * (100 * (float) sentence_count / 
                  (float) word_count) - 15.8;
    if (grade < 16 && grade > 0)
    {
        printf("Grade %i\n", (int) round(grade));
    }
    else if (grade >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Before Grade 1\n");
    }
}