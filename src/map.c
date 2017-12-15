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
#include <string.h>
#include <errno.h>
#include <sys/mman.h>

unsigned vc4mem_bus_to_phys(const unsigned bus)
{
    return bus & ~0xc0000000;
}

void* vc4mem_map_phys_to_user(const unsigned phys, const size_t size,
        struct vc4mem_config *cfg)
{
    void *user;

    user = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED,
            cfg->priv->fd, phys);
    if (user == MAP_FAILED) {
        pr_err("mmap: %s\n", strerror(errno));
        user = NULL;
    }

    return user;
}

int vc4mem_unmap_phys_to_user(void * const user, const size_t size,
        struct vc4mem_config *cfg)
{
    int ret;

    UNUSED(cfg);

    ret = munmap(user, size);
    if (ret != 0)
        pr_err("munmap: %s\n", strerror(errno));

    return ret;
}

void *vc4mem_map_bus_to_user(const unsigned bus, const size_t size,
        struct vc4mem_config *cfg)
{
    return vc4mem_map_phys_to_user(vc4mem_bus_to_phys(bus), size, cfg);
}

int vc4mem_unmap_bus_to_user(void * const user, const size_t size,
        struct vc4mem_config *cfg)
{
    return vc4mem_unmap_phys_to_user(user, size, cfg);
}
