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

#define DEFAULT_VC4MEM_DEV "/dev/vc4mem"

void vc4mem_clear_config(struct vc4mem_config *cfg)
{
    cfg->is_noncached = 0;
    cfg->dev = NULL;
    cfg->priv = NULL;
}

int vc4mem_init(struct vc4mem_config *cfg)
{
    int ret = 0;
    int flags;

    flags = O_NONBLOCK | O_RDWR;
    if (cfg->is_noncached)
        flags |= O_SYNC;

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

    if (cfg->dev == NULL) {
        cfg->dev = strdup(DEFAULT_VC4MEM_DEV);
        if (cfg->dev == NULL) {
            pr_err("strdup: %s\n", strerror(errno));
            ret = 1;
            goto err;
        }
    }

    cfg->priv->fd = open(cfg->dev, flags);
    if (cfg->priv->fd == -1) {
        pr_err("open: %s: %s\n", cfg->dev, strerror(errno));
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

    if (cfg->priv != NULL) {
        if (cfg->priv->fd != -1) {
            ret = close(cfg->priv->fd);
            if (ret != 0) {
                pr_err("close: %s: %s\n", cfg->dev, strerror(errno));
                /* Keep going. */
            }
        }

        free(cfg->priv);
    }

    free(cfg->dev);

    vc4mem_clear_config(cfg);

    return ret;
}
