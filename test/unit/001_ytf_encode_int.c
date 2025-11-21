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
  int64_t tests[] = {
    0,
    1,
    -1,
    127,
    -127,
    255,
    -255,
    256,
    -256,
    2147483647,
    -2147483648,
    2147483648,
    4294967295,
    INT64_C(4294967296),
    77777777777777777,
    9223372036854775807,
    -9223372036854775807,
  };
  for (unsigned i=0; i < 17; i++) {
    ytf_parse_t ytf = { 0 };
    ytf_encode_int(&ytf, tests[ i ]);
    //flogmem(stderr, ytf.buf.data, ytf.buf.size);
    ytf.byteoffset = 0;
    ytf.bitoffset = 0;
    int64_t compare;
    ytf_decode_int(&ytf, &compare);
    if (tests[ i ] == compare) {
      fprintf(stderr, "Test encode_int %"PRId64" succeeded.\n", tests[ i ]);
    } else {
      fprintf(stderr,
        "Test encode_int %"PRId64" failed (got %"PRId64").\n"
        , tests[ i ]
        , compare
      );
      return ~0;
    }
  }
  return 0;
}
