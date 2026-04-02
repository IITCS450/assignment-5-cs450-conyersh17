#ifndef __ULIB_H__
#define __ULIB_H__

int printf(const char *fmt, ...);
int fork(void);
void exit(void);
int wait(void);
int kill(int pid);
int getpid(void);
int open(const char *path, int mode);
int read(int fd, void *buf, int n);
int write(int fd, void *buf, int n);
int close(int fd);
int unlink(const char *path);
int fstat(int fd, struct stat *st);
int stat(const char *path, struct stat *st);
int exec(const char *path, char *const argv[]);
int pipe(int fd[2]);
#endif