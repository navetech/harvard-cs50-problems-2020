#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

// Encrypt messages using Caesar’s cipher
int main(int argc, string argv[])
{
    // Check if there is a single command-line argument
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    // Check if each character of that command line argument is a decimal digit
    for (int i = 0, n = strlen(argv[1]); i < n; i++)
    {
        if (!isdigit(argv[1][i]))
        {
            printf("Usage: ./caesar key\n");
            return 1;
        }
    }

    // Convert command line argument from string to integer
    int key = atoi(argv[1]);

    // Get plaintext from user
    string plaintext = get_string("plaintext: ");

    // Shift only the letters of plaintext by the key value, preserving case
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
                c = (((plaintext[i] - 'A') + key) % 26) + 'A';
            }
            else
            {
                c = (((plaintext[i] - 'a') + key) % 26) + 'a';
            }

            printf("%c", c);
        }
    }

    printf("\n");

    return 0;

}