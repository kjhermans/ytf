#include <gpeg/lib/gpeg/gpeg_capturelist.h>

#ifdef _DEBUG
#include <stdio.h>
#endif

extern int flat_grammar_process_node(gpeg_capture_t*,void*);
static int do_node(gpeg_capture_t*,unsigned,gpeg_capture_t*,void*);

extern int flat_handle_YTF(gpeg_capture_t*,unsigned,gpeg_capture_t*,void*);
extern int flat_handle_post_YTF(gpeg_capture_t*,unsigned,gpeg_capture_t*,void*);
extern int flat_handle_NEWLINE(gpeg_capture_t*,unsigned,gpeg_capture_t*,void*);
extern int flat_handle_post_NEWLINE(gpeg_capture_t*,unsigned,gpeg_capture_t*,void*);
extern int flat_handle_SPACE(gpeg_capture_t*,unsigned,gpeg_capture_t*,void*);
extern int flat_handle_post_SPACE(gpeg_capture_t*,unsigned,gpeg_capture_t*,void*);
extern int flat_handle_MULTILINECOMMENT(gpeg_capture_t*,unsigned,gpeg_capture_t*,void*);
extern int flat_handle_post_MULTILINECOMMENT(gpeg_capture_t*,unsigned,gpeg_capture_t*,void*);
extern int flat_handle_COMMENT(gpeg_capture_t*,unsigned,gpeg_capture_t*,void*);
extern int flat_handle_post_COMMENT(gpeg_capture_t*,unsigned,gpeg_capture_t*,void*);
extern int flat_handle_OBJECTS(gpeg_capture_t*,unsigned,gpeg_capture_t*,void*);
extern int flat_handle_post_OBJECTS(gpeg_capture_t*,unsigned,gpeg_capture_t*,void*);
extern int flat_handle_SEPARATOR(gpeg_capture_t*,unsigned,gpeg_capture_t*,void*);
extern int flat_handle_post_SEPARATOR(gpeg_capture_t*,unsigned,gpeg_capture_t*,void*);
extern int flat_handle_OBJECT(gpeg_capture_t*,unsigned,gpeg_capture_t*,void*);
extern int flat_handle_post_OBJECT(gpeg_capture_t*,unsigned,gpeg_capture_t*,void*);
extern int flat_handle_NVPAIR(gpeg_capture_t*,unsigned,gpeg_capture_t*,void*);
extern int flat_handle_post_NVPAIR(gpeg_capture_t*,unsigned,gpeg_capture_t*,void*);
extern int flat_handle_NAME(gpeg_capture_t*,unsigned,gpeg_capture_t*,void*);
extern int flat_handle_post_NAME(gpeg_capture_t*,unsigned,gpeg_capture_t*,void*);
extern int flat_handle_COLON(gpeg_capture_t*,unsigned,gpeg_capture_t*,void*);
extern int flat_handle_post_COLON(gpeg_capture_t*,unsigned,gpeg_capture_t*,void*);
extern int flat_handle_VALUE(gpeg_capture_t*,unsigned,gpeg_capture_t*,void*);
extern int flat_handle_post_VALUE(gpeg_capture_t*,unsigned,gpeg_capture_t*,void*);
extern int flat_handle_NULL(gpeg_capture_t*,unsigned,gpeg_capture_t*,void*);
extern int flat_handle_post_NULL(gpeg_capture_t*,unsigned,gpeg_capture_t*,void*);
extern int flat_handle_BOOL(gpeg_capture_t*,unsigned,gpeg_capture_t*,void*);
extern int flat_handle_post_BOOL(gpeg_capture_t*,unsigned,gpeg_capture_t*,void*);
extern int flat_handle_FLOAT(gpeg_capture_t*,unsigned,gpeg_capture_t*,void*);
extern int flat_handle_post_FLOAT(gpeg_capture_t*,unsigned,gpeg_capture_t*,void*);
extern int flat_handle_INT(gpeg_capture_t*,unsigned,gpeg_capture_t*,void*);
extern int flat_handle_post_INT(gpeg_capture_t*,unsigned,gpeg_capture_t*,void*);
extern int flat_handle_STRING(gpeg_capture_t*,unsigned,gpeg_capture_t*,void*);
extern int flat_handle_post_STRING(gpeg_capture_t*,unsigned,gpeg_capture_t*,void*);
extern int flat_handle_BLOB(gpeg_capture_t*,unsigned,gpeg_capture_t*,void*);
extern int flat_handle_post_BLOB(gpeg_capture_t*,unsigned,gpeg_capture_t*,void*);
extern int flat_handle_END(gpeg_capture_t*,unsigned,gpeg_capture_t*,void*);
extern int flat_handle_post_END(gpeg_capture_t*,unsigned,gpeg_capture_t*,void*);

int flat_grammar_process_node
  (
    gpeg_capture_t* capture,
    void* ptr
  )
{
  return do_node(0, 0, capture, ptr);
}

