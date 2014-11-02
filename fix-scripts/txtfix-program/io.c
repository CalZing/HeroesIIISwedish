#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>


#define READ_SIZE 4096

/*
 * Reads filename into a newly allocated buffer. Returns NULL on error
 */
void *file2buf(const char *filename, ssize_t *size)
{
  int fd = STDIN_FILENO;

  ssize_t read_bytes;
  ssize_t read_pos = 0;
  void *buf;
  void *ret = NULL;

  if (filename) {
    fd = open(filename, O_RDONLY);
    if (fd < 0) {
      perror("Error opening file: ");
      goto out;
    }
  }

  buf = malloc(READ_SIZE);
  if (!buf) {
    perror("Could not allocate input buffer: ");
    goto out_close;
  }

  while (1) {
    buf = realloc(buf, read_pos + READ_SIZE);
    read_bytes = read(fd, buf + read_pos, READ_SIZE);
    if (read_bytes < 0 && errno != EAGAIN) {
      goto out_free;
      perror("Error reading file: ");
      /* EOF */
    } else if (read_bytes == 0) {
      break;
    }
    read_pos += read_bytes;
  }

  ret = buf;
  *size = read_pos;
out_close:
  close(fd);
out:
  return ret;
out_free:
  free(buf);
  close(fd);
  return NULL;
}

/*
 * Writes filename with data from buf. Returns 0 on success and -1 on error
 */
int buf2file(const char *filename, void *buf, ssize_t buflen)
{
  int fd = STDOUT_FILENO;

  ssize_t bytes_left, written_bytes;
  int ret = -1;

  if (filename ) {
    fd = open(filename, O_CREAT | O_TRUNC | O_WRONLY, 0666);
    if (fd < 0) {
      perror("Error opening file: ");
      goto out;
    }
  }

  bytes_left = buflen;
  while (bytes_left) {
    written_bytes = write(fd, buf, bytes_left);
    if (written_bytes < 0) {
      perror("Error writing file: ");
      goto out_close;
    }
    bytes_left -= written_bytes;
  }
  ret = 0;

out_close:
  close(fd);
out:
  return ret;
}
