#ifndef _MCU_I3C_PROTOCOL_H_
#define _MCU_I3C_PROTOCOL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "mcu.h"
//#include "command.h"

//>>>>>>>>>>>master.h >>>>>>>>> 

#define I3C_HOT_JOIN_ADDR		0x2
#define I3C_BROADCAST_ADDR		0x7e
#define I3C_MAX_ADDR			GENMASK(6, 0)

//<<<<<<<<<<<<<<<<<<<<<<

#define   __be16    uint16_t 

#define BITS_PER_LONG 64
#define GENMASK(h, l) \
	(((~0UL) - (1UL << (l)) + 1) & (~0UL >> (BITS_PER_LONG - 1 - (h))))

#define I3C_PID_MANUF_ID(pid)		(((pid) & GENMASK_ULL(47, 33)) >> 33)
#define I3C_PID_RND_LOWER_32BITS(pid)	(!!((pid) & BIT_ULL(32)))
#define I3C_PID_RND_VAL(pid)		((pid) & GENMASK_ULL(31, 0))
#define I3C_PID_PART_ID(pid)		(((pid) & GENMASK_ULL(31, 16)) >> 16)
#define I3C_PID_INSTANCE_ID(pid)	(((pid) & GENMASK_ULL(15, 12)) >> 12)
#define I3C_PID_EXTRA_INFO(pid)		((pid) & GENMASK_ULL(11, 0))

#define I3C_BCR_DEVICE_ROLE(bcr)	((bcr) & GENMASK(7, 6))
#define I3C_BCR_I3C_SLAVE		(0 << 6)
#define I3C_BCR_I3C_MASTER		(1 << 6)
#define I3C_BCR_HDR_CAP			BIT(5)
#define I3C_BCR_BRIDGE			BIT(4)
#define I3C_BCR_OFFLINE_CAP		BIT(3)
#define I3C_BCR_IBI_PAYLOAD		BIT(2)
#define I3C_BCR_IBI_REQ_CAP		BIT(1)
#define I3C_BCR_MAX_DATA_SPEED_LIM	BIT(0)


/**device assoiciated */ 
/**
 * enum i3c_error_code - I3C error codes
 *
 * These are the standard error codes as defined by the I3C specification.
 * @I3C_ERROR_UNKNOWN: unknown error, usually means the error is not I3C
 *		       related
 * @I3C_ERROR_M0: M0 error
 * @I3C_ERROR_M1: M1 error
 * @I3C_ERROR_M2: M2 error
 */
enum i3c_error_code {
	I3C_ERROR_UNKNOWN = 0,
	I3C_ERROR_M0 = 1,
	I3C_ERROR_M1,
	I3C_ERROR_M2,
};

/**
 * enum i3c_hdr_mode - HDR mode ids
 * @I3C_HDR_DDR: DDR mode
 * @I3C_HDR_TSP: TSP mode
 * @I3C_HDR_TSL: TSL mode
 */
enum i3c_hdr_mode {
	I3C_HDR_DDR,
	I3C_HDR_TSP,
	I3C_HDR_TSL,
};

/**
 * struct i3c_priv_xfer - I3C SDR private transfer
 * @rnw: encodes the transfer direction. true for a read, false for a write
 * @len: transfer length in bytes of the transfer
 * @data: input/output buffer
 * @data.in: input buffer. Must point to a DMA-able buffer
 * @data.out: output buffer. Must point to a DMA-able buffer
 * @err: I3C error code
 */
struct i3c_priv_xfer {
	uint8_t rnw;
	uint16_t len;
	union {
		void *in;
		const void *out;
	} data;
	enum i3c_error_code err;
};

typedef  struct i3c_priv_xfer     i3c_priv_xfer_t ; 
/**
 * enum i3c_dcr - I3C DCR values
 * @I3C_DCR_GENERIC_DEVICE: generic I3C device
 */
enum i3c_dcr {
	I3C_DCR_GENERIC_DEVICE = 0,
};



