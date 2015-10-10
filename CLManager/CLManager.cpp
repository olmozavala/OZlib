/*
 * File:   CLManager.cpp
 * Author: olmozavala
 *
 * Created on September 29, 2011, 4:16 PM
 */

#include "CLManager.h"
#include <GL/glx.h>
#include "debug.h"
#include <stdio.h>

#pragma OPENCL EXTENSION CL_APPLE_gl_sharing : enable 
#pragma OPENCL EXTENSION CL_KHR_gl_sharing : enable

CLManager::CLManager(const CLManager& orig) {
    cout << "---- CLManager copy constructor" << endl;
    _platformList = orig._platformList;
    _context = orig._context;
    _devices = orig._devices;
    _program = orig._program;
    _queue = orig._queue;
}

CLManager::~CLManager() {
    cout << "------ Inside CLManager destructor " << endl;
}

CLManager::CLManager() {
    deviceNum = 0;
}

/**
 * Converts 3 ints into a size_t object with the tree values
 */
cl::size_t < 3 > CLManager::getSizeT(int x, int y, int z) {
    cl::size_t < 3 > origin;
    origin[0] = x;
    origin[1] = y;
    origin[2] = z;
    return origin;
}

/**
 * Initializes an OpenCL context using 
 */
int CLManager::initContext(bool OpenGL) {
	// Select a list of platforms
	cl::Platform::get(&_platformList);
	
    cout << "Assuming the first Device in the list of platforms is the GPU we want to use!" << endl;
    if (OpenGL) {
        cout << "-------------- Using OpenGL for OpenCL context initialization ------" << endl;
        cl_context_properties cprops[] = {
            CL_GL_CONTEXT_KHR, (cl_context_properties) glXGetCurrentContext(),
            CL_GLX_DISPLAY_KHR, (cl_context_properties) glXGetCurrentDisplay(),
            CL_CONTEXT_PLATFORM, (cl_context_properties) (_platformList[0])(),
            0
        };
		
        cout << " Initializing OpenCL context.... " << endl;
        _context = cl::Context(CL_DEVICE_TYPE_GPU, cprops);
		
        // Query the set of devices attched to the context
        cout << " Querying available devices...." << endl;
        _devices = _context.getInfo<CL_CONTEXT_DEVICES > ();
		
        cout << " Done with OpenCL context initalization. " << endl;
		
        size_t size;
        int* result = 0;
        //        clGetGLContextInfoKHR(cprops, CL_DEVICES_FOR_GL_CONTEXT_KHR, 
        //        32 * sizeof (cl_device_id),(void*) result, &size);
    } else {
        //	This initialization is for normal OpenCL without OpenGL
        cl_context_properties cprops[] = {
            CL_CONTEXT_PLATFORM,
            (cl_context_properties) (_platformList[0])(), 0
        };

		
        cout << " Initializing OpenCL context.... " << endl;
        _context = cl::Context(CL_DEVICE_TYPE_GPU, cprops);
		
        // Query the set of devices attched to the context
        cout << " Querying available devices...." << endl;
        _devices = _context.getInfo<CL_CONTEXT_DEVICES > ();
		
        cout << " Done with OpenCL context initalization. " << endl;

    }
		
    return 0;
}

/*
 * Initializes the queue for the selected device num.
 */
void CLManager::initQueue() {
    this->initQueue(deviceNum);
}

/*
 * Initializes the queue for an specific device
 * It also sets the default device number to use.
 */
void CLManager::initQueue(int devNum) {
    deviceNum = deviceNum;
    try {
        cl_command_queue_properties props[] = {
            //					CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE ,
            CL_QUEUE_PROFILING_ENABLE
        };
        //        _queue = cl::CommandQueue(_context, _devices[0], props);
        _queue = cl::CommandQueue(_context, _devices[deviceNum]);
    } catch (cl::Error ex) {
        this->printError(ex);
    }
}

// TODO it should be MULTIPLE no 2 sources :)
/**
 * Adds multiple sources into the OpenCL context. 
 * @param char* fileName1 Name of the first source code
 * @param char* fileName2 Name of the second source code
 */
