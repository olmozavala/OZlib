/*
 * File:   ImageManager.cpp
 * Author: olmozavala
 *
 * Created on September 29, 2011, 6:06 PM
 */

#include "ImageManager.h"
#include <stdio.h>
#include <string.h>
#include <debug.h>
#include <sstream>

ImageManager::ImageManager() {
}

ImageManager::ImageManager(const ImageManager& orig) {
}

ImageManager::~ImageManager() {
}

void ImageManager::FreeImageErrorHandler(FREE_IMAGE_FORMAT fif, const char *message) {
	eout << "\n*** ERROR *** " << endl;
	if(fif != FIF_UNKNOWN) {
		eout << FreeImage_GetFormatFromFIF(fif) << message << endl;
	}
	eout << "*** ERROR *** " << endl;
}

/**
 * This function displays a lot of information from the image like the 
 * format, color type, pixel size in bytes, etc.
 * @param fileName
 * @param outBuf
 * @param format
 * @param width
 * @param height
 */
void ImageManager::displayImageInfo(FIBITMAP* image, char* fileName){
	
    cout << "---- Displaying info for image: " << fileName << endl;
	
    FREE_IMAGE_FORMAT format = FreeImage_GetFileType(fileName, 0);
    cout << "---- Image format: "; ImageManager::printImageFormat(format);
	
	int width = FreeImage_GetWidth(image);
	int height = FreeImage_GetHeight(image);
	cout << "---- Width:" << width << "   Height:" << height << endl;
	
	cout << "---- Number of bits per pixel: " << FreeImage_GetBPP(image) << endl;
	cout << "---- Size of the bitmap (GetLine): " << FreeImage_GetLine(image) << "(bytes)" << endl;
	//	cout << "Bytes per pixel: " << FreeImage_GetLine(image) / FreeImage_GetWidth(image) << endl;
	
	cout << "---- Image Type:" << endl; 
	ImageManager::printImageType( FreeImage_GetImageType(image) );
	
	cout << "---- ColorType: " << endl;
	printColorType(FreeImage_GetColorType(image));
	
	cout << "---- Has background color: " << FreeImage_HasBackgroundColor(image) << endl;	
	cout << "---- Red bit pattern: " << FreeImage_GetRedMask(image)  << endl;
	cout << "---- Green bit pattern: " << FreeImage_GetGreenMask(image)  << endl;
	cout << "---- Blue bit pattern: " << FreeImage_GetBlueMask(image)  << endl;
	
}
/**
 * Returns the 1D index from 2D coordinates. It can be used with 
 * RGB images where dim = 3
 * @param width
 * @param row
 * @param col
 * @param dim
 * @return 
 */
int ImageManager::indxFromCoord(int width, int row, int col, int dim) {
    return width * row * dim + dim*col;
}
/**
 * Returns the 2D coord from 1D index. It can be used with 
 * RGB images where dim = 3
 * @param width
 * @param row
 * @param col
 * @param dim
 * @return 
 */
void ImageManager::coordFromIndx(int indx, int width, int& i, int& j, int dim) {
    i = floor(indx / (width * dim));
    j = floor((indx - width * dim) / dim);
}
/**
 * Returns the 1D index from 2D coordinates. It can be used with 
 * RGB images where dim = 3
 * @param width
 * @param row
 * @param col
 * @param dim
 * @return 
 */
int ImageManager::indxFromCoord3D(int width, int height, int row, int col, int z) {
    return width * height * z + width * row + col;
}
/**
 * Returns the 3D coord from 1D index. (row major) 
 * @param width
 * @param row
 * @param col
 * @param dim
 * @return 
 */
void ImageManager::coordFromIndx3D(int indx, int width, int height, int& row, int& col, int& z) {
	int wtimesh = width * height;
    z = floor(indx / (wtimesh));
	
	int whz = width * height* z;// This function should go to the first index of the last 'plane'
    row = floor( (indx - whz) / width);// Computes the current row
	
    col = floor( indx - whz - (row * width));//Computes the current col ()
}
/**
 * This function writes a GrayScale 3D cube of data, as a colored image for SDF
 * @param folderName 
 * @param outBuf
 * @param format
 * @param width
 * @param height
 */
