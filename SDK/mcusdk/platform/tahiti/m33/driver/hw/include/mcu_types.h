
#ifndef MCU_TYPES_H_
#define MCU_TYPES_H_

//#define NULL 					0

#define TRUE					1
#define FALSE					0

#define CHK_ALIGNED				0x3

#define LOCAL                   static
#define STATIC_INLINE           static inline

#define noof(a)		    (sizeof(a)/sizeof(a[0]))

typedef void                    *PVOID;
typedef int                     STATUS;
typedef void                    VOID;
typedef char                    INT8;
typedef short 		            INT16;
typedef int                     INT32;

//typedef unsigned char UINT8;
typedef unsigned int            UINT32;
typedef unsigned short          UINT16;
typedef unsigned char           UINT8;

typedef unsigned int            uint;
//typedef unsigned long           ulong;
//typedef unsigned long           dword;
typedef unsigned int			dword;

//typedef volatile unsigned long 	addr_t;
typedef volatile unsigned int 	addr_t;
typedef unsigned int			size_t;

typedef unsigned short			word;
typedef volatile dword			dword_rw;
typedef volatile dword			dword_ro;
typedef volatile word			word_ro;
typedef volatile word			word_rw;
typedef unsigned short          blknr;
typedef unsigned char           byte;

//typedef volatile unsigned long  dmaaddr_t;
typedef volatile unsigned int   dmaaddr_t;
typedef unsigned int            dwsize_t;
typedef volatile unsigned int	hwint32;
typedef volatile unsigned char	hwint8;
typedef unsigned long           ulong;

#define q8_23_t int


// BIT definition
#define BIT0                          (0x1 << 0) // 0x00000001
#define BIT1                          (0x1 << 1) // 0x00000002
#define BIT2                          (0x1 << 2) // 0x00000004
#define BIT3                          (0x1 << 3) // 0x00000008
#define BIT4                          (0x1 << 4) // 0x00000010
#define BIT5                          (0x1 << 5) // 0x00000020
#define BIT6                          (0x1 << 6) // 0x00000040
#define BIT7                          (0x1 << 7) // 0x00000080
#define BIT8                          (0x1 << 8) // 0x00000100
#define BIT9                          (0x1 << 9) // 0x00000200
#define BIT10                         (0x1 << 10)// 0x00000400
#define BIT11                         (0x1 << 11)// 0x00000800
#define BIT12                         (0x1 << 12)// 0x00001000
#define BIT13                         (0x1 << 13)// 0x00002000
#define BIT14                         (0x1 << 14)// 0x00004000
#define BIT15                         (0x1 << 15)// 0x00008000
#define BIT16                         (0x1 << 16)// 0x00010000
#define BIT17                         (0x1 << 17)// 0x00020000
#define BIT18                         (0x1 << 18)// 0x00040000
#define BIT19                         (0x1 << 19)// 0x00080000
#define BIT20                         (0x1 << 20)// 0x00100000
#define BIT21                         (0x1 << 21)// 0x00200000
#define BIT22                         (0x1 << 22)// 0x00400000
#define BIT23                         (0x1 << 23)// 0x00800000
#define BIT24                         (0x1 << 24)// 0x01000000
#define BIT25                         (0x1 << 25)// 0x02000000
#define BIT26                         (0x1 << 26)// 0x04000000
#define BIT27                         (0x1 << 27)// 0x08000000
#define BIT28                         (0x1 << 28)// 0x10000000
#define BIT29                         (0x1 << 29)// 0x20000000
#define BIT30                         (0x1 << 30)// 0x40000000
#define BIT31                         (0x1 << 31)// 0x80000000

#endif //#ifndef MCU_TYPES_H_