/**
 * struct i3c_device_info - I3C device information
 * @pid: Provisional ID
 * @bcr: Bus Characteristic Register
 * @dcr: Device Characteristic Register
 * @static_addr: static/I2C address
 * @dyn_addr: dynamic address
 * @hdr_cap: supported HDR modes
 * @max_read_ds: max read speed information
 * @max_write_ds: max write speed information
 * @max_ibi_len: max IBI payload length
 * @max_read_turnaround: max read turn-around time in micro-seconds
 * @max_read_len: max private SDR read length in bytes
 * @max_write_len: max private SDR write length in bytes
 *
 * These are all basic information that should be advertised by an I3C device.
 * Some of them are optional depending on the device type and device
 * capabilities.
 * For each I3C slave attached to a master with
 * i3c_master_add_i3c_dev_locked(), the core will send the relevant CCC command
 * to retrieve these data.
 */
struct i3c_device_info {
	uint64_t pid;
	uint8_t bcr;
	uint8_t dcr;
	uint8_t static_addr;
	uint8_t dyn_addr;
	uint8_t hdr_cap;
	uint8_t max_read_ds;
	uint8_t max_write_ds;
	uint8_t max_ibi_len;
	uint32_t max_read_turnaround;
	uint16_t max_read_len;
	uint16_t max_write_len;
	uint32_t addr_table_idx; 
	uint32_t sub_addr_len; 
	uint16_t status ; 
};

typedef  struct i3c_device_info i3c_device_info_t ; 

struct i3c_ibi_payload {
	uint32_t len;
	const void *data;
};

/**
 * struct i3c_ibi_setup - IBI setup object
 * @max_payload_len: maximum length of the payload associated to an IBI. If one
 *		     IBI appears to have a payload that is bigger than this
 *		     number, the IBI will be rejected.
 * @num_slots: number of pre-allocated IBI slots. This should be chosen so that
 *	       the system never runs out of IBI slots, otherwise you'll lose
 *	       IBIs.
 * @handler: IBI handler, every time an IBI is received. This handler is called
 *	     in a workqueue context. It is allowed to sleep and send new
 *	     messages on the bus, though it's recommended to keep the
 *	     processing done there as fast as possible to avoid delaying
 *	     processing of other queued on the same workqueue.
 *
 * Temporary structure used to pass information to i3c_device_request_ibi().
 * This object can be allocated on the stack since i3c_device_request_ibi()
 * copies every bit of information and do not use it after
 * i3c_device_request_ibi() has returned.
 */
struct i3c_ibi_setup {
	uint32_t max_payload_len;
	uint32_t num_slots;
	void (*handler)(uint32_t dev,const struct i3c_ibi_payload *payload);
};


struct i3c_dev_desc {
	struct i3c_device_info info;
	//struct mutex ibi_lock;
	//struct i3c_device_ibi_info *ibi;
	uint32_t dev;
};

typedef  struct i3c_dev_desc  i3c_dev_desc_t ; 

/** CCC commands ***/  

/* I3C CCC (Common Command Codes) related definitions */
#define I3C_CCC_DIRECT			BIT(7)
#define I3C_CCC_ID(id, broadcast)	\
	((id) | ((broadcast) ? 0 : I3C_CCC_DIRECT))

/* Commands valid in both broadcast and unicast modes */
#define I3C_CCC_ENEC(broadcast)		I3C_CCC_ID(0x0, broadcast)
#define I3C_CCC_DISEC(broadcast)	I3C_CCC_ID(0x1, broadcast)
#define I3C_CCC_ENTAS(as, broadcast)	I3C_CCC_ID(0x2 + (as), broadcast)
#define I3C_CCC_RSTDAA(broadcast)	I3C_CCC_ID(0x6, broadcast)
#define I3C_CCC_SETMWL(broadcast)	I3C_CCC_ID(0x9, broadcast)
#define I3C_CCC_SETMRL(broadcast)	I3C_CCC_ID(0xa, broadcast)
#define I3C_CCC_SETXTIME(broadcast)	((broadcast) ? 0x28 : 0x98)
#define I3C_CCC_VENDOR(id, broadcast)	((id) + ((broadcast) ? 0x61 : 0xe0))

