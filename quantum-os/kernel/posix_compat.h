/*
 * Copyright 2025 Arbitrary Number Project Team
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 /*
 * POSIX Compatibility Layer for Quantum OS
 * 
 * Provides Linux system call compatibility while maintaining
 * Quantum Number precision and mathematical computation capabilities.
 * 
 * Copyright (C) 2025 Arbitrary Number Project Team
 * Licensed under Apache License 2.0
 */
#ifndef POSIX_COMPAT_H
#define POSIX_COMPAT_H

#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>
#include "quantum_number.h"
#include "quantix_qfs.h"
#include "quantum_syscalls.h"

/* POSIX System Call Numbers */
#define POSIX_SYS_READ          0
#define POSIX_SYS_WRITE         1
#define POSIX_SYS_OPEN          2
#define POSIX_SYS_CLOSE         3
#define POSIX_SYS_STAT          4
#define POSIX_SYS_FSTAT         5
#define POSIX_SYS_LSTAT         6
#define POSIX_SYS_POLL          7
#define POSIX_SYS_LSEEK         8
#define POSIX_SYS_MMAP          9
#define POSIX_SYS_MPROTECT      10
#define POSIX_SYS_MUNMAP        11
#define POSIX_SYS_BRK           12
#define POSIX_SYS_RT_SIGACTION  13
#define POSIX_SYS_RT_SIGPROCMASK 14
#define POSIX_SYS_RT_SIGRETURN  15
#define POSIX_SYS_IOCTL         16
#define POSIX_SYS_PREAD64       17
#define POSIX_SYS_PWRITE64      18
#define POSIX_SYS_READV         19
#define POSIX_SYS_WRITEV        20
#define POSIX_SYS_ACCESS        21
#define POSIX_SYS_PIPE          22
#define POSIX_SYS_SELECT        23
#define POSIX_SYS_SCHED_YIELD   24
#define POSIX_SYS_MREMAP        25
#define POSIX_SYS_MSYNC         26
#define POSIX_SYS_MINCORE       27
#define POSIX_SYS_MADVISE       28
#define POSIX_SYS_SHMGET        29
#define POSIX_SYS_SHMAT         30
#define POSIX_SYS_SHMCTL        31
#define POSIX_SYS_DUP           32
#define POSIX_SYS_DUP2          33
#define POSIX_SYS_PAUSE         34
#define POSIX_SYS_NANOSLEEP     35
#define POSIX_SYS_GETITIMER     36
#define POSIX_SYS_ALARM         37
#define POSIX_SYS_SETITIMER     38
#define POSIX_SYS_GETPID        39
#define POSIX_SYS_SENDFILE      40
#define POSIX_SYS_SOCKET        41
#define POSIX_SYS_CONNECT       42
#define POSIX_SYS_ACCEPT        43
#define POSIX_SYS_SENDTO        44
#define POSIX_SYS_RECVFROM      45
#define POSIX_SYS_SENDMSG       46
#define POSIX_SYS_RECVMSG       47
#define POSIX_SYS_SHUTDOWN      48
#define POSIX_SYS_BIND          49
#define POSIX_SYS_LISTEN        50
#define POSIX_SYS_GETSOCKNAME   51
#define POSIX_SYS_GETPEERNAME   52
#define POSIX_SYS_SOCKETPAIR    53
#define POSIX_SYS_SETSOCKOPT    54
#define POSIX_SYS_GETSOCKOPT    55
#define POSIX_SYS_CLONE         56
#define POSIX_SYS_FORK          57
#define POSIX_SYS_VFORK         58
#define POSIX_SYS_EXECVE        59
#define POSIX_SYS_EXIT          60
#define POSIX_SYS_WAIT4         61
#define POSIX_SYS_KILL          62
#define POSIX_SYS_UNAME         63
#define POSIX_SYS_SEMGET        64
#define POSIX_SYS_SEMOP         65
#define POSIX_SYS_SEMCTL        66
#define POSIX_SYS_SHMDT         67
#define POSIX_SYS_MSGGET        68
#define POSIX_SYS_MSGSND        69
#define POSIX_SYS_MSGRCV        70
#define POSIX_SYS_MSGCTL        71
#define POSIX_SYS_FCNTL         72
#define POSIX_SYS_FLOCK         73
#define POSIX_SYS_FSYNC         74
#define POSIX_SYS_FDATASYNC     75
#define POSIX_SYS_TRUNCATE      76
#define POSIX_SYS_FTRUNCATE     77
#define POSIX_SYS_GETDENTS      78
#define POSIX_SYS_GETCWD        79
#define POSIX_SYS_CHDIR         80
#define POSIX_SYS_FCHDIR        81
#define POSIX_SYS_RENAME        82
#define POSIX_SYS_MKDIR         83
#define POSIX_SYS_RMDIR         84
#define POSIX_SYS_CREAT         85
#define POSIX_SYS_LINK          86
#define POSIX_SYS_UNLINK        87
#define POSIX_SYS_SYMLINK       88
#define POSIX_SYS_READLINK      89
#define POSIX_SYS_CHMOD         90
#define POSIX_SYS_FCHMOD        91
#define POSIX_SYS_CHOWN         92
#define POSIX_SYS_FCHOWN        93
#define POSIX_SYS_LCHOWN        94
#define POSIX_SYS_UMASK         95
#define POSIX_SYS_GETTIMEOFDAY  96
#define POSIX_SYS_GETRLIMIT     97
#define POSIX_SYS_GETRUSAGE     98
#define POSIX_SYS_SYSINFO       99
#define POSIX_SYS_TIMES         100