void ImageManager::write3DImageSDF(char* folderName, float* outBuf, int width, int height, int depth) {
	
	string fileNameSub = (string)folderName;
	string newFile;
	
	for(int k=0; k < depth; k++){
		stringstream sstm;
		sstm << fileNameSub << k << ".png"; 
		newFile = sstm.str();
		dout << "Saving file: " << newFile << endl;
		ImageManager::writeGrayScaleImageSDF( (char*) newFile.c_str(), &outBuf[k*width*height], FIF_PNG, width, height);
	}
}
/**
 * This function saves a 3D image as a set of 2D images
 * @param folderName 
 * @param outBuf
 * @param format
 * @param width
 * @param height
 * @param norm Indicates if the data comes normalized from 0-1
 */
void ImageManager::write3DImage(char* folderName, float* outBuf, int width, 
											int height, int depth, bool norm) {
	
	string fileNameSub = (string)folderName;
	string newFile;
	
	for(int k=0; k < depth; k++){
		stringstream sstm;
		sstm << fileNameSub << k << ".png"; 
		newFile = sstm.str();
		ImageManager::writeGrayScaleImage( (char*) newFile.c_str(), &outBuf[k*width*height], 
																FIF_PNG, width, height,norm);
	}
}

/**
 * This function saves a 3D image as a set of 2D images
 * @param folderName 
 * @param outBuf
 * @param format
 * @param width
 * @param height
 */
void ImageManager::write3DImageSDF(char* folderName, unsigned char* inputBuffer, int width, int height, int depth) {
	
	int buffSize = width*height*depth;
	float* tempBuffer = new float[buffSize];
	
	for(int i=0; i < buffSize; i++){
		tempBuffer[i] = (float)inputBuffer[i];
	}
	write3DImage(folderName, tempBuffer, width, height, depth,0);
}

/**
 * It writes float buffer as a red colored image for the results
 * of the SDF for my PhD
 * @param fileName
 * @param outBuf
 * @param format
 * @param width
 * @param height
 */
void ImageManager::writeGrayScaleImage(char* fileName, float* outBuf, FREE_IMAGE_FORMAT format, 
					int width, int height, bool norm) {
	
    FreeImage_Initialise();
    //    FIBITMAP* bitmap = FreeImage_Allocate(width, height, 8);
    FIBITMAP* bitmap = FreeImage_Allocate(width, height, 24);
	
    RGBQUAD color;
    int indx = 0;
	
	int value =0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
			indx = ImageManager::indxFromCoord(width, i, j, 1);
			if(norm){
				value = (255)*outBuf[indx];
			}else{
				value = outBuf[indx]; 
			}
			color.rgbRed = value;
			color.rgbGreen = value; 
			color.rgbBlue = value;
			FreeImage_SetPixelColor(bitmap, j, i, &color);
		}
	}

	FreeImage_Save(format, bitmap, fileName);
	FreeImage_DeInitialise();

}
/**
 * It writes float buffer as a red colored image for the results
 * of the SDF for my PhD
 * @param fileName
 * @param outBuf
 * @param format
 * @param width
 * @param height
 */
void ImageManager::writeGrayScaleImageSDF(char* fileName, float* outBuf, FREE_IMAGE_FORMAT format, int width, int height) {
	
    FreeImage_Initialise();
    //    FIBITMAP* bitmap = FreeImage_Allocate(width, height, 8);
    FIBITMAP* bitmap = FreeImage_Allocate(width, height, 24);
	
    RGBQUAD color;
    int indx = 0;
	
    float normalize = (float) 255 / (float) max(width, height); //It used to have some brightness on the image (assume the maximum distance is max width or height)
	
    //THis part is only to give color, it is for the SDF
    float thres = .5;
	
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            indx = ImageManager::indxFromCoord(width, i, j, 1);
            if (outBuf[indx] <= thres && outBuf[indx] >= -thres) {
                color.rgbRed = 100; 
                color.rgbGreen = 0;
                color.rgbBlue = 0;
            }else{
                if (outBuf[indx] > 0) {
                    color.rgbRed = 0;
                    color.rgbGreen = outBuf[indx]*2 * normalize; 
                    color.rgbBlue = 0;
                } else {
                    color.rgbGreen = 0;
                    color.rgbRed = -outBuf[indx]*5 * normalize;
                    color.rgbBlue = -outBuf[indx]*5 * normalize;
                }
            }
            FreeImage_SetPixelColor(bitmap, j, i, &color);
        }
    }
	
    FreeImage_Save(format, bitmap, fileName);
    FreeImage_DeInitialise();
	
}

