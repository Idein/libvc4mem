#ifndef VC4MEM_H_
#define VC4MEM_H_

#include <sys/types.h>

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

#endif /* VC4MEM_H_ */
