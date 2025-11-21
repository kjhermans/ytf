#include <gpeg/lib/gpeg/gpeg_capturelist.h>

#ifdef _DEBUG
#include <stdio.h>
#endif

extern int json_grammar_process_node(gpeg_capture_t*,void*);
static int do_node(gpeg_capture_t*,unsigned,gpeg_capture_t*,void*);

extern int json_handle_UTF8CHAR(gpeg_capture_t*,unsigned,gpeg_capture_t*,void*);
extern int json_handle_post_UTF8CHAR(gpeg_capture_t*,unsigned,gpeg_capture_t*,void*);
extern int json_handle_HASH(gpeg_capture_t*,unsigned,gpeg_capture_t*,void*);
extern int json_handle_post_HASH(gpeg_capture_t*,unsigned,gpeg_capture_t*,void*);
extern int json_handle_HASHELT(gpeg_capture_t*,unsigned,gpeg_capture_t*,void*);
extern int json_handle_post_HASHELT(gpeg_capture_t*,unsigned,gpeg_capture_t*,void*);
extern int json_handle_ARRAY(gpeg_capture_t*,unsigned,gpeg_capture_t*,void*);
extern int json_handle_post_ARRAY(gpeg_capture_t*,unsigned,gpeg_capture_t*,void*);
extern int json_handle_BIGSTRING(gpeg_capture_t*,unsigned,gpeg_capture_t*,void*);
extern int json_handle_post_BIGSTRING(gpeg_capture_t*,unsigned,gpeg_capture_t*,void*);
extern int json_handle_STRING(gpeg_capture_t*,unsigned,gpeg_capture_t*,void*);
extern int json_handle_post_STRING(gpeg_capture_t*,unsigned,gpeg_capture_t*,void*);
extern int json_handle_STRING_0(gpeg_capture_t*,unsigned,gpeg_capture_t*,void*);
extern int json_handle_post_STRING_0(gpeg_capture_t*,unsigned,gpeg_capture_t*,void*);
extern int json_handle_INT(gpeg_capture_t*,unsigned,gpeg_capture_t*,void*);
extern int json_handle_post_INT(gpeg_capture_t*,unsigned,gpeg_capture_t*,void*);
extern int json_handle_FLOAT(gpeg_capture_t*,unsigned,gpeg_capture_t*,void*);
extern int json_handle_post_FLOAT(gpeg_capture_t*,unsigned,gpeg_capture_t*,void*);
extern int json_handle_BOOL(gpeg_capture_t*,unsigned,gpeg_capture_t*,void*);
extern int json_handle_post_BOOL(gpeg_capture_t*,unsigned,gpeg_capture_t*,void*);
extern int json_handle_NULL(gpeg_capture_t*,unsigned,gpeg_capture_t*,void*);
extern int json_handle_post_NULL(gpeg_capture_t*,unsigned,gpeg_capture_t*,void*);