void CLManager::addMultipleSources(char* fileName1, char* fileName2) {
    try {
        int size1;
        int size2;
        char* kernelSource1 = FileManager::readFile(fileName1, size1);
        char* kernelSource2 = FileManager::readFile(fileName2, size2);
		
        char* kernelSource = new char[size1 + size2];
		
        strcpy(kernelSource, kernelSource1);
        strcat(kernelSource, kernelSource2);
		
        /*
		 cout << kernelSource1 << endl;
		 cout << "***********************************************" << endl;
		 cout << kernelSource2 << endl;
		 cout << "***********************************************" << endl;
		 cout << kernelSource << endl;
		 cout << "************************************************************" << endl;
		 */
		
        //Compiling the kernels
        cl::Program::Sources sources(1, make_pair(kernelSource, size1 + size2));
		
        _program = cl::Program(_context, sources);
        _program.build(_devices);
		
        delete[] kernelSource;
        delete[] kernelSource1;
        delete[] kernelSource2;
    } catch (cl::Error ex) {
        this->printError(ex);
    }
}

/**
 * I left this function so I don't have to fix all my previous codes.
 */
void CLManager::addSource(char* fileName) {
    this->addSource(fileName, (char*)"", -1);
}

/**
 * This function adds a file and compiles it. It recibes 
 * a string of options that are sent when building the file
 */
void CLManager::addSource(char* fileName, char* options, int dev_num) {
	
    vector<cl::Device> build_devices;
	
    if(dev_num < 0){//In this case we copile for all the devices
        build_devices = _devices;
    }else{// In this case we compile only for one device ( 'dev_num' )
        build_devices.assign(0, _devices[dev_num] );
    }
	
    try {
        int size;
        char* kernelSource = FileManager::readFile(fileName, size);
        //		cout << kernelSource << endl;
		
        cl::Program::Sources sources(1, make_pair(kernelSource, size));
		
        _program = cl::Program(_context, sources);
        _program.build(build_devices, options);
		
        delete[] kernelSource;
    } catch (cl::Error ex) {
        this->printError(ex);
    }
}

/**
 * Prints the last error obtained in OpenCL, if any
 */
void CLManager::printError(cl::Error ex) {
    ErrorCodes obj;
	
    eout << "----------------------- ERROR ------------------" << endl;
    eout << "ERROR: " << ex.what() << endl;
    eout << "Specific: " << obj.print_cl_error(ex.err()) << endl;
	
    eout << "Build info: " << _program.getBuildInfo<CL_PROGRAM_BUILD_LOG > (_devices[deviceNum]).c_str() << endl;
    exit(EXIT_FAILURE);
}

/**
 * Obtains the local memory size of the requested device.
 * @param int device The device number needed for the request
 */
int CLManager::getLocalMemSize(int device) {
    return _devices[device].getInfo<CL_DEVICE_LOCAL_MEM_SIZE> ();
}

/**
 * Obtains the mas number of work-items per workgroup
 * @param int device The device number needed for the request
 */
int CLManager::getMaxWorkGroupSize(int device) {
    return _devices[device].getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE > ();
}

/**
 * Prints the most important information about an OpenCL device.
 * @param int device The device number needed for the request
 */
