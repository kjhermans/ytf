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

static
ytf_t* ytf_parse_tuple
  (ytf_parse_t* p)
{
  unsigned type;
  unsigned tmp;
  ytf_t* ytf = calloc(1, sizeof(ytf_t));

  ytf_decode_type(p, &type);
  ytf->type = type;
  switch (type) {
  case 0:
  case YTF_TYPE_NULL:
    break;
  case YTF_TYPE_BOOLEAN:
    ytf_decode_bit(p, &tmp);
    ytf->value.boolint = tmp;
    break;
  case YTF_TYPE_INTEGER:
    ytf_decode_int(p, &(ytf->value.boolint));
    break;
  case YTF_TYPE_FLOAT:
    ytf_decode_float(p, &(ytf->value.fraction));
    break;
  case YTF_TYPE_STRING:
    ytf_decode_buffer(p, &(ytf->value.string.data), &(ytf->value.string.size));
    break;
  case YTF_TYPE_ARRAY:
    ytf_decode_bit(p, &tmp);
    while (tmp) {
      ytf_t* elt = ytf_parse_tuple(p);
      ytf_array_push(&(ytf->value.array), elt);
      ytf_decode_bit(p, &tmp);
    }
    break;
  case YTF_TYPE_HASHTABLE:
    ytf_decode_bit(p, &tmp);
    while (tmp) {
      vec_t key = { 0 };
      ytf_decode_buffer(p, &(key.data), &(key.size));
      ytf_t* value = ytf_parse_tuple(p);
      ytf_hash_put(&(ytf->value.hash), (char*)(key.data), value);
      ytf_decode_bit(p, &tmp);
    }
    break;
  }
  return ytf;
}

/**
 *
 */
void ytf_parse_bin
  (const vec_t* string, ytf_t* ytf)
{
  ytf_parse_t p = { .buf = *string };

  ytf->type = YTF_TYPE_ARRAY;
  while (!(p.eof)) {
    ytf_t* elt = ytf_parse_tuple(&p);
    ytf_array_push(&(ytf->value.array), elt);
  }
}
