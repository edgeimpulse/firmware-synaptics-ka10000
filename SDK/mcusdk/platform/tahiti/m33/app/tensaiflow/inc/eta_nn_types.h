/***************************************************************************
 *
 * @file eta_nn_types.h
 *
 * @brief This file contains eta_m33 nnfunction definitions.
 *
 * Copyright (C) 2021 Eta Compute, Inc
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This is part of the Tensai Chip Support Package.
 *
 ******************************************************************************/

/***************************************************************************
 * @addtogroup 
 * @addtogroup 
 *
 * @defgroup 
 * @defgroup 
 *
 * @ingroup 
 * @ingroup 
 * @{
 ******************************************************************************/

/* ----------------------------------------------------------------------
 * Title:        eta_nn_types.h
 * Description:  Public header file for Eta NN Library
 *
 * Target Processor:  m33, CAPE2, LLE, NPU
 *
 * v01: Convert type definition
 *  - q7_t, q15_t, q31_t => int8_t, int16_t, int32_t
 * -------------------------------------------------------------------- */


#ifndef _ETA_NN_TYPES_H
#define _ETA_NN_TYPES_H

#include <stdint.h>
#include "arm_nn_types.h"

#if 0
/** Eta-NN common object for the Traditional/Depthwise convolution layer parameters */
/*
   * @param[in]      input_dims     Input (activation) tensor dimensions. Format: [N, H, W, C_IN]
   * @param[in]      filter_dims    Filter tensor dimensions. Format: [C_OUT, HK, WK, C_IN] where HK and WK are the spatial filter dimensions
   * @param[in]      bias_dims      Bias tensor dimensions. Format: [C_OUT]
   * @param[in]      output_dims    Output tensor dimensions. Format: [N, H, W, C_OUT]
*/
typedef struct
{
    int32_t         input_offset;  /**< Zero value for the input tensor : [-127, 128] */
    int32_t         output_offset; /**< Zero value for the output tensor : [-127, 128] */
    int32_t         ch_mult;       /**< Channel Multiplier. ch_mult * in_ch = out_ch */
    cmsis_nn_tile   stride;
    cmsis_nn_tile   padding;
    // cmsis_nn_tile   dilation;
    cmsis_nn_activation activation;

    // cmsis_nn_context    ctx;
    // cmsis_nn_per_channel_quant_params   quant_params;
    cmsis_nn_dims   input_dims;
    cmsis_nn_dims   output_dims;
    cmsis_nn_dims   filter_dims;
} eta_nn_dsp_conv_params;
#endif  // 0/1



/** Eta-NN object for the convolution layer parameters */
/*
   * @param[in]      input_dims     Input (activation) tensor dimensions. Format: [N, H, W, C_IN]
   * @param[in]      filter_dims    Filter tensor dimensions. Format: [C_OUT, HK, WK, C_IN] where HK and WK are the spatial filter dimensions
   * @param[in]      bias_dims      Bias tensor dimensions. Format: [C_OUT]
   * @param[in]      output_dims    Output tensor dimensions. Format: [N, H, W, C_OUT]
*/
typedef struct
{
    int32_t         input_offset;  /**< Zero value for the input tensor  : [-127, 128] */
    int32_t         output_offset; /**< Zero value for the output tensor : [-127, 128] */
    cmsis_nn_tile   stride;
    cmsis_nn_tile   padding;
    cmsis_nn_tile   dilation;
    cmsis_nn_activation activation;

    // cmsis_nn_context    ctx;
    // cmsis_nn_per_channel_quant_params   quant_params;
    cmsis_nn_dims   input_dims;
    cmsis_nn_dims   output_dims;
    cmsis_nn_dims   filter_dims;
} eta_nn_conv_params;

typedef struct
{
    int32_t         input_offset;  /**< Zero value for the input tensor : [-127, 128] */
    int32_t         output_offset; /**< Zero value for the output tensor : [-127, 128] */
    cmsis_nn_tile   stride;
    cmsis_nn_tile   padding;
    cmsis_nn_activation activation;

    cmsis_nn_dims   input_dims;
    cmsis_nn_dims   output_dims;
    cmsis_nn_dims   filter_dims;
} eta_nn_npu_conv2d_params;