void CLManager::getDeviceInfo(int device) {
    cout << "----------------------- Device Info ------------------" << endl;
	//    cout << "Device type: " << _devices[device].getInfo<CL_DEVICE_TYPE > () << endl;
    cout << "Device name: " << _devices[device].getInfo<CL_DEVICE_NAME > () << endl;
    cout << "Image support: " << _devices[device].getInfo<CL_DEVICE_IMAGE_SUPPORT > ()  << endl;
    cout << "Max parameter size: " << _devices[device].getInfo<CL_DEVICE_MAX_PARAMETER_SIZE > () << endl;
	
    cout << "Max compute units (CUDA multiprocessors): " << _devices[device].getInfo<CL_DEVICE_MAX_COMPUTE_UNITS > () << endl;
    cout << "Max work group size (Threads per block): " << _devices[device].getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE > () << endl;
    cout << "Max work item dimensions: " << _devices[device].getInfo<CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS > () << endl;
    cout << "Max READ image args: " << _devices[device].getInfo<CL_DEVICE_MAX_READ_IMAGE_ARGS > () << endl;
    cout << "Max WRITE image args: " << _devices[device].getInfo<CL_DEVICE_MAX_WRITE_IMAGE_ARGS > () << endl;
	
    cout << "Image 2D max size: " << _devices[device].getInfo<CL_DEVICE_IMAGE2D_MAX_WIDTH > () << " x "
			<< _devices[device].getInfo<CL_DEVICE_IMAGE2D_MAX_HEIGHT > () << endl;
    cout << "Image 3D max size: " << _devices[device].getInfo<CL_DEVICE_IMAGE3D_MAX_WIDTH > () << 
			" x " << _devices[device].getInfo<CL_DEVICE_IMAGE3D_MAX_HEIGHT > () << 
			" x " << _devices[device].getInfo<CL_DEVICE_IMAGE3D_MAX_DEPTH > () << endl;
	
    cout << "Max samplers: " << _devices[device].getInfo<CL_DEVICE_MAX_SAMPLERS > () << endl;
	float locMem = (float)_devices[device].getInfo<CL_DEVICE_LOCAL_MEM_SIZE > ();
    float globMem = (float) _devices[device].getInfo<CL_DEVICE_GLOBAL_MEM_SIZE> ();
    float maxMemAlloc = (float) _devices[device].getInfo<CL_DEVICE_MAX_MEM_ALLOC_SIZE >();
    float maxConstSize = (float) _devices[device].getInfo<CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE>();
	float mb = (float)pow(2,20);
	float gb = (float)pow(2,30);
    cout << "Local memory size: " << (float)locMem/mb << " MBytes (" << locMem << " bytes)" << endl;
    cout << "Global memory size: " << (float)globMem/mb << " MBytes (" << globMem << " bytes)" << endl;
    cout << "Constant memory size: " << (float)maxConstSize/mb << " MBytes (" << maxConstSize << " bytes)" << endl;
    cout << "Max memory allocation size: " << (float)maxMemAlloc/mb << " MBytes (" << maxMemAlloc<< " bytes)" << endl;
	
    cout << "Available extensions: " << _devices[device].getInfo<CL_DEVICE_EXTENSIONS > () << endl;
    //    cout << "Found gl Sharing support: " << _devices[device].getInfo<CL_GL_CONTEXT_KHR> () << endl;
	
}

cl::CommandQueue* CLManager::getQueue() {
    return &_queue;
}

cl::Program* CLManager::getProgram() {
    return &_program;
}

vector<cl::Device>* CLManager::getDevices() {
    return &_devices;
}

cl::Context* CLManager::getContext() {
    return &_context;
}

vector<cl::Platform>* CLManager::getPlatformList() {
    return &_platformList;
}

/**
 * This function obtains the maximum group size on x and y for an specific image size
 * @param maxWorkGroupSize Max warp size 
 * @param rows 		Rows of the image
 * @param cols		Columns of the image
 * @param grp_size_x The group size on x
 * @param grp_size_y The group size on Y
 * @param tot_grps_x The number of groups on x
 * @param tot_grps_y The number of groups on y
 */
