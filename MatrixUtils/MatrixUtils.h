/*
 * File:   MatrixUtils.h
 * Author: Olmo Sebastian Zavala Romero 
 * Created on May 21, 2009, 7:19 PM
 */

#ifndef _MatrixUtils_H
#define	_MatrixUtils_H

#include <iostream>
#include <string>

template <class T>
class MatrixUtils {
    public:
        MatrixUtils();
        MatrixUtils(const MatrixUtils& orig);

        //Print functions
        static void changeStorageOrder(T* arr_normal, T* arr_col_wise, int width, int height);
        //static T str_to_T(const char * elem);

        static int indxFromCoord(int width, int i, int j, int dim);
        static void printImage(int width, int height, T* buf, int skip);
        static void print3DImage(int width, int height, int depth, T* buf);
        static void print3DImage(int width, int height, int depth, int limw, int limh, int limd, T* buf);
        //Prints an array fo dimensions width and height, but only prints the first
        // onlyWidth x onlyHeight cells
        static void printImageCutted(int width, int height, T* buf, int skip, int onlyWidth, int onlyHeight);

        virtual ~MatrixUtils();
    private:

};

using namespace std;

template <class T>
MatrixUtils<T>::MatrixUtils(const MatrixUtils& orig) 
{
}

template <class T>
MatrixUtils<T>::~MatrixUtils() 
{
}

template <class T>
int MatrixUtils<T>::indxFromCoord(int width, int row, int col, int dim) {
    return width * row * dim + dim*col;
}

template <class T>
void MatrixUtils<T>::printImage(int width, int height, T* buf, int skip){
    cout << "Printing image of *" << endl;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int indx = MatrixUtils<T>::indxFromCoord(width, i, j, skip);
            if( sizeof(T) > 1){
                cout << "\t" << (T) buf[indx];
            }else{
                cout << "\t" << (int) buf[indx];
            }
        }
        cout << endl;
    }

}

template <class T>
void MatrixUtils<T>::print3DImage(int width, int height, int depth, 
        int limw, int limh, int limd, T* buf){

    int minw = min(width, limw);
    int minh = min(height, limh);
    int mind = min(depth, limd);

	for (int z = 0; z < mind; z++) {
		cout << "--------------------"<< z <<"------------------------" << endl;
		for (int col = 0; col < minh; col++) {
			for (int row = 0; row < minw; row++) {

				int indx = row + col*width + z*width*height;
				if( sizeof(T) > 1){
					cout << "\t" << (T) buf[indx];
				}else{
					cout << "\t" << (int) buf[indx];
				}
            }
			cout << endl;
        }
    }
}
template <class T>
void MatrixUtils<T>::print3DImage(int width, int height, int depth,  T* buf){
	for (int z = 0; z < depth; z++) {
		cout << "--------------------"<< z <<"------------------------" << endl;
		for (int col = 0; col < height; col++) {
			for (int row = 0; row < width; row++) {
				int indx = row + col*width + z*width*height;
				if( sizeof(T) > 1){
					cout << "\t" << (T) buf[indx];
				}else{
					cout << "\t" << (int) buf[indx];
				}
            }
			cout << endl;
        }
    }
}

template <class T>
void MatrixUtils<T>::printImageCutted(int width, int height, T* buf, int skip, int onlyWidth, int onlyHeight){
    cout << "Printing image" << endl;
    for (int row  = 0; row < onlyHeight; row++) {
        for (int col = 0; col < onlyWidth; col++) {
            int indx = MatrixUtils<T>::indxFromCoord(width, row, col, skip);
            if( sizeof(T) > 1){
                cout << "\t" << (T) buf[indx];
            }else{
                cout << "\t" << (int) buf[indx];
            }
			
        }
        cout << endl;
    }
}

/**
 * Should switch the order of the buffer, from column-wise to row-wise
 * and viseversa.
 */
template <class T>
void MatrixUtils<T>::changeStorageOrder(T* arr_normal, T* arr_col_wise, int width, int height){
    int indx = 0;
    for (int col = 0; col < width; col++) {
        for (int row = 0; row < height; row++) {
            arr_col_wise[indx] = arr_normal[width*row + col];
            indx++ ;
        }
    }
}

/*
 template<class T>
 T MatrixUtils::str_to_T(const char * elem) {
 // unknown type, so just throw
 throw;
 }
 
 double MatrixUtils::str_to_T<double>(const char * elem) {
 return atof(elem);
 }
 
 unsigned MatrixUtils::str_to_T<unsigned>(const char * elem) {
 return atoi(elem);
 }
 */

#endif	
