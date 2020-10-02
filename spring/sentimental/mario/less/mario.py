#!/usr/bin/env python3

# Print out a half-pyramid of a specified height

import cs50

# Get height from the user
while True:
    height = cs50.get_int("Height: ")
    if height >= 1 and height <= 8:
        break

# Print half-pyramid
# For each layer
for i in range(1, height + 1):

    # Print spaces
    print(" " * (height - i), end="")

    # Print hashes
    print("#" * i, end="")

    # Print new line
    print()