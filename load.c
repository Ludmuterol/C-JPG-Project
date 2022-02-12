#include <windows.h>
#include <stdio.h>

typedef struct {
        BYTE    Y;
        BYTE    Cb;
        BYTE    Cr;
} YCbCrTRIPLE;

typedef struct {
        BITMAPFILEHEADER bf;
        BITMAPINFOHEADER bi;
        RGBTRIPLE* imageData;
} BMPIMAGE;
void load(BMPIMAGE *image, char *path){
    //open bmp file
    FILE *in = fopen(path, "rb");

    //reading fileheader
    //BITMAPFILEHEADER bf;
    fread(&(image->bf), sizeof (BITMAPFILEHEADER), 1, in);
    
    //reading infoheader
    //BITMAPINFOHEADER bi;
    fread(&(image->bi), sizeof (BITMAPINFOHEADER), 1, in);
 
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