  call YTF
  end 0

YTF:
  opencapture 0
  call OBJECTS
  call END
  closecapture 0
  ret

NEWLINE:
  opencapture 1
  char 0a
  closecapture 1
  ret

SPACE:
  opencapture 2
  set 002e000001000000000000000000000000000000000000000000000000000000
  catch __L1
__L2:
  set 002e000001000000000000000000000000000000000000000000000000000000
  partialcommit __L2
__L1:
  closecapture 2
  ret

MULTILINECOMMENT:
  opencapture 3
  char 2f
  char 2a
  catch __L3
__L4:
  catch __L5
  char 2a
  char 2f
  failtwice
__L5:
  any
  partialcommit __L4
__L3:
  char 2a
  char 2f
  closecapture 3
  ret

COMMENT:
  opencapture 4
  char 2f
  char 2f
  catch __L7
__L8:
  set fffbffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff
  partialcommit __L8
__L7:
  char 0a
  closecapture 4
  ret

__prefix:
  catch __L9
__L10:
  catch __L11
  catch __L13
  call MULTILINECOMMENT
  commit __L14
__L13:
  call COMMENT
__L14:
  commit __L12
__L11:
  call SPACE
__L12:
  partialcommit __L10
__L9:
  ret

OBJECTS:
  call __prefix
  opencapture 5
  catch __L15
  call SEPARATOR
  commit __NEXT__
__L15:
  catch __L16
__L17:
  call OBJECT
  call SEPARATOR
  partialcommit __L17
__L16:
  call OBJECT
  closecapture 5
  ret

SEPARATOR:
  call __prefix
  opencapture 6
  char 23
  catch __L18
__L19:
  set fffbffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff
  partialcommit __L19
__L18:
  char 0a
  closecapture 6
  ret

OBJECT:
  call __prefix
  opencapture 7
  call NVPAIR
  catch __L20
__L21:
  call NVPAIR
  partialcommit __L21
__L20:
  closecapture 7
  ret

NVPAIR:
  call __prefix
  opencapture 8
  call NAME
  call COLON
  call VALUE
  catch __L22
__L23:
  set 0002000001000000000000000000000000000000000000000000000000000000
  partialcommit __L23
__L22:
  call NEWLINE
  closecapture 8
  ret

NAME:
  call __prefix
  opencapture 9
  set 000000000060ff03feffff87feffff0700000000000000000000000000000000
  catch __L24
  counter 1 63
__L25:
  set 000000000060ff03feffff87feffff0700000000000000000000000000000000
  partialcommit __NEXT__
  condjump 1 __L25
  commit __NEXT__
__L24:
  closecapture 9
  ret

COLON:
  call __prefix
  opencapture 10
  char 3a
  closecapture 10
  ret

VALUE:
  call __prefix
  opencapture 11
  catch __L26
  catch __L28
  catch __L30
  catch __L32
  catch __L34
  call NULL
  commit __L35
__L34:
  call BOOL
__L35:
  commit __L33
__L32:
  call FLOAT
__L33:
  commit __L31
__L30:
  call INT
__L31:
  commit __L29
__L28:
  call STRING
__L29:
  commit __L27
__L26:
  call BLOB
__L27:
  closecapture 11
  ret

NULL:
  call __prefix
  opencapture 12
  quad 6e756c6c
  closecapture 12
  ret

BOOL:
  call __prefix
  opencapture 13
  catch __L36
  quad 66616c73
  char 65
  commit __L37
__L36:
  quad 74727565
__L37:
  closecapture 13
  ret

FLOAT:
  call __prefix
  opencapture 14
  catch __L38
__L39:
  set 000000000000ff03000000000000000000000000000000000000000000000000
  partialcommit __L39
__L38:
  char 2e
  set 000000000000ff03000000000000000000000000000000000000000000000000
  catch __L40
__L41:
  set 000000000000ff03000000000000000000000000000000000000000000000000
  partialcommit __L41
__L40:
  closecapture 14
  ret

INT:
  call __prefix
  opencapture 15
  set 000000000000ff03000000000000000000000000000000000000000000000000
  catch __L42
__L43:
  set 000000000000ff03000000000000000000000000000000000000000000000000
  partialcommit __L43
__L42:
  closecapture 15
  ret

STRING:
  call __prefix
  opencapture 16
  char 22
  catch __L44
__L45:
  catch __L46
  char 5c
  set 0000000004000000000000100000000000000000000000000000000000000000
  commit __L47
__L46:
  catch __L48
  char 22
  failtwice
__L48:
  any
__L47:
  partialcommit __L45
__L44:
  char 22
  closecapture 16
  ret

BLOB:
  call __prefix
  opencapture 17
  quad 62363428
  set 000600000188ff03feffff07feffff0700000000000000000000000000000000
  catch __L50
__L51:
  set 000600000188ff03feffff07feffff0700000000000000000000000000000000
  partialcommit __L51
__L50:
  catch __L52
__L53:
  set 0006000001000020000000000000000000000000000000000000000000000000
  partialcommit __L53
__L52:
  char 29
  closecapture 17
  ret

END:
  call __prefix
  opencapture 18
  catch __L54
  any
  failtwice
__L54:
  closecapture 18
  ret

