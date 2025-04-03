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

/**
 *
 */
void ytf_encode_json
  (ytf_t* ytf, json_t* json)
{
  switch (json->type) {
  case JSON_TYPE_NULL:
    ytf_encode_type(ytf, YTF_TYPE_NULL);
    break;
  case JSON_TYPE_BOOLEAN:
    ytf_encode_type(ytf, YTF_TYPE_BOOLEAN);
    ytf_encode_bit(ytf, (json->value.boolint ? 1 : 0));
    break;
  case JSON_TYPE_INTEGER:
    ytf_encode_type(ytf, YTF_TYPE_INTEGER);
    ytf_encode_int(ytf, json->value.boolint);
    break;
  case JSON_TYPE_FLOAT:
    ytf_encode_type(ytf, YTF_TYPE_FLOAT);
    ytf_encode_float(ytf, json->value.fraction);
    break;
  case JSON_TYPE_STRING:
    ytf_encode_type(ytf, YTF_TYPE_STRING);
    ytf_encode_string(ytf, &(json->value.string));
    break;
  case JSON_TYPE_ARRAY:
    ytf_encode_type(ytf, YTF_TYPE_ARRAY);
    ytf_encode_array(ytf, json);
    break;
  case JSON_TYPE_HASHTABLE:
    ytf_encode_type(ytf, YTF_TYPE_HASHTABLE);
    ytf_encode_hashtable(ytf, json);
    break;
  }
}

void ytf_decode_json
  (ytf_t* ytf, json_t* json)
{
  unsigned type = 0;
  unsigned bool = 0;

  ytf_decode_type(ytf, &type);
  switch (type) {
  default:
  case YTF_TYPE_NULL:
    json->type = JSON_TYPE_NULL;
    break;
  case YTF_TYPE_BOOLEAN:
    ytf_decode_bit(ytf, &bool);
    json->type = JSON_TYPE_BOOLEAN;
    json->value.boolint = bool;
    break;
  case YTF_TYPE_INTEGER:
    ytf_decode_int(ytf, &(json->value.boolint));
    json->type = JSON_TYPE_INTEGER;
    break;
  case YTF_TYPE_FLOAT:
    ytf_decode_float(ytf, &(json->value.fraction));
    json->type = JSON_TYPE_FLOAT;
    break;
  case YTF_TYPE_STRING:
    ytf_decode_string(ytf, &(json->value.string));
    json->type = JSON_TYPE_STRING;
    break;
  case YTF_TYPE_ARRAY:
    ytf_decode_array(ytf, json);
    json->type = JSON_TYPE_ARRAY;
    break;
  case YTF_TYPE_HASHTABLE:
    ytf_decode_hashtable(ytf, json);
    json->type = JSON_TYPE_HASHTABLE;
    break;
  }
}

void ytf_encode_array
  (ytf_t* ytf, json_t* array)
{
  for (unsigned i=0; i < array->value.array.count; i++) {
    ytf_encode_bit(ytf, 1);
    ytf_encode_json(ytf, array->value.array.list[ i ]);
  }
  ytf_encode_bit(ytf, 0);
}

void ytf_decode_array
  (ytf_t* ytf, json_t* array)
{
  while (!(ytf->eof)) {
    unsigned cont = 0;
    ytf_decode_bit(ytf, &cont);
    if (!cont) {
      break;
    }
    json_t* elt = json_new(JSON_TYPE_NULL);
    ytf_decode_json(ytf, elt);
    json_array_push(&(array->value.array), elt);
  }
}

void ytf_encode_hashtable
  (ytf_t* ytf, json_t* hashtable)
{
  for (unsigned i=0; i < hashtable->value.hashtable.count; i++) {
    ytf_encode_bit(ytf, 1);
    ytf_encode_string(ytf, &(hashtable->value.hashtable.keys[ i ]));
    ytf_encode_json(ytf, hashtable->value.hashtable.values[ i ]);
  }
  ytf_encode_bit(ytf, 0);
}

void ytf_decode_hashtable
  (ytf_t* ytf, json_t* hashtable)
{
  while (!(ytf->eof)) {
    unsigned cont = 0;
    ytf_decode_bit(ytf, &cont);
    if (!cont) {
      break;
    }
    json_string_t key = { 0 };
    json_t* value = json_new(JSON_TYPE_NULL);
    ytf_decode_string(ytf, &key);
    ytf_decode_json(ytf, value);
    if (json_hashtable_has(&(hashtable->value.hashtable), key)) {
      free(key.value.bytes);
      json_free(value);
      continue;
    }
    json_hashtable_put(&(hashtable->value.hashtable), key, value);
  }
}

void ytf_encode_string
  (ytf_t* ytf, json_string_t* string)
{
  if (string->wide) {
    ytf_encode_bit(ytf, 1);
    for (unsigned i=0; i < string->length; i++) {
      ytf_encode_bit(ytf, 1);
      ytf_encode_int(ytf, string->value.unicode[ i ]);
    }
    ytf_encode_bit(ytf, 0);
  } else {
    ytf_encode_bit(ytf, 0);
    ytf_encode_buffer(ytf, string->value.bytes, string->length);
  }
}

void ytf_decode_string
  (ytf_t* ytf, json_string_t* string)
{
  unsigned wide = 0, cont = 0;
  int64_t widechar = 0;

  ytf_decode_bit(ytf, &wide);
  if (wide) {
    while (!(ytf->eof)) {
      ytf_decode_bit(ytf, &cont);
      if (!cont) {
        break;
      }
      ytf_decode_int(ytf, &widechar);
      string->value.unicode =
        (uint32_t*)realloc(
          string->value.unicode,
          string->length + 2 * sizeof(uint32_t)
        );
      string->value.unicode[ string->length ] = widechar;
      string->value.unicode[ ++(string->length) ] = 0;
    }
  } else {
    ytf_decode_buffer(ytf, &(string->value.bytes), &(string->length));
  }
}
