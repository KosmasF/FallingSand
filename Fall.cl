// bool isEqual(__global void* A, __private void* B, int size){
//     bool same = true;
//     for(int i = 0; i < size; i++){
//         if(!(*(__global char*)A ^ *(__private char*)B)){
//             same = false;
//         }
//     }  

//     return same;
// }

__kernel void Fall(
    __global void* objects,
    int obejct_size,
    int height,
    int sand,
    int air
){
    int x = get_global_id(0);
    int width = get_global_size(0);

    __global char* below = ((__global char*)objects + (obejct_size * (x + (width*(height-1)))));
    for(int y = height - 1 - 1; y > -1; y--){
        __global char* current =((__global char*)below - (obejct_size * width));
        if(*((__global int*)current) == sand && *((__global int*)below) == air){
            for(int i = 0; i < obejct_size; i++){
                below[i] = current[i];
            }
            *((__global int*)current) = air;

        }
        below = current;
    }
}