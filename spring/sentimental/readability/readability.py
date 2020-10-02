#!/usr/bin/env python3

# Compute the approximate grade level needed to comprehend some text,

from cs50 import get_string


def main():

    # Get text from user
    text = get_string("Text: ")

    # Count number of letters
    letters = count_letters(text)

    # Count number of words
    words = count_words(text)

    # Count number of sentences
    sentences = count_sentences(text)

    # Compute Coleman-Liau index
    L = (letters / words) * 100
    S = (sentences / words) * 100
    index = round(0.0588 * L - 0.296 * S - 15.8)

    # Print results
    if index < 1:
        print("Before Grade 1")
    elif index >= 16:
        print("Grade 16+")
    else:
        print(f"Grade {index}")


# Count number of letters
def count_letters(text):

    # Repeat for each character in text
    letters = 0
    for c in text:

        # If letter, increment number of letters
        if c.isalpha():
            letters += 1

    return letters


# Count number of words
def count_words(text):

    words = text.split()

    return len(words)

    """
    # Repeat for each character in text
    in_word = False
    for c in text:

        # If space, set that is not in a word
        if c.isspace():
            in_word = False
        # Else (not a space)
        else:
            # if not in a word, increment number of words
            if not in_word:
                words += 1
                in_word = True

    return words
    """


def count_sentences(text):

    # Repeat for each character in text
    sentences = 0
    for c in text:

        # If period, interrogation mark or exclamantion point, increment number of sentences
        if c == "." or c == "?" or c == "!":
            sentences += 1

    return sentences


if __name__ == "__main__":
    # execute only if run as a script
    main()
