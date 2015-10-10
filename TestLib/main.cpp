/* 
 * File:   NAME.cpp
 * Author: Olmo Zavala Romero 
 * 
 */

#include <stdlib.h>
#include <iostream>
#include <FileManager/FileManager.h>

using namespace std;
int main(int argc, char** argv) {

    char* fileName = (char*)"data/data.txt";
    int rows = 0;
    int cols = 0;

    char* data = FileManager::readCommaSepFile<char>(fileName, rows, cols);

    int size = rows*cols;

    cout << "Writing ..... " << endl;
    for(int i = 0; i < size; i++){
        cout << (int)data[i] << endl;
    }

    char* binName = (char*)"data/binDat.txt";
    FileManager::writeBinaryFile<char>(binName, data, rows * cols, false); 

    
    char* newData = FileManager::readBinFile<char>(binName, size);
    cout << "Reading ... " << endl;
    for(int i = 0; i < size; i++){
        cout << (int)data[i] << endl;
    }

    char* normalData = FileManager::readFile(fileName, size);
    cout << "Reading ... " << endl;
    for(int i = 0; i < size; i++){
        cout << normalData[i] << endl;
    }


    return (EXIT_SUCCESS);
}

