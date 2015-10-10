/*
 * File:   FileManager.cpp
 * Author: Olmo Sebastian Zavala Romero 
 * This class has several methods to manage data in files
 * Created on May 21, 2009, 7:19 PM
 */

#include "FileManager.h"
#include <fstream>
#include <stdlib.h>

using namespace std;
FileManager::FileManager() 
{
}

FileManager::FileManager(const FileManager& orig) 
{
}

FileManager::~FileManager() 
{
}

/**
 * Displays into the console the data of a file
 */
void FileManager::displayFileData(char* fileName)
{
    ifstream fileObj;
    fileObj.open(fileName);
    //Check if the file can be open, if not we end the method
    if (!fileObj.is_open())
    {
        return;
    }   
    char ch;
    //****************************Read char by char
    while(fileObj.get(ch))
    {
        cout<<ch;//***************Print the char
    }
    fileObj.close();
    //****************************Read word by word
    fileObj.open(fileName);
    char* word = new char[100];
    while(fileObj>>word)
    {//In this case the spaces are not read it
        cout<<word;
        cout<<' ';
    }
    fileObj.close();
}

/**
 * Writes into a file the intput string. The text can be append it to the file
 * or can be rewrited.
 */
void FileManager::writeToFile(string text, char* fileName,bool append)
{
    const char* resultChar = text.c_str();//Change the string into a char*
    FileManager::writeToFile((char*)resultChar,fileName,append);//writes the text into the file
}

void FileManager::writeCommaSeparatedFile(float* data, char* fileName, int width, int height, bool append)
{
    ofstream fileObj;//Stream to file object

    if(append)//If append=true then append the text into the file
        fileObj.open(fileName,ios::app);//Writes to file
    else//If not, then rewrite the content of the file
        fileObj.open(fileName);


    int indx = 0;//Start index of the array. 
    for(int row = 0; row < height; row++){
        for(int col = 0; col < width; col++){
            fileObj<< data[indx];
            fileObj<< ",";
            indx++;
        }
            fileObj<< endl;
    }

    fileObj.close();
}

/**
 *  Writes into a file the intput char*. The text can be append it to the file
 * or can be rewrited.
 */
void FileManager::writeToFile(char* text, char* fileName,bool append)
{
    ofstream fileObj;//Stream to file object
    if(append)//If append=true then append the text into the file
        fileObj.open(fileName,ios::app);//Writes to file
    else//If not, then rewrite the content of the file
        fileObj.open(fileName);
    fileObj<<text;
    fileObj.close();
}

/**
 * Reads a file and returns its content as a string
 */
std::string FileManager::readFile(const char* fileName)
{
    std::ifstream fileObj;
    fileObj.open(fileName);

    //Check if the file can be open, if not we end the method
    if (!fileObj.is_open())
    {
        std::cout<< "Failed to open file: " << fileName << std::endl;
        return NULL;
    }

    std::string str;

    fileObj.seekg(0, std::ios::end);   
    str.reserve(fileObj.tellg());
    fileObj.seekg(0, std::ios::beg);

    str.assign((std::istreambuf_iterator<char>(fileObj)),
                        std::istreambuf_iterator<char>());

    return str;
}

char* FileManager::readFile(const char* fileName, int& m_size){
    ifstream progtxt (fileName, ios::in | ios::ate);
    if(!progtxt.is_open()){
        cerr<<"Failed to open file for reading: "<< fileName << endl;
        return NULL;
    }

    m_size = size_t(progtxt.tellg()) + 1;
    char* m_source = new char[m_size];
    progtxt.seekg( 0, std::ios::beg );
    progtxt.read( m_source, m_size-1 );
    progtxt.close();
    m_source[m_size-1] = '\0';

    return m_source;
}