/* Broadcast-only commands */
#define I3C_CCC_ENTDAA			I3C_CCC_ID(0x7, true)
#define I3C_CCC_DEFSLVS			I3C_CCC_ID(0x8, true)
#define I3C_CCC_ENTTM			I3C_CCC_ID(0xb, true)
#define I3C_CCC_ENTHDR(x)		I3C_CCC_ID(0x20 + (x), true)


/* Unicast-only commands */
#define I3C_CCC_SETDASA			I3C_CCC_ID(0x7, false)
#define I3C_CCC_SETNEWDA		I3C_CCC_ID(0x8, false)
#define I3C_CCC_GETMWL			I3C_CCC_ID(0xb, false)
#define I3C_CCC_GETMRL			I3C_CCC_ID(0xc, false)
#define I3C_CCC_GETPID			I3C_CCC_ID(0xd, false)
#define I3C_CCC_GETBCR			I3C_CCC_ID(0xe, false)
#define I3C_CCC_GETDCR			I3C_CCC_ID(0xf, false)
#define I3C_CCC_GETSTATUS		I3C_CCC_ID(0x10, false)
#define I3C_CCC_GETACCMST		I3C_CCC_ID(0x11, false)
#define I3C_CCC_SETBRGTGT		I3C_CCC_ID(0x13, false)
#define I3C_CCC_GETMXDS			I3C_CCC_ID(0x14, false)
#define I3C_CCC_GETHDRCAP		I3C_CCC_ID(0x15, false)
#define I3C_CCC_GETXTIME		I3C_CCC_ID(0x19, false)

#define I3C_CCC_EVENT_SIR		BIT(0)
#define I3C_CCC_EVENT_MR		BIT(1)
#define I3C_CCC_EVENT_HJ		BIT(3)

/**
 * struct i3c_ccc_events - payload passed to ENEC/DISEC CCC
 *
 * @events: bitmask of I3C_CCC_EVENT_xxx events.
 *
 * Depending on the CCC command, the specific events coming from all devices
 * (broadcast version) or a specific device (unicast version) will be
 * enabled (ENEC) or disabled (DISEC).
 */
struct i3c_ccc_events {
	uint8_t events;
};

/**
 * struct i3c_ccc_mwl - payload passed to SETMWL/GETMWL CCC
 *
 * @len: maximum write length in bytes
 *
 * The maximum write length is only applicable to SDR private messages or
 * extended Write CCCs (like SETXTIME).
 */
struct i3c_ccc_mwl {
	__be16 len;
};

/**
 * struct i3c_ccc_mrl - payload passed to SETMRL/GETMRL CCC
 *
 * @len: maximum read length in bytes
 * @ibi_len: maximum IBI payload length
 *
 * The maximum read length is only applicable to SDR private messages or
 * extended Read CCCs (like GETXTIME).
 * The IBI length is only valid if the I3C slave is IBI capable
 * (%I3C_BCR_IBI_REQ_CAP is set).
 */
struct i3c_ccc_mrl {
	__be16 read_len;
	uint8_t ibi_len;
} __packed;

/**
 * struct i3c_ccc_dev_desc - I3C/I2C device descriptor used for DEFSLVS
 *
 * @dyn_addr: dynamic address assigned to the I3C slave or 0 if the entry is
 *	      describing an I2C slave.
 * @dcr: DCR value (not applicable to entries describing I2C devices)
 * @lvr: LVR value (not applicable to entries describing I3C devices)
 * @bcr: BCR value or 0 if this entry is describing an I2C slave
 * @static_addr: static address or 0 if the device does not have a static
 *		 address
 *
 * The DEFSLVS command should be passed an array of i3c_ccc_dev_desc
 * descriptors (one entry per I3C/I2C dev controlled by the master).
 */
struct i3c_ccc_dev_desc {
	uint8_t dyn_addr;
	union {
		uint8_t dcr;
		uint8_t lvr;
	};
	uint8_t bcr;
	uint8_t static_addr;
};

