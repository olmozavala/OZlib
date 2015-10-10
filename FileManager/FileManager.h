/*
 * File:   FileManager.h
 * Author: Olmo Sebastian Zavala Romero 
 * Created on May 21, 2009, 7:19 PM
 */

#ifndef _FileManager_H
#define	_FileManager_H
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>
#include <stdlib.h>

class FileManager {
public:
    FileManager();
    FileManager(const FileManager& orig);

    //Print functions
    static void displayFileData(char* fileName);

    //Write file functions
    static void writeToFile(std::string text, char* fileName, bool append);
    static void writeToFile(char* text, char* fileName, bool append);
    static void writeCommaSeparatedFile(float* data, char* fileName, int width, int height, bool append);

    template<class T>
    static void writeBinaryFile(char* fname, T* values, int length, bool append); 

    //Read file
    static std::string readFile(const char* fileName);
    static char* readFile(const char* fileName, int& m_size);
    static float* readFile(const char* fileName, char* splitby, int& rows, int& cols);

	template<class T>
    static T* readBinFile(char* fileName, int size);
	template<class T>
	static T* readCommaSepFile(char* fileName, int& rows, int& cols);

    virtual ~FileManager();
private:

};

template<class T>
T* FileManager::readBinFile(char* fname, int size) {
    FILE *fptr;
	T* data = (T*)malloc(sizeof(T)*size);

	fptr = fopen(fname, "rb"); //Writes to file

    if (fptr == NULL) {
        fprintf(stderr, "Couldn't open %s\n", fname);
    } else {
        fread(data, sizeof(T), size, fptr);
        fclose(fptr);
    }

	return data;
}

template<class T>
void FileManager::writeBinaryFile(char* fname, T* values, int length, bool append) {
    FILE *fptr;

    if (append)//If append=true then append the text into the file
        fptr = fopen(fname, "ab"); //Writes to file
    else//If not, then rewrite the content of the file
        fptr = fopen(fname, "wb"); //Writes to file

    if (fptr == NULL) {
        fprintf(stderr, "Couldn't open %s\n", fname);
    } else {
        fwrite(&values[0], sizeof (T), length, fptr);
        fclose(fptr);
    }
}

template<class T>
T* FileManager::readCommaSepFile(char* fileName, int& rows, int& cols){
	using namespace std;

    vector<float> vec_data;
	T* finalData;

    ifstream fileData;
    fileData.open(fileName);

    if(!fileData.is_open()){
        cerr<<"-------------- Failed to open file for reading: "<< fileName << endl;
        return 0;
    }

    cols = 0;
    rows = 0;

    string line;
	int tempVal = 0;

    while( fileData.good()) {
        getline( fileData, line);
        istringstream lineStream (line);

        string strVal;
        while( lineStream.good()) {

            getline( lineStream, strVal, ',');
			tempVal = atoi(strVal.c_str());
            vec_data.push_back( (T)tempVal );

            if(rows == 0){
                cols++;//Increments the number of colss
            }
        }
        //cout << endl;
        rows++;
    }

    rows--;//It counts one more always

//    cout << "--------- " << " Rows: " << rows << " Cols: " << cols << endl;

	int buf_size = rows*cols;
    finalData = (T*)malloc(sizeof(int)*buf_size);

    for(int i = 0; i < buf_size; i++){
        finalData[i] = vec_data[i];
//		cout << (float)finalData[i] << endl;
    }

    fileData.close();
	return finalData;
}


#endif	/* _FileManager_H */
