#include "GPU.h"

void GPU::Fall()
{
    cl_int ret;
    cl_program program = BuildFromFile("Fall.cl", "");

    cl_mem objects_buffer = clCreateBuffer(kernelData.context, CL_MEM_READ_WRITE, numObjects * sizeof(Object), nullptr, &ret);

    ret = clEnqueueWriteBuffer(kernelData.command_queue, objects_buffer, CL_TRUE, 0, numObjects * sizeof(Object), objects, 0, nullptr, nullptr);

    int dimensions = 1;
    size_t global_work_size[] = { WIDTH/CELL_SIZE };
    size_t max_local_work_size = GetMaxLocalWorkSize();
    size_t local_work_size[] = { max_local_work_size > global_work_size[0] ? WIDTH/CELL_SIZE : max_local_work_size};

    int object_size = sizeof(Object);
    int height = HEIGHT/CELL_SIZE;
    int sand = Sand;
    int air = Air;

    cl_kernel kernel = clCreateKernel(program, "Fall", &ret);

    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), &objects_buffer);
    ret = clSetKernelArg(kernel, 1, sizeof(int), &object_size);
    ret = clSetKernelArg(kernel, 2, sizeof(int), &height);
    ret = clSetKernelArg(kernel, 3, sizeof(int), &sand);
    ret = clSetKernelArg(kernel, 4, sizeof(int), &air);

    ret = clEnqueueNDRangeKernel(kernelData.command_queue, kernel, dimensions, nullptr, global_work_size, local_work_size, 0, nullptr, nullptr);
    
    ret = clFinish(kernelData.command_queue);

    ret = clEnqueueReadBuffer(kernelData.command_queue, objects_buffer, CL_TRUE, 0, numObjects * sizeof(Object), objects, 0, nullptr, nullptr);

    ret = clReleaseMemObject(objects_buffer);
    ret = clReleaseProgram(program);
    ret = clReleaseKernel(kernel);
}
