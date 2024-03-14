
//Maximum number of buffers which are simultaneously active at any point of time during infer
#define PRE_ALLOC_MAX_SIMUL_ACTIVE_BUFFS 3

//Total heap size required to fit the model
#define PRE_ALLOC_TOTAL_HEAP_MEMORY_SIZE 61448

//Total number of allocations during infer
#define PRE_ALLOC_MAX_ALLOCS_PER_INFER 68

#ifndef H_WORKQ_COMMON_
// preallocArray[x][0] : allocation size
// preallocArray[x][1] : offset to be allocated from the base address
unsigned int preallocArray[PRE_ALLOC_MAX_ALLOCS_PER_INFER][2] = {
{12288, 0},
{8192, 16384},
{8192, 8192},
{8192, 0},
{8192, 49152},
{49152, 0},
{12288, 49152},
{2048, 24576},
{12288, 12288},
{12288, 0},
{2048, 14336},
{2048, 12288},
{12288, 0},
{3072, 12288},
{512, 6656},
{3072, 3072},
{3072, 0},
{512, 3072},
{512, 6144},
{3072, 3072},
{3072, 0},
{512, 3584},
{512, 3072},
{3072, 0},
{768, 3072},
{128, 1536},
{768, 768},
{768, 0},
{128, 768},
{128, 1664},
{768, 768},
{768, 0},
{128, 768},
{128, 1536},
{768, 768},
{768, 0},
{128, 768},
{128, 0},
{768, 768},
{768, 0},
{256, 3328},
{1536, 1536},
{1536, 0},
{256, 1536},
{256, 3072},
{1536, 1536},
{1536, 0},
{256, 1792},
{256, 1536},
{1536, 0},
{384, 1536},
{64, 832},
{384, 384},
{384, 0},
{64, 384},
{64, 768},
{384, 384},
{384, 0},
{64, 384},
{64, 0},
{384, 384},
{384, 0},
{128, 5120},
{5120, 0},
{32, 5120},
{32, 0},
{8, 32},
{8, 0},
};
#endif