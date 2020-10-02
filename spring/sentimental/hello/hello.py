#!/usr/bin/env python3

# Prints out a simple greeting to the user

import cs50

# Get user name
name = cs50.get_string("What is your name? ")

# Print out greeting
print(f"hello, {name}")