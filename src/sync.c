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
#include <sys/ioctl.h>

int vc4mem_sync_mem(const unsigned long req,
        const unsigned bus, const size_t size, struct vc4mem_config *cp)
{
    int ret = 0;
    struct dmatest_mem_area ioparam = {
        .addr = bus,
        .size = size
    };

    ret = ioctl(cp->priv->vc4mem_fd, req, &ioparam);
    if (ret < 0)
        pr_err("ioctl: %s\n", strerror(errno));

    return ret;
}
