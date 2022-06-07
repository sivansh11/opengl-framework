#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>
#include <glad/glad.h>

GLenum glCheckError_(const char *file, int line);

#ifndef NDEBUG 
#define glCheckError() glCheckError_(__FILE__, __LINE__)

#define glCall(x) x; glCheckError()  // macro for error checking for opengl

#define ASSERT(condition, message) \
    do { \
        if (! (condition)) { \
            std::cerr << "Assertion `" #condition "` failed in " << __FILE__ \
                      << " line " << __LINE__ << ": " << message << std::endl; \
            std::terminate(); \
        } \
    } while (false)

#define WEAK_ASSERT(condition, message) \
    if (! (condition)) \
    { \
        std::cerr << "Weak Assertion '" #condition "' failed in " << __FILE__\
                << " line " << __LINE__ << ": " << message << std::endl; \
    }

// in release mode, it only displays the message, in debug, it shows the line and filename 
#define RUNTIME_ASSERT(condition, message) \
    do { \
        if (! (condition)) { \
            std::cerr << "Runtime assertion `" #condition "` failed in " << __FILE__ \
                      << " line " << __LINE__ << ": " << message << std::endl; \
            std::terminate(); \
        } \
    } while (false)

#define RUNTIME_WEAK_ASSERT(condition, message) \
    do { \
        if (! (condition)) { \
            std::cerr << "Runtime weak assertion '" #condition "' failed in " << __FILE__\
                << " line " << __LINE__ << ": " << message << std::endl; \
        } \
    } while (false)

#else
#define glCall(x) x;

#define WEAK_ASSERT(condition, message) condition;
#define ASSERT(condition, message) condition;

#define RUNTIME_ASSERT(condition, message) \
    do { \
        if (! (condition)) { \
            std::cerr << "Runtime assertion `" #condition "` failed: " << message << std::endl; \
            std::terminate(); \
        } \
    } while (false)

#define RUNTIME_WEAK_ASSERT(condition, message) \
    do { \
        if (! (condition)) { \
            std::cerr << "Runtime weak assertion '" #condition "' failed in " << __FILE__; \
        } \
    } while (false)

#endif

#endif