/* POSIX Error Codes */
#define POSIX_EPERM             1   /* Operation not permitted */
#define POSIX_ENOENT            2   /* No such file or directory */
#define POSIX_ESRCH             3   /* No such process */
#define POSIX_EINTR             4   /* Interrupted system call */
#define POSIX_EIO               5   /* I/O error */
#define POSIX_ENXIO             6   /* No such device or address */
#define POSIX_E2BIG             7   /* Argument list too long */
#define POSIX_ENOEXEC           8   /* Exec format error */
#define POSIX_EBADF             9   /* Bad file number */
#define POSIX_ECHILD            10  /* No child processes */
#define POSIX_EAGAIN            11  /* Try again */
#define POSIX_ENOMEM            12  /* Out of memory */
#define POSIX_EACCES            13  /* Permission denied */
#define POSIX_EFAULT            14  /* Bad address */
#define POSIX_ENOTBLK           15  /* Block device required */
#define POSIX_EBUSY             16  /* Device or resource busy */
#define POSIX_EEXIST            17  /* File exists */
#define POSIX_EXDEV             18  /* Cross-device link */
#define POSIX_ENODEV            19  /* No such device */
#define POSIX_ENOTDIR           20  /* Not a directory */
#define POSIX_EISDIR            21  /* Is a directory */
#define POSIX_EINVAL            22  /* Invalid argument */
#define POSIX_ENFILE            23  /* File table overflow */
#define POSIX_EMFILE            24  /* Too many open files */
#define POSIX_ENOTTY            25  /* Not a typewriter */
#define POSIX_ETXTBSY           26  /* Text file busy */
#define POSIX_EFBIG             27  /* File too large */
#define POSIX_ENOSPC            28  /* No space left on device */
#define POSIX_ESPIPE            29  /* Illegal seek */
#define POSIX_EROFS             30  /* Read-only file system */
#define POSIX_EMLINK            31  /* Too many links */
#define POSIX_EPIPE             32  /* Broken pipe */

/* POSIX File Descriptor Structure */
typedef struct {
    int fd;                         /* File descriptor number */
    qfs_file_handle_t qfs_handle;   /* QFS file handle */
    uint32_t flags;                 /* File flags */
    uint64_t offset;                /* Current file offset */
    bool is_quantum_file;           /* True if file contains Quantum Numbers */
    quantum_number_t *quantum_buffer; /* Buffer for Quantum Number operations */
    size_t buffer_size;             /* Size of quantum buffer */
    bool mathematical_mode;         /* Enable mathematical operations */
} posix_fd_t;

/* POSIX Process Structure */
typedef struct {
    pid_t pid;                      /* Process ID */
    pid_t ppid;                     /* Parent process ID */
    pid_t pgid;                     /* Process group ID */
    pid_t sid;                      /* Session ID */
    uid_t uid;                      /* User ID */
    gid_t gid;                      /* Group ID */
    char *cwd;                      /* Current working directory */
    posix_fd_t *fd_table;           /* File descriptor table */
    int max_fds;                    /* Maximum file descriptors */
    uint32_t signal_mask;           /* Signal mask */
    void *signal_handlers[64];      /* Signal handlers */
    quantum_number_t *env_vars;     /* Environment variables with Quantum Number support */
    int env_count;                  /* Number of environment variables */
    bool mathematical_process;      /* Process uses mathematical operations */
    quantum_number_t process_priority; /* Mathematical priority */
} posix_process_t;

/* POSIX Signal Structure */
typedef struct {
    int signum;                     /* Signal number */
    void (*handler)(int);           /* Signal handler */
    uint32_t flags;                 /* Signal flags */
    quantum_number_t precision_context; /* Mathematical precision context */
} posix_signal_t;

