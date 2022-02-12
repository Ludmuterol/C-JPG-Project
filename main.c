#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "load.c"

void main()
{
    //load image
    BMPIMAGE image;
    load(&image, "parrot.bmp");

    //convert image from RGB to YCbCr (convImage has no infos anymore, just image data)
    YCbCrTRIPLE *convImage = convert(&image);

    //free imagedata since not needed anymore
    free(image.imageData);

    //allocate memory for each component
    BYTE *YImage  = (BYTE *)malloc(image.bi.biSizeImage / 3);
    BYTE *CbImage = (BYTE *)malloc(image.bi.biSizeImage / 3);
    BYTE *CrImage = (BYTE *)malloc(image.bi.biSizeImage / 3);

    for (int i = 0; i < image.bi.biHeight; i++)
    {
        for (int j = 0; j < image.bi.biWidth; j++)
        {
            //current position in array
            int curPos = i * image.bi.biWidth + j;

            //split into three separate arrays
            YImage[curPos]  = convImage[curPos].Y;
            CbImage[curPos] = convImage[curPos].Cb;
            CrImage[curPos] = convImage[curPos].Cr;
        }
    }
    
    //free convImage, since not needed anymore
    free(convImage);

    //create output file
    FILE *out = fopen("out.bmp", "wb");

    //write fileheader
    fwrite(&(image.bf), sizeof(BITMAPFILEHEADER), 1, out);

    //write infoheader
    fwrite(&(image.bi), sizeof(BITMAPINFOHEADER), 1, out);

    //write pixels
    //fwrite(convImage, image.bi.biSizeImage, 1, out); //to use this line instead of for loop move free from above to below
    for (int i = 0; i<image.bi.biSizeImage / 3;i++)
    {
        fwrite(YImage  + i,1, 1, out);
        fwrite(CbImage + i,1, 1, out);
        fwrite(CrImage + i,1, 1, out);
    }

    //close and free stuff
    fclose(out);
    free(YImage);
    free(CbImage);
    free(CrImage);

    //quick way to see if ran to the end
    printf("successful\n");
}