/**
 * struct i3c_ccc_defslvs - payload passed to DEFSLVS CCC
 *
 * @count: number of dev descriptors
 * @master: descriptor describing the current master
 * @slaves: array of descriptors describing slaves controlled by the
 *	    current master
 *
 * Information passed to the broadcast DEFSLVS to propagate device
 * information to all masters currently acting as slaves on the bus.
 * This is only meaningful if you have more than one master.
 */
struct i3c_ccc_defslvs {
	uint8_t count;
	struct i3c_ccc_dev_desc master;
	struct i3c_ccc_dev_desc slaves[0];
} __packed;

/**
 * enum i3c_ccc_test_mode - enum listing all available test modes
 *
 * @I3C_CCC_EXIT_TEST_MODE: exit test mode
 * @I3C_CCC_VENDOR_TEST_MODE: enter vendor test mode
 */
enum i3c_ccc_test_mode {
	I3C_CCC_EXIT_TEST_MODE,
	I3C_CCC_VENDOR_TEST_MODE,
};

/**
 * struct i3c_ccc_enttm - payload passed to ENTTM CCC
 *
 * @mode: one of the &enum i3c_ccc_test_mode modes
 *
 * Information passed to the ENTTM CCC to instruct an I3C device to enter a
 * specific test mode.
 */
struct i3c_ccc_enttm {
	uint8_t mode;
};

/**
 * struct i3c_ccc_setda - payload passed to SETNEWDA and SETDASA CCCs
 *
 * @addr: dynamic address to assign to an I3C device
 *
 * Information passed to the SETNEWDA and SETDASA CCCs to assign/change the
 * dynamic address of an I3C device.
 */
struct i3c_ccc_setda {
	uint8_t addr;
};

/**
 * struct i3c_ccc_getpid - payload passed to GETPID CCC
 *
 * @pid: 48 bits PID in big endian
 */
struct i3c_ccc_getpid {
	uint8_t pid[6];
};

/**
 * struct i3c_ccc_getbcr - payload passed to GETBCR CCC
 *
 * @bcr: BCR (Bus Characteristic Register) value
 */
struct i3c_ccc_getbcr {
	uint8_t bcr;
};

/**
 * struct i3c_ccc_getdcr - payload passed to GETDCR CCC
 *
 * @dcr: DCR (Device Characteristic Register) value
 */
struct i3c_ccc_getdcr {
	uint8_t dcr;
};

#define I3C_CCC_STATUS_PENDING_INT(status)	((status) & GENMASK(3, 0))
#define I3C_CCC_STATUS_PROTOCOL_ERROR		BIT(5)
#define I3C_CCC_STATUS_ACTIVITY_MODE(status)	\
	(((status) & GENMASK(7, 6)) >> 6)

/**
 * struct i3c_ccc_getstatus - payload passed to GETSTATUS CCC
 *
 * @status: status of the I3C slave (see I3C_CCC_STATUS_xxx macros for more
 *	    information).
 */
struct i3c_ccc_getstatus {
	__be16 status;
};

/**
 * struct i3c_ccc_getaccmst - payload passed to GETACCMST CCC
 *
 * @newmaster: address of the master taking bus ownership
 */
struct i3c_ccc_getaccmst {
	uint8_t newmaster;
};

/**
 * struct i3c_ccc_bridged_slave_desc - bridged slave descriptor
 *
 * @addr: dynamic address of the bridged device
 * @id: ID of the slave device behind the bridge
 */
struct i3c_ccc_bridged_slave_desc {
	uint8_t addr;
	__be16 id;
} __packed;

/**
 * struct i3c_ccc_setbrgtgt - payload passed to SETBRGTGT CCC
 *
 * @count: number of bridged slaves
 * @bslaves: bridged slave descriptors
 */
struct i3c_ccc_setbrgtgt {
	uint8_t count;
	struct i3c_ccc_bridged_slave_desc bslaves[0];
} __packed;

/**
 * enum i3c_sdr_max_data_rate - max data rate values for private SDR transfers
 */
