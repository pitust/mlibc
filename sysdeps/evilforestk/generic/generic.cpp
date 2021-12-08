#include <bits/ensure.h>
#include <mlibc/debug.hpp>
#include <mlibc/all-sysdeps.hpp>
#include <errno.h>
#include <dirent.h>
#include <fcntl.h>
#include <limits.h>


static inline void outb(uint16_t port, uint8_t val)
{
    asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}


extern "C" void frg_log(const char *message) {
	while (*message) {
        outb(0xe9, *message++);
    }
    outb(0xe9, '\n');
}

extern "C" void frg_panic(const char* msg) {
    mlibc::infoLogger() << "\e[31mfrg: panic: " << msg << frg::endlog;
    while (1) {}
}


namespace mlibc {

void sys_libc_log(const char *message) {
	while (*message) {
        outb(0xe9, *message++);
    }
    outb(0xe9, '\n');
}

void sys_libc_panic() {
    mlibc::infoLogger() << "\e[31mmlibc: panic!" << frg::endlog;
    while (1) {}
}

void sys_exit(int status) {
    sys_libc_log("Exiting...\n");
    sys_libc_panic();
}

int sys_open(const char *path, int flags, int *fd) {
    return -1;
}

int sys_close(int fd) {
    return 0;
}

int sys_read(int fd, void *buf, size_t count, ssize_t *bytes_read) {
    *bytes_read = 0;
    return -EINVAL;
}

int sys_write(int fd, const void *buf, size_t count, ssize_t *bytes_written) {
    if (fd == 1 || fd == 2) {
        for (size_t i = 0;i < count;i++) {
            outb(0xe9, ((const uint8_t*)buf)[i]);
        }
        *bytes_written = count;
        return 0;
    }
    return -EINVAL;
}

int sys_tcb_set(void *pointer) {
    return -ENOSYS;
}

int sys_vm_map(void *hint, size_t size, int prot, int flags, int fd, off_t offset, void **window) {
    return -ENOSYS;
}

int sys_anon_free(void *pointer, size_t size) {
    return -ENOSYS;
}

// TODO: Actually implement this
int sys_futex_wait(int *pointer, int expected){
    return 0;
}

int sys_futex_wake(int *pointer){
    return 0;
}

constexpr uint64_t POOLSZ = 1024 * 1024 * 8;
uint8_t pool[POOLSZ];
uint64_t pooloff = 0;

int sys_anon_allocate(size_t size, void **pointer) {
    uint64_t a = pooloff;
    pooloff += size;
    if (pooloff > POOLSZ) mlibc::sys_libc_panic();
    *pointer = &pool[a];
    return 0;
}

int sys_clock_get(int clock, time_t *secs, long *nanos) {
    return 0;
}

int sys_seek(int fd, off_t offset, int whence, off_t *new_offset) {
    return -EINVAL;
}

} // namespace mlibc
