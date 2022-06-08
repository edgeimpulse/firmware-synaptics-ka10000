////////////////////////////////////////////////////////////////////////////////
//
// NDA AND NEED-TO-KNOW REQUIRED
//
// Copyright (C) 2021 Synaptics Incorporated. All rights reserved.
//
// This file contains information that is proprietary to Synaptics
// Incorporated ("Synaptics"). The holder of this file shall treat all
// information contained herein as confidential, shall use the
// information only for its intended purpose, and shall not duplicate,
// disclose, or disseminate any of this information in any manner
// unless Synaptics has otherwise provided express, written
// permission.
//
// Use of the materials may require a license of intellectual property
// from a third party or from Synaptics. This file conveys no express
// or implied licenses to any intellectual property rights belonging
// to Synaptics.
//
// INFORMATION CONTAINED IN THIS DOCUMENT IS PROVIDED "AS-IS," AND
// SYNAPTICS EXPRESSLY DISCLAIMS ALL EXPRESS AND IMPLIED WARRANTIES,
// INCLUDING ANY IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE, AND ANY WARRANTIES OF NON-INFRINGEMENT OF ANY
// INTELLECTUAL PROPERTY RIGHTS. IN NO EVENT SHALL SYNAPTICS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, PUNITIVE, OR
// CONSEQUENTIAL DAMAGES ARISING OUT OF OR IN CONNECTION WITH THE USE
// OF THE INFORMATION CONTAINED IN THIS DOCUMENT, HOWEVER CAUSED AND
// BASED ON ANY THEORY OF LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// NEGLIGENCE OR OTHER TORTIOUS ACTION, AND EVEN IF SYNAPTICS WAS
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. IF A TRIBUNAL OF
// COMPETENT JURISDICTION DOES NOT PERMIT THE DISCLAIMER OF DIRECT
// DAMAGES OR ANY OTHER DAMAGES, SYNAPTICS' TOTAL CUMULATIVE LIABILITY
// TO ANY PARTY SHALL NOT EXCEED ONE HUNDRED U.S. DOLLARS.
//
////////////////////////////////////////////////////////////////////////////////

/* ---------------------------------------------------------------------------------------------
	Below Macro 'PRINT_LEVEL_SET', Should be first line in this .c file, before any includes.
		Valid options are:
			> ASSERT_DISABLE
			> ASSERT_LEVEL_ERR
			> ASSERT_LEVEL_WARN
			> ASSERT_LEVEL_INFO
			> ASSERT_LEVEL_ALL
*/
#define PRINT_LEVEL_SET ASSERT_LEVEL_DBG
/* --------------------------------------------------------------------------------------------- */
#ifndef __DOMAIN_NS
#define __DOMAIN_NS
#endif

#include "mcu.h"
#include "driver.h"
#include "usr_demo.h"
#if (defined(MCU_SDK_ADC_DEMO) && (MCU_SDK_ADC_DEMO == 1))
#include "adc_drv.h"
#include "adma_drv.h"

os_SemHandle	adma_notify;
os_TaskHandle   adc_task_handle;


void user_callback(void *msg)
{
	//This callback will be called every 1 ms to
	//indicate 1 ms data is ready in ring buffer
	os_SemGive(adma_notify);
}

int adc_demo_init_dmic(void)
{
	int ret = 0;
	//setup DMIC0-L/DMIC1-R (Sample-rate 16K, Stereo, DMIC clock==1.024MHz) as an example
	//This DMIC is already installed on Katana EVK

	//step1 : config DMIC/ADC
	mic_cfg_u mic_config;
	mic_config.dmic.channel                  = ADC_PAR_CH_0 | ADC_PAR_CH_3; //DMIC0 LEFT + DMIC1 RIGHT 
	mic_config.dmic.low_high_perf_filter     = ADC_CFG_LOW_PERF_FILTER; 
	mic_config.dmic.micbias_output_voltage   = ADC_MICBIAS_OUPUT_1P8V; 
	mic_config.dmic.control_by_sync_module   = 0; //Not Control by sync
	mic_config.dmic.input_phase              = 1; 
	mic_config.dmic.output_swap              = 0; 
	mic_config.dmic.output_polary            = 0; 
	mic_config.dmic.micbias_index            = 0; 
	ret = adc_set_config(MIC_TYPE_ALL_DMIC, (mic_cfg_u*)&mic_config, ADC_DMIC_CONFIG);

	//#step2 : config CH Gains
	//DMIC0 left CH and DMIC1 right CH
	adc_vol_t adc_vol                       = {0}; 
	adc_vol.channels                        = ADC_PAR_CH_0 | ADC_PAR_CH_3; 
	adc_vol.format                          = ADC_PAR_VOL_FORMAT_Q_8_23;
	adc_vol.vol_gain                        = 0x0; //0.0 as q8_23
	adc_vol.boost_gain                      = 0x06000000;  //12.0 as q8_23
	adc_vol.balance_gain                    = 0x01800000; //3.0 as q8_23
	ret = adc_set_vol((adc_vol_t *)&adc_vol, 0);

	//#step3 : config HPF (high pass filter)
	adc_hpf_cfg_t  hpf_config; 
	hpf_config.channel  = ADC_PAR_CH_0 | ADC_PAR_CH_3;
	hpf_config.k1       = 7; 
	hpf_config.k2       = 8; 
	hpf_config.order    = 0; 
	ret = adc_hpf_set_config((adc_hpf_cfg_t*)&hpf_config, 0);

	//#step4 :enabble ADC 
	ret = adc_set_enable(MIC_TYPE_ALL_DMIC, ADC_PAR_CH_0 | ADC_PAR_CH_3, 16000, 1, 0, ADC_FLAGS_AHEAD_OF_DECIMATOR);
	ret = adc_set_enable(MIC_TYPE_ALL_DMIC, ADC_PAR_CH_0 | ADC_PAR_CH_3, 16000, 1, 0, ADC_FLAGS_DECIMATOR);
	return ret;
}

