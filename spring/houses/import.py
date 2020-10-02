#!/usr/bin/env python3

# TODO

# Import student data from a .csv file into a database

import sys
import csv
from cs50 import SQL

# Check the correct program's usage
if len(sys.argv) != 2:
    print("Usage: python import.py data.csv")
    sys.exit(1)

# Set up a database connection
db = SQL("sqlite:///students.db")

with open(sys.argv[1]) as file:
    # Get students data
    rows = csv.DictReader(file)

    # Repeat for each student
    for row in rows:
        name = row["name"]
        house = row["house"]
        birth = row["birth"]

        # Get first, middle, an last name
        names = name.split()
        first = names[0]
        if len(names) == 2:
            middle = None
        else:
            middle = names[1]

        last = names[len(names) - 1]

        # Insert student data into database
        db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES (?, ?, ?, ?, ?)", first, middle, last, house, birth)

