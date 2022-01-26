#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
typedef struct {
        BYTE    Y;
        BYTE    Cb;
        BYTE    Cr;
} YCbCrTRIPLE;
void main()
{
    //open bmp file
    FILE *in = fopen("parrot.bmp", "rb");

    //reading fileheader
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof (BITMAPFILEHEADER), 1, in);
    
    //reading infoheader
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof (BITMAPINFOHEADER), 1, in);
 
    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0. If not, quit
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
                    bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(in);
        printf("Unsupported file format.\n");
        return;
    }

    //allocating memory for image data
    RGBTRIPLE* imageData;
    imageData = (RGBTRIPLE *)malloc(bi.biSizeImage);

    //reading image data into variable
    fread(imageData, bi.biSizeImage, 1, in);

    //closing inputfile since no longer needed
    fclose(in);

    //allocating memory for converted image
    YCbCrTRIPLE* convImage;
    convImage = (YCbCrTRIPLE *)malloc(bi.biSizeImage);

    //each pixel gets converted from RGB to YCbCr
    for (int i = 0; i < bi.biHeight; i++)
    {
        for (int j = 0; j < bi.biWidth; j++)
        {
            //current position on converted image
            int convPos = i * bi.biWidth + j;

            //current position on bmp image
            int origPos = (bi.biHeight - 1 - i) * bi.biWidth + j;

            //conversion table from: https://www.w3.org/Graphics/JPEG/jfif3.pdf (26.01.22)
            convImage[convPos].Y =    0.299   * imageData[origPos].rgbtRed + 0.5876 * imageData[origPos].rgbtGreen + 0.114  * imageData[origPos].rgbtBlue;
            convImage[convPos].Cb = - 0.1687  * imageData[origPos].rgbtRed - 0.3313 * imageData[origPos].rgbtGreen + 0.5    * imageData[origPos].rgbtBlue + 128;
            convImage[convPos].Cr =   0.5     * imageData[origPos].rgbtRed - 0.4187 * imageData[origPos].rgbtGreen - 0.0813 * imageData[origPos].rgbtBlue + 128;
        }
    }
    
    //create output file
    FILE *out = fopen("out.bmp", "wb");

    //write fileheader
    fwrite(&bf, sizeof (BITMAPFILEHEADER), 1, out);

    //write infoheader
    fwrite(&bi, sizeof (BITMAPINFOHEADER), 1, out);

    //write pixels
    fwrite(convImage, bi.biSizeImage, 1, out);
    
    //close and free stuff
    fclose(out);
    free(imageData);
    free(convImage);

    //quick way to see if ran to the end
    printf("successful\n");
}