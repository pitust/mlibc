
#ifndef _DIRENT_H
#define _DIRENT_H

#include <bits/posix/ino_t.h>
#include <bits/posix/off_t.h>

#ifdef __cplusplus
extern "C" {
#endif

#define DT_UNKNOWN 0
#define DT_FIFO 1
#define DT_CHR 2
#define DT_DIR 4
#define DT_BLK 6
#define DT_REG 8
#define DT_LNK 10
#define DT_SOCK 12
#define DT_WHT 14

struct __mlibc_Dir { };
typedef struct __mlibc_Dir DIR;

struct dirent {
	ino_t d_ino;
	off_t d_off;
	unsigned short d_reclen;
	unsigned char d_type;
	char d_name[1024];
};

int alphasort(const struct dirent **, const struct dirent **);
int closedir(DIR *);
int dirfd(DIR *);
DIR *fdopendir(int);
DIR *opendir(const char *);
struct dirent *readdir(DIR *);
int readdir_r(DIR *__restrict, struct dirent *__restrict, struct dirent **__restrict);
void rewinddir(DIR *);
int scandir(const char *, struct dirent ***, int (*)(const struct dirent *),
		int (*)(const struct dirent **, const struct dirent **));
void seekdir(DIR *, long);
long telldir(DIR *);

#ifdef __cplusplus
}
#endif

#endif // _DIRENT_H

