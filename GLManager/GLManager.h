
/* 
 * File:   GLManager.h
 * Author: Olmo Zavala Romero
 *
 */
#ifndef GLMANAGER_H
#define	GLMANAGER_H

#include <GL/gl.h>
#include <string>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <stdlib.h>

class GLManager {
public:
    GLManager();
    GLManager(const GLManager& orig);
    virtual ~GLManager();
    
    static GLuint CreateShader(GLenum eShaderType, const std::string &strShaderFile);
    static GLuint CreateProgram(const std::vector<GLuint> &shaderList);

    //All these next functions are used to create vertex buffers
    static void CreateBuffer(GLuint& vbo,const void* vbuffer,int buffSize, GLuint target, GLuint usage,
                    GLuint vAtribIndx, GLint size, int norm, GLsizei stride, const GLvoid* pointer, GLenum type);

    static void CreateElementBuffer(GLuint& vbo,const void* vElementbuffer, int buffSize, GLuint usage);

    //Function to create Textures
    static void Create2DTexture(GLuint& txtPtr, void* image, int width, int height, GLenum type,
                                GLint internalFormat, GLfloat magfilter, GLfloat minfilt);

	static void Debug_gl_callback(unsigned int source, unsigned int type, unsigned int id, unsigned int severity,
			int length, const char* message, void* userParam );

private:

};

#endif 
