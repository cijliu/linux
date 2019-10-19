#ifndef __HI3536DV100_IO_H
#define __HI3536DV100_IO_H

/*  phys_addr       virt_addr
 *  0x1100_0000 <-----> 0xFE00_0000
 *  0x1104_0000 <-----> 0xFE04_0000
 */
#define HI3536DV100_IOCH1_VIRT (0xFE000000)
#define HI3536DV100_IOCH1_PHYS (0x11000000)
#define HI3536DV100_IOCH1_SIZE (0x00040000)

/* phys_addr        virt_addr
 * 0x1200_0000 <-----> 0xFE10_0000
 * 0x121B_0000 <-----> 0xFE2B_0000
 */
#define HI3536DV100_IOCH2_VIRT (0xFE100000)
#define HI3536DV100_IOCH2_PHYS (0x12000000)
#define HI3536DV100_IOCH2_SIZE (0x001B0000)

/* phys_addr        virt_addr
 * 0x1300_0000 <-----> 0xFE30_0000
 * 0x1321_0000 <-----> 0xFE51_0000
 */
#define HI3536DV100_IOCH3_VIRT (0xFE300000)
#define HI3536DV100_IOCH3_PHYS (0x13000000)
#define HI3536DV100_IOCH3_SIZE (0x00210000)

#define IO_OFFSET_LOW       (0xEB300000)
#define IO_OFFSET_MID       (0xEC100000)
#define IO_OFFSET_HIGH      (0xED000000)

#define IO_ADDRESS_LOW(x)   ((x) + IO_OFFSET_LOW)
#define IO_ADDRESS_MID(x)   ((x) + IO_OFFSET_MID)
#define IO_ADDRESS_HIGH(x)  ((x) + IO_OFFSET_HIGH)

#define __IO_ADDRESS_HIGH(x) ((x >= HI3536DV100_IOCH2_PHYS) ? IO_ADDRESS_MID(x) \
                : IO_ADDRESS_HIGH(x))
#define IO_ADDRESS(x)   ((x) >= HI3536DV100_IOCH3_PHYS ? IO_ADDRESS_LOW(x) \
                : __IO_ADDRESS_HIGH(x))

#define __io_address(x) (IOMEM(IO_ADDRESS(x)))

#endif

