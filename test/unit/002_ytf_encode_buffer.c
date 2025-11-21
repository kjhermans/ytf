/**
 * This file is part of the YTF project.

Copyright (c) 2024, Kees-Jan Hermans <kees.jan.hermans@gmail.com>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the organization nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL Kees-Jan Hermans BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 *
 * \file
 * \brief
 */

#include <ytf/ytf.h>
#include <stdint.h>

/**
 *
 */
int main
  (int argc, char* argv[])
{
  (void)argc;
  (void)argv;
  char* tests[] = {
    "QXZK",
    "QXZKQXKZ",
    "QXZKQXKZQXKZ",
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ",
    "'t Was the best of times, 't was the worst of times.",
    "HaVe I GoT NeWs FoR YoU",
    NULL
  };
  unsigned i = 0;
  unsigned enclen;

  while (1) {
    ytf_parse_t ytf = { 0 };
    vec_t compare = { 0 };
    char* test = tests[ i++ ];

    if (test == NULL) {
      break;
    }

    ytf_encode_buffer(&ytf, (unsigned char*)test, strlen(test));
    enclen = ytf.buf.size;
    ytf.byteoffset = 0;
    ytf.bitoffset = 0;
    ytf_decode_buffer(&ytf, &(compare.data), &(compare.size));
    if (compare.size == strlen(test) &&
        0 == memcmp(test, compare.data, compare.size))
    {
      fprintf(stderr,
        "Test encode_buffer %s succeeded (compression %u -> %u).\n"
        , test
        , (unsigned)strlen(test)
        , enclen
      );
    } else {
      fprintf(stderr,
        "Test encode_buffer %s failed. Got: ", test
      );
      flogmem(stderr, ytf.buf.data, ytf.buf.size);
      flogmem(stderr, compare.data, compare.size);
      return ~0;
    }
  }
  return 0;
}
