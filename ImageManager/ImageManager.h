/*
 * File:   ImageManager.h
 * Author: olmozavala
 *
 * Created on September 29, 2011, 6:06 PM
 */

#ifndef IMAGEMANAGER_H
#define	IMAGEMANAGER_H

#include <FreeImage.h>
#include <iostream>
#include <math.h>

using namespace std;

class ImageManager {
public:
    ImageManager();
    ImageManager(const ImageManager& orig);
    virtual ~ImageManager();

    static int* loadImage(char* fileName, int& width, int& height);
    static BYTE* loadImageByte(char* fileName, int& width, int& height);
    static unsigned char* loadImageChar(char* fileName, int& width, int& height);
    static int* loadImageGrayScale(char* fileName, int& width, int& height);
	static unsigned char* load3dImageGif(char* filename, int& width, int& height, int& pages);

    static void writeImage(char* fileName, float* outBuf, FREE_IMAGE_FORMAT format, int width, int height);

    static void write3DImage(char* folder, float* outBuf, int width, int height, int depth, bool norm);

    static void write3DImageSDF(char* folder, float* outBuf, int width, int height, int depth);
    static void write3DImageSDF(char* folder, unsigned char* outBuf, int width, int height, int depth);

    static void writeGrayScaleImageSDF(char* fileName, float* outBuf, FREE_IMAGE_FORMAT format, int width, int height);
    static void writeGrayScaleImage(char* fileName, float* outBuf, FREE_IMAGE_FORMAT format, int width, int height, bool norm);

	static void displayImageInfo( FIBITMAP* image, char* fileName);
	static void print3DImageGif(int width, int height, int pages, int* buf);
    static void printImage(int width, int height, int* buf, int skip);
    static void printImage(int width, int height, float* buf, int skip);

    static void printImageBGRA(int width, int height, float* buf);
    //Prints an array fo dimensions width and height, but only prints the first
    // onlyWidth x onlyHeight cells
    static void printImageCutted(int width, int height, float* buf, int skip, int onlyWidth, int onlyHeight);
    static void printImageCutted(int width, int height, int* buf, int skip, int onlyWidth, int onlyHeight);
    static void printImageCutted(int width, int height, unsigned int* buf, int skip, int onlyWidth, int onlyHeight);
    static void printImageCutted(int width, int height, char* buf, int skip, int onlyWidth, int onlyHeight);
    static void printImageFormat(FREE_IMAGE_FORMAT format);

    static float* intToFloat(int* array, int size);
    static float* byteToFloat(BYTE* array, int size);
    static float* byteToFloatNorm(BYTE* array, int size);

    static int indxFromCoord(int width, int i, int j, int dim);
    static void coordFromIndx(int indx, int width, int& i, int& j, int dim);

    static int indxFromCoord3D(int width, int height, int i, int j, int k);
    static void coordFromIndx3D(int indx, int width, int height, int& row, int& col, int& z); 

	// Extras for managing the error
	static void FreeImageErrorHandler(FREE_IMAGE_FORMAT fif, const char *message);
	static void printVariablesSizes();

private:
	static void printImageType(int imageType);
	static void printColorType(int colorType);

};

#endif	/* IMAGEMANAGER_H */

