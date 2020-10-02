#include <cs50.h>
#include <stdio.h>
#include <math.h>

int main(void)
{
    // Get card number from the user
    long cardNumber;
    do
    {
        cardNumber = get_long("Number: ");
    }
    while (cardNumber <= 0);

    // Multiply every other digit by 2, starting with second-to-last digit
    int sum = 0;
    long aux = cardNumber / 10;
    while (aux > 0)
    {
        int prod = 2 * (aux % 10);

        // Add those products' digits together
        while (prod > 0)
        {
            sum += prod % 10;
            prod /= 10;
        }

        aux /= 100;
    }

    // Add the sum to the sum of the digits that weren't multiplied by 2
    aux = cardNumber;
    while (aux > 0)
    {
        sum += aux % 10;
        aux /= 100;
    }

    // If the total last digit is not 0, the card number is invalid
    if (sum % 10 != 0)
    {
        printf("INVALID\n");
        return 0;
    }

    // Check if card is VISA
    if (cardNumber / (long)pow(10, 12) == 4)
    {
        printf("VISA\n");
        return 0;
    }
    if (cardNumber / (long)pow(10, 15) == 4)
    {
        printf("VISA\n");
        return 0;
    }

    // Check if card is AMEX
    aux = cardNumber / (long)pow(10, 13);
    if ((aux == 34) || (aux == 37))
    {
        printf("AMEX\n");
        return 0;
    }

    // Check if card is MASTERCARD
    aux = cardNumber / (long)pow(10, 14);
    if ((aux == 51) || (aux == 52) || (aux == 53) || (aux == 54) || (aux == 55))
    {
        printf("MASTERCARD\n");
        return 0;
    }

    // Card number is INVALID
    printf("INVALID\n");
    return 0;

}
