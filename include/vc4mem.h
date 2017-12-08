#ifndef VC4MEM_H_
#define VC4MEM_H_

    struct vc4mem_config{
        char *mmap_dev;
        int mmap_open_flags;
        char *vc4mem_dev;
        int vc4mem_open_flags;

        struct vc4mem_private_config *priv;
    };

    void vc4mem_clear_config(struct vc4mem_config *cfg);
    int vc4mem_init(struct vc4mem_config *cfg);
    int vc4mem_finalize(struct vc4mem_config *cfg);

#endif /* VC4MEM_H_ */