typedef struct
{
    int32_t         input_offset;  /**< Zero value for the input tensor : [-127, 128] */
    int32_t         output_offset; /**< Zero value for the output tensor : [-127, 128] */
    cmsis_nn_activation activation;

    cmsis_nn_dims   input_dims;
    cmsis_nn_dims   output_dims;
    cmsis_nn_dims   filter_dims;
} eta_nn_npu_pw_conv_params;

/** Eta-NN object for Depthwise convolution layer parameters */
/*
   * @param[in]      input_dims     Input (activation) tensor dimensions. Format: [H, W, C_IN]
   * @param[in]      filter_dims    Filter tensor dimensions. Format: [1, HK, WK, C_OUT] where HK and WK are the spatial filter dimensions
   * @param[in]      bias_dims      Bias tensor dimensions. Format: [C_OUT]
   * @param[in]      output_dims    Output tensor dimensions. Format: [1, H_OUT, W_OUT, C_OUT]
*/
typedef struct
{
    int32_t         input_offset;  /**< Zero value for the input tensor : [-127, 128] */
    int32_t         output_offset; /**< Zero value for the output tensor : [-127, 128] */
    int32_t         ch_mult;       /**< Channel Multiplier. ch_mult * in_ch = out_ch */
    cmsis_nn_tile   stride;
    cmsis_nn_tile   padding;
    cmsis_nn_tile   dilation;
    cmsis_nn_activation activation;

    // cmsis_nn_context    ctx;
    // cmsis_nn_per_channel_quant_params   quant_params;
    cmsis_nn_dims   input_dims;
    cmsis_nn_dims   output_dims;
    cmsis_nn_dims   filter_dims;
} eta_nn_dw_conv_params;

typedef struct
{
    int32_t         input_offset;  /**< Zero value for the input tensor : [-127, 128] */
    int32_t         output_offset; /**< Zero value for the output tensor : [-127, 128] */
    int32_t         ch_mult;       /**< Channel Multiplier. ch_mult * in_ch = out_ch */
    cmsis_nn_tile   stride;
    cmsis_nn_tile   padding;
    cmsis_nn_activation activation;

    cmsis_nn_dims   input_dims;
    cmsis_nn_dims   output_dims;
    cmsis_nn_dims   filter_dims;
} eta_nn_npu_dw_conv_params;


/** CMSIS-NN object for Fully Connected layer parameters */
/*
    * @param[in]      quant_params   Per-tensor quantization info.
    *                                It contains the multiplier and shift values to be applied to the output tensor.
    * @param[in]      input_dims     Input (activation) tensor dimensions. Format: [N, H, W, C_IN]
    *                                Input dimension is taken as Nx(H * W * C_IN)
    * @param[in]      filter_dims    Two dimensional filter dimensions. Format: [N, C]
    *                                N : accumulation depth and equals (H * W * C_IN) from input_dims
    *                                C : output depth and equals C_OUT in output_dims
    *                                H & W : Not used
    * @param[in]      output_dims    Output tensor dimensions. Format: [N, C_OUT]
    *                                N : Batches
    *                                C_OUT : Output depth
    *                                H & W : Not used.
*/
typedef struct
{
    int32_t         input_offset;  /**< Zero value for the input tensor */
    int32_t         filter_offset; /**< Zero value for the filter tensor */
    int32_t         output_offset; /**< Zero value for the output tensor */
    cmsis_nn_activation activation;

    // cmsis_nn_context    ctx;
    cmsis_nn_per_tensor_quant_params    quant_params;
    cmsis_nn_dims   input_dims;
    cmsis_nn_dims   output_dims;
    cmsis_nn_dims   filter_dims;
} eta_nn_fc_params;