/* POSIX Memory Mapping Structure */
typedef struct {
    void *addr;                     /* Mapped address */
    size_t length;                  /* Mapping length */
    int prot;                       /* Protection flags */
    int flags;                      /* Mapping flags */
    int fd;                         /* File descriptor */
    off_t offset;                   /* File offset */
    bool quantum_mapping;           /* True if mapping contains Quantum Numbers */
    quantum_number_t *quantum_data; /* Quantum Number data */
} posix_mmap_t;

/* Function Prototypes */

/* Module initialization and cleanup */
int posix_compat_init(void);
void posix_compat_cleanup(void);

/* System call translation */
long posix_syscall_handler(long syscall_num, long arg1, long arg2, long arg3, 
                          long arg4, long arg5, long arg6);

/* File operations */
int posix_open(const char *pathname, int flags, mode_t mode);
int posix_close(int fd);
ssize_t posix_read(int fd, void *buf, size_t count);
ssize_t posix_write(int fd, const void *buf, size_t count);
off_t posix_lseek(int fd, off_t offset, int whence);
int posix_stat(const char *pathname, struct stat *statbuf);
int posix_fstat(int fd, struct stat *statbuf);
int posix_access(const char *pathname, int mode);
int posix_dup(int oldfd);
int posix_dup2(int oldfd, int newfd);

/* Process management */
pid_t posix_fork(void);
pid_t posix_vfork(void);
int posix_execve(const char *filename, char *const argv[], char *const envp[]);
void posix_exit(int status);
pid_t posix_wait4(pid_t pid, int *wstatus, int options, struct rusage *rusage);
pid_t posix_getpid(void);
pid_t posix_getppid(void);
int posix_kill(pid_t pid, int sig);

/* Memory management */
void *posix_mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
int posix_munmap(void *addr, size_t length);
int posix_mprotect(void *addr, size_t len, int prot);
void *posix_brk(void *addr);

/* Signal handling */
int posix_rt_sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);
int posix_rt_sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
int posix_rt_sigreturn(void);

/* Directory operations */
int posix_mkdir(const char *pathname, mode_t mode);
int posix_rmdir(const char *pathname);
int posix_chdir(const char *path);
char *posix_getcwd(char *buf, size_t size);
int posix_rename(const char *oldpath, const char *newpath);

/* File system operations */
int posix_link(const char *oldpath, const char *newpath);
int posix_unlink(const char *pathname);
int posix_symlink(const char *target, const char *linkpath);
ssize_t posix_readlink(const char *pathname, char *buf, size_t bufsiz);
int posix_chmod(const char *pathname, mode_t mode);
int posix_fchmod(int fd, mode_t mode);
int posix_chown(const char *pathname, uid_t owner, gid_t group);
int posix_fchown(int fd, uid_t owner, gid_t group);

/* Time operations */
int posix_gettimeofday(struct timeval *tv, struct timezone *tz);
int posix_nanosleep(const struct timespec *req, struct timespec *rem);

/* Quantum Number extensions */
int posix_quantum_read(int fd, quantum_number_t *qn_buf, size_t count);
int posix_quantum_write(int fd, const quantum_number_t *qn_buf, size_t count);
int posix_set_mathematical_mode(int fd, bool enable);
int posix_get_mathematical_precision(int fd, quantum_number_t *precision);

/* Process mathematical extensions */
int posix_set_process_mathematical_priority(quantum_number_t priority);
int posix_get_process_mathematical_context(quantum_number_t *context);

/* Error handling */
int posix_get_errno(void);
void posix_set_errno(int error);
const char *posix_strerror(int error);

/* Utility functions */
posix_process_t *posix_get_current_process(void);
posix_fd_t *posix_get_fd(int fd);
int posix_allocate_fd(void);
void posix_free_fd(int fd);

/* Mathematical file operations */
int posix_quantum_file_create(const char *pathname, quantum_number_t initial_value);
int posix_mathematical_file_operation(int fd, int operation, quantum_number_t *operand);

/* Shell integration */
int posix_shell_execute(const char *command, char *const argv[], char *const envp[]);
int posix_shell_set_env(const char *name, const quantum_number_t *value);
int posix_shell_get_env(const char *name, quantum_number_t *value);

/* Gaming optimizations */
int posix_set_realtime_priority(pid_t pid, int priority);
int posix_set_cpu_affinity(pid_t pid, const cpu_set_t *mask);
int posix_lock_memory(void *addr, size_t len);
int posix_unlock_memory(void *addr, size_t len);

/* Office application support */
int posix_document_processing_mode(bool enable);
int posix_mathematical_computation_offload(quantum_number_t *data, size_t count);

#endif /* POSIX_COMPAT_H */
