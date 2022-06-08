/** \file UART_Hal.h
 * \brief
 *
 * \author Shahabuddin K
 * \version 0.1
 *
 * \remark Copyright:            (c) 2011 Conexant Systems Inc.
 *
 */
 
#ifndef _MCU_DRV_UART_H_
#define _MCU_DRV_UART_H_

#include "mcu.h"
#include "driver.h"

#define FDMA_0      0

#define UART_TX_FDMA_CH  	0
#define UART_RX_FDMA_CH  	1

#ifdef CONFIG_MCU_DRV_UART_FDMA
#include "fdma_drv.h"

#define UART_FIFO_BASE_ADDR			(0x42040800)	//no this value in tahiti_register_ahb.h
#define MAX_BUFFER_SIZE_IN_DWORD  	1024

#define UART_FIFO_UNPACK_TRUE       1
#define UART_FIFO_UNPACK_FALSE      0
#endif

#ifdef __cplusplus
extern "C" {
#endif
 
#if (defined(CONFIG_MCU_DRV_UART_NON_SECURE) && defined(__DOMAIN_NS)) || \
					(defined(CONFIG_MCU_DRV_UART_SECURE) && !defined(__DOMAIN_NS))

#define DRV_UART_IO_CMD_SET_BAUDRATE DRV_STD_CMD_USER_SPACE + 1
#define DRV_UART_IO_CMD_GET_BAUDRATE DRV_STD_CMD_USER_SPACE + 2

typedef void (*uart_cb)(void * data);

struct uart_ops_s
{
    DECLARE_STANDARD_DEVICE_API;
#ifdef CONFIG_MCU_DRV_UART_FDMA
    void (*uart_set_callback) (uart_cb tx_cb, uart_cb rx_cb);
    int (*uart_putchars) (uint dev, dmaaddr_t memSrc, dwsize_t len);
    int (*uart_getchars) (uint dev, dmaaddr_t memDst, dwsize_t len, uint unpack);
#endif
    int (*uart_config) (uint baud);
    int (*uart_get_baud)();
    int (*uart_flush_fifo) (void);
    int (*uart_putchar)(uint32_t data);
    int (*uart_getchar)(void);
    int (*uart_get_status)(void);
};

static inline struct uart_ops_s *uart_driver_info(uint32_t dev)
{
    return (struct uart_ops_s *)os_get_driver_info(dev);
}

static inline int uart_putchar(uint32_t data)
{
    struct uart_ops_s *op;
    if ((op = uart_driver_info(DEV_UART0)) != NULL)
    {
        op->uart_putchar(data);
        return 0;
    }
    return -ENODEV;
}

static inline int uart_getchar(void)
{
    struct uart_ops_s *op;
    if ((op = uart_driver_info(DEV_UART0)) != NULL)
    {
        return op->uart_getchar();
    }
    return -ENODEV;
}

static int uart_get_status(void)
{
    struct uart_ops_s *op;
    if ((op = uart_driver_info(DEV_UART0)) != NULL)
    {
        return op->uart_get_status();
    }
    return -ENODEV;
}

static int uart_flush_fifo(void)
{
    struct uart_ops_s *op;
    if ((op = uart_driver_info(DEV_UART0)) != NULL)
    {
        return op->uart_flush_fifo();
    }
    return -ENODEV;
}


#ifdef CONFIG_MCU_DRV_UART_FDMA
static void uart_set_callback(uart_cb tx_cb, uart_cb rx_cb)
{
    struct uart_ops_s *op;
    if ((op = uart_driver_info(DEV_UART0)) != NULL)
    {
        return op->uart_set_callback(tx_cb, rx_cb);
    }
    
}

static int uart_putchars(uint dev, dmaaddr_t memSrc, dwsize_t len)
{
    struct uart_ops_s *op;
    if ((op = uart_driver_info(DEV_UART0)) != NULL)
    {
        return op->uart_putchars(dev, memSrc, len);
    }
    return -ENODEV;
}

static int uart_getchars(uint dev, dmaaddr_t memDst, dwsize_t len, uint unpack)
{
    struct uart_ops_s *op;
    if ((op = uart_driver_info(DEV_UART0)) != NULL)
    {
        return op->uart_getchars(dev, memDst, len, unpack);
    }
    return -ENODEV;
}

static int uart_write_dwords(dmaaddr_t memSrc, dwsize_t len)
{
    struct uart_ops_s *op;
    if ((op = uart_driver_info(DEV_UART0)) != NULL)
    {
        return op->uart_putchars(DEV_FDMA_UART(FDMA_0, UART_TX_FDMA_CH), memSrc, len);
    }
    return -ENODEV;
}

static int uart_read_dwords(dmaaddr_t memDst, dwsize_t len, uint unpack)
{
    struct uart_ops_s *op;
    if ((op = uart_driver_info(DEV_UART0)) != NULL)
    {
        return op->uart_getchars(DEV_FDMA_UART(FDMA_0, UART_RX_FDMA_CH), memDst, len, unpack);
    }
    return -ENODEV;
}
#endif

static int uart_config(uint baud)
{
    struct uart_ops_s *op;
    if ((op = uart_driver_info(DEV_UART0)) != NULL)
    {
        return op->uart_config(baud);
    }
    return -ENODEV;
}

static int uart_get_baud()
{
    struct uart_ops_s *op;
    if ((op = uart_driver_info(DEV_UART0)) != NULL)
    {
        return op->uart_get_baud();
    }
    return -ENODEV;
}



int drv_uart_start(void);

#endif // (defined(CONFIG_MCU_DRV_UART_NON_SECURE) && defined(__DOMAIN_NS)) || (defined(CONFIG_MCU_DRV_UART_SECURE) && !defined(__DOMAIN_NS))

#ifdef __cplusplus
}
#endif

#endif // _MCU_DRV_UART_H_
