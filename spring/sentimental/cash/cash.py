#!/usr/bin/env python3

# Calculates the minimum number of coins required to give a user change

import cs50

# Get change owed in dollars from the user
while True:
    changeInDollars = cs50.get_float("Change owed: ")
    if changeInDollars >= 0:
        break

# Convert change owed to cents
changeInCents = round(changeInDollars * 100)

# List of available coin
#coinsAvailable = [1, 5, 10, 25]
coinsAvailable = [25, 10, 5, 1]

# Initialize number of coins
numberOfCoins = 0

# Calculate number of coins
iCoin = 0
while changeInCents > 0:
    if changeInCents >= coinsAvailable[iCoin]:
        numberOfCoins += changeInCents // coinsAvailable[iCoin]
        changeInCents %= coinsAvailable[iCoin]

    iCoin += 1

# Print number of coins required
print(numberOfCoins)
