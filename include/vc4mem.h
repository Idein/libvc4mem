#ifndef VC4MEM_H_
#define VC4MEM_H_

#include <sys/types.h>
#include <fcntl.h> /* For O_* flags */
#include <linux/ioctl.h>

    /* From dmatest.h */
    /* xxx: Need to separate files. */

#ifndef _DMATEST_H
#define _DMATEST_H

#include <linux/ioctl.h>

struct dmatest_mem_area {
	unsigned addr;
	unsigned long size;
};

#define DMATEST_IOC_MAGIC 'd'

#define DMATEST_IOC_SYNC_FOR_CPU_TO_DEVICE        _IOR( DMATEST_IOC_MAGIC, 0, struct dmatest_mem_area )
#define DMATEST_IOC_SYNC_FOR_CPU_FROM_DEVICE      _IOR( DMATEST_IOC_MAGIC, 1, struct dmatest_mem_area )
#define DMATEST_IOC_SYNC_FOR_CPU_BIDIRECTIONAL    _IOR( DMATEST_IOC_MAGIC, 2, struct dmatest_mem_area )
#define DMATEST_IOC_SYNC_FOR_DEVICE_TO_DEVICE     _IOR( DMATEST_IOC_MAGIC, 3, struct dmatest_mem_area )
#define DMATEST_IOC_SYNC_FOR_DEVICE_FROM_DEVICE   _IOR( DMATEST_IOC_MAGIC, 4, struct dmatest_mem_area )
#define DMATEST_IOC_SYNC_FOR_DEVICE_BIDIRECTIONAL _IOR( DMATEST_IOC_MAGIC, 5, struct dmatest_mem_area )
#define DMATEST_IOC_DMA_ALLOC                     _IOWR(DMATEST_IOC_MAGIC, 6, struct dmatest_mem_area )
#define DMATEST_IOC_ACCESS_MEM                    _IOWR(DMATEST_IOC_MAGIC, 7, struct dmatest_mem_area )
#define DMATEST_IOC_INVERT_MEM                    _IOR( DMATEST_IOC_MAGIC, 8, struct dmatest_mem_area )

#endif /* _DMATEST_H */


    struct vc4mem_config{
        char *mmap_dev;
        int mmap_open_flags;
        char *vc4mem_dev;
        int vc4mem_open_flags;

        struct vc4mem_private_config *priv;
    };

    /* main.c */
    void vc4mem_clear_config(struct vc4mem_config *cfg);
    int vc4mem_init(struct vc4mem_config *cfg);
    int vc4mem_finalize(struct vc4mem_config *cfg);

    /* map.c */
    void* vc4mem_map_phys_to_user(const unsigned phys, const size_t size,
            struct vc4mem_config *cfg);
    int vc4mem_unmap_phys_to_user(void * const user, const size_t size);
    unsigned vc4mem_bus_to_phys(const unsigned bus);

    /* sync.h */
    int vc4mem_sync_mem(const unsigned long req,
            const unsigned bus, const size_t size, struct vc4mem_config *cp);

#endif /* VC4MEM_H_ */
