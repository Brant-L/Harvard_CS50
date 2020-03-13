#include <cs50.h>
#include <stdio.h>
#include <math.h>

int main(void)
{
    long n;
    do
    {
        n = get_long("Number: ");
    }
    while (n < 1 || n > 9999999999999999);

    long temp_n = n;
    int c = 2;

    while (temp_n > 100)
    {
        temp_n = temp_n / 10;
        c++;
    }

    long temp_n2 = n;
    int sum = 0;

    for (int i = 1; i <= c; i++)
    {
        int digit = temp_n2 % 10;
        if (i % 2 == 0)
        {
            if (digit * 2 > 9)
            {
                sum += (digit * 2) - 9;
            }
            else
            {
                sum += digit * 2;
            }
        }
        else
        {
            sum += digit;
        }  
        temp_n2 /= 10;
    }

    bool flag = (sum % 10 == 0) ? true : false;
  
    // Combine all parameters to check card numbers
    if (temp_n > 50 && temp_n < 56 && c == 16 && flag)
    {
        printf("MASTERCARD\n");
    }
    else if ((temp_n == 34 || temp_n == 37) && (c == 15) && flag)
    {
        printf("AMEX\n");
    }
    else if ((c == 13 || c == 16) && (temp_n / 10 == 4) && flag)
    {
        printf("VISA\n");
    }
    else
    {
        printf("INVALID\n");
    }
}
