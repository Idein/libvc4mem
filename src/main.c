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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#define DEFAULT_MMAP_DEV "/dev/mem"
#define DEFAULT_VC4MEM_DEV "/dev/dmatest"

void vc4mem_clear_config(struct vc4mem_config *cfg)
{
    cfg->mmap_dev = NULL;
    cfg->mmap_open_flags = 0;
    cfg->vc4mem_dev = NULL;
    cfg->vc4mem_open_flags = 0;
    cfg->priv = NULL;
}

int vc4mem_init(struct vc4mem_config *cfg)
{
    int ret = 0;

    if (cfg->mmap_dev == NULL) {
        cfg->mmap_dev = strdup(DEFAULT_MMAP_DEV);
        if (cfg->mmap_dev == NULL) {
            pr_err("strdup: %s\n", strerror(errno));
            ret = 1;
            goto err;
        }
    }
    if (cfg->mmap_open_flags == 0)
        cfg->mmap_open_flags = O_RDWR | O_SYNC;
    cfg->mmap_open_flags &= ~O_CREAT;

    if (cfg->vc4mem_dev == NULL) {
        cfg->vc4mem_dev = strdup(DEFAULT_VC4MEM_DEV);
        if (cfg->vc4mem_dev == NULL) {
            pr_err("strdup: %s\n", strerror(errno));
            ret = 1;
            goto err;
        }
    }
    if (cfg->vc4mem_open_flags == 0)
        cfg->vc4mem_open_flags = O_NONBLOCK;
    cfg->vc4mem_open_flags &= ~O_CREAT;

    if (cfg->priv != NULL) {
        pr_err("Call clear_config before init and don't set cfg->priv\n");
        ret = 1;
        goto err;
    }
    cfg->priv = malloc(sizeof(*cfg->priv));
    if (cfg->priv == NULL) {
        pr_err("malloc: %s\n", strerror(errno));
        ret = 1;
        goto err;
    }

    cfg->priv->mmap_fd = open(cfg->mmap_dev, cfg->mmap_open_flags);
    if (cfg->priv->mmap_fd == -1) {
        pr_err("open: %s: %s\n", cfg->mmap_dev, strerror(errno));
        ret = 1;
        goto err;
    }

    cfg->priv->vc4mem_fd = open(cfg->vc4mem_dev, cfg->vc4mem_open_flags);
    if (cfg->priv->vc4mem_fd == -1) {
        pr_err("open: %s: %s\n", cfg->vc4mem_dev, strerror(errno));
        ret = 1;
        goto err;
    }

    return ret;

err:
    (void) vc4mem_finalize(cfg);
    return ret;
}

int vc4mem_finalize(struct vc4mem_config *cfg)
{
    int ret = 0;

    if (cfg->priv->vc4mem_fd != -1) {
        ret = close(cfg->priv->vc4mem_fd);
        if (ret != 0) {
            pr_err("close: %s: %s\n", cfg->vc4mem_dev, strerror(errno));
            /* Not going to err. */
        }
    }

    if (cfg->priv->mmap_fd != -1) {
        ret = close(cfg->priv->mmap_fd);
        if (ret != 0) {
            pr_err("close: %s: %s\n", cfg->mmap_dev, strerror(errno));
            /* Not going to err. */
        }
    }

    free(cfg->priv);

    cfg->vc4mem_open_flags = 0;
    cfg->mmap_open_flags = 0;

    free(cfg->vc4mem_dev);
    free(cfg->mmap_dev);

    return ret;
}