/**
    * @brief s8 requantize according to new scale and zero-point
    * @param[in]       input_offset         offset for input 1. Range: Range: -127 to 128
    * @param[in]       output_offset        output offset
    * @param[in]       multiplier           output multiplier
    * @param[in]       shift                output shift
    * @param[in]       size                 number of samples
    * @return          The function returns    ARM_MATH_SUCCESS
    */
typedef struct
{
    int32_t     input_offset;  /**< Zero value for the input tensor */
    int32_t     output_offset; /**< Zero value for the output tensor */
    int32_t     multiplier;
    int32_t     shift;
    uint32_t    size;
} eta_nn_quantize_params;


/**
    * @brief s8 element wise add of two vectors
    * @param[in]       input_1_offset          offset for input 1. Range: Range: -127 to 128
    * @param[in]       input_1_mult            multiplier for input 1
    * @param[in]       input_1_shift           shift for input 1
    * @param[in]       input_2_offset          offset for input 2. Range: Range: -127 to 128
    * @param[in]       input_2_mult            multiplier for input 2
    * @param[in]       input_2_shift           shift for input 2
    * @param[in]       left_shift              input left shift
    * @param[in]       out_offset              output offset
    * @param[in]       out_mult                output multiplier
    * @param[in]       out_shift               output shift
    * @param[in]       out_activation_min      minimum value to clamp output to
    * @param[in]       out_activation_max      maximum value to clamp output to
    * @param[in]       block_size              number of samples
    * @return          The function returns    ARM_MATH_SUCCESS
    */
typedef struct
{
    int32_t input_1_offset;
    int32_t input_1_mult;
    int32_t input_1_shift;
    int32_t input_2_offset;
    int32_t input_2_mult;
    int32_t input_2_shift;
    int32_t left_shift;
    int32_t out_offset;
    int32_t out_mult;
    int32_t out_shift;
    cmsis_nn_activation activation;
    uint32_t block_size;
} eta_nn_elementwise_add_params;

/**
    * @brief s8 element wise multiplication
    * @param[in]       input_1_offset          offset for input 1. Range: Range: -127 to 128
    * @param[in]       input_2_offset          offset for input 2. Range: Range: -127 to 128
    * @param[in]       out_offset              output offset
    * @param[in]       out_mult                output multiplier
    * @param[in]       out_shift               output shift
    * @param[in]       out_activation_min      minimum value to clamp output to
    * @param[in]       out_activation_max      maximum value to clamp output to
    * @param[in]       block_size              number of samples
    * @return          The function returns    ARM_MATH_SUCCESS
    *
    * @details   Supported framework: TensorFlow Lite micro
    */
typedef struct
{
    int32_t input_1_offset;
    int32_t input_2_offset;
    int32_t out_offset;
    int32_t out_mult;
    int32_t out_shift;
    cmsis_nn_activation activation;
    uint32_t block_size;
} eta_nn_elementwise_mul_params;

/**
    * @brief s8 ReLU6 function
    * @param[in]       size        number of elements
    */
typedef struct
{
    uint32_t    size;
} eta_nn_size_params;

/** CMSIS-NN object for pooling layer parameters */
/**
    * @brief s8 average pooling function.
    *
    * @param[in]      input_dims     Input (activation) tensor dimensions. Format: [H, W, C_IN]
    *                                Argument 'N' is not used.
    * @param[in]      filter_dims    Filter tensor dimensions. Format: [H, W]
    *                                Argument N and C are not used.
    * @param[in]      output_dims    Output tensor dimensions. Format: [H, W, C_OUT]
    *                                Argument N is not used.
    *                                C_OUT equals C_IN.
    * @return                        The function returns
    *                                    <code>ARM_MATH_SUCCESS</code> - Successful operation
    *
    * @details
    *    - Supported Framework: TensorFlow Lite
    *
    */
typedef struct
{
    cmsis_nn_tile   stride;
    cmsis_nn_tile   padding;
    cmsis_nn_activation activation;

    // cmsis_nn_context    ctx;
    cmsis_nn_dims   input_dims;
    cmsis_nn_dims   output_dims;
    cmsis_nn_dims   filter_dims;
} eta_nn_pool_params;

