#ifndef LOCAL_H_
#define LOCAL_H_

    struct vc4mem_private_config {
        int mmap_fd;
        int vc4mem_fd;
    };

void vc4mem_pr_err_core(const char *file, const int line, const char *func,
        const char *fmt, ...);
#define pr_err(fmt, ...) \
        vc4mem_pr_err_core(__FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)

#endif /* LOCAL_H_ */
