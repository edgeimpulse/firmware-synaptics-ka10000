/***************************************************************************
 *
 * @file eta_m33_nnfunctions.h
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
 * This is part of revision 1.0.0a8 of the Tensai Chip Support Package.
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
 * Title:        eta_m33_nnfunctions.h
 * Description:  Public header file for Eta NN Library
 *
 * Target Processor:  m33
 *
 * v01: Convert type definition
 *  - q7_t, q15_t, q31_t => int8_t, int16_t, int32_t
 * -------------------------------------------------------------------- */


#ifndef _ETA_M33_NNFUNCTIONS_H
#define _ETA_M33_NNFUNCTIONS_H

#include "arm_nnfunctions.h"
#include "eta_nn_types.h"
#include "executor_common.h"

#ifdef __cplusplus
extern    "C"
{
#endif

// arm_status arm_convolve_1x1_s8_fast_test(const cmsis_nn_context *ctx,
//                                     const cmsis_nn_conv_params *conv_params,
//                                     const cmsis_nn_per_channel_quant_params *quant_params,
//                                     const cmsis_nn_dims *input_dims,
//                                     const q7_t *input_data,
//                                     const cmsis_nn_dims *filter_dims,
//                                     const q7_t *filter_data,
//                                     const cmsis_nn_dims *bias_dims,
//                                     const int32_t *bias_data,
//                                     const cmsis_nn_dims *output_dims,
//                                     q7_t *output_data);
// arm_status arm_convolve_wrapper_s8_test(const cmsis_nn_context *ctx,
//                                    const cmsis_nn_conv_params *conv_params,
//                                    const cmsis_nn_per_channel_quant_params *quant_params,
//                                    const cmsis_nn_dims *input_dims,
//                                    const q7_t *input_data,
//                                    const cmsis_nn_dims *filter_dims,
//                                    const q7_t *filter_data,
//                                    const cmsis_nn_dims *bias_dims,
//                                    const int32_t *bias_data,
//                                    const cmsis_nn_dims *output_dims,
//                                    q7_t *output_data);
//     arm_status arm_convolve_s8_test(const cmsis_nn_context *ctx,
//                                const cmsis_nn_conv_params *conv_params,
//                                const cmsis_nn_per_channel_quant_params *quant_params,
//                                const cmsis_nn_dims *input_dims,
//                                const int8_t *input_data,
//                                const cmsis_nn_dims *filter_dims,
//                                const int8_t *filter_data,
//                                const cmsis_nn_dims *bias_dims,
//                                const int32_t *bias_data,
//                                const cmsis_nn_dims *output_dims,
//                                int8_t *output_data);

/**
 * @defgroup NNConv Convolution Functions
 *
 * Collection of convolution, depthwise convolution functions and their variants.
 *
 * The convolution is implemented in 2 steps: im2col and GEMM
 *
 * im2col is a process of converting each patch of image data into
 * a column. After im2col, the convolution is computed as matrix-matrix
 * multiplication.
 *
 * To reduce the memory footprint, the im2col is performed partially.
 * Each iteration, only a few column (i.e., patches) are generated and
 * computed with GEMM kernels similar to CMSIS-DSP arm_mat_mult functions.
 *
 */

/**
   * @brief Basic s8 convolution function

   * @param[in]      input_data     Input (activation) data pointer. Data type: int8 [1, H, W, C_IN]
   * @param[out]     output_data    Output data pointer. Data type: int8 [1, H_OUT, W_OUT, C_OUT]
   * @param[in]      filter_data    Filter data pointer. Data type: int8 [C_OUT, HK, WK, C_IN] where HK and WK are the spatial filter dimensions
   * @param[in]      bias_data      Optional bias data pointer. Data type: int32 [C_OUT]
   * @param[in]      multiplier     Output multiplier data pointer. Data type: int32 [C_OUT]
   * @param[in]      shift          Output shift data pointer. Data type: int32 [C_OUT]

   * @return     The function returns <code>ARM_MATH_SUCCESS</code>
   *
   */
arm_status eta_m33_convolve_s8(
        const int8_t    *input_data,
        int8_t          *output_data,
        const int8_t    *filter_data,
        const int32_t   *bias_data,
        const int32_t   *multiplier,
        const int32_t   *shift,
        const void      *options        // eta_nn_conv_params
        );

/**
   * @brief Get the required buffer size for s8 convolution function
   *
   * @return          The function returns  required buffer size(bytes)
   *
   */
int32_t eta_m33_convolve_s8_get_buffer_size( const void *options );



/**
   * @brief Basic s8 depthwise convolution function.
   *
   * @param[in]      input_data     Input (activation) data pointer. Data type: int8 [1, H, W, C_IN]
   * @param[out]     output_data    Output data pointer. Data type: int8 [1, H, W, C_OUT]
   * @param[in]      filter_data    Filter data pointer. Data type: int8 [1, H, W, C_OUT]
   * @param[in]      bias_data      Bias data pointer. Data type: int32 [C_OUT]
   * @param[in]      multiplier     Output multiplier data pointer. Data type: int32 [C_OUT]
   * @param[in]      shift          Output shift data pointer. Data type: int32 [C_OUT]
   *
   * @return     The function returns <code>ARM_MATH_SUCCESS</code>
   *
   */
arm_status eta_m33_depthwise_conv_s8(
        const int8_t    *input_data,
        int8_t          *output_data,
        const int8_t    *filter_data,
        const int32_t   *bias_data,
        const int32_t   *multiplier,
        const int32_t   *shift,
        const void      *options        // eta_nn_dw_conv_params
        );

/**
   * @brief Get size of additional buffer required by eta_depthwise_conv_wrapper_s8()
   *
   * @return                        Size of additional memory required for optimizations in bytes.
   *
   */
int32_t eta_m33_depthwise_conv_s8_get_buffer_size( const void *options );


/**
   * @brief Basic s8 Fully Connected function.
   *
   * @param[in]      input_data     Input (activation) data pointer. Data type: int8 [N, H * W * C_IN]
   * @param[in, out] output_data    Output data pointer. Data type: int8 [N, C_OUT]
   * @param[in]      filter_data    Filter data pointer. Data type: int8 [H * W * C_IN, C_OUT]
   * @param[in]      bias_data      Bias data pointer. Data type: int32 [C_OUT]
   *
   * @return     The function returns <code>ARM_MATH_SUCCESS</code>
   *
   */
arm_status eta_m33_fully_connected_s8(
        const int8_t    *input_data,
        int8_t          *output_data,
        const int8_t    *filter_data,
        const int32_t   *bias_data,
        const void      *options        // eta_nn_fc_params
        );

/**
   * @brief Get the required buffer size for S8 basic fully-connected and
   * matrix multiplication layer function for TF Lite
   *
   * @return         The function returns    required buffer size in bytes
   *
   */
int32_t eta_m33_fully_connected_s8_get_buffer_size( const void *options );


/**
 * @brief     Q7 vector average pool
 * @param[in]     *input        Pointer to the input vector
 * @param[out]    *output       Pointer to the output vector
 * @param[in]     *opt          Structure containing average pool parameters
 *
 * @return The function returns <code>ETA_STATUS_OK</code>.
 * <b>Scaling and Overflow Behavior:</b>
 * \par
 * The function uses saturating arithmetic.
 * Results outside of the allowable Q7 range [0x80 0x7F] will be saturated.
 */
arm_status eta_m33_quantize_s8(
        const int8_t    *input,
        int8_t          *output,
        const void      *options        // eta_nn_quantize_params
        );

/**
    * @brief s8 element wise add of two vectors
    * @param[in]       input_1_vect            pointer to input vector 1
    * @param[in]       input_2_vect            pointer to input vector 2
    * @param[in,out]   output                  pointer to output vector
    * @return          The function returns    ARM_MATH_SUCCESS
    */
arm_status eta_m33_elementwise_add_s8(
        const int8_t    *input_1_vect,
        const int8_t    *input_2_vect,
        int8_t          *output,
        const void      *options        // eta_nn_elementwise_add_params
        );

/**
    * @brief s8 element wise multiplication
    * @param[in]       input_1_vect            pointer to input vector 1
    * @param[in]       input_2_vect            pointer to input vector 2
    * @param[in,out]   output                  pointer to output vector
    * @return          The function returns    ARM_MATH_SUCCESS
    *
    * @details   Supported framework: TensorFlow Lite micro
    */
arm_status eta_m33_elementwise_mul_s8(
        const int8_t    *input_1_vect,
        const int8_t    *input_2_vect,
        int8_t          *output,
        const void      *options        // eta_nn_elementwise_mul_params
        );

// /**
//     * @brief s8 ReLU6 function
//     * @param[in,out]   data        pointer to input
//     */

// void eta_m33_relu6_s8(
//         int8_t          *data,
//         const void      *options        // eta_nn_size_params
//         );

/**
    * @brief s8 average pooling function.
    *
    * @param[in]      input_data     Input (activation) data pointer. Data type: int8
    * @param[in, out] output_data    Output data pointer. Data type: int8
    * @return                        The function returns
    *                                    <code>ARM_MATH_SUCCESS</code> - Successful operation
    *
    * @details
    *    - Supported Framework: TensorFlow Lite
    *
    */
arm_status eta_m33_avgpool_s8(
        const int8_t    *input_data,
        int8_t          *output_data,
        const void      *options        // eta_nn_pool_params
        );

/**
    * @brief Get the required buffer size for S8 average pooling function
    * @param[in]       dim_dst_width         output tensor dimension
    * @param[in]       ch_src                number of input tensor channels
    * @return          The function returns  required buffer size in bytes
    *
    */
int32_t eta_m33_avgpool_s8_get_buffer_size( const void * options );

/**
    * @brief s8 max pooling function.
    *
    * @param[in]      pool_params    Pooling parameters
    * @param[in]      input_data     Input (activation) data pointer. Data type: int8 [H, W, C_IN]
    * @param[in, out] output_data    Output data pointer. Data type: int8
    * @return                        The function returns
    *                                    <code>ARM_MATH_SUCCESS</code> - Successful operation
    *
    */
arm_status eta_m33_max_pool_s8(
        const int8_t    *input_data,
        int8_t          *output_data,
        const void      *options        // eta_nn_pool_params
        );

/**
    * @brief S8 softmax function
    * @param[in]  input     Pointer to the input tensor
    * @param[out] output    Pointer to the output tensor
    *
    * @note Supported framework: TensorFlow Lite micro (bit-accurate)
    *
    */
void eta_m33_softmax_s8(
        const int8_t    *input,
        int8_t          *output,
        const void      *options        // eta_nn_softmax_params
        );

/**
    * @brief Reshape a s8 vector into another with different shape
    * @param[in]  input      points to the s8 input vector
    * @param[out] output     points to the s8 output vector
    *
    * @note The output is expected to be in a memory area that does not overlap with the input's
    *
    */
void eta_m33_reshape_s8(
        const int8_t    *input,
        int8_t          *output,
        const void      *options        // eta_nn_size_params
        );

/**
    * @brief int8/uint8 concatenation function to be used for concatenating N-tensors along the X axis - Channel
    *        This function should be called for each input tensor to concatenate. The argument offset_x
    *        will be used to store the input tensor in the correct position in the output tensor
    *
    *        i.e.    offset_x = 0
    *                for(i = 0 i < num_input_tensors; ++i)
    *                {
    *                    eta_m33_concatenation_s8_x(&input[i], ..., &output, ..., ..., offset_x)
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
    * @param[in]  input    Pointer to input tensorr
    * @param[out] output   Pointer to output tensor
    * @param[in]  output_x Width of output tensor
    * @param[in]  offset_x The offset (in number of elements) on the X axis to start concatenating the input tensor
    *                      It is user responsibility to provide the correct value
    *
    * <b> Input constraints</b>
    * offset is less than output_len
    *
    */
// concatenate along the channel axis
void eta_m33_concatenation_s8_c(
        const int8_t    *input,
        int8_t          *output,
        const void      *options        // eta_nn_concatenation_params
        );

// concatenate along the width axis
void eta_m33_concatenation_s8_w(
        const int8_t    *input,
        int8_t          *output,
        const void      *options        // eta_nn_concatenation_params
        );

// concatenate along the height axis
void eta_m33_concatenation_s8_h(
        const int8_t    *input,
        int8_t          *output,
        const void      *options        // eta_nn_concatenation_params
        );

// concatenate along the batch axis
void eta_m33_concatenation_s8_n(
        const int8_t    *input,
        int8_t          *output,
        const void      *options        // eta_nn_concatenation_params
        );

/**
     * @brief s8 SVDF function
     *
     * @param[in]   input_data Pointer to input tensor
     * @param[in]   state_data Pointer to state tensor
     * @param[in]   weights_feature_data Pointer to the weights (feature) tensor
     * @param[in]   weights_time_data Pointer to the weights (time) tensor
     * @param[in]   bias_data Pointer to bias tensor
     * @param[out]  output_data Pointer to the output tensor
     *
     * @return     The function returns <code>ARM_MATH_SUCCESS</code>
     *
     */
arm_status eta_m33_svdf_s8(
        const int8_t    *input_data,
        int16_t         *state_data,
        int8_t          *output_data,
        const int8_t    *weights_feature_data,
        const int16_t   *weights_time_data,
        const int32_t   *bias_data,
        const void      *options        // eta_nn_svdf_params
        );

arm_status eta_arm_max_pool_s8(
        const cmsis_nn_context *ctx,
        const cmsis_nn_pool_params *pool_params,
        const cmsis_nn_dims *input_dims,
        const int8_t *src,
        const cmsis_nn_dims *filter_dims,
        const cmsis_nn_dims *output_dims,
        int8_t *dst
        );


// inline void eta_memcpy( uint8_t *d, uint8_t *s, uint32_t len );
inline void eta_m33_memcpy( uint8_t *d, uint8_t *s, uint32_t len )
{
    if ( ( ( (uint32_t)d | (uint32_t)s ) & 3) == 0  &&  (len & 3) != 3 ) {
        memcpy( d, s, len );
    } else {
        while ( len-- > 0 ) {
            // *d++ = *s++;
            uint8_t t;

            __asm volatile(
                "ldrb  %[t], [%[src]], #1\n\t"
                "strb  %[t], [%[dst]], #1\n\t"
                :
                : [ t ] "r"(t), [ src ] "r"(s), [ dst ] "r"(d)
                : "memory" );
        }
    }
}

#ifdef __cplusplus
}
#endif

int eta_m33_prefun_dummy(int8_t *in_array,
                     int8_t * out_array,
                     const int8_t *wt,
                     const int32_t *bias,
                     conv2d_opt *opt0);

int eta_m33_kernel_dummy(int8_t *in_array,
                     int8_t * out_array,
                     const int8_t *wt,
                     const int32_t *bias,
                     conv2d_opt *opt0);

int eta_m33_postfun_dummy(int8_t *in_array,
                     int8_t * out_array,
                     const int8_t *wt,
                     const int32_t *bias,
                     conv2d_opt *opt0);

#endif // _ETA_M33_NNFUNCTIONS_H

/** @}*/

