/*

 *
 * (C) Copyright 2014, Andreas Irestaal
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */
#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <stdlib.h>

#include "io.h"
#include "operations.h"

static void
display_help(const char *procname)
{
  printf("Usage:\n"
      "txtfix [options] [operations]\n"
      "Perform various text operations on infile depending on options. If\n"
      "no options are specified, input will just be copied to output \n"
      "unmodified. Operations are executed in \n"
      "Options:\n"
      "-i --infile [file]    Use [file] instead of stdin\n"
      "-o --outfile [file]   Use [file] instead of stdout\n"
      "-l --linefile [file]  Use [file] for line input (see below)\n"
      "\n"
      "Operations:\n"
      "-b --linebreak        Alter line breaks to match linefile. All\n"
      "                      \\r\\n or \\n in input file will be replaced\n"
      "                      with the next line break in linefile\n"
      "-c --linecmp          Compare number of lines so that input and linefile\n"
      "                      matches in number of lines\n"
      "-t --to [coding]      Convert special symbols. Valid options are \n"
      "                      utf8 and latin1. Only swedish symbols supported\n"
      "-h --help             this message\n");
}

int
main(int argc, char **argv)
{
  int c;
  int res;
  int ret = EXIT_FAILURE;
  char *infile = NULL;
  char *outfile = NULL;
  char *linefile = NULL;
  char *procname = argv[0];

  void *indata;
  void *outdata;
  void *linedata;

  ssize_t inlen;
  ssize_t outlen;
  ssize_t linelen;

  int to_utf8 = 0;
  int to_latin1 = 0;
  int linebreak = 0;
  int linecmp = 0;

  outdata = NULL;
  linedata = NULL;

  outlen = 1;
  while (1) {
    int option_index = 0;
    const char *short_options = "bchi:l:t:o:";
    const struct option long_options[] = {
      {"help", no_argument, 0, 'h'},
      {"infile", required_argument, 0, 'i'},
      {"outfile", required_argument, 0, 'o'},
      {"linefile", required_argument, 0, 'l'},
      {"linebreak", no_argument, 0, 'b'},
      {"linecmp", no_argument, 0, 'c'},
      {"to", required_argument, 0, 't'},
      {0, 0, 0, 0},
    };

    c = getopt_long(argc, argv, short_options, long_options, &option_index);

    if (c == EOF)
      break;

    switch (c) {
    case 'h':
      display_help(procname);
      ret = EXIT_SUCCESS;
      goto out;
    case 'i':
      infile = optarg;
      break;
    case 'o':
      outfile = optarg;
      break;
    case 'l':
      linefile = optarg;
      break;
    case 'b':
      linebreak = 1;
      break;
    case 'c':
      linecmp = 1;
      break;
    case 't':
      if (!strcmp(optarg, "latin1")) {
        to_latin1 = 1;
      } else if (!strcmp(optarg, "utf8")) {
        to_utf8 = 1;
      } else {
        display_help(procname);
      }
      break;
    default:
      display_help(procname);
      goto out;
    }
  }

  if (argc < 2) {
    display_help(procname);
    ret = EXIT_SUCCESS;
    goto out;
  }

  indata = file2buf(infile, &inlen);
  if (!indata)
    goto out;

  if (linefile) {
    linedata = file2buf(linefile, &linelen);
    if (!linedata)
      goto out;
  }

  /* Done processing input and loading files... */

  /* If we do this, do nothing else */
  if (linecmp) {
    if (!linefile) {
      fprintf(stderr, "No linefile specified!\n");
      goto out;
    }
    ret = do_linecmp(indata, inlen, linedata, linelen);
    free(indata);
    goto out;
  }

  if (to_latin1) {
    outdata = do_to_latin1(indata, inlen, &outlen);
    if (!outdata)
      goto out;
    free(indata);
    indata = outdata;
    inlen = outlen;
  }

  if (to_utf8) {
    outdata = do_to_utf8(indata, inlen, &outlen);
    if (!outdata)
      goto out;
    free(indata);
    indata = outdata;
    inlen = outlen;
  }

  if (linebreak) {
    if (!linefile) {
      fprintf(stderr, "No linefile specified!\n");
      goto out;
    }
    outdata = do_linebreak(indata, inlen, linedata, linelen, &outlen);
    if (!outdata)
      goto out;
    free(indata);
    indata = outdata;
    inlen = outlen;
  }

  /* Write output (if any) */
  if (outdata) {
    res = buf2file(outfile, outdata, outlen);
    if (res < 0)
      goto out;
  }

  ret = EXIT_SUCCESS;
out:
  if (outdata)
    free (outdata);
  if (linedata)
    free (linedata);
  exit(ret);
}
