#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Get height between 1 and 8 from the user
    int height;
    do
    {
        height = get_int("Height: ");
    }
    while ((height < 1) || (height > 8));

    // Repeat from row #1 up to row #height
    for (int row = 1; row <= height; row++)
    {
        // Print #(height - rows) spaces 
        for (int j = 1; j <= height - row; j++)
        {
            printf(" ");
        }
    
        // Print #row hashes
        for (int j = 1; j <= row; j++)
        {
            printf("#");
        }

        // Print #2 spaces
        printf("  ");

        // Print #row hashes
        for (int j = 1; j <= row; j++)
        {
            printf("#");
        }

        // Print new line
        printf("\n");
    }
}
