#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "load.c"
YCbCrTRIPLE * convert(BMPIMAGE *image){
    YCbCrTRIPLE* convImage;

    //allocating memory for converted image
    convImage = (YCbCrTRIPLE *)malloc(image->bi.biSizeImage);

    //each pixel gets converted from RGB to YCbCr
    for (int i = 0; i < image->bi.biHeight; i++)
    {
        for (int j = 0; j < image->bi.biWidth; j++)
        {
            //current position on converted image
            int convPos = i * image->bi.biWidth + j;

            //current position on bmp image
            int origPos = (image->bi.biHeight - 1 - i) * image->bi.biWidth + j;

            //conversion table from: https://www.w3.org/Graphics/JPEG/jfif3.pdf (26.01.22)
            convImage[convPos].Y =    0.299   * image->imageData[origPos].rgbtRed + 0.5876 * image->imageData[origPos].rgbtGreen + 0.114  * image->imageData[origPos].rgbtBlue;
            convImage[convPos].Cb = - 0.1687  * image->imageData[origPos].rgbtRed - 0.3313 * image->imageData[origPos].rgbtGreen + 0.5    * image->imageData[origPos].rgbtBlue + 128;
            convImage[convPos].Cr =   0.5     * image->imageData[origPos].rgbtRed - 0.4187 * image->imageData[origPos].rgbtGreen - 0.0813 * image->imageData[origPos].rgbtBlue + 128;
        
        }
    }
    return(convImage);
}

void main()
{
    BMPIMAGE image;
    load(&image,"parrot.bmp");
    
    YCbCrTRIPLE* convImage = convert(&image);

    //create output file
    FILE *out = fopen("out.bmp", "wb");

    //write fileheader
    fwrite(&(image.bf), sizeof (BITMAPFILEHEADER), 1, out);

    //write infoheader
    fwrite(&(image.bi), sizeof (BITMAPINFOHEADER), 1, out);

    //write pixels
    fwrite(convImage, image.bi.biSizeImage, 1, out);
    
    //close and free stuff
    fclose(out);
    free(image.imageData);
    free(convImage);
    //quick way to see if ran to the end
    printf("successful\n");
}