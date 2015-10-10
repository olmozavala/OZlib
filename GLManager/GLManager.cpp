/* 
 * File:   GLManager.h
 * Author: olmozavala
 *
 * Created on January 23, 2012, 12:15 PM
 */

#ifndef GLMANANGER_CPP
#define	GLMANANGER_CPP

#include <GL/glew.h>
#include <GL/gl.h>
#include <string>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include "GLManager.h"
#include "debug.h"

/**
 * Creates a shader from a string and a shader type
 * eShaderType:
 *          GL_VERTEX_SHADER,
 *          GL_TESS_CONTROL_SHADER,
 *          GL_TESS_EVALUATION_SHADER,
 *          GL_GEOMETRY_SHADER,
 *          GL_FRAGMENT_SHADER.
 */ 
GLuint GLManager::CreateShader(GLenum eShaderType, const std::string &strShaderFile)
{
    GLuint shader = glCreateShader(eShaderType);//Creates shader object
    const char *strFileData = strShaderFile.c_str();//char* file name
    glShaderSource(shader, 1, &strFileData, NULL);//

    dout << "Compiling shader..." << std::endl;
    glCompileShader(shader);//Compiles the shader

    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)//If it doesn't compile then print the problem
    {
        GLint infoLogLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

        GLchar *strInfoLog = new GLchar[infoLogLength + 1];
        glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);

        const char *strShaderType = NULL;
        switch(eShaderType)
        {
            case GL_VERTEX_SHADER: strShaderType = "vertex"; break;
            case GL_GEOMETRY_SHADER: strShaderType = "geometry"; break;
            case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
        }

        fprintf(stderr, "Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
        delete[] strInfoLog;
    }

    dout << "Done compiling!"<< std::endl;
    return shader;
}

/**
 * Creats a program from a vector of shaders
 */
GLuint GLManager::CreateProgram(const std::vector<GLuint> &shaderList)
{
    GLuint program = glCreateProgram();//Program object

    for(size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
        glAttachShader(program, shaderList[iLoop]);//Atach all the shaders

    glLinkProgram(program);//Link the program

    GLint status;
    glGetProgramiv (program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLint infoLogLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

        GLchar *strInfoLog = new GLchar[infoLogLength + 1];
        glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
        fprintf(stderr, "Linker failure: %s\n", strInfoLog);
        delete[] strInfoLog;
    }

    for(size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
        glDetachShader(program, shaderList[iLoop]);

    return program;
}

/**
 * Creates a buffer of any type. The parameters are as follows:
 * vbo: GLuint& pointer to buffer.
 * vbuffer: void* Pointer to data of the buffer
 * buffSize: int Buffer size
 * 
 * target:  GL_ARRAY_BUFFER,
 * 			GL_ELEMENT_ARRAY_BUFFER,
 * 			GL_PIXEL_PACK_BUFFER,
 * 			GL_PIXEL_UNPACK_BUFFER
 *
 * usage: GL_STREAM_DRAW,
 *        GL_STREAM_READ, 
 *        GL_STREAM_COPY,
 *        GL_STATIC_DRAW, 
 *        GL_STATIC_READ,
 *        GL_STATIC_COPY, 
 *        GL_DYNAMIC_DRAW, 
 *        GL_DYNAMIC_READ, 
 *        GL_DYNAMIC_COPY
 *
 * vAtribIndx: index of the atribute on the shader
 * size: Size of each 'cell' in the buffer. If it is a color it should be 4 RGBA
 * norm: If the values are normalized [0:1]
 * stride: Offset between consecutive genertic vertex attributes
 * ponter: Offset of the first component fo the array
 *
 * type:    GL_BYTE,
 *	    GL_UNSIGNED_BYTE,
 *	    GL_SHORT,
 *	    GL_UNSIGNED_SHORT,
 *	    GL_INT,
 *	    GL_UNSIGNED_INT,
 *	    GL_FLOAT, 
 *      GL_DOUBLE
 */
void GLManager::CreateBuffer(GLuint& vbo,const void* vbuffer, int buffSize,  GLuint target, GLuint usage,
                    GLuint vAtribIndx, GLint size, int norm, GLsizei stride, const GLvoid* pointer, GLenum type)
{
    glGenBuffers(1,&vbo);
    glBindBuffer(target, vbo);

    glBufferData(target, buffSize, vbuffer, usage);

    /*
    using namespace std;
    dout << " Creating buffer with " << "Atrib indx: " << vAtribIndx 
        << " Size: " << size << " Norm: " << norm << " Stride: " 
        << stride << " Pointer: " << pointer << endl;
        */

    glVertexAttribPointer(vAtribIndx, size, type, norm, stride, pointer);//Define where to read the data
    glEnableVertexAttribArray(vAtribIndx);//Enable the vertex atrib array
}

/**
 * Creates an Elemen (index) buffer
 * usage: GL_STREAM_DRAW,
 *        GL_STREAM_READ, 
 *        GL_STREAM_COPY,
 *        GL_STATIC_DRAW, 
 *        GL_STATIC_READ,
 *        GL_STATIC_COPY, 
 *        GL_DYNAMIC_DRAW, 
 *        GL_DYNAMIC_READ, 
 *        GL_DYNAMIC_COPY
 */
void GLManager::CreateElementBuffer(GLuint& vbo,const void* vElementbuffer, int buffSize, GLuint usage)
{
    glGenBuffers(1,&vbo);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, buffSize, vElementbuffer, usage);
}

/**
 * Creates a 2D texture with the specified parameters. The options are:
 *
 * internalFormat, Color components in the texture must be 1,2,3 or 4 or one of the following:
    GL_ALPHA,                          GL_COMPRESSED_ALPHA,             GL_RGB,   
    GL_ALPHA4,                         GL_COMPRESSED_LUMINANCE,         GL_RGB4,  
    GL_ALPHA8,                         GL_COMPRESSED_LUMINANCE_ALPHA,   GL_RGB5,  
    GL_ALPHA12,                        GL_COMPRESSED_INTENSITY,         GL_RGB8,  
    GL_ALPHA16,                        GL_COMPRESSED_RGB,               GL_RGB10,
    GL_LUMINANCE,                      GL_COMPRESSED_RGBA,              GL_RGB12,
    GL_LUMINANCE4,                     GL_DEPTH_COMPONENT,              GL_RGB16,
    GL_LUMINANCE8,                     GL_DEPTH_COMPONENT16,            GL_RGBA,
    GL_LUMINANCE12,                    GL_DEPTH_COMPONENT24,            GL_RGBA2,
    GL_LUMINANCE16,                    GL_DEPTH_COMPONENT32,            GL_RGBA4,
    GL_LUMINANCE_ALPHA,                GL_INTENSITY,                    GL_RGB5_A1,
    GL_LUMINANCE4_ALPHA4,              GL_INTENSITY4,                   GL_RGBA8,
    GL_LUMINANCE6_ALPHA2,              GL_INTENSITY8,                   GL_RGB10_A2,
    GL_LUMINANCE8_ALPHA8,              GL_INTENSITY12,                  GL_RGBA12,
    GL_LUMINANCE12_ALPHA4,             GL_INTENSITY16,                  GL_RGBA16,  
    GL_LUMINANCE12_ALPHA12,            GL_SLUMINANCE,
    GL_LUMINANCE16_ALPHA16,            GL_SLUMINANCE8,
    GL_R3_G3_B2,                       GL_SLUMINANCE_ALPHA,
                                       GL_SLUMINANCE8_ALPHA8,
                                       GL_SRGB,
                                       GL_SRGB8,
                                       GL_SRGB_ALPHA, or 
                                       GL_SRGB8_ALPHA8         
    
                   
    internal format, Specifies the format of the pixel data:
                    GL_COLOR_INDEX,         GL_RGB,
                    GL_RED,                 GL_BGR,
                    GL_GREEN,               GL_RGBA,
                    GL_BLUE,                GL_BGRA,
                    GL_ALPHA,               GL_LUMINANCE, and
                                            GL_LUMINANCE_ALPHA.
    type
    Specifies the data type of the pixel data. The following symbolic values are accepted: 
     GL_UNSIGNED_BYTE,      GL_UNSIGNED_BYTE_3_3_2,         GL_UNSIGNED_SHORT_5_5_5_1,
     GL_BYTE,               GL_UNSIGNED_BYTE_2_3_3_REV,     GL_UNSIGNED_SHORT_1_5_5_5_REV,
     GL_UNSIGNED_SHORT,     GL_UNSIGNED_SHORT_5_6_5,        GL_UNSIGNED_INT_8_8_8_8,
     GL_SHORT,              GL_UNSIGNED_SHORT_5_6_5_REV,    GL_UNSIGNED_INT_8_8_8_8_REV,
     GL_UNSIGNED_INT,       GL_UNSIGNED_SHORT_4_4_4_4,      GL_UNSIGNED_INT_10_10_10_2,
     GL_INT,                GL_UNSIGNED_SHORT_4_4_4_4_REV,  and GL_UNSIGNED_INT_2_10_10_10_REV
     GL_FLOAT,              
                            
    magfilt:
            GL_NEAREST
            GL_LINEAR
            the others need to use mip maps which are not supported on this function
    minfilt 
            GL_NEAREST
            GL_LINEAR
            the others need to use mip maps which are not supported on this function
 */
void GLManager::Create2DTexture(GLuint& txtPtr, void* image, int width, int height, GLenum type,
                                GLint internalFormat, GLfloat magfilter, GLfloat minfilter)
{

    glGenTextures(1, &txtPtr);
    glBindTexture(GL_TEXTURE_2D, txtPtr);

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, GL_BGRA, type, image);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, magfilter);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, minfilter);

    glActiveTexture(GL_TEXTURE0+txtPtr);
}

