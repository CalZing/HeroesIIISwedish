#ifndef _OPERATIONS_H_
#define _OPERATIONS_H_

void *do_linebreak(const char *indata, ssize_t inlen, const char *linedata, ssize_t linelen, ssize_t *outlen);

int do_linecmp(const char *indata, ssize_t inlen, const char *linedata, ssize_t linelen);

void *do_to_latin1(const unsigned char *indata, ssize_t inlen, ssize_t *outlen);

void *do_to_utf8(const unsigned char *indata, ssize_t inlen, ssize_t *outlen);
#endif