void ImageManager::writeImage(char* fileName, float* outBuf,
        FREE_IMAGE_FORMAT format, int width, int height) {
    //        FIMEMORY *imem = FreeImage_OpenMemory(outBuf,width*height*4);
    //        FREE_IMAGE_FORMAT outformat = FreeImage_GetFileTypeFromMemory(imem,0);
    //        cout << outformat << endl;
    //        FIBITMAP* outBitMapImage = FreeImage_LoadFromMemory(outformat, imem);
    //        FIBITMAP* outBitMapImage = FreeImage_ConvertFromRawBits(outBuf,
    //                                    width, height, width*4,8,0,0,0,FALSE);
	
    FreeImage_Initialise();
    FIBITMAP* bitmap = FreeImage_Allocate(width, height, 24);
	
    RGBQUAD color;
	
    int row = 0;
    int col = 0;
	
    FreeImage_Save(format, bitmap, fileName);
    FreeImage_DeInitialise();
	
}

int* ImageManager::loadImage(char* fileName, int& width, int& height) {
	
    FREE_IMAGE_FORMAT format = FreeImage_GetFileType(fileName, 0);
	
    FIBITMAP* image = FreeImage_Load(format, fileName);
    width = FreeImage_GetWidth(image);
    height = FreeImage_GetHeight(image);
	
    cout << "---- Reading image " << fileName << endl;
    cout << "---- Image format: ";
    ImageManager::printImageFormat(format);
    cout << "---- Width:" << width << "   Height:" << height << endl;
    cout << "---- Pitch:" << FreeImage_GetPitch(image) << endl;
    cout << "---- Number of bits per pixel: " << FreeImage_GetBPP(image) << endl;
    cout << "---- Channel mask: " << FreeImage_GetRedMask(image) << endl;
	
    image = FreeImage_ConvertTo32Bits(image);
	
    int totSize = width * height * 4;
    int* imgbuf = new int[totSize];
	
    BYTE* imgBytes = (BYTE*) FreeImage_GetBits(image);
    //The reading of the image is from bottom left to upper right
    for (int i = 0; i < totSize; i++) {
        imgbuf[i] = (int) imgBytes[i];
    }
	
    return imgbuf;
}

/**
 * Reads an image and returns an array of chars.
 * In opengl this shoudl be of type:
 */
unsigned char* ImageManager::loadImageChar(char* fileName, int& width, int& height) {
	
    FREE_IMAGE_FORMAT format = FreeImage_GetFileType(fileName, 0);
	
    FIBITMAP* image = FreeImage_Load(format, fileName);
    width = FreeImage_GetWidth(image);
    height = FreeImage_GetHeight(image);
	
    cout << "---- Reading image " << fileName << endl;
    cout << "---- Image format: ";
    ImageManager::printImageFormat(format);
    cout << "---- Width:" << width << "   Height:" << height << endl;
    cout << "---- Pitch:" << FreeImage_GetPitch(image) << endl;
    cout << "---- Number of bits per pixel: " << FreeImage_GetBPP(image) << endl;
    cout << "---- Channel mask: " << FreeImage_GetRedMask(image) << endl;
	
    image = FreeImage_ConvertTo32Bits(image);
	
    unsigned char* imgbuf = new unsigned char[width * height * 4];
    memcpy(imgbuf, FreeImage_GetBits(image), sizeof (unsigned char) *width * height * 4);
	
    return imgbuf;
}

/**
 * This function reads an image and returns an array of bytes
 * where each pixel is 32 bits long and BGR order 
 */
BYTE* ImageManager::loadImageByte(char* fileName, int& width, int& height) {
	
    FREE_IMAGE_FORMAT format = FreeImage_GetFileType(fileName, 0);
	
    FIBITMAP* image = FreeImage_Load(format, fileName);
    width = FreeImage_GetWidth(image);
    height = FreeImage_GetHeight(image);
	
    cout << "---- Reading image " << fileName << endl;
    cout << "---- Image format: ";
    ImageManager::printImageFormat(format);
    cout << "---- Width:" << width << "   Height:" << height << endl;
    cout << "---- Pitch:" << FreeImage_GetPitch(image) << endl;
    cout << "---- Number of bits per pixel: " << FreeImage_GetBPP(image) << endl;
    cout << "---- Channel mask: " << FreeImage_GetRedMask(image) << endl;
	
    image = FreeImage_ConvertTo32Bits(image);
	
    BYTE* imgBytes = (BYTE*) FreeImage_GetBits(image);
	
    return imgBytes;
}