void GLManager::Debug_gl_callback(unsigned int source, unsigned int type, unsigned int id, unsigned int severity,
			int length, const char* message, void* userParam ){
	char src_str[2048];// Source
	char type_str[2048];// Type
	char sev_str[2048];// Severity

	switch(source){
		case 0x8246:
			strcpy(src_str, "API");
			break;
		case 0x8247:
			strcpy(src_str, "WINDOW_SYSTEM");
			break;
		case 0x8248:
			strcpy(src_str, "SHADER_COMPILER");
			break;
		case 0x8249:
			strcpy(src_str, "THIRD_PARTY");
			break;
		case 0x824A:
			strcpy(src_str, "APPLICATION");
			break;
		case 0x824B:
			strcpy(src_str, "OTHER");
			break;
		default:
			strcpy(src_str, "undefined");
			break;
	}

	switch(type){
		case 0x824C:
			strcpy(type_str, "ERROR");
			break;
		case 0x824D:
			strcpy(type_str, "DEPRECATED_BEHAVIOR");
			break;
		case 0x824E:
			strcpy(type_str, "UNDEFINED_BEHAVIOR");
			break;
		case 0x824F:
			strcpy(type_str, "PORTABILITY");
			break;
		case 0x8250:
			strcpy(type_str, "PERFORMANCE");
			break;
		case 0x8251:
			strcpy(type_str, "OTHER");
			break;
		case 0x8268:
			strcpy(type_str, "MARKER");
			break;
		case 0x8269:
			strcpy(type_str, "PUSH_GROUP");
			break;
		case 0x826A:
			strcpy(type_str, "POP_GROUP");
			break;
		default:
			strcpy(type_str, "undefined");
			break;
	}

	switch(severity){
		case 0x9146:
			strcpy(sev_str, "HIGH");
			break;
		case 0x9147:
			strcpy(sev_str, "MEDIUM");
			break;
		case 0x9148:
			strcpy(sev_str, "LOW");
			break;
		case 0x826B:
			strcpy(sev_str, "NOTIFICATION");
			break;
		default:
			strcpy(sev_str, "undefined");
			break;
	}

	std::cout << "Source: " << src_str << " type: " << type_str << " id:" << id <<
			" Severity" << sev_str << " length:" << length << " message: " << message <<
			" UserParam" << userParam << std::endl;
}

#endif	/* GLMANANGER_CPP */
