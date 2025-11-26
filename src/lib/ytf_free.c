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
 * Frees NOT the pointer ytf, but everything underneath.
 */
void ytf_free
  (ytf_t* ytf)
{
  switch (ytf->type) {
  case YTF_TYPE_STRING:
    free(ytf->value.string.data);
    break;
  case YTF_TYPE_ARRAY:
    for (unsigned i=0; i < ytf->value.array.count; i++) {
      ytf_free(ytf->value.array.list[ i ]);
      free(ytf->value.array.list[ i ]);
    }
    free(ytf->value.array.list);
    break;
  case YTF_TYPE_HASHTABLE:
    for (unsigned i=0; i < ytf->value.hash.count; i++) {
      free(ytf->value.hash.keys[ i ]);
      ytf_free(ytf->value.hash.values[ i ]);
      free(ytf->value.hash.values[ i ]);
    }
    free(ytf->value.hash.keys);
    free(ytf->value.hash.values);
    break;
  }
  memset(ytf, 0, sizeof(ytf_t));
}
