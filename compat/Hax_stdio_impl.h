#ifndef _HAX_STDIO_IMPL_H
#define _HAX_STDIO_IMPL_H

#include "Hax_stdio.h"
#include "Hax_stdarg.h"

#ifndef SIZE_T_DEFINED
typedef __SIZE_TYPE__ size_t;
#define SIZE_T_DEFINED
#endif

#ifndef SSIZE_T_DEFINED
typedef __PTRDIFF_TYPE__ ssize_t;
#define SSIZE_T_DEFINED
#endif

#ifndef OFF_T_DEFINED
typedef __UINT64_TYPE__ off_t;
#define OFF_T_DEFINED
#endif

#define UNGET 8

#define FFINALLOCK(f)
#define FLOCK(f)
#define FUNLOCK(f)

#define F_PERM 1
#define F_NORD 4
#define F_NOWR 8
#define F_EOF 16
#define F_ERR 32
#define F_SVB 64
#define F_APP 128

struct _IO_FILE;
typedef struct _IO_FILE FILE;

struct _IO_FILE {
	unsigned flags;
	unsigned char *rpos, *rend;
	int (*close)(FILE *);
	unsigned char *wend, *wpos;
	unsigned char *mustbezero_1;
	unsigned char *wbase;
	size_t (*read)(FILE *, unsigned char *, size_t);
	size_t (*write)(FILE *, const unsigned char *, size_t);
	off_t (*seek)(FILE *, off_t, int);
	unsigned char *buf;
	size_t buf_size;
	FILE *prev, *next;
	int fd;
	int pipe_pid;
	long lockcount;
	int mode;
	volatile int lock;
	int lbf;
	void *cookie;
	off_t off;
	char *getln_buf;
	void *mustbezero_2;
	unsigned char *shend;
	off_t shlim, shcnt;
	FILE *prev_locked, *next_locked;
	struct __locale_struct *locale;
};

extern FILE *volatile __stdin_used;
extern FILE *volatile __stdout_used;
extern FILE *volatile __stderr_used;

extern FILE *const Hax_stdin;
extern FILE *const Hax_stdout;
extern FILE *const Hax_stderr;

#define stdin  (Hax_stdin)
#define stdout (Hax_stdout)
#define stderr (Hax_stderr)

int __lockfile(FILE *);
void __unlockfile(FILE *);

size_t Hax___stdio_read(FILE *, unsigned char *, size_t);
size_t Hax___stdio_write(FILE *, const unsigned char *, size_t);
size_t Hax___stdout_write(FILE *, const unsigned char *, size_t);
off_t Hax___stdio_seek(FILE *, off_t, int);
int Hax___stdio_close(FILE *);

size_t Hax___string_read(FILE *, unsigned char *, size_t);

int Hax___toread(FILE *);
int Hax___towrite(FILE *);

void __stdio_exit(void);
void __stdio_exit_needed(void);

#if defined(__PIC__) && (100*__GNUC__+__GNUC_MINOR__ >= 303)
__attribute__((visibility("protected")))
#endif
int __overflow(FILE *, int), Hax___uflow(FILE *);

int __fseeko(FILE *, off_t, int);
int __fseeko_unlocked(FILE *, off_t, int);
off_t __ftello(FILE *);
off_t __ftello_unlocked(FILE *);
size_t Hax___fwritex(const unsigned char *, size_t, FILE *);
int __putc_unlocked(int, FILE *);

FILE *__fdopen(int, const char *);
int __fmodeflags(const char *);

FILE *__ofl_add(FILE *f);
FILE **__ofl_lock(void);
void __ofl_unlock(void);

struct __pthread;
void __register_locked_file(FILE *, struct __pthread *);
void __unlist_locked_file(FILE *);
void __do_orphaned_stdio_locks(void);

#define MAYBE_WAITERS 0x40000000

void __getopt_msg(const char *, const char *, const char *, size_t);

#define feof(f) ((f)->flags & F_EOF)
#define ferror(f) ((f)->flags & F_ERR)

#define getc_unlocked(f) \
	( ((f)->rpos != (f)->rend) ? *(f)->rpos++ : Hax___uflow((f)) )

#define putc_unlocked(c, f) \
	( (((unsigned char)(c)!=(f)->lbf && (f)->wpos!=(f)->wend)) \
	? *(f)->wpos++ = (unsigned char)(c) \
	: __overflow((f),(unsigned char)(c)) )

/* Caller-allocated FILE * operations */
FILE *__fopen_rb_ca(const char *, FILE *, unsigned char *, size_t);
int __fclose_ca(FILE *);

#define BUFSIZ 1024

int Hax_vfprintf(FILE * restrict stream, const char * restrict format, va_list ap);
int Hax_vsprintf(char * restrict str, const char * restrict format, va_list ap);
int Hax_vsprintf(char * restrict str, const char * restrict format, va_list ap);
int Hax_vsnprintf(char * restrict str, size_t, const char * restrict format, va_list ap);
int Hax_vsscanf(const char * restrict str, const char * restrict format, va_list ap);
int Hax_vfscanf(FILE * restrict stream, const char * restrict format, va_list ap);

size_t strnlen(const char *s, size_t maxlen);
void *memchr(const void *b, int c, size_t len);

struct iovec { void *iov_base; size_t iov_len; };

#endif
