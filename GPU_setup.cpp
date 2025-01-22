#include "GPU.h"

GPU::GPU()
{
    //--------------------------------Setup---------------------------------------------
    kernelData = Setup();
	printf("GPU SETTING UP!!!\n");
    // Setup_vector_matrix_multiplication();
}

GPU::~GPU()
{
    // Destroy_vector_matrix_multiplication();
    cl_uint ret;
    ret = clReleaseCommandQueue(kernelData.command_queue);
    ret = clReleaseContext(kernelData.context);
}

cl_uint GPU::GetPlatformIndex(cl_platform_id* platforms, cl_uint ret_num_platforms) {

    char* required_platform_subname = (char*)malloc(strlen("Graphics") + 1);
    cl_uint selected_platform_index = 3; //Start at max

    strcpy(required_platform_subname, "Graphics"); //Names as per CapsBasic

    //std::cout << "Reqd name = " << required_platform_subname << std::endl;
    for (cl_uint i = 0; i < ret_num_platforms; ++i)
    {
    cl_int err;

    // Determine platform name length
    size_t platform_name_length = 0;
    err = clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, 0, nullptr, &platform_name_length);
    if (err != CL_SUCCESS) {
        // Handle error
        fprintf(stderr, "Error getting platform name length: %u\n", err);// << std::endl;
        return -1;
    }

        // Get the name itself for the i-th platform
        char* platform_name = new char[platform_name_length];
        clGetPlatformInfo(
            platforms[i],
            CL_PLATFORM_NAME,
            platform_name_length,
            platform_name,
            0
        );

        // decide if this i-th platform is what we are looking for
        // we select the first one matched skipping the next one if any
        if (
            strstr(platform_name, required_platform_subname)
            //&&            selected_platform_index == num_of_platforms // have not selected yet
            )
        {
           // std::cout << " [Selected] " << i << std::endl;
            selected_platform_index = i;
            delete[] platform_name;
            //free(required_platform_subname);
            return selected_platform_index;
            // return the first match
        }

        //        cout << endl;
        //        delete [] platform_name;
    }
    free(required_platform_subname);
    return -1;
}

FileData GPU::LoadFile(const char* path)
{
    FILE* fp;
    char* source_str;

    fp = fopen(path, "r");
    if (!fp) {
        fprintf(stderr, "Failed to load file.\n");
        exit(1);
    }
    fseek(fp, 0, SEEK_END);
    int size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    source_str = (char*)malloc(size + 1);
    int sz = fread(source_str,sizeof(char), size, fp);
    fclose(fp);

    source_str[size - (size - sz)] = 0;

    return { source_str, size - (size - sz) };
}

KernelData GPU::Setup()
{
    cl_device_type platformType = CL_DEVICE_TYPE_GPU;

    printf("Platform: %lu\n", platformType);// << std::endl;//" Matrix size " << SIZE << "x" << SIZE << " Tile size " << TILE_SIZE << std::endl;

    //Init variables
    cl_device_id device_id = NULL;
    cl_uint ret_num_devices;
    cl_uint ret_num_platforms = 0;
    
    //Get number of platforms
    cl_int ret = clGetPlatformIDs(0, nullptr, &ret_num_platforms);


    cl_platform_id* platform_id = new cl_platform_id[ret_num_platforms]; //List of platforms

    printf("Num platforms: %u\n", ret_num_platforms);// << std::endl;

    // Get platform and device information
    ret = clGetPlatformIDs(ret_num_platforms, platform_id, 0); //Returns the list of platforms found. Minimum of arg1 and arg3.

    // std::cout << "clGetPlatformIDs List Ret = " << ret << std::endl;

    cl_uint selected_platform_index = 0;//GetPlatformIndex(platform_id);

    printf("Selected Platform: %u\n", selected_platform_index);// << std::endl;

    cl_platform_id platformCPU = platform_id[selected_platform_index];

    ret = clGetDeviceIDs(platformCPU, platformType, 1, &device_id, &ret_num_devices); //Returns the devices found
    // std::cout << "clGetDeviceIDs " << ret << std::endl;
    // Create an OpenCL context
    //An OpenCL context is created with one or more devices. Contexts are used by the OpenCL runtime for managing objects such as command-queues, memory, program and kernel objects and for executing kernels on one or more devices specified in the context.
    cl_context context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);

    // Create a command queue with the capability of performance profiling for target device
    cl_command_queue command_queue = clCreateCommandQueue(context, device_id, CL_QUEUE_PROFILING_ENABLE, &ret);

    delete[] platform_id;

    return {
        device_id,
        context,
        command_queue,
        selected_platform_index
    };
}

cl_program GPU::BuildFromFile(const char* path, const char* args)
{
    cl_int ret;
    FileData file = LoadFile(path);
    size_t size = file.size;

    cl_program program = clCreateProgramWithSource(kernelData.context, 1, &(file.src), & (size), &ret);
    // Build and compile the OpenCL kernel program
    //std::string build_option = "-DTILE_SIZE=" + std::to_string(TILE_SIZE);
    ret = clBuildProgram(program, 1, &kernelData.device_id, args, NULL, NULL);
    if (ret == CL_BUILD_PROGRAM_FAILURE) { // If compile failed, print the error message
        // Determine the size of the log
        size_t log_size;
        clGetProgramBuildInfo(program, kernelData.device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
        char* log = (char*)malloc(log_size);

        // Get the log and print it
        clGetProgramBuildInfo(program, kernelData.device_id, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);
        printf("%s\n", log);
    }

    free((void*)file.src);
    return program;
}

size_t GPU::GetMaxLocalWorkSize()
{
    cl_int ret;

    cl_uint maxDimensions;
    ret = clGetDeviceInfo(kernelData.device_id, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, sizeof(cl_uint), &maxDimensions, NULL);
    size_t* max_work_sizes = (size_t*)malloc(sizeof(size_t) * maxDimensions);
    ret = clGetDeviceInfo(kernelData.device_id, CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(size_t) * maxDimensions, max_work_sizes, NULL);
    size_t max_work_size = max_work_sizes[0];
    free(max_work_sizes);


    return max_work_size; 
}