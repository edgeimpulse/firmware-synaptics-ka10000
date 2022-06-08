#ifndef _EXECUTOR_COMMON_H_
#define _EXECUTOR_COMMON_H_

#include  "arm_nn_types.h"
#include  "eta_nn_types.h"

#define CONFIG_EXEC_MAX_INPUTS    4
#define CONFIG_EXEC_MAX_SCRATCH   3
#define CONFIG_EXEC_MAX_OUTPUTS   2
#define CONFIG_EXEC_MAX_WEIGHTS   4
#define CONFIG_EXEC_MAX_BIAS  	  4

#ifndef __GNUC__
typedef struct conv2d_relu_avgpool_opt_st
{
  uint16_t in_rows;     // input tensor dimension height
  uint16_t in_cols;     // input tensor dimension width
  uint16_t in_depth;    // number of input tensor channels
  uint16_t num_filt;    // number of filters
  uint16_t bias_lshift; // amount of left-shift for bias
  uint16_t out_rshift;  // amount of right-shift for output
  uint16_t act_min;     // the minimum value of the output range
  uint16_t act_max;     // the maximum value of the output range
} conv2d_relu_avgpool_opt;

#endif
typedef struct conv2d_opt_st
{
  uint16_t in_rows;     /**< input tensor dimension height */
  uint16_t in_cols;     /**< input tensor dimension width */
  uint16_t in_depth;    /**< number of input tensor channels */
  uint16_t num_filt;    /**< number of filters */
  uint16_t filt_rows;   /**< filter kernel size height */
  uint16_t filt_cols;   /**< filter kernel size width */
  uint16_t row_pad;     /**< padding size height */
  uint16_t col_pad;     /**< padding size width */
  uint16_t row_stride;  /**< convolution stride height */
  uint16_t col_stride;  /**< convolution stride width */
  int16_t bias_lshift;  /**< amount of left-shift for bias */
  int16_t out_rshift;   /**< amount of right-shift for output */
  uint16_t out_rows;    /**< output tensor dimension height */
  uint16_t out_cols;    /**< output tensor dimension width */
  int16_t act_min;      /**< the minimum value of the output range */
  int16_t act_max;      /**< the maximum value of the output range */
} conv2d_opt;

typedef  struct
{
  uint16_t offset;
  uint16_t size;
  uint16_t ahbAddrHi;
  uint16_t ahbAddrLo;
} dspShmemBuf;

typedef struct dsp_math_fft_opt_st
{
  uint16_t fft_length;  // input fft length
} tDsp_math_fft_opt;

typedef struct dsp_math_func_opt_st
{
  uint16_t len;  // input length
} tDsp_math_func_opt;

typedef union
{
  cmsis_nn_conv_params    cmsis_conv_params;
  conv2d_opt              conv2d_params;
  eta_nn_conv_params      conv_params;
  eta_nn_dw_conv_params   dw_conv_params;
  eta_nn_fc_params        fc_params;
  eta_nn_quantize_params  quantize_params;
  eta_nn_elementwise_add_params   elem_add_params;
  eta_nn_elementwise_mul_params   elem_mul_params;
  eta_nn_size_params      size_params;
  eta_nn_pool_params      pool_params;
  eta_nn_softmax_params   softmax_params;
  eta_nn_concatenation_params concat_params;
  eta_nn_svdf_params      svdf_params;
  eta_nn_npu_conv2d_params    nn_conv2d_params;
  eta_nn_npu_dw_conv_params   nn_dw_conv_params;
  eta_nn_npu_pw_conv_params   nn_pw_conv_params;
} tExecutorRpcParams;

#ifdef _WIN32
typedef struct {
  uint8_t status;
  uint8_t execHwId;
  void* inbufs[CONFIG_EXEC_MAX_INPUTS];
  void* outbufs[CONFIG_EXEC_MAX_OUTPUTS];
  void* scratchbufs[CONFIG_EXEC_MAX_SCRATCH];
  void* weights[CONFIG_EXEC_MAX_WEIGHTS];
  void* bias[CONFIG_EXEC_MAX_BIAS];
  uint32_t preOp;
  uint32_t postOp;
  tExecutorRpcParams params;
} tExecutorRpcWork;
#else
typedef struct {
  uint8_t execHwId;
  uint8_t status;
  uint32_t inbufs[CONFIG_EXEC_MAX_INPUTS];
  uint32_t outbufs[CONFIG_EXEC_MAX_OUTPUTS];
  uint32_t scratchbufs[CONFIG_EXEC_MAX_SCRATCH];
  uint32_t weights[CONFIG_EXEC_MAX_WEIGHTS];
  uint32_t bias[CONFIG_EXEC_MAX_BIAS];
  uint32_t preOp;
  uint32_t postOp;
  tExecutorRpcParams params;
} tExecutorRpcWork;
#endif //_WIN32

#endif /*_EXECUTOR_COMMON_H_*/