int* ImageManager::loadImageGrayScale(char* fileName, int& width, int& height) {
	
    FREE_IMAGE_FORMAT format = FreeImage_GetFileType(fileName, 0);
	
    cout << "Reading image " << fileName << endl;
    cout << "Image format: " << format << endl;
	
    FIBITMAP* image = FreeImage_Load(format, fileName);
    image = FreeImage_ConvertTo32Bits(image);
    image = FreeImage_ConvertToGreyscale(image);
	
    width = FreeImage_GetWidth(image);
    height = FreeImage_GetHeight(image);
	
    cout << "Width:" << width << endl << "Height:" << height << endl;
	
    int* imgbuf = new int[width * height];
    memcpy(imgbuf, FreeImage_GetBits(image), sizeof (unsigned char) *width * height);
	
    return imgbuf;
}

/**
 * Loads data from a gif file as a 3D data and stores the result in a 
 * int* array
 * @param fileName
 * @param width
 * @param height
 * @param pages
 * @return 
 */
unsigned char* ImageManager::load3dImageGif(char* fileName, int& width, int& height, int& pages){
	
    FREE_IMAGE_FORMAT format = FreeImage_GetFileType(fileName, 0);
    FIMULTIBITMAP* image = FreeImage_OpenMultiBitmap(format, fileName, false, true);
	
	pages = FreeImage_GetPageCount(image);//Gets the number of pages
	
    dout << "---- Page count: " << pages << endl;
	
	FIBITMAP* page1 = FreeImage_LockPage(image, 0);	
#ifdef DEBUG
	ImageManager::displayImageInfo(page1, fileName);
#endif
	//Assuming all the pages have the same size
	width = FreeImage_GetWidth(page1);
	height = FreeImage_GetHeight(page1);
	
	FreeImage_UnlockPage(image, page1, false);	
	
	string fileNameSub = "./images/3Dimages/part";
	string newFile;
	
	unsigned char* buffer = new unsigned char[width*height*pages];
	int index = 0;
	
	for(int pageNum = 0; pageNum < pages; pageNum++){
		stringstream sstm;
		FIBITMAP* currPage = FreeImage_LockPage(image, pageNum);	
		sstm << fileNameSub << pageNum << ".png"; 
		newFile = sstm.str();
		bool success = FreeImage_Save(FIF_PNG, currPage, newFile.c_str());
		if(success) 
			dout << " ... Success" << endl;
		else
			dout << " ... Failure" << endl;
		
		//-------------------
		FIBITMAP* currImage = FreeImage_ConvertTo8Bits(currPage);
		
		//The reading of the currImage is from bottom left to upper right
		BYTE* imgBytes8 = (BYTE*) FreeImage_GetBits(currImage);
		for (int j = 0; j < height; j++) {
			for (int i = 0; i < width; i++) {
				//				dout << (int)imgBytes8[i+j*width] << " ";
				buffer[index] = (unsigned char)imgBytes8[i+j*width];
				index++;
			}
			//			dout << endl;
		}
		//		dout << "===================" << endl;
		
		FreeImage_UnlockPage(image, currPage, false);	
	}
	
	
	FreeImage_CloseMultiBitmap(image, 0);
	return buffer;
	
}

/**
 * This function prints the 3d info stored on a int* buffer.
 * @param width
 * @param height
 * @param pages
 * @param buf
 */
void ImageManager::print3DImageGif(int width, int height, int pages, int* buf){
	for(int k = 0; k < pages; k++){
		cout << " ===================" << " Page " << k << " ===================" << endl;
		for (int j = 0; j < height; j++) {
			for (int i = 0; i < width; i++) {
				cout << (int)buf[i + j*width + k*width*height] << " ";
			}
			cout << endl;
		}
	}
}
/**
 * This function prints an image in a matrix form. The last
 * parameter is used for images with more than one layer
 * @param width
 * @param height
 * @param buf
 * @param skip
 */
void ImageManager::printImage(int width, int height, int* buf, int skip) {
    cout << "Printing image of char *" << endl;

    for (int row  = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            int indx = ImageManager::indxFromCoord(width, row, col, skip);
            cout << "\t" << (int) buf[indx];
        }
        cout << endl;
    }
}

