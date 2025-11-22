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
#include "flat_slotmap.h"

static
void flat_parse_nvpair
  (gpeg_capture_t* c, ytf_t* ytf)
{
}

/**
 *
 */
ytf_t* flat_parse
  (gpeg_capture_t* c)
{
  switch (c->type) {
  case SLOT_YTF:
    break;
  case SLOT_NEWLINE:
    break;
  case SLOT_SPACE:
    break;
  case SLOT_MULTILINECOMMENT:
    break;
  case SLOT_COMMENT:
    break;
  case SLOT_OBJECTS:
    break;
  case SLOT_SEPARATOR:
    break;
  case SLOT_OBJECT:
    {
      ytf_t* ytf = calloc(1, sizeof(ytf_t));
    }
    break;
  case SLOT_NVPAIR:
    flat_parse_nvpair(c, ytf);
    break;
  case SLOT_NAME:
    break;
  case SLOT_NAMEELT:
    break;
  case SLOT_DOT:
    break;
  case SLOT_COLON:
    break;
  case SLOT_VALUE:
    break;
  case SLOT_NULL:
    break;
  case SLOT_BOOL:
    break;
  case SLOT_FLOAT:
    break;
  case SLOT_INT:
    break;
  case SLOT_STRING:
    break;
  case SLOT_BLOB:
    break;
  case SLOT_END:
    break;
  }
  return ytf;
}
