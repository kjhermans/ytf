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
#include <ytf/ytf_dictionary.h>

static
unsigned char dictionary[ ][ 4 ] = { YTF_DICTIONARY };

static
int ytf_dict_get
  (unsigned char b, unsigned char record[ 3 ])
{
  for (unsigned i=0; i < 256; i++) {
    if (dictionary[ i ][ 2 ] == 0) {
      break;
    }
    if (dictionary[ i ][ 0 ] == b) {
      if (record) {
        record[ 0 ] = dictionary[ i ][ 1 ];
        record[ 1 ] = dictionary[ i ][ 2 ];
        record[ 2 ] = dictionary[ i ][ 3 ];
      }
      return 1;
    }
  }
  return 0;
}

static
int ytf_dict_get_reverse
  (unsigned char l, unsigned char e, unsigned char* r)
{
  for (unsigned i=0; i < 256; i++) {
    if (dictionary[ i ][ 2 ] == 0) {
      break;
    }
    if (dictionary[ i ][ 1 ] == l && dictionary[ i ][ 3 ] == e) {
      if (r) {
        *r = dictionary[ i ][ 0 ];
      }
      return 1;
    }
  }
  return 0;
}

static
void ytf_encode_buffer_compressed
  (ytf_parse_t* ytf, vec_t* vec)
{
  ytf_encode_bit(ytf, 1); // continue
  ytf_encode_bit(ytf, 1); // compressed
  ytf_encode_bits(ytf, vec->size - 1, 6);
  for (unsigned i=0; i < vec->size; i++) {
    unsigned char record[ 3 ] = { 0 };
    ytf_dict_get(vec->data[ i ], record);
    ytf_encode_bits(ytf, record[ 0 ], 2);
    ytf_encode_bits(ytf, record[ 2 ], record[ 1 ]);
  }
}

static
void ytf_encode_buffer_uncompressed
  (ytf_parse_t* ytf, vec_t* vec)
{
  ytf_encode_bit(ytf, 1); // continue
  ytf_encode_bit(ytf, 0); // uncompressed
  ytf_encode_bits(ytf, vec->size - 1, 3);
  for (unsigned i=0; i < vec->size; i++) {
    ytf_encode_byte(ytf, vec->data[ i ]);
  }
}

/**
 *
 */
void ytf_encode_buffer
  (ytf_parse_t* ytf, unsigned char* buf, unsigned size)
{
  vec_t compr = { 0 };
  vec_t uncompr = { 0 };
  int iscompr = 0;
  int ischanged = 0;

  if (!size) {
    ytf_encode_bit(ytf, 0);
    return;
  }
  if (ytf_dict_get(buf[ 0 ], 0)) {
    iscompr = 1;
  }
  for (unsigned i=0; i < size; i++) {
    if (ytf_dict_get(buf[ i ], 0)) {
      if (!iscompr) {
        iscompr = 1;
        ischanged = 1;
      }
    } else {
      if (iscompr) {
        iscompr = 0;
        ischanged = 1;
      }
    }
    if (iscompr) {
      vec_appendchr(&compr, buf[ i ]);
    } else {
      vec_appendchr(&uncompr, buf[ i ]);
    }
    if (ischanged) {
      if (iscompr) {
        if (uncompr.size) {
          ytf_encode_buffer_uncompressed(ytf, &uncompr);
          uncompr.size = 0;
        }
      } else {
        if (compr.size) {
          ytf_encode_buffer_compressed(ytf, &compr);
          compr.size = 0;
        }
      }
    } else {
      if (iscompr && compr.size == 64) {
        ytf_encode_buffer_compressed(ytf, &compr);
        compr.size = 0;
      } else if (!iscompr && uncompr.size == 8) {
        ytf_encode_buffer_uncompressed(ytf, &uncompr);
        uncompr.size = 0;
      }
    }
    ischanged = 0;
  }
  if (compr.size) { 
    ytf_encode_buffer_compressed(ytf, &compr);
  }
  if (compr.data) {
    free(compr.data);
  }
  if (uncompr.size) {
    ytf_encode_buffer_uncompressed(ytf, &uncompr);
  }
  if (uncompr.data) {
    free(uncompr.data);
  }
  ytf_encode_bit(ytf, 0); // stop
}

/**
 * Callers will have to (optionally) relinquish *buf before calling,
 * and free *buf after return, because this function mallocs it.
 */
void ytf_decode_buffer
  (ytf_parse_t* ytf, unsigned char** buf, unsigned* size)
{
  vec_t result = { 0 };
  unsigned cont, compr, chunksize, B, Clen, Cpayload;
  unsigned char reverse = 0;

  while (!(ytf->eof))
  {
    ytf_decode_bit(ytf, &cont);
    if (!cont) {
      break;
    }
    ytf_decode_bit(ytf, &compr);
    if (compr) {
      ytf_decode_bits(ytf, &chunksize, 6);
      for (unsigned i=0; i < chunksize+1; i++) {
        ytf_decode_bits(ytf, &Clen, 2);
        ytf_decode_bits(ytf, &Cpayload, Clen+1);
        ytf_dict_get_reverse(Clen, Cpayload, &reverse);
        vec_appendchr(&result, reverse);
      }
    } else {
      ytf_decode_bits(ytf, &chunksize, 3);
      for (unsigned i=0; i < chunksize+1; i++) {
        ytf_decode_byte(ytf, (unsigned char*)(&B));
        vec_appendchr(&result, B);
      }
    }
  }
  *buf = result.data;
  *size = result.size;
}