/**
    * @brief S8 softmax function
    *
    * @param[in]  num_rows  Number of rows in the input tensor
    * @param[in]  row_size  Number of elements in each input row
    * @param[in]  mult      Input quantization multiplier
    * @param[in]  shift     Input quantization shift within the range [0, 31]
    * @param[in]  diff_min  Minimum difference with max in row. Used to check if
    *                       the quantized exponential operation can be performed
    *
    * @note Supported framework: TensorFlow Lite micro (bit-accurate)
    *
    */
typedef struct
{
    int32_t num_rows;
    int32_t row_size;
    int32_t mult;
    int32_t shift;
    int32_t diff_min;
} eta_nn_softmax_params;


/**
    * @brief int8/uint8 concatenation function to be used for concatenating N-tensors along the X axis
    *        This function should be called for each input tensor to concatenate. The argument offset_x
    *        will be used to store the input tensor in the correct position in the output tensor
    *
    *        i.e.    offset_x = 0
    *                for(i = 0 i < num_input_tensors; ++i)
    *                {
    *                    arm_concatenation_s8_x(&input[i], ..., &output, ..., ..., offset_x)
    *                    offset_x += input_x[i]
    *                }
    *
    *        This function assumes that the output tensor has:
    *        -# The same height of the input tensor
    *        -# The same number of channels of the input tensor
    *        -# The same batch size of the input tensor
    *
    *        Unless specified otherwise, arguments are mandatory.
    *
    * @note This function, data layout independent, can be used to concatenate either int8 or uint8 tensors because it
    *      does not involve any arithmetic operation
    *
    * @param[in]  input_dims.w  Width of input tensor
    * @param[in]  input_dims.h  Height of input tensor
    * @param[in]  input_dims.c  Channels in input tensor
    * @param[in]  input_dims.n  Batch size in input tensor
    * @param[in]  output_len Width of output tensor
    * @param[in]  offset   The offset (in number of elements) on the X axis to start concatenating the input tensor
    *                      It is user responsibility to provide the correct value
    *
    * <b> Input constraints</b>
    * offset_x is less than output_x
    *
    */
typedef struct
{
    cmsis_nn_dims   input_dims;
    uint16_t        output_len;
    uint32_t        offset;
} eta_nn_concatenation_params;

/** CMSIS-NN object for SVDF layer parameters */
/**
    * @brief s8 SVDF function
    *
    * @param[in]   input_ctx Temporary scratch buffer
    * @param[in]   output_ctx Temporary output scratch buffer
    * @param[in]   input_quant_params Input quantization parameters
    * @param[in]   output_quant_params Output quantization parameters
    * @param[in]   input_dims Input tensor dimensions
    * @param[in]   state_dims State tensor dimensions
    * @param[in]   weights_feature_dims Weights (feature) tensor dimensions
    * @param[in]   weights_time_dims Weights (time) tensor dimensions
    * @param[in]   output_dims Output tensor dimensions
    *
    * @return     The function returns <code>ARM_MATH_SUCCESS</code>
    *
    */
typedef struct
{
    int32_t rank;
    int32_t input_offset;  /**< Zero value for the input tensor */
    int32_t output_offset; /**< Zero value for the output tensor */
    cmsis_nn_activation input_activation;
    cmsis_nn_activation output_activation;

    cmsis_nn_context    input_ctx;
    cmsis_nn_context    output_ctx;
    cmsis_nn_per_tensor_quant_params    input_quant_params;
    cmsis_nn_per_tensor_quant_params    output_quant_params;
    cmsis_nn_dims   input_dims;
    cmsis_nn_dims   state_dims;
    cmsis_nn_dims   weights_feature_dims;
    cmsis_nn_dims   weights_time_dims;
    cmsis_nn_dims   output_dims;
} eta_nn_svdf_params;

#endif // _ETA_NN_TYPES_H

