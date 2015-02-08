#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

static struct timespec last_time = {0, 0};
static int (* send_real)(int, void *, size_t, int, void *, int) = NULL;

#define ENDPACKET 0x1e
#define NANO 1E9
#define ADJUSTMENT (62.21389 / 60.0)

static __attribute__((__constructor__)) void init(void) {
    send_real = dlsym(RTLD_NEXT, "send");
}

void handle(char packet) {
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);

    printf("[%ld.%ld] 0x%02x\n", time.tv_sec, time.tv_nsec, packet);

    if (packet == ENDPACKET) {
        double diff = (time.tv_sec - last_time.tv_sec) +
                      (time.tv_nsec - last_time.tv_nsec) / NANO;
        printf("diff: [%lf]\n", diff * ADJUSTMENT);
    }

    last_time = time;
}

ssize_t send(int fd, char *m, size_t len, int x, void *y, int z) {
    if (len == 4 && m && ((*m) > 0x1d)) {
        handle(*m);
    }
    return send_real(fd, m, len, x, y, z);
}