enum i3c_sdr_max_data_rate {
	I3C_SDR0_FSCL_MAX,
	I3C_SDR1_FSCL_8MHZ,
	I3C_SDR2_FSCL_6MHZ,
	I3C_SDR3_FSCL_4MHZ,
	I3C_SDR4_FSCL_2MHZ,
};

/**
 * enum i3c_tsco - clock to data turn-around
 */
enum i3c_tsco {
	I3C_TSCO_8NS,
	I3C_TSCO_9NS,
	I3C_TSCO_10NS,
	I3C_TSCO_11NS,
	I3C_TSCO_12NS,
};

#define I3C_CCC_MAX_SDR_FSCL_MASK	GENMASK(2, 0)
#define I3C_CCC_MAX_SDR_FSCL(x)		((x) & I3C_CCC_MAX_SDR_FSCL_MASK)

/**
 * struct i3c_ccc_getmxds - payload passed to GETMXDS CCC
 *
 * @maxwr: write limitations
 * @maxrd: read limitations
 * @maxrdturn: maximum read turn-around expressed micro-seconds and
 *	       little-endian formatted
 */
struct i3c_ccc_getmxds {
	uint8_t maxwr;
	uint8_t maxrd;
	uint8_t maxrdturn[3];
} __packed;

#define I3C_CCC_HDR_MODE(mode)		BIT(mode)

/**
 * struct i3c_ccc_gethdrcap - payload passed to GETHDRCAP CCC
 *
 * @modes: bitmap of supported HDR modes
 */
struct i3c_ccc_gethdrcap {
	uint8_t modes;
} __packed;

/**
 * enum i3c_ccc_setxtime_subcmd - SETXTIME sub-commands
 */
enum i3c_ccc_setxtime_subcmd {
	I3C_CCC_SETXTIME_ST = 0x7f,
	I3C_CCC_SETXTIME_DT = 0xbf,
	I3C_CCC_SETXTIME_ENTER_ASYNC_MODE0 = 0xdf,
	I3C_CCC_SETXTIME_ENTER_ASYNC_MODE1 = 0xef,
	I3C_CCC_SETXTIME_ENTER_ASYNC_MODE2 = 0xf7,
	I3C_CCC_SETXTIME_ENTER_ASYNC_MODE3 = 0xfb,
	I3C_CCC_SETXTIME_ASYNC_TRIGGER = 0xfd,
	I3C_CCC_SETXTIME_TPH = 0x3f,
	I3C_CCC_SETXTIME_TU = 0x9f,
	I3C_CCC_SETXTIME_ODR = 0x8f,
};

/**
 * struct i3c_ccc_setxtime - payload passed to SETXTIME CCC
 *
 * @subcmd: one of the sub-commands ddefined in &enum i3c_ccc_setxtime_subcmd
 * @data: sub-command payload. Amount of data is determined by
 *	  &i3c_ccc_setxtime->subcmd
 */
struct i3c_ccc_setxtime {
	uint8_t subcmd;
	uint8_t data[0];
} __packed;

#define I3C_CCC_GETXTIME_SYNC_MODE	BIT(0)
#define I3C_CCC_GETXTIME_ASYNC_MODE(x)	BIT((x) + 1)
#define I3C_CCC_GETXTIME_OVERFLOW	BIT(7)

/**
 * struct i3c_ccc_getxtime - payload retrieved from GETXTIME CCC
 *
 * @supported_modes: bitmap describing supported XTIME modes
 * @state: current status (enabled mode and overflow status)
 * @frequency: slave's internal oscillator frequency in 500KHz steps
 * @inaccuracy: slave's internal oscillator inaccuracy in 0.1% steps
 */
struct i3c_ccc_getxtime {
	uint8_t supported_modes;
	uint8_t state;
	uint8_t frequency;
	uint8_t inaccuracy;
} __packed;

/**
 * struct i3c_ccc_cmd_payload - CCC payload
 *
 * @len: payload length
 * @data: payload data. This buffer must be DMA-able
 */
struct i3c_ccc_cmd_payload {
	uint16_t len;
	void *data;
};

