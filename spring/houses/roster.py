#!/usr/bin/env python3

# TODO

# Produce class rosters from student data on a database

import sys
import csv
from cs50 import SQL

# Check the correct program's usage
if len(sys.argv) != 2:
    print("Usage: python roster.py house")
    sys.exit(1)

# Set up a database connection
db = SQL("sqlite:///students.db")

# Get data from the database
rows = db.execute("SELECT first, middle, last, birth FROM students WHERE house=? ORDER BY last, first", sys.argv[1])

# Repeat for each student
for row in rows:

    # Coumpound name
    if row["middle"]:
        name = row["first"] + " " + row["middle"] + " " + row["last"]
    else:
        name = row["first"] + " " + row["last"]

    # Print student data
    print(f"{name}, born {row['birth']}")

