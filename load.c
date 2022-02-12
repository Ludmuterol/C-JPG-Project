#include <windows.h>
#include <stdio.h>

typedef struct
{
    BYTE Y;
    BYTE Cb;
    BYTE Cr;
} YCbCrTRIPLE;

typedef struct
{
    BITMAPFILEHEADER bf;
    BITMAPINFOHEADER bi;
    RGBTRIPLE *imageData;
} BMPIMAGE;

void load(BMPIMAGE *image, char *path)
{
    //open bmp file
    FILE *in = fopen(path, "rb");

    //reading fileheader
    //BITMAPFILEHEADER bf;
    fread(&(image->bf), sizeof(BITMAPFILEHEADER), 1, in);

    //reading infoheader
    //BITMAPINFOHEADER bi;
    fread(&(image->bi), sizeof(BITMAPINFOHEADER), 1, in);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0. If not, quit
    if (image->bf.bfType != 0x4d42 || image->bf.bfOffBits != 54 || image->bi.biSize != 40 ||
        image->bi.biBitCount != 24 || image->bi.biCompression != 0)
    {
        fclose(in);
        printf("Unsupported file format.\n");
        return;
    }

    //allocating memory for image data
    //RGBTRIPLE* imageData;
    image->imageData = (RGBTRIPLE *)malloc(image->bi.biSizeImage);

    //reading image data into variable
    fread(image->imageData, image->bi.biSizeImage, 1, in);

    //closing inputfile since no longer needed
    fclose(in);
}

YCbCrTRIPLE *convert(BMPIMAGE *image)
{
    YCbCrTRIPLE *convImage;

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
            convImage[convPos].Y  =  0.299  * image->imageData[origPos].rgbtRed + 0.5876 * image->imageData[origPos].rgbtGreen + 0.114  * image->imageData[origPos].rgbtBlue;
            convImage[convPos].Cb = -0.1687 * image->imageData[origPos].rgbtRed - 0.3313 * image->imageData[origPos].rgbtGreen + 0.5    * image->imageData[origPos].rgbtBlue + 128;
            convImage[convPos].Cr =  0.5    * image->imageData[origPos].rgbtRed - 0.4187 * image->imageData[origPos].rgbtGreen - 0.0813 * image->imageData[origPos].rgbtBlue + 128;
        }
    }
    return (convImage);
}