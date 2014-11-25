#ifndef _IO_H_
#define _IO_H_

void *file2buf(const char *filename, ssize_t *size);
int buf2file(const char *filename, void *buf, ssize_t buflen);

#endif
