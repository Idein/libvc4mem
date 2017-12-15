#ifndef VC4MEM_H_
#define VC4MEM_H_

#include <sys/types.h>
#include <linux/vc4mem.h>

    struct vc4mem_config{
        int is_noncached;
        char *dev;
        struct vc4mem_private_config *priv;
    };

    /* main.c */

    void vc4mem_clear_config(struct vc4mem_config *cfg);
    int vc4mem_init(struct vc4mem_config *cfg);
    int vc4mem_finalize(struct vc4mem_config *cfg);

    /* map.c */

    unsigned vc4mem_bus_to_phys(const unsigned bus);

    void* vc4mem_map_phys_to_user(const unsigned phys, const size_t size,
            struct vc4mem_config *cfg);
    int vc4mem_unmap_phys_to_user(void * const user, const size_t size,
            struct vc4mem_config *cfg);

    void* vc4mem_map_bus_to_user(const unsigned phys, const size_t size,
            struct vc4mem_config *cfg);
    int vc4mem_unmap_bus_to_user(void * const user, const size_t size,
            struct vc4mem_config *cfg);

    /* mem.c */

    void* vc4mem_malloc(const unsigned size, unsigned *busp,
            struct vc4mem_config *cfg);
    int vc4mem_malloc_v(struct vc4mem_config *cfg, const unsigned n, ...);

    int vc4mem_free(void * const user, const unsigned bus, const unsigned size,
            struct vc4mem_config *cfg);
    int vc4mem_free_v(struct vc4mem_config *cfg, const unsigned n, ...);

    int vc4mem_cpu_cache_op(const vc4mem_cpu_cache_op_t op,
            const unsigned bus, const unsigned size, struct vc4mem_config *cfg);
    int vc4mem_cpu_cache_op_v(struct vc4mem_config *cfg, const unsigned n, ...);

#endif /* VC4MEM_H_ */