void CLManager::getGroupSize(int maxWorkGroupSize, int rows, int cols, int& grp_size_x, int& grp_size_y, int& tot_grps_x, int& tot_grps_y, bool print) {
    grp_size_x = 0;
    grp_size_y = 0;
	
    int maxSizeX = 0;
    int maxSizeY = 0;
    float maxSquare = 0; //Maximum squared value
	
    // Computes the maximum number of threads for each dimension taking into
	// account only the max number of threads per work group
    if (cols == 1) {//This is the case of only one dimension on rows
        maxSizeX = maxWorkGroupSize;
        maxSizeY = 1;
    } else {
        if (rows == 1) {//Only one dimension on columns
            maxSizeY = maxWorkGroupSize;
            maxSizeX = 1;
        } else {
            int exp = 5; // We start with 32 threads per dimension, maybe at some point this will be obsolete 
            maxSizeX = pow(2, exp);// 32
            maxSizeY = pow(2, exp);// 32
            maxSquare = sqrt(maxWorkGroupSize);// if maxworkgroup = 1024 then assigns 32 
            while (maxSizeY > maxSquare) {
                exp--;
                maxSizeY = pow(2, exp);
            }
            exp = 6;
            while (maxSizeX * maxSizeY > maxWorkGroupSize) {
                exp--;
                maxSizeX = pow(2, exp);
            }
        }
    }
	
    // ------- Computes the maximum dimensions for specific problem ---
	// This loop obtains an even split on the 'rows' for each group. 
    for (int i = maxSizeX; i > 0; i--) {
        if (rows % i == 0) {
            grp_size_x = i;
            break;
        }
    }
	
	// This loop obtains an even split on the 'cols' for each group. 
    for (int i = maxSizeY; i > 0; i--) {
        if (cols % i == 0) {
            grp_size_y = i;
            break;
        }
    }
	
    if (grp_size_x > 0 && grp_size_y > 0) {
        tot_grps_x = (int) rows / grp_size_x;
        tot_grps_y = (int) cols / grp_size_y;
    }
	
    if (print) {
        cout << "\n-------- Getting threads and groups sizes -------" << endl;
		
        cout << "Rows and Cols " << rows << " x " << cols << endl;
        cout << "Max Group size " << maxWorkGroupSize << endl;
		cout << "Max Work items x: " << maxSizeX << " y: " << maxSizeY << endl;
        cout << "Grp size x:" << grp_size_x << " y: " << grp_size_y << endl;
        cout << "Tot grps x: " << tot_grps_x <<  "  y:" << tot_grps_y << endl;
    }
}

/**
 * Obtains the best division of a 3D space into the GPU. 
 * It is assumed that none of the dimensions is < 32
 * @param maxWorkGroupSize Max warp size 
 * @param rows 		Rows of the image
 * @param cols		Columns of the image
 * @param grp_size_x The group size on x
 * @param grp_size_y The group size on Y
 * @param tot_grps_x The number of groups on x
 * @param tot_grps_y The number of groups on y
 */
void CLManager::getGroupSize3D(int maxWorkGroupSize, int rows, int cols, int depth,
		int& grp_size_x, int& grp_size_y, int& grp_size_z, 
		int& tot_grps_x, int& tot_grps_y, int& tot_grps_z, bool print) {
	
	//We need this to happen:
	// grp_size_x * tot_grps_x = rows
	// grp_size_y * tot_grps_y = cols
	// grp_size_z * tot_grps_z = depth
	
	// And if the MAX group size is 1024 then grp_size_x*grp_size_y*grp_size_z <= 1024
	// And we would like to have (grp_size_x*grp_size_y*grp_size_z) % 32 = 0
    grp_size_x = 0;
    grp_size_y = 0;
    grp_size_z = 0;
	
	float def_grp_size = pow(maxWorkGroupSize,0.333); // We start with even number of worksizes 
	
    int maxSizeX = (int)def_grp_size;
    int maxSizeY = (int)def_grp_size;
    int maxSizeZ = (int)def_grp_size;
	
	// This loop obtains an even split on the 'rows' for each group. 
    for (int i = maxSizeX; i > 1; i--) {
        if (rows % i == 0) {
            grp_size_x = i;
            break;
        }
    }
	
	// This loop obtains an even split on the 'cols' for each group. 
    for (int i = maxSizeY; i > 0; i--) {
        if (cols % i == 0) {
            grp_size_y = i;
            break;
        }
    }
	
	// This loop obtains an even split on the 'depth' for each group. 
    for (int i = maxSizeZ; i > 0; i--) {
        if (depth% i == 0) {
            grp_size_z = i;
            break;
        }
    }
	
	tot_grps_x = (int) rows / grp_size_x;
	tot_grps_y = (int) cols / grp_size_y;
	tot_grps_z = (int) depth / grp_size_z;
	
    if (print) {
        cout << "\n-------- Getting threads and groups sizes -------" << endl;
		
        cout <<  rows << " x " << cols << " x " << depth << endl;
        cout << "Max Group size " << maxWorkGroupSize << endl;
        cout << "Grp size x:" << grp_size_x << " y: " << grp_size_y << " z: " << grp_size_z << endl;
        cout << "Tot grps x: " << tot_grps_x <<  "  y:" << tot_grps_y << "  z:" << tot_grps_z << endl;
    }
}
