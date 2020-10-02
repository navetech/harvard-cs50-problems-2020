#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image.\n");
        return 1;
    }

    // remember filenames
    char *infile = argv[1];
    char outfile[] = {"XXX.jpg"};

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // iterate over infile's blocks
    FILE *outptr = NULL;
    int n_images = 0;
    unsigned char block[512];
    while (fread(block, sizeof(block), 1, inptr) == 1)
    {
        // check the start of a JPEG image
        if ((block[0] == 0xff) &&
            (block[1] == 0xd8) &&
            (block[2] == 0xff) &&
            ((block[3] & 0xf0) == 0xe0))
        {
            // start of a image:
            // if there was already a image found
            if (n_images > 0)
            {
                // close last image outfile
                if (fclose(outptr))
                {
                    fprintf(stderr, "Could not close %s.\n", outfile);
                    return 4;
                }
            }

            // determine next image outfile name
            sprintf(outfile, "%03i.jpg", n_images);
            n_images++;

            // open image outfile
            outptr = fopen(outfile, "w");
            if (outptr == NULL)
            {
                fclose(inptr);
                fprintf(stderr, "Could not create %s.\n", outfile);
                return 3;
            }

            // write block to image outfile
            fwrite(block, sizeof(block), 1, outptr);
        }
        else  // not start of a image:
        {
            // if there is a image found
            if (n_images > 0)
            {
                // write block to image outfile
                fwrite(block, sizeof(block), 1, outptr);
            }
        }
    }

    // close infile
    fclose(inptr);

    // close outfile
    if (outptr != NULL)
    {
        fclose(outptr);
    }

    // success
    return 0;

}
