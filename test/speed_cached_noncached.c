#include <vc4mem.h>
#include <stdio.h>
#include <sys/time.h>

static double get_time()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return (double) t.tv_sec + t.tv_usec * 1e-6;
}

static void read_speed(const unsigned * const p, const size_t size)
{
    size_t i;
    double start, end;
    start = get_time();
    for (i = 0; i < size / 4; i ++)
        *((volatile unsigned*) p);
    end = get_time();
    printf("%g [s], %g [B/s]\n", end - start, size / (end - start));
}

int main()
{
    int i;
    unsigned *p1_user, *p2_user;
    unsigned p1_bus, p2_bus;
    const unsigned size = 65536 * 512;
    struct vc4mem_config cfg1, cfg2;

    vc4mem_clear_config(&cfg1);
    cfg1.is_noncached = 0;
    if (vc4mem_init(&cfg1))
        return 1;

    vc4mem_clear_config(&cfg2);
    cfg2.is_noncached = 1;
    if (vc4mem_init(&cfg2))
        return 1;

    p1_user = vc4mem_malloc(size, &p1_bus, &cfg1);
    if (p1_user == NULL)
        return 1;
    p2_user = vc4mem_malloc(size, &p2_bus, &cfg2);
    if (p2_user == NULL)
        return 1;

    printf("Using size=%g [B]. Measuring 4 times each.\n", (double) size);
    for (i = 0; i < 4; i ++) {
        printf("cached #%d:     ", i);
        read_speed(p1_user, size);
        printf("non-cached #%d: ", i);
        read_speed(p2_user, size);
    }

    if (vc4mem_free(p1_user, p1_bus, size, &cfg1))
        return 1;
    if (vc4mem_free(p2_user, p2_bus, size, &cfg2))
        return 1;

    if (vc4mem_finalize(&cfg1))
        return 1;
    if (vc4mem_finalize(&cfg2))
        return 1;

    return 0;
}