int json_grammar_process_node
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
  unsigned indices[ 11 ] = { 0 };

  switch (capture->type) {
  case 0:
    {
      ++indices[ 0 ];
      if ((e = json_handle_UTF8CHAR(parent, index, capture, ptr)) != 0) {
        return e;
      }
      for (unsigned i=0; i < capture->children.count; i++) {
        if ((e = do_node(capture, indices[ 0 ], &(capture->children.list[ i ]), ptr)) != 0) {
          return e;
        }
      }
      if ((e = json_handle_post_UTF8CHAR(parent, index, capture, ptr)) != 0) {
        return e;
      }
    }
    break;
  case 1:
    {
      ++indices[ 1 ];
      if ((e = json_handle_HASH(parent, index, capture, ptr)) != 0) {
        return e;
      }
      for (unsigned i=0; i < capture->children.count; i++) {
        if ((e = do_node(capture, indices[ 1 ], &(capture->children.list[ i ]), ptr)) != 0) {
          return e;
        }
      }
      if ((e = json_handle_post_HASH(parent, index, capture, ptr)) != 0) {
        return e;
      }
    }
    break;
  case 2:
    {
      ++indices[ 2 ];
      if ((e = json_handle_HASHELT(parent, index, capture, ptr)) != 0) {
        return e;
      }
      for (unsigned i=0; i < capture->children.count; i++) {
        if ((e = do_node(capture, indices[ 2 ], &(capture->children.list[ i ]), ptr)) != 0) {
          return e;
        }
      }
      if ((e = json_handle_post_HASHELT(parent, index, capture, ptr)) != 0) {
        return e;
      }
    }
    break;
  case 3:
    {
      ++indices[ 3 ];
      if ((e = json_handle_ARRAY(parent, index, capture, ptr)) != 0) {
        return e;
      }
      for (unsigned i=0; i < capture->children.count; i++) {
        if ((e = do_node(capture, indices[ 3 ], &(capture->children.list[ i ]), ptr)) != 0) {
          return e;
        }
      }
      if ((e = json_handle_post_ARRAY(parent, index, capture, ptr)) != 0) {
        return e;
      }
    }
    break;
  case 4:
    {
      ++indices[ 4 ];
      if ((e = json_handle_BIGSTRING(parent, index, capture, ptr)) != 0) {
        return e;
      }
      for (unsigned i=0; i < capture->children.count; i++) {
        if ((e = do_node(capture, indices[ 4 ], &(capture->children.list[ i ]), ptr)) != 0) {
          return e;
        }
      }
      if ((e = json_handle_post_BIGSTRING(parent, index, capture, ptr)) != 0) {
        return e;
      }
    }
    break;
  case 5:
    {
      ++indices[ 5 ];
      if ((e = json_handle_STRING(parent, index, capture, ptr)) != 0) {
        return e;
      }
      for (unsigned i=0; i < capture->children.count; i++) {
        if ((e = do_node(capture, indices[ 5 ], &(capture->children.list[ i ]), ptr)) != 0) {
          return e;
        }
      }
      if ((e = json_handle_post_STRING(parent, index, capture, ptr)) != 0) {
        return e;
      }
    }
    break;
  case 6:
    {
      ++indices[ 6 ];
      if ((e = json_handle_STRING_0(parent, index, capture, ptr)) != 0) {
        return e;
      }
      for (unsigned i=0; i < capture->children.count; i++) {
        if ((e = do_node(capture, indices[ 6 ], &(capture->children.list[ i ]), ptr)) != 0) {
          return e;
        }
      }
      if ((e = json_handle_post_STRING_0(parent, index, capture, ptr)) != 0) {
        return e;
      }
    }
    break;
  case 7:
    {
      ++indices[ 7 ];
      if ((e = json_handle_INT(parent, index, capture, ptr)) != 0) {
        return e;
      }
      for (unsigned i=0; i < capture->children.count; i++) {
        if ((e = do_node(capture, indices[ 7 ], &(capture->children.list[ i ]), ptr)) != 0) {
          return e;
        }
      }
      if ((e = json_handle_post_INT(parent, index, capture, ptr)) != 0) {
        return e;
      }
    }
    break;
  case 8:
    {
      ++indices[ 8 ];
      if ((e = json_handle_FLOAT(parent, index, capture, ptr)) != 0) {
        return e;
      }
      for (unsigned i=0; i < capture->children.count; i++) {
        if ((e = do_node(capture, indices[ 8 ], &(capture->children.list[ i ]), ptr)) != 0) {
          return e;
        }
      }
      if ((e = json_handle_post_FLOAT(parent, index, capture, ptr)) != 0) {
        return e;
      }
    }
    break;
  case 9:
    {
      ++indices[ 9 ];
      if ((e = json_handle_BOOL(parent, index, capture, ptr)) != 0) {
        return e;
      }
      for (unsigned i=0; i < capture->children.count; i++) {
        if ((e = do_node(capture, indices[ 9 ], &(capture->children.list[ i ]), ptr)) != 0) {
          return e;
        }
      }
      if ((e = json_handle_post_BOOL(parent, index, capture, ptr)) != 0) {
        return e;
      }
    }
    break;
  case 10:
    {
      ++indices[ 10 ];
      if ((e = json_handle_NULL(parent, index, capture, ptr)) != 0) {
        return e;
      }
      for (unsigned i=0; i < capture->children.count; i++) {
        if ((e = do_node(capture, indices[ 10 ], &(capture->children.list[ i ]), ptr)) != 0) {
          return e;
        }
      }
      if ((e = json_handle_post_NULL(parent, index, capture, ptr)) != 0) {
        return e;
      }
    }
    break;
  }
  return 0;
}