/**
 * struct i3c_ccc_cmd_dest - CCC command destination
 *
 * @addr: can be an I3C device address or the broadcast address if this is a
 *	  broadcast CCC  
 * @payload: payload to be sent to this device or broadcasted
 */
struct i3c_ccc_cmd_dest {
	uint8_t addr;
	uint8_t addr_idx; // address table idx  
	uint8_t addr_table_idx; 
	struct i3c_ccc_cmd_payload payload;
};

/**
 * struct i3c_ccc_cmd - CCC command
 *
 * @rnw: true if the CCC should retrieve data from the device. Only valid for
 *	 unicast commands
 * @id: CCC command id
 * @ndests: number of destinations. Should always be one for broadcast commands
 * @dests: array of destinations and associated payload for this CCC. Most of
 *	   the time, only one destination is provided
 * @err: I3C error code
 */
struct i3c_ccc_cmd {
	uint8_t rnw;
	uint8_t id;
	uint32_t ndests;
	struct i3c_ccc_cmd_dest *dests;
	enum i3c_error_code err;
};


/* Function declration */  
int32_t i3c_master_entdaa(uint32_t dev);  //ENTDAA
int32_t i3c_master_disec(uint32_t dev, uint8_t addr, uint8_t evts);  //DISEC 
int32_t i3c_master_enec(uint32_t dev, uint8_t addr, uint8_t evts);  //ENEC 
int32_t i3c_master_rstdaa(uint32_t dev ,uint8_t addr); //RSTDAA 
int32_t i3c_master_setda(uint32_t dev, uint8_t oldaddr, uint8_t newaddr, uint32_t setdasa);
int32_t i3c_master_setdasa(uint32_t dev, uint8_t static_addr, uint8_t dyn_addr);//SETDASA
int32_t i3c_master_setnewda(uint32_t dev,uint8_t oldaddr, uint8_t newaddr); //SETNEWDA
int32_t i3c_master_getmrl(uint32_t dev,struct i3c_device_info *info); //GETMRL 
int32_t i3c_master_getmwl(uint32_t dev,struct i3c_device_info *info);//GETMWL
int32_t i3c_master_getmxds(uint32_t dev,struct i3c_device_info *info); //GETMXDS
int32_t i3c_master_gethdrcap(uint32_t dev,struct i3c_device_info *info); //GETHDRCAP
int32_t i3c_master_getpid(uint32_t dev,struct i3c_device_info *info);//GETPID
int32_t i3c_master_getbcr(uint32_t dev,struct i3c_device_info *info);//GETBCR
int32_t i3c_master_getdcr(uint32_t dev,struct i3c_device_info *info); //GETDCR 
int32_t i3c_master_getstatus(uint32_t dev,i3c_device_info_t *info); //GETSTATUS 
int32_t i3c_master_entas0(uint32_t dev,i3c_device_info_t *info);//ENTAS0 : 1us  activity  
int32_t i3c_master_entas1(uint32_t dev,i3c_device_info_t *info);//ENTAS1 : 100us activity 
int32_t i3c_master_entas2(uint32_t dev,i3c_device_info_t *info);//ENTAS2 : 2ms   activity
int32_t i3c_master_entas3(uint32_t dev,i3c_device_info_t *info);//ENTAS3 : 50ms  activity
int32_t i3c_master_setmrl(uint32_t dev,i3c_device_info_t *info);//SETMRL 
int32_t i3c_master_setmwl(uint32_t dev,i3c_device_info_t *info);//SETMWL 


#ifndef MAKE_I3C_DEV_ADD_TABLE
#define MAKE_I3C_DEV_ADD_TABLE(pentry, _is_legacy_i2c, _retry, _dynamic_addr, _static_addr) \
do {                                    		           			\
   (pentry)->is_legacy_i2c	=  (_is_legacy_i2c);            		\
   (pentry)->retry 			=  (_retry);                       		\
   (pentry)->dynamic_addr 	=  (_dynamic_addr);  					\
   (pentry)->static_addr 	=  (_static_addr);						\
   } while(0)
#endif 

#ifdef __cplusplus
}
#endif
#endif // _MCU_I3C_PROTOCL_H_