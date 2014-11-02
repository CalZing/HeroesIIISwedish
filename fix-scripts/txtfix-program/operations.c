#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Find next linebreak from offset before max_len. Return max_len if not found
 */
static ssize_t find_next_lb(const char *data, ssize_t offset, ssize_t max_len)
{
  while (offset < max_len) {
    if (!strncmp(data + offset, "\r\n", 2) || !strncmp(data + offset, "\n", 1))
      return offset;
    offset++;
  }
  return max_len;
}


/* Replace all linebreaks in indata with the ones in linedata. */
void *do_linebreak(const char *indata, ssize_t inlen, const char *linedata, ssize_t linelen, ssize_t *outlen)
{
  ssize_t inpos, linepos;
  ssize_t prev_inpos;
  ssize_t outpos;
  char *outbuf;
  void *ret = NULL;

  inpos = 0;
  prev_inpos = 0;
  linepos = 0;
  outpos = 0;

  outbuf = malloc(inlen * 2);
  if (!outbuf) {
    perror("Could not allocate output buffer");
    goto out;
  }

  while (1) {
    prev_inpos = inpos;
    inpos = find_next_lb(indata, inpos, inlen);
    linepos = find_next_lb(linedata, linepos, linelen);

    if (inpos == inlen || linepos == linelen)
      break;

    /* Copy all non-linebreak characters from input to output file */
    memcpy(outbuf + outpos, indata + prev_inpos, inpos - prev_inpos);
    outpos += inpos - prev_inpos;

    /* Skip linebreak from input file */
    if (!strncmp(indata + inpos, "\r\n", 2)) {
      inpos += 2;
    } else if (!strncmp(indata + inpos, "\n", 1)) {
      inpos++;
    }

    /* Copy linebreak from linefile */
    if (!strncmp(linedata + linepos, "\r\n", 2)) {
      outbuf[outpos++] = '\r';
      outbuf[outpos++] = '\n';
      linepos += 2;
    } else if (!strncmp(linedata + linepos, "\n", 1)) {
      outbuf[outpos++] = '\n';
      linepos++;
    }
  }

  *outlen = outpos;
  ret = outbuf;

out:
  return ret;
}

int do_linecmp(const char *indata, ssize_t inlen, const char *linedata, ssize_t linelen)
{
  ssize_t inpos, linepos;

  inpos = 0;
  linepos = 0;

  while (inpos < inlen && linepos < linelen) {
    inpos = find_next_lb(indata, inpos, inlen);
    linepos = find_next_lb(linedata, linepos, linelen);

    /* Skip linebreaks from input file */
    if (!strncmp(indata + inpos, "\r\n", 2)) {
      if (strncmp(linedata + linepos, "\r\n", 2))
        goto out_nonmatch;
      inpos += 2;
      linepos += 2;
    } else if (indata[inpos] == '\n') {
      if (linedata[linepos] != '\n')
        goto out_nonmatch;
      inpos++;
      linepos++;
    } else {
      goto out_nonmatch;
    }
  }

  if (inpos != inlen || linepos != linelen) {
    fprintf(stderr, "Number of lines does not match!\n");
  }
  return EXIT_SUCCESS;

out_nonmatch:
  fprintf(stderr, "Linebreaks don't match!\n");
  return EXIT_FAILURE;

}

void *do_to_latin1(const unsigned char *indata, ssize_t inlen, ssize_t *outlen) {
  ssize_t inpos, outpos;
  unsigned char *outdata;

  void *ret = NULL;
  inpos = 0;
  outpos = 0;

  outdata = malloc(inlen);
  if (!outdata) {
    perror("Could not allocate memory: \n");
    goto out;
  }

  while (inpos < inlen) {
    if (indata[inpos] == 0xC3) {
      inpos++;
      switch (indata[inpos]) {
        case 0x84:
          outdata[outpos++] = 0xC4; break;
        case 0x85:
          outdata[outpos++] = 0xC5; break;
        case 0x96:
          outdata[outpos++] = 0xD6; break;
        case 0xA4:
          outdata[outpos++] = 0xE4; break;
        case 0xA5:
          outdata[outpos++] = 0xE5; break;
        case 0xB6:
          outdata[outpos++] = 0xF6; break;
        case 0xA9:
          outdata[outpos++] = 0xE9; break;
        case 0x89:
          outdata[outpos++] = 0xC9; break;
        default:
          fprintf(stderr, "Unrecognized Unicode character 0xC0%2x\n", indata[inpos]);
          goto out_free;
      }
      inpos++;
    } else {
      outdata[outpos++] = indata[inpos++];
    }
  }

  ret = outdata;
  *outlen = outpos;
out:
  return ret;
out_free:
  free(outdata);
  return NULL;
}

void *do_to_utf8(const unsigned char *indata, ssize_t inlen, ssize_t *outlen) {
  ssize_t inpos, outpos;
  unsigned char *outdata;

  inpos = 0;
  outpos = 0;

  outdata = malloc(inlen * 2);
  if (!outdata) {
    perror("Could not allocate memory: \n");
    return NULL;
  }

  while (inpos < inlen) {
    switch (indata[inpos]) {
      case 0xC4:
        outdata[outpos++] = 0xC3;
        outdata[outpos++] = 0x84;
        break;
      case 0xC5:
        outdata[outpos++] = 0xC3;
        outdata[outpos++] = 0x85;
        break;
      case 0xD6:
        outdata[outpos++] = 0xC3;
        outdata[outpos++] = 0x96;
        break;
      case 0xE4:
        outdata[outpos++] = 0xC3;
        outdata[outpos++] = 0xA4;
        break;
      case 0xE5:
        outdata[outpos++] = 0xC3;
        outdata[outpos++] = 0xA5;
        break;
      case 0xF6:
        outdata[outpos++] = 0xC3;
        outdata[outpos++] = 0xB6;
        break;
      case 0xE9:
        outdata[outpos++] = 0xC3;
        outdata[outpos++] = 0xA9;
        break;
      case 0xC9:
        outdata[outpos++] = 0xC3;
        outdata[outpos++] = 0x89;
        break;
      default:
        outdata[outpos++] = indata[inpos];
        break;
    }
    inpos++;
  }

  *outlen = outpos;
  return outdata;
}
