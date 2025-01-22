#pragma once
#include <CL/cl.h>

#include "Objects.h"
#include "Units.h"

struct FileData {
    const char* src;
    const int size;
};

struct KernelData {
    cl_device_id device_id;
    cl_context context;
    cl_command_queue command_queue;
    cl_uint selected_platform_index;
};


extern Object* objects;
extern const int numObjects;

class GPU
{
public:
    GPU();
    ~GPU();

    void Fall();

private:
    cl_uint GetPlatformIndex(cl_platform_id* platforms, cl_uint ret_num_platforms);
    FileData LoadFile(const char* path);
    KernelData Setup();
    cl_program BuildFromFile(const char* path, const char* args);
    size_t GetMaxLocalWorkSize();

    KernelData kernelData;
};