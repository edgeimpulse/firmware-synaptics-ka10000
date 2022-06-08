#ifndef	_EXECUTOR_OP_H_
#define	_EXECUTOR_OP_H_

#define M33_OP_BASE (0)
#define M33_OP_END (M33_OP_BASE + 49)

#define NPU_OP_BASE (50)
#define NPU_OP_END (NPU_OP_BASE + 49)

#define CAPE2_OP_BASE (100)
#define CAPE2_OP_END (CAPE2_OP_BASE + 49)

#define LLE_OP_BASE (150)
#define LLE_OP_END (LLE_OP_BASE + 49)

#define VALID_CAPE2_PRE_POST_ID(X)    ((X >= CAPE2_OP_BASE) && (X <= CAPE2_OP_END))
#define VALID_NPU_PRE_POST_ID(X)    ((X >= NPU_OP_BASE) && (X <= NPU_OP_END))

enum {
    M33_KERNEL_START_FROM_HERE = M33_OP_BASE,
    M33_eta_m33_convolve_s8,
    M33_eta_m33_depthwise_conv_s8,
    M33_eta_m33_fully_connected_s8,
    M33_eta_m33_quantize_s8,
    M33_eta_m33_elementwise_add_s8,
    M33_eta_m33_elementwise_mul_s8,
    // M33_eta_m33_relu6_s8,                   // Eta does not support
    M33_eta_m33_avgpool_s8,
    M33_eta_m33_max_pool_s8,
    M33_eta_m33_softmax_s8,
    M33_eta_m33_reshape_s8,
    M33_eta_m33_concatenation_s8_c,
    M33_eta_m33_concatenation_s8_w,
    M33_eta_m33_concatenation_s8_h,
    M33_eta_m33_concatenation_s8_n,
    M33_eta_m33_svdf_s8,
    M33_eta_m33_kernel_dummy,
    M33_end_dummy = M33_OP_END,

    NPU_KERNEL_START_FROM_HERE = NPU_OP_BASE,
    NPU_npu_matrix_test,
    NPU_eta_npu_depthwise_conv_s8,
    NPU_eta_npu_depthwise_conv_3x3_s8,
    NPU_eta_npu_pointwise_conv_s8,
    NPU_eta_npu_conv2d_s8,
    NPU_eta_conv2d_q7_per_channel,
    NPU_prefun_dummy,
    NPU_postfun_dummy,
    NPU_end_dummy = NPU_OP_END,

    CAPE2_KERNEL_START_FROM_HERE = CAPE2_OP_BASE,
    // convolutions
    CAPE2_eta_cape2_convolve_s8,
    CAPE2_eta_cape2_convolve_8_s8,
    CAPE2_eta_cape2_convolve_1x1_s8,
    CAPE2_eta_cape2_convolve_1x1_8_s8,
    CAPE2_eta_cape2_convolve_s8_arm,        // arm wrapper
    // depthwise separable convolutions
    CAPE2_eta_cape2_depthwise_conv_s8,
    CAPE2_eta_cape2_depthwise_conv_4_s8,
    CAPE2_eta_cape2_depthwise_conv_8_s8,
    CAPE2_eta_cape2_depthwise_conv_1_s8,
    CAPE2_eta_cape2_depthwise_conv_1_8_s8,
    CAPE2_eta_cape2_depthwise_conv_s8_arm,  // arm wrapper
    // fully-connected
    CAPE2_eta_cape2_fully_connected_s8,
    CAPE2_eta_cape2_fully_connected_8_s8,
    CAPE2_eta_cape2_fully_connected_s8_arm, // arm wrapper
    // miscellaneous kernels
    CAPE2_eta_cape2_quantize_s8,
    CAPE2_eta_cape2_avgpool_s8,
    // CAPE2_eta_cape2_avgpool_s8_arm,         // arm wrapper
    CAPE2_eta_cape2_max_pool_s8,
    // CAPE2_eta_cape2_max_pool_s8_arm,        // arm wrapper
    // miscellaneous arm wrappers
    CAPE2_eta_cape2_elementwise_add_s8,     
    CAPE2_eta_cape2_elementwise_mul_s8,
    // CAPE2_eta_cape2_relu6_s8,               // Eta does not support
    CAPE2_eta_cape2_softmax_s8,
    CAPE2_eta_cape2_reshape_s8,
    CAPE2_eta_cape2_concatenation_s8_c,
    CAPE2_eta_cape2_concatenation_s8_w,
    CAPE2_eta_cape2_concatenation_s8_h,
    CAPE2_eta_cape2_concatenation_s8_n,
    CAPE2_eta_cape2_svdf_s8,
    CAPE2_prefun_dummy,
    CAPE2_postfun_dummy,
} KernelId_e;
#endif /*_EXECUTOR_OP_H_*/
