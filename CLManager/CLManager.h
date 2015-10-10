/*
 * File:   CLManager.h
 * Author: olmozavala
 *
 * Created on September 29, 2011, 4:16 PM
 */
#define __CL_ENABLE_EXCEPTIONS

#ifndef CLMANAGER_H
#define	CLMANAGER_H

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <math.h>
#include <sstream>
#include <CL/cl.hpp>

#include "ErrorCodes.h"
#include "FileManager/FileManager.h"

using namespace std;

/**
 * This class is a small wrapper over OpenCL 1.2 to
 * avoid coding repetitive tasks.
 */ 
class CLManager {
public:
    CLManager();
    CLManager(const CLManager& orig);
    virtual ~CLManager();
    void getDeviceInfo(int device);
	void addMultipleSources(char* fileName1, char* fileName2);
    void addSource(char* fileName, char* options, int dev_num);
    void addSource(char* fileName);
    /**
     * Initializes the queue for device 0 (I only have it because
     * a lot of my previous code use it in this way
     */
    void initQueue();
    /**
     * Initializes the queue for an specific device
     */
    void initQueue(int deviceNum);
    /**
     * Initializes the context for using OpenGL or not.
     * It returns the number of devices found.
     */
	int initContext(bool OpenGL);
    void printError(cl::Error ex);
	int getMaxWorkGroupSize(int device);
	int getLocalMemSize(int device);

    static void getGroupSize(int maxSize, int rows, int cols, int& grp_size_x, int& grp_size_y, int& tot_grps_x, int& tot_grps_y, bool print);
	static void getGroupSize3D(int maxWorkGroupSize, int rows, int cols, int depth, 
						int& grp_size_x, int& grp_size_y, int& grp_size_z, 
						int& tot_grps_x, int& tot_grps_y, int& tot_grps_z, bool print);
    static cl::size_t < 3 > getSizeT(int x, int y, int z);

    cl::CommandQueue* getQueue();
    cl::Program* getProgram();
    vector<cl::Device>* getDevices();
    cl::Context* getContext();
    vector<cl::Platform>* getPlatformList();


private:
    vector<cl::Platform> _platformList;
    cl::Context _context;
    vector<cl::Device> _devices;
    cl::Program _program;
    cl::CommandQueue _queue;

    int deviceNum;//Device number to use, by default is 0
};

#endif	/* CLMANAGER_H */