#define BUF_DW_SIZE  (256)
#define ADMA_CH_NUM   (2)
uint32_t data_buf[BUF_DW_SIZE];

int adc_demo_init_adma(void)
{
	//Setup ADMA for the two DMICs in M33 side.
	//ADMA moves Audio samples into ring buffer as interleaved stereo.
	//Each audio sample width is 24 bit and store as 32bit integer in memory.

	//Step 1 :config ADMA channels for DMIC0 (one Stereo)
	int ret = 0;
	adma_channel_ctrl  ch_ctrl         = {0}; 
	ch_ctrl.buffer_size                = BUF_DW_SIZE/ADMA_CH_NUM; //count by samples
	ch_ctrl.ctrl.bf.samp_size          = ADMA_SAMPLE_SIZE_32D;
	ch_ctrl.ctrl.bf.step_ofst          = 1 ; //interleave 
	ch_ctrl.ctrl.bf.address_direction  = ADMA_ADDR_DIR_INCR;
	ch_ctrl.ctrl.bf.burst_size         = ADMA_BURST_SIZE_1;
	ch_ctrl.ctrl.bf.block_interrupt    = ADMA_BLOCK_INTERRUPT_N;
	uint32_t dev_en[ADMA_CH_NUM] = {0}; 
	dev_en[0] =  DEV_ADMA0_LINEIN_DECIMATOR_0; //DMIC0 LEFT ADMA CH 
	dev_en[1] =  DEV_ADMA0_HQ_LP_DECIMATOR_1; //DMIC1 RIGHT ADMA CH 
	ADMA_START_ADDRESS start_addr = {0}; 

	for (int i =0; i< ADMA_CH_NUM; i++)
	{
		start_addr.dw = (uint32_t )data_buf + (i * 4);
		ret = adma_set_channel_ctrl(dev_en[i], &ch_ctrl, &start_addr);
	}

	//Step2 : Set-up callback 
	adma_irq_cb irq_cb 				   = user_callback; //Note: User set-up callback accordingly 
	struct adma_callback cb 		   = {irq_cb, NULL};
	ret = adma_set_irq_cb(ADMA_CH_NUM, dev_en, &cb, 1);

	//Step3 : enable ADMA CH 
	ret = adma_enable(ADMA_CH_NUM, dev_en, 1);
	return ret;
}

void adc_demo_task(void *param)
{
    Print_ERR("adc Demo\n");
    while(1)
    {
        os_SemTake(adma_notify, OS_SEM_WAIT_FOREVER);
        Print_ERR("Captured 1ms data!!\n");
    }
}

int adc_demo_init(void)
{
	os_TaskInit TaskInit;
	memset(&TaskInit, 0, sizeof(TaskInit));

	os_SemInit SemInit;
	SemInit.uSemType = SEM_BINARY;
	adma_notify       = os_SemCreate(&SemInit);

	TaskInit.ContextID     = _ID('A', 'D', 'C', 'D');
	TaskInit.TaskFunc      = adc_demo_task;
	TaskInit.TaskHookFunc  = NULL;
	TaskInit.TaskName      = "ADCD";
	TaskInit.TaskStackSize = 256;
	TaskInit.TaskPriority  = OS_TASK_HIGH_PRIORITY;
	TaskInit.TaskTZModule  = 2U;

	adc_task_handle = os_TaskCreate(&TaskInit);

	if (adc_task_handle == NULL)
	{
		Print_ERR("Create Task failed\n");
		return -1;
	}

	//Init HW when task is ready
	adc_demo_init_dmic();
	adc_demo_init_adma();

	return 0;
}

#endif //CONFIG_MCU_DRV_ADC