/**
 * This function prints only one part of an image in a matrix form. 
 * @param width
 * @param height
 * @param buf
 * @param skip
 * @param onlyWidth Number of columns to print
 * @param onlyHeight Number of rows to print
 */
void ImageManager::printImageCutted(int width, int height, float* buf, int skip, int onlyWidth, int onlyHeight) {
    cout << "Printing image of float*" << endl;

    int minWidth = min(onlyWidth, width);
    int minHeight = min(onlyHeight, height);

    for (int row  = 0; row < minHeight; row++) {
        for (int col = 0; col < minWidth; col++) {
            int indx = ImageManager::indxFromCoord(width, row, col, skip);
            cout << "\t" << (float) buf[indx];
        }
        cout << endl;
    }
}

void ImageManager::printImageCutted(int width, int height, int* buf, int skip, int onlyWidth, int onlyHeight){
    cout << "Printing image of int*" << endl;

    int minWidth = min(onlyWidth, width);
    int minHeight = min(onlyHeight, height);

    for (int row  = 0; row < minHeight; row++) {
        for (int col = 0; col < minWidth; col++) {
            int indx = ImageManager::indxFromCoord(width, row, col, skip);
            cout << "\t" << (int) buf[indx];
        }
        cout << endl;
    }
}

void ImageManager::printImageCutted(int width, int height, char* buf, int skip, int onlyWidth, int onlyHeight){
    cout << "Printing image of char*" << endl;

    int minWidth = min(onlyWidth, width);
    int minHeight = min(onlyHeight, height);

    for (int row  = 0; row < minHeight; row++) {
        for (int col = 0; col < minWidth; col++) {
            int indx = ImageManager::indxFromCoord(width, row, col, skip);
            cout << "\t" << (int) buf[indx];
        }
        cout << endl;
    }
}

void ImageManager::printImageCutted(int width, int height, unsigned int* buf, int skip, int onlyWidth, int onlyHeight){
    cout << "Printing image of unsigned int*" << endl;

    int minWidth = min(onlyWidth, width);
    int minHeight = min(onlyHeight, height);

    for (int row  = 0; row < minHeight; row++) {
        for (int col = 0; col < minWidth; col++) {
            int indx = ImageManager::indxFromCoord(width, row, col, skip);
            cout << "\t" << (unsigned int) buf[indx];
        }
        cout << endl;
    }
}

void ImageManager::printImageFormat(FREE_IMAGE_FORMAT format) {
    switch (format) {
        case FIF_BMP:
            cout << "BMP" << endl;
            break;
        case FIF_GIF:
            cout << "GIF" << endl;
            break;
        case FIF_JPEG:
            cout << "JPEG" << endl;
            break;
        case FIF_PNG:
            cout << "PNG" << endl;
            break;
        case FIF_TIFF:
            cout << "TIFF" << endl;
            break;
        case FIF_TARGA:
            cout << "TARGA" << endl;
            break;
        case FIF_PCX:
            cout << "PCX" << endl;
            break;
        case FIF_UNKNOWN:
            cout << "UNKNOWN" << endl;
            break;
    }
}

/**
 * This function prints an image in a matrix form. The last
 * parameter is used for images with more than one layer
 * @param width
 * @param height
 * @param buf
 * @param skip
 */
void ImageManager::printImage(int width, int height, float* buf, int skip) {
    cout << "Printing image of float *" << endl;
	//    for (int i = 0; i < height; i++) {
    for (int i = height-1; i >= 0; i--) {
        for (int j = 0; j < width; j++) {
            int indx = ImageManager::indxFromCoord(width, i, j, skip);
            cout << "\t" << (float) buf[indx];
        }
        cout << endl;
    }
}

/**
 * This function prints BGRA images as (R,G,B,A) cells in a matrix form.
 * It assumes that the first index is the bottom left part of the image
 * and that the pixel order is 
 * @param width
 * @param height
 * @param buf
 * @param skip
 */
void ImageManager::printImageBGRA(int width, int height, float* buf) {
    cout << "Printing BGRA image as RGBA" << endl;
    //for (int j = 0; j < height; j++) {
    for (int j = height-1; j >= 0; j--) {
        for (int i = 0; i < width; i++) {
            cout << "(" <<  buf[ (width * j + i)*4 + 2] << ","; //Red
            cout << buf[ (width * j + i)*4 + 1] << ","; //Green
            cout << buf[ (width * j + i)*4] << ","; //Blue
            cout << buf[ (width * j + i)*4 + 3] << ") \t" ; //Alpha
        }
        cout << endl;
    }
}



