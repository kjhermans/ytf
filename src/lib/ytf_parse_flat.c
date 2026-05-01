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
#include "flat_bytecode.h"

vec_t ytf_parse_error = { 0 };

int ytf_parse_flat
  (const vec_t* string, ytf_array_t* array)
{
  DEBUGFUNCTION
  ASSERT(string)
  ASSERT(obj)

  vec_t bytecode = { flat_byc, flat_byc_len };
  gpege_result_t result = { 0 };
  int e;

  ytf_parse_error.size = 0;
  gpeg_engine_set_maxinstr(0);
  if ((e = gpeg_engine_run(&bytecode, string, 0, &result)) != 0) {
    char* errs[] = GPEGE_ERR_STRINGS;
    vec_printf(&ytf_parse_error,
      "YTF parse flat ended in error: %s.\n", errs[ e ]
    );
    return ~0;
  } else if (!(result.success)) {
    unsigned yx[ 2 ] = { 0 };
    e = strxypos((char*)(string->data), result.maxinputptr, yx);
    vec_printf(&ytf_parse_error,
      "YTF flat parser ended in no match;\n"
      "Furthest input position reached: %u, which is line %u, character %u.\n"
      , result.maxinputptr
      , yx[ 0 ]
      , yx[ 1 ]
    );
    return ~0;
  } else {
    gpege_node_t* tree = gpeg_result_to_tree(&result);
    flat_parse(tree->children[ 0 ], array);
    gpeg_result_free(tree);
    return 0;
  }
}
