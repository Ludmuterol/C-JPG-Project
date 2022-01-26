#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
void main()
{
    FILE *in = fopen("parrot.bmp", "r");
    if (in == NULL)
    {
        return;
    }
    FILE *out = fopen("out.bmp", "wb");
    if (out == NULL)
    {
        return;
    }

    BITMAPFILEHEADER bf;
    fread(&bf, sizeof (BITMAPFILEHEADER), 1, in);
 
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof (BITMAPINFOHEADER), 1, in);
 
    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0. If not, quit
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
                    bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(in);
        fclose(out);
        fprintf(stderr, "Unsupported file format.\n");
        return;
    }
    fwrite(&bf, sizeof (BITMAPFILEHEADER), 1, out);
    fwrite(&bi, sizeof (BITMAPINFOHEADER), 1, out);


    unsigned char* imageData;
    imageData = (unsigned char*)malloc(bi.biSizeImage);
    fread(imageData,bi.biSizeImage,1,in);
    fwrite(imageData,bi.biSizeImage,1,out);

    fclose(in);
    fclose(out);
    printf("woop\n");
}