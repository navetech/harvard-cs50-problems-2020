#!/usr/bin/env python3

# Identify a person based on their DNA

import sys
import csv
import cs50

# Check the correct program's usage
if len(sys.argv) != 3:
    print("Usage: python dna.py data.csv sequence.txt")
    sys.exit(1)

# Get DNA sequence
fseq = open(sys.argv[2])
dna_seq = fseq.read()

# Get people DNA data
fpeople = open(sys.argv[1], newline='')
people_dna = csv.DictReader(fpeople)

# Get STRs of people DNA data
strs = people_dna.fieldnames

# Repeat for each STR
str_repeats = {}
for str in strs:
    if (str == "name"):
        continue

    # Repeat for each nucleotide nucleotide in DNA sequence
    maxcount = 0
    sseq = 0
    while sseq < (len(dna_seq) - len(str)):

        # Count max repetition number of the STR in DNA sequence
        eseq = len(dna_seq)
        countseq = 0
        while (True):
            iseq = dna_seq.find(str, sseq, eseq)
            if (iseq < 0):
                sseq += 1
                break
            sseq = iseq + len(str)
            eseq = sseq + len(str)

            countseq += 1
            if countseq > maxcount:
                maxcount = countseq

    # Set the maximum repetition number of the STR
    str_repeats[str] = maxcount

# Repeat for each person
for person_dna in people_dna:

    # For each STR
    match = 0
    for str in strs:
        if (str == "name"):
            continue

        # Check the STR repeats against that of the person
        if str_repeats[str] == int(person_dna[str]):
            match += 1
        else:
            match = 0
            break

    # If person found print its name
    if (match > 0):
        print(person_dna["name"])
        fseq.close()
        fpeople.close()
        sys.exit()

# No person found
print("No match")
fseq.close()
fpeople.close()
sys.exit()
