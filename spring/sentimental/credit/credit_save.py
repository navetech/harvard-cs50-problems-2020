# Determine whether a provided credit card number is valid according to Luhn’s algorithm

import sys
import cs50

# Get credit card number from the user
while True:
    cardNumber = cs50.get_int("Number: ")
    if cardNumber >= 0:
        break

# If credit card number has less than 13 or more than 16 or exactly 14 digits, is invalid
if (cardNumber < 10 ** 12) or cardNumber >= 10 ** 16 or ((cardNumber >= 10 ** 13) and (cardNumber < 10 ** 14)):
    print("INVALID")
    sys.exit()

# Luhn’s Algorithm

# Multiply every other digit by 2, starting with the number’s second-to-last digit,
# and then add those products’ digits together
sum = 0
tmp = cardNumber // 10
while tmp > 0:
    prod = 2 * (tmp % 10)
    while prod > 0:
        sum += prod % 10
        prod //= 10

    tmp //= 100

# Add the sum to the sum of the digits that weren’t multiplied by 2
tmp = cardNumber
while tmp > 0:
    sum += tmp % 10
    tmp //= 100

# If the total’s last digit is 0 (or, put more formally,
# if the total modulo 10 is not congruent to 0), the number is invalid
if (sum % 10) != 0:
    print("INVALID")
    sys.exit()

# If number of digits is 16 and card number starts with 51, 52, 53, 54 or 55, is Master Card
tmp = cardNumber // (10 ** 14)
if (tmp == 51) or (tmp == 52) or (tmp == 53) or (tmp == 54) or (tmp == 55):
    print("MASTERCARD")

# Else if number of digits is 16 and card number starts with 4, is Visa
elif cardNumber // (10 ** 15) == 4:
    print("VISA")

# Else if number of digits is 15 and card number starts with 34 or 37, is American Express
elif (cardNumber // (10 ** 13) == 34) or (cardNumber // (10 ** 13) == 37):
    print("AMEX")

# Else if number of digits is 13 and card number starts with 4, is Visa
elif cardNumber // (10 ** 12) == 4:
    print("VISA")

# Else is invalid
else:
    print("INVALID")
