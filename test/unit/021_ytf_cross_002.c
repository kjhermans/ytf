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

  char* test = "#foo\n"
               "bar:b64(AAAAAAAAAAAA)\n"
               "#test\n"
               "oi:3.141592\n";

  vec_t        stage0_flat   = { (unsigned char*)test, strlen(test) };
  ytf_array_t  stage0_ytf    = { 0 };
  ytf_parse_t  stage0_format = { 0 };
  vec_t        stage1_bin    = { 0 };
  ytf_t        stage1_ytf    = { 0 };
  vec_t        stage2_flat   = { 0 };
  vec_t        stage3_bin    = { 0 };
  vec_t        stage4_flat   = { 0 };

  if (ytf_parse_flat(&stage0_flat, &stage0_ytf)) {
    fprintf(stderr, "Stage0 parse flat failed.\n");
    exit(1);
  }
  for (unsigned i=0; i < stage0_ytf.count; i++) {
    ytf_format_bin_continuous(stage0_ytf.list[ i ], &stage0_format);
  }

  stage1_bin = stage0_format.buf;
  ytf_parse_bin(&stage1_bin, &stage1_ytf);

  ytf_format_flat(&stage1_ytf, &stage2_flat);
  fprintf(stderr, "%s", stage2_flat.data);

  return 0;
}
