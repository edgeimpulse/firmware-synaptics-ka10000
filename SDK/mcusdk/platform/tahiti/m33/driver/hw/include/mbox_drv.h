
#ifndef _MCU_MBOX_DRV_H_
#define _MCU_MBOX_DRV_H_

#include "mcu.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef CONFIG_MCU_DRV_MBOX

#define MBOX_0				0
#define MBOX_1				1
#define MBOX_2				2
#define MBOX_3				3
#define MBOX_4				4
#define MBOX_5				5
#define MBOX_MAX_IDS		6

/* -- use are these if debug mode is disabled. Disabling debug mode only allow one mailbox per CPU (in design)
#define ARM_MBOX_ID			MBOX_0
#define CAPE_A_MBOX_ID		MBOX_1
#define CAPE_B_MBOX_ID		MBOX_2
#define LLE_A_MBOX_ID		MBOX_3
#define LLE_B_MBOX_ID		MBOX_4
#define HIFI3_MBOX_ID		MBOX_5
*/

// MBOX_0 and MBOX_1 are used by local CPU. This means, other CPU cores can write to these two MBOX to send msg to local CPU. 
// Or local CPU can write to its own FIFO register to send messages to other threads locally.
#define MBOX_LOCAL_USE_LIST				{1, 0, 0, 0, 0, 0} 

#define MBOX_BUF_SIZE		32

// Interrupt callback function
typedef void (*mbox_irq_cb) (void *data);

//
//  Driver's exported functions
//

struct mbox_ops_s
{
  DECLARE_STANDARD_DEVICE_API;
  int (*mbox_read)      (uint32_t dev, void *addr, int len); // len = number of DWORDS
  int (*mbox_write)     (uint32_t dev, void *addr, int len); // len = number of DWORDS
  int (*set_irq_cb)     (uint32_t dev, mbox_irq_cb cb, void *cb_data);
  int (*get_local_id)	(uint32_t dev);
  int (*mbox_exit)      (uint32_t dev);
};

static inline struct mbox_ops_s *mbox_driver_info (uint dev)
{
	return (struct mbox_ops_s *)os_get_driver_info (dev);
}

static inline int mbox_read (uint32_t dev, void *addr, int len)
{
	struct mbox_ops_s *op;

	if ((op=mbox_driver_info(dev))!=NULL)
	{
		return op->mbox_read(dev, addr, len);
	}

	return -ENODEV;
}
  
static inline int mbox_write (uint32_t dev, void *addr, int len)
{
	struct mbox_ops_s *op;

	if ((op=mbox_driver_info(dev))!=NULL)
	{
		return op->mbox_write(dev, addr, len);
	}

	return -ENODEV;
}

static inline int mbox_set_irq_cb (uint32_t dev, mbox_irq_cb cb, void *cb_data)
{
	struct mbox_ops_s *op;

	if ((op=mbox_driver_info(dev))!=NULL)
	{
		return op->set_irq_cb(dev, cb, cb_data);
	}

	return -ENODEV;
}

static inline int mbox_get_local_id (uint32_t dev)
{
	struct mbox_ops_s *op;

	if ((op=mbox_driver_info(dev))!=NULL)
	{
		return op->get_local_id(dev);
	}

	return -ENODEV;
}

static inline int mbox_exit (uint32_t dev)
{
	struct mbox_ops_s *op;

	if ((op=mbox_driver_info(dev))!=NULL)
	{
	return op->mbox_exit(dev);
	}

	return -ENODEV;
}

int drv_mbox_start(void);
  
#endif // CONFIG_MCU_DRV_MBOX

#ifdef __cplusplus
}
#endif

#endif // _MCU_MBOX_DRV_H_
