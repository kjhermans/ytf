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

#include <ytf/lib.h>

/**
 *
 */
void ytf_encode_int
  (ytf_parse_t* ytf, int64_t n)
{
  unsigned i=0;

  if (n < 0) {
    n = 0 - n;
    ytf_encode_bit(ytf, 1);
  } else {
    ytf_encode_bit(ytf, 0);
  }
  for (; i < 8; i++) {
    unsigned char B = (n & 0xff);
    n >>= 8;
    ytf_encode_bit(ytf, 1);
    if (i < 7) {
      ytf_encode_byte(ytf, B);
    } else {
      ytf_encode_bits(ytf, B, 7);
    }
    if (0 == n) {
      break;
    }
  }
  if (i < 8) {
    ytf_encode_bit(ytf, 0);
  }
}

void ytf_decode_int
  (ytf_parse_t* ytf, int64_t* n)
{
  unsigned i=0;
  unsigned sign = 0;

  *n = 0;
  ytf_decode_bit(ytf, &sign);
  for (; i < 8; i++) {
    unsigned cont;
    ytf_decode_bit(ytf, &cont);
    if (!cont) {
      break;
    }
    uint64_t B = 0;
    if (i < 7) {
      ytf_decode_byte(ytf, (unsigned char*)(&B));
    } else {
      ytf_decode_bits(ytf, (unsigned*)(&B), 7);
    }
    *n |= (uint64_t)(B << (i*8));
  }
  if (sign) {
    *n = 0 - *n;
  }
}
