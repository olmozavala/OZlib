/* 
 * File:   debug.h
 * Author: andrew
 *
 * Created on January 33, 2012, 1:18 PM
 */

#ifndef DEBUG_H
#define	DEBUG_H

#include <iostream>
#include <string.h>
#ifndef dout 
    #ifdef DEBUG
        #define FLE (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__ )
        #define dout std::cout<<"\033[35m"<<FLE<<": \033[36m"<<__LINE__<<": \033[32m"<<__func__<<": \033[0m"
    #else
        #define dout 0 && std::cout
    #endif
#endif
#ifndef eout 
    //This is for errors and is always defined
    #define FLE (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__ )
    #define eout std::cout<<"\033[35m"<<FLE<<": \033[36m"<<__LINE__<<": \033[31m"<<__func__<<": \033[0m"
#endif
#endif	/* DEBUG_H */
