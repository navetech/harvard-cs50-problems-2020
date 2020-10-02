#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

// Compute the approximate grade level needed to comprehend some text,
int main(void)
{
    // Get text from user
    string text = get_string("Text: ");

    // Count number of letters
    int letters = count_letters(text);

    // Count number of words
    int words = count_words(text);

    // Count number of sentences
    int sentences = count_sentences(text);

    // Compute Coleman-Liau index
    float L = ((float)letters / words) * 100.;
    float S = ((float)sentences / words) * 100.;
    int index = round(0.0588 * L - 0.296 * S - 15.8);

    // Print results
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }

}

// Count number of letters
int count_letters(string text)
{
    // Repeat for each character in text
    int letters = 0;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        // If letter, increment number of letters
        if (isalpha(text[i]))
        {
            letters++;
        }
    }

    return letters;
}

// Count number of words
int count_words(string text)
{
    // Repeat for each character in text
    int words = 0;
    bool in_word = false;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        // If space, set that is not in a word
        if (isspace(text[i]))
        {
            in_word = false;
        }
        // Else (not a space)
        else
        {
            // if not in a word, increment number of words
            if (!in_word)
            {
                words++;
                in_word = true;
            }
        }
    }

    return words;
}

int count_sentences(string text)
{
    // Repeat for each character in text
    int sentences = 0;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        // If period, interrogation mark or exclamantion point, increment number of sentences
        if ((text[i] == '.') || (text[i] == '?') || (text[i] == '!'))
        {
            sentences++;
        }
    }

    return sentences;
}