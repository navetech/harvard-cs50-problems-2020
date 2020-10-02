#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

// Encrypt messages using substitution cipher
int main(int argc, string argv[])
{
    // Check if there is a single command-line argument
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    // Check if the key contains exactly 26 characters
    int len = strlen(argv[1]);
    if (len != 26)
    {
        printf("Key must contain 26 characters\n");
        return 1;
    }

    // Check if each the key contains only letters and not repeated letters
    for (int i = 0; i < len; i++)
    {
        if (!isalpha(argv[1][i]))
        {
            printf("Key must contain letters\n");
            return 1;
        }
        else
        {
            argv[1][i] = toupper(argv[1][i]);
            for (int j = 0; j < i; j++)
            {
                if (argv[1][i] == argv[1][j])
                {
                    printf("Key must not contain repeated letters\n");
                    return 1;
                }
            }
        }
    }

    // Get plaintext from user
    string plaintext = get_string("plaintext: ");

    // Substitute each alphabetical character in the plaintext for the corresponding character in the key, preserving case
    printf("ciphertext: ");
    for (int i = 0, n = strlen(plaintext); i < n; i++)
    {
        if (!isalpha(plaintext[i]))
        {
            printf("%c", plaintext[i]);
        }
        else
        {
            char c;
            if (isupper(plaintext[i]))
            {
                c = toupper(argv[1][plaintext[i] - 'A']);
            }
            else
            {
                c = tolower(argv[1][plaintext[i] - 'a']);
            }

            printf("%c", c);
        }
    }

    printf("\n");

    return 0;

}