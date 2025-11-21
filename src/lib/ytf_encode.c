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

//#define YTF_HIGHENDIAN

/**
 * 
 */
void ytf_encode_bit
  (ytf_parse_t* ytf, unsigned bit)
{
  if (ytf->bitoffset == 0) {
    vec_appendchr(&(ytf->buf), 0);
  }
  if (bit) {
#ifdef _YTF_HIGHENDIAN
    ytf->buf.data[ ytf->buf.size - 1 ] |= (1 << (7 - ytf->bitoffset));
#else
    ytf->buf.data[ ytf->buf.size - 1 ] |= (1 << ytf->bitoffset);
#endif
  }
  (ytf->bitoffset)++;
  (ytf->bitoffset) %= 8;
}

/**
 *
 */
void ytf_decode_bit
  (ytf_parse_t* ytf, unsigned* bit)
{
  if (ytf->byteoffset == ytf->buf.size) {
    *bit = 0;
    ytf->eof = 1;
    return;
  }
  ytf->eof = 0;
#ifdef _YTF_HIGHENDIAN
  *bit = (ytf->buf.data[ ytf->byteoffset ] >> (7 - ytf->bitoffset)) & 0x01;
#else
  *bit = (ytf->buf.data[ ytf->byteoffset ] >> ytf->bitoffset) & 0x01;
#endif
  ++(ytf->bitoffset);
  if (ytf->bitoffset > 7) {
    ytf->bitoffset = 0;
    ++(ytf->byteoffset);
  }
}

/**
 * 
 */
void ytf_encode_bits
  (ytf_parse_t* ytf, unsigned bits, unsigned nbits)
{
  for (unsigned i=0; i < nbits; i++) {
    ytf_encode_bit(ytf, ((bits >> i) & 0x01));
  }
}

/**
 *
 */
void ytf_decode_bits
  (ytf_parse_t* ytf, unsigned* bits, unsigned nbits)
{
  unsigned bit = 0;

  *bits = 0;
  for (unsigned i=0; i < nbits; i++) {
    ytf_decode_bit(ytf, &bit);
    if (bit) {
      *bits |= (1 << i);
    }
  }
}

/**
 * 
 */
void ytf_encode_type
  (ytf_parse_t* ytf, unsigned type)
{
  ytf_encode_bits(ytf, type, 3);
}

/**
 *
 */
void ytf_decode_type
  (ytf_parse_t* ytf, unsigned* type)
{
  ytf_decode_bits(ytf, type, 3);
}

/**
 * 
 */
void ytf_encode_byte
  (ytf_parse_t* ytf, unsigned char B)
{
  ytf_encode_bits(ytf, B, 8);
}

/**
 *
 */
void ytf_decode_byte
  (ytf_parse_t* ytf, unsigned char* B)
{
  ytf_decode_bits(ytf, (unsigned*)B, 8);
}
