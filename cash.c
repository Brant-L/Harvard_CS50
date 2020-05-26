// Abstraction and scope

#include <cs50.h>
#include <stdio.h>
#include <math.h>

int main(void)
{
    float dollars;
    do
    {
        dollars = get_float("Change owed: ");
    }
    while (dollars < 0);
    printf("%.2f\n", dollars);

    int coins = round(dollars * 100);
    int count = 0;
    int amount_left = coins;

    while (amount_left >= 25)
    {
        count++;
        amount_left -= 25;
    }
    while (amount_left >= 10)
    {
        count++;
        amount_left -= 10;
    }
    while (amount_left >= 5)
    {
        count++;
        amount_left -= 5;
    }
    while (amount_left >= 1)
    {
        count++;
        amount_left -= 1;
    }
    printf("%i\n", count);
}