/**
 * Normalize an image to a float image with values from 0 to 1
 * @param array
 * @param size
 * @return 
 */
float* ImageManager::byteToFloatNorm(BYTE* array, int size) {
    float* farr = new float[size];
    for (int i = 0; i < size; i++) {
        farr[i] = (float) (array[i])/255;
    }
    return farr;
}

float* ImageManager::byteToFloat(BYTE* array, int size) {
    float* farr = new float[size];
    for (int i = 0; i < size; i++) {
        farr[i] = (float) (array[i]);
    }
    return farr;
}

float* ImageManager::intToFloat(int* array, int size) {
    float* farr = new float[size];
    for (int i = 0; i < size; i++) {
        farr[i] = (float) (array[i]);
    }
    return farr;
}

void ImageManager::printImageType(int imageType){
	switch(imageType){
		case FIT_BITMAP:
			cout << "\t Standard image: 1-, 4-, 8-, 16-, 24-, 32-bit " << endl; break;
		case FIT_UINT16:
			cout << "\t Array of unsigned short: unsigned 16-bit "<< endl; break;
		case FIT_INT16: 
			cout <<"\t Array of short: signed 16-bit" << endl; break;
		case FIT_UINT32:
			cout << "\t Array of unsigned long: unsigned 32-bit" << endl; break;
		case FIT_INT32:
			cout << "\t Array of long: signed 32-bit" << endl; break;
		case FIT_FLOAT:
			cout << "\tArray of float: 32-bit IEEE floating point" << endl; break;
		case FIT_DOUBLE:
			cout << "\t Array of double: 64-bit IEEE floating point" << endl; break;
		case FIT_COMPLEX: 
			cout << "\t Array of FICOMPLEX: 2 x 64-bit IEEE floating point" << endl; break;
		case FIT_RGB16:
			cout << "\t 48-bit RGB image: 3 x 16-bit" << endl; break;
		case FIT_RGBA16:
			cout << "\t 64-bit RGBA image: 4 x 16-bit" << endl; break;
		case FIT_RGBF:
			cout <<"\t 96-bit RGB float image: 3 x 32-bit IEEE floating point" << endl; break;
		case FIT_RGBAF:
			cout << "\t 128-bit RGBA float image: 4 x 32-bit IEEE floating point" << endl; break;
	}
	cout << endl;
}

/**
 * Prints the color type of an image.  
 * @param colorType
 */
void ImageManager::printColorType(int colorType){
	switch(colorType){
		case FIC_MINISBLACK:
			cout << "\t Monochrome bitmap (1-bit)"; break;
		case FIC_MINISWHITE:
			cout << "\t Monochrome bitmap (1-bit)"; break;
		case FIC_PALETTE:
			cout << "\t Palettized bitmap (1,4 or 8 bit) "; break;
		case FIC_RGB:
			cout << "\t High-color bitmap (16, 24 or 32 bit)"; break;
		case FIC_RGBALPHA:
			cout << "\t High-color bitmap with alpha (32 bit)"; break;
		case FIC_CMYK:
			cout << "\t CMYK bitmap (32 bit only)"; break;
	}
	cout << endl;
}

/**
 * Prints the different sizes in bytes of the c++ basic variables
 */
void ImageManager::printVariablesSizes(){
	cout << "Size of byte: \t" << sizeof(BYTE) << "(bytes)" << endl;
	cout << "Size of bool: \t" << sizeof(bool) << "(bytes)" << endl;
	cout << "Size of unsigned char: \t" << sizeof(unsigned char) << "(bytes)" << endl;
	cout << "Size of char : \t" << sizeof(char) << "(bytes)" << endl;
	cout << "Size of int: \t" << sizeof(int) << "(bytes)" << endl;
	//	cout << "Size of uint: \t" << sizeof(uint) << "(bytes)" << endl;
	cout << "Size of float: \t" << sizeof(float) << "(bytes)" << endl;
	cout << "Size of double:\t" << sizeof(double) << "(bytes)" << endl;
	cout << "Size of long: \t" << sizeof(long) << "(bytes)" << endl;
}