static
int do_node
  (
    gpeg_capture_t* parent,
    unsigned index,
    gpeg_capture_t* capture,
    void* ptr
  )
{
  int e;
  unsigned indices[ 19 ] = { 0 };

  switch (capture->type) {
  case 0:
    {
      ++indices[ 0 ];
      if ((e = flat_handle_YTF(parent, index, capture, ptr)) != 0) {
        return e;
      }
      for (unsigned i=0; i < capture->children.count; i++) {
        if ((e = do_node(capture, indices[ 0 ], &(capture->children.list[ i ]), ptr)) != 0) {
          return e;
        }
      }
      if ((e = flat_handle_post_YTF(parent, index, capture, ptr)) != 0) {
        return e;
      }
    }
    break;
  case 1:
    {
      ++indices[ 1 ];
      if ((e = flat_handle_NEWLINE(parent, index, capture, ptr)) != 0) {
        return e;
      }
      for (unsigned i=0; i < capture->children.count; i++) {
        if ((e = do_node(capture, indices[ 1 ], &(capture->children.list[ i ]), ptr)) != 0) {
          return e;
        }
      }
      if ((e = flat_handle_post_NEWLINE(parent, index, capture, ptr)) != 0) {
        return e;
      }
    }
    break;
  case 2:
    {
      ++indices[ 2 ];
      if ((e = flat_handle_SPACE(parent, index, capture, ptr)) != 0) {
        return e;
      }
      for (unsigned i=0; i < capture->children.count; i++) {
        if ((e = do_node(capture, indices[ 2 ], &(capture->children.list[ i ]), ptr)) != 0) {
          return e;
        }
      }
      if ((e = flat_handle_post_SPACE(parent, index, capture, ptr)) != 0) {
        return e;
      }
    }
    break;
  case 3:
    {
      ++indices[ 3 ];
      if ((e = flat_handle_MULTILINECOMMENT(parent, index, capture, ptr)) != 0) {
        return e;
      }
      for (unsigned i=0; i < capture->children.count; i++) {
        if ((e = do_node(capture, indices[ 3 ], &(capture->children.list[ i ]), ptr)) != 0) {
          return e;
        }
      }
      if ((e = flat_handle_post_MULTILINECOMMENT(parent, index, capture, ptr)) != 0) {
        return e;
      }
    }
    break;
  case 4:
    {
      ++indices[ 4 ];
      if ((e = flat_handle_COMMENT(parent, index, capture, ptr)) != 0) {
        return e;
      }
      for (unsigned i=0; i < capture->children.count; i++) {
        if ((e = do_node(capture, indices[ 4 ], &(capture->children.list[ i ]), ptr)) != 0) {
          return e;
        }
      }
      if ((e = flat_handle_post_COMMENT(parent, index, capture, ptr)) != 0) {
        return e;
      }
    }
    break;
  case 5:
    {
      ++indices[ 5 ];
      if ((e = flat_handle_OBJECTS(parent, index, capture, ptr)) != 0) {
        return e;
      }
      for (unsigned i=0; i < capture->children.count; i++) {
        if ((e = do_node(capture, indices[ 5 ], &(capture->children.list[ i ]), ptr)) != 0) {
          return e;
        }
      }
      if ((e = flat_handle_post_OBJECTS(parent, index, capture, ptr)) != 0) {
        return e;
      }
    }
    break;
  case 6:
    {
      ++indices[ 6 ];
      if ((e = flat_handle_SEPARATOR(parent, index, capture, ptr)) != 0) {
        return e;
      }
      for (unsigned i=0; i < capture->children.count; i++) {
        if ((e = do_node(capture, indices[ 6 ], &(capture->children.list[ i ]), ptr)) != 0) {
          return e;
        }
      }
      if ((e = flat_handle_post_SEPARATOR(parent, index, capture, ptr)) != 0) {
        return e;
      }
    }
    break;
  case 7:
    {
      ++indices[ 7 ];
      if ((e = flat_handle_OBJECT(parent, index, capture, ptr)) != 0) {
        return e;
      }
      for (unsigned i=0; i < capture->children.count; i++) {
        if ((e = do_node(capture, indices[ 7 ], &(capture->children.list[ i ]), ptr)) != 0) {
          return e;
        }
      }
      if ((e = flat_handle_post_OBJECT(parent, index, capture, ptr)) != 0) {
        return e;
      }
    }
    break;
  case 8:
    {
      ++indices[ 8 ];
      if ((e = flat_handle_NVPAIR(parent, index, capture, ptr)) != 0) {
        return e;
      }
      for (unsigned i=0; i < capture->children.count; i++) {
        if ((e = do_node(capture, indices[ 8 ], &(capture->children.list[ i ]), ptr)) != 0) {
          return e;
        }
      }
      if ((e = flat_handle_post_NVPAIR(parent, index, capture, ptr)) != 0) {
        return e;
      }
    }
    break;
  case 9:
    {
      ++indices[ 9 ];
      if ((e = flat_handle_NAME(parent, index, capture, ptr)) != 0) {
        return e;
      }
      for (unsigned i=0; i < capture->children.count; i++) {
        if ((e = do_node(capture, indices[ 9 ], &(capture->children.list[ i ]), ptr)) != 0) {
          return e;
        }
      }
      if ((e = flat_handle_post_NAME(parent, index, capture, ptr)) != 0) {
        return e;
      }
    }
    break;
  case 10:
    {
      ++indices[ 10 ];
      if ((e = flat_handle_COLON(parent, index, capture, ptr)) != 0) {
        return e;
      }
      for (unsigned i=0; i < capture->children.count; i++) {
        if ((e = do_node(capture, indices[ 10 ], &(capture->children.list[ i ]), ptr)) != 0) {
          return e;
        }
      }
      if ((e = flat_handle_post_COLON(parent, index, capture, ptr)) != 0) {
        return e;
      }
    }
    break;
  case 11:
    {
      ++indices[ 11 ];
      if ((e = flat_handle_VALUE(parent, index, capture, ptr)) != 0) {
        return e;
      }
      for (unsigned i=0; i < capture->children.count; i++) {
        if ((e = do_node(capture, indices[ 11 ], &(capture->children.list[ i ]), ptr)) != 0) {
          return e;
        }
      }
      if ((e = flat_handle_post_VALUE(parent, index, capture, ptr)) != 0) {
        return e;
      }
    }
    break;
  case 12:
    {
      ++indices[ 12 ];
      if ((e = flat_handle_NULL(parent, index, capture, ptr)) != 0) {
        return e;
      }
      for (unsigned i=0; i < capture->children.count; i++) {
        if ((e = do_node(capture, indices[ 12 ], &(capture->children.list[ i ]), ptr)) != 0) {
          return e;
        }
      }
      if ((e = flat_handle_post_NULL(parent, index, capture, ptr)) != 0) {
        return e;
      }
    }
    break;
  case 13:
    {
      ++indices[ 13 ];
      if ((e = flat_handle_BOOL(parent, index, capture, ptr)) != 0) {
        return e;
      }
      for (unsigned i=0; i < capture->children.count; i++) {
        if ((e = do_node(capture, indices[ 13 ], &(capture->children.list[ i ]), ptr)) != 0) {
          return e;
        }
      }
      if ((e = flat_handle_post_BOOL(parent, index, capture, ptr)) != 0) {
        return e;
      }
    }
    break;
  case 14:
    {
      ++indices[ 14 ];
      if ((e = flat_handle_FLOAT(parent, index, capture, ptr)) != 0) {
        return e;
      }
      for (unsigned i=0; i < capture->children.count; i++) {
        if ((e = do_node(capture, indices[ 14 ], &(capture->children.list[ i ]), ptr)) != 0) {
          return e;
        }
      }
      if ((e = flat_handle_post_FLOAT(parent, index, capture, ptr)) != 0) {
        return e;
      }
    }
    break;
  case 15:
    {
      ++indices[ 15 ];
      if ((e = flat_handle_INT(parent, index, capture, ptr)) != 0) {
        return e;
      }
      for (unsigned i=0; i < capture->children.count; i++) {
        if ((e = do_node(capture, indices[ 15 ], &(capture->children.list[ i ]), ptr)) != 0) {
          return e;
        }
      }
      if ((e = flat_handle_post_INT(parent, index, capture, ptr)) != 0) {
        return e;
      }
    }
    break;
  case 16:
    {
      ++indices[ 16 ];
      if ((e = flat_handle_STRING(parent, index, capture, ptr)) != 0) {
        return e;
      }
      for (unsigned i=0; i < capture->children.count; i++) {
        if ((e = do_node(capture, indices[ 16 ], &(capture->children.list[ i ]), ptr)) != 0) {
          return e;
        }
      }
      if ((e = flat_handle_post_STRING(parent, index, capture, ptr)) != 0) {
        return e;
      }
    }
    break;
  case 17:
    {
      ++indices[ 17 ];
      if ((e = flat_handle_BLOB(parent, index, capture, ptr)) != 0) {
        return e;
      }
      for (unsigned i=0; i < capture->children.count; i++) {
        if ((e = do_node(capture, indices[ 17 ], &(capture->children.list[ i ]), ptr)) != 0) {
          return e;
        }
      }
      if ((e = flat_handle_post_BLOB(parent, index, capture, ptr)) != 0) {
        return e;
      }
    }
    break;
  case 18:
    {
      ++indices[ 18 ];
      if ((e = flat_handle_END(parent, index, capture, ptr)) != 0) {
        return e;
      }
      for (unsigned i=0; i < capture->children.count; i++) {
        if ((e = do_node(capture, indices[ 18 ], &(capture->children.list[ i ]), ptr)) != 0) {
          return e;
        }
      }
      if ((e = flat_handle_post_END(parent, index, capture, ptr)) != 0) {
        return e;
      }
    }
    break;
  }
  return 0;
}
