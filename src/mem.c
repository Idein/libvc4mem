/*
 * Copyright (c) 2017 Sugizaki Yukimasa (ysugi@idein.jp)
 * All rights reserved.
 *
 * This software is licensed under a Modified (3-Clause) BSD License.
 * You should have received a copy of this license along with this
 * software. If not, contact the copyright holder above.
 */

#include "vc4mem.h"
#include "local.h"
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <sys/ioctl.h>

void* vc4mem_malloc(const unsigned size, unsigned *busp,
        struct vc4mem_config *cfg)
{
    int ret;
    void *user;

    ret = vc4mem_malloc_v(cfg, 1, size, busp, &user);
    if (ret) {
        pr_err("Failed to malloc_v\n");
        return NULL;
    }

    return user;
}

/* cfg, n, size1, &bus1, &user1, size2, &bus2, &user2, ... */
int vc4mem_malloc_v(struct vc4mem_config *cfg, const unsigned n, ...)
{
    int ret = 0;
    unsigned i;
    va_list ap;
    unsigned size[VC4MEM_MAX_NUM_REQS];
    unsigned *busp[VC4MEM_MAX_NUM_REQS];
    void **userp[VC4MEM_MAX_NUM_REQS];
    struct vc4mem_alloc_mem ioparam;

    if (n > VC4MEM_MAX_NUM_REQS) {
        pr_err("n(%d) is greater than MAX_NUM_REQS(%d)\n",
                n, VC4MEM_MAX_NUM_REQS);
        return 1;
    }

    va_start(ap, n);

    for (i = 0; i < n; i ++) {
        size[i]  = va_arg(ap, unsigned);
        busp[i]  = va_arg(ap, unsigned*);
        userp[i] = va_arg(ap, void**);
    }

    ioparam.user.n = n;
    (void) memcpy(ioparam.user.size, size, sizeof(size));
    ret = ioctl(cfg->priv->fd, VC4MEM_IOC_ALLOC_MEM, &ioparam);
    if (ret) {
        pr_err("ioctl: %s\n", strerror(errno));
        goto failed_ioctl;
    }

    for (i = 0; i < n; i ++) {
        const unsigned bus = ioparam.kern.dma[i];
        void * const user = vc4mem_map_bus_to_user(bus, size[i], cfg);
        if (user == NULL) {
            pr_err("Failed to map_bus_to_user\n");
            ret = 1;
            goto failed_map;
        }
        *busp[i] = bus;
        *userp[i] = user;
    }

    goto end;

failed_map:
    {
        struct vc4mem_free_mem ioparam;
        ioparam.user.n = n;
        for (i = 0; i < n; i ++)
            ioparam.user.dma[i] = *busp[i];
        (void) memcpy(ioparam.user.size, size, sizeof(size));
        if (ioctl(cfg->priv->fd, VC4MEM_IOC_FREE_MEM, &ioparam))
            pr_err("ioctl: %s\n", strerror(errno));
    }
failed_ioctl:
end:
    va_end(ap);
    return ret;
}

int vc4mem_free(void * const user, const unsigned bus, const unsigned size,
        struct vc4mem_config *cfg)
{
    return vc4mem_free_v(cfg, 1, user, bus, size);
}

int vc4mem_free_v(struct vc4mem_config *cfg, const unsigned n, ...)
{
    int ret = 0;
    unsigned i;
    va_list ap;
    void *user[VC4MEM_MAX_NUM_REQS];
    unsigned bus[VC4MEM_MAX_NUM_REQS];
    unsigned size[VC4MEM_MAX_NUM_REQS];
    struct vc4mem_free_mem ioparam;

    if (n > VC4MEM_MAX_NUM_REQS) {
        pr_err("n(%d) is greater than MAX_NUM_REQS(%d)\n",
                n, VC4MEM_MAX_NUM_REQS);
        return 1;
    }

    va_start(ap, n);

    for (i = 0; i < n; i ++) {
        user[i] = va_arg(ap, void*);
        bus[i]  = va_arg(ap, unsigned);
        size[i] = va_arg(ap, unsigned);
    }

    for (i = 0; i < n; i ++) {
        ret = vc4mem_unmap_bus_to_user(user[i], size[i], cfg);
        if (ret) {
            pr_err("Failed to unmap_bus_to_user\n");
            goto end;
        }
    }

    ioparam.user.n = n;
    (void) memcpy(ioparam.user.dma, bus, sizeof(bus));
    (void) memcpy(ioparam.user.size, size, sizeof(size));
    ret = ioctl(cfg->priv->fd, VC4MEM_IOC_FREE_MEM, &ioparam);
    if (ret) {
        pr_err("ioctl: %s\n", strerror(errno));
        goto failed_ioctl;
    }

    goto end;

failed_ioctl:
end:
    va_end(ap);
    return ret;
}

int vc4mem_cpu_cache_op(const vc4mem_cpu_cache_op_t op,
        const unsigned bus, const unsigned size, struct vc4mem_config *cfg)
{
    return vc4mem_cpu_cache_op_v(cfg, 1, op, bus, size);
}

int vc4mem_cpu_cache_op_v(struct vc4mem_config *cfg, const unsigned n, ...)
{
    int ret;
    unsigned i;
    va_list ap;
    struct vc4mem_cpu_cache_op ioparam;

    if (n > VC4MEM_MAX_NUM_REQS) {
        pr_err("n(%d) is greater than MAX_NUM_REQS(%d)\n",
                n, VC4MEM_MAX_NUM_REQS);
        return 1;
    }

    va_start(ap, n);

    ioparam.user.n = n;
    for (i = 0; i < n; i ++) {
        /* So cpu_cache_op_t must be a simple type. */
        ioparam.user.op[i]   = va_arg(ap, vc4mem_cpu_cache_op_t);
        ioparam.user.dma[i]  = va_arg(ap, unsigned);
        ioparam.user.size[i] = va_arg(ap, unsigned);
    }

    ret = ioctl(cfg->priv->fd, VC4MEM_IOC_CPU_CACHE_OP, &ioparam);
    if (ret) {
        pr_err("ioctl: %s\n", strerror(errno));
        goto failed_ioctl;
    }

    goto end;

failed_ioctl:
end:
    va_end(ap);
    return ret;
}
