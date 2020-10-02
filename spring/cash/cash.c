#include <cs50.h>
#include <stdio.h>
#include <math.h>

int main(void)
{
    // Get change owed in dollars from the user
    float changeInDollars;
    do
    {
        changeInDollars = get_float("Change owed: ");
    }
    while (changeInDollars < 0);

    // Convert change owed to cents
    int changeInCents = round(changeInDollars * 100);

    int coinsAvailable[] = {25, 10, 5, 1};

    // Calculate number of coins
    int numberOfCoins = 0;
    int iCoin = 0;
    while (changeInCents > 0)
    {
        if (changeInCents >= coinsAvailable[iCoin])
        {
            numberOfCoins += changeInCents / coinsAvailable[iCoin];
            changeInCents %= coinsAvailable[iCoin];
        }

        iCoin++;
    }

    // Print number of coins
    printf("%i\n", numberOfCoins);

}
