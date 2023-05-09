
//Maximum number of buffers which are simultaneously active at any point of time during infer
#define PRE_ALLOC_MAX_SIMUL_ACTIVE_BUFFS 3

//Total heap size required to fit the model
#define PRE_ALLOC_TOTAL_HEAP_MEMORY_SIZE 138248

//Total number of allocations during infer
#define PRE_ALLOC_MAX_ALLOCS_PER_INFER 67

#ifndef H_WORKQ_COMMON_
// preallocArray[x][0] : allocation size
// preallocArray[x][1] : offset to be allocated from the base address
unsigned int preallocArray[PRE_ALLOC_MAX_ALLOCS_PER_INFER][2] = {
{27648, 0},
{36864, 36864},
{36864, 0},
{18432, 110592},
{110592, 0},
{27648, 110592},
{4608, 55296},
{27648, 27648},
{27648, 0},
{4608, 32256},
{4608, 27648},
{27648, 0},
{6912, 27648},
{2304, 34560},
{13824, 13824},
{13824, 0},
{2304, 13824},
{2304, 27648},
{13824, 13824},
{13824, 0},
{2304, 16128},
{2304, 13824},
{13824, 0},
{3456, 13824},
{864, 10368},
{5184, 5184},
{5184, 0},
{864, 5184},
{864, 11232},
{5184, 5184},
{5184, 0},
{864, 5184},
{864, 10368},
{5184, 5184},
{5184, 0},
{864, 5184},
{864, 0},
{5184, 5184},
{5184, 0},
{1152, 14976},
{6912, 6912},
{6912, 0},
{1152, 6912},
{1152, 13824},
{6912, 6912},
{6912, 0},
{1152, 8064},
{1152, 6912},
{6912, 0},
{1728, 6912},
{504, 8640},
{3024, 3024},
{3024, 0},
{504, 3024},
{504, 6048},
{3024, 3024},
{3024, 0},
{504, 3024},
{504, 0},
{3024, 3024},
{3024, 0},
{1008, 11520},
{11520, 0},
{96, 11520},
{96, 0},
{8, 96},
{8, 0},
};
#endif