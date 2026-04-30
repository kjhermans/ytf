-- GPEG compiler, release 0.5.6

  call YTF
  end 0

YTF:
  opencapture 1
  call OBJECTS
  call END
  closecapture 1
  ret

NEWLINE:
  opencapture 2
  range 0a
  closecapture 2
  ret

SPACE:
  opencapture 3
  catch MACRO2_1
  range 09 0b
  commit L2
MACRO2_1:
  catch MACRO2_2
  range 0d
  commit L2
MACRO2_2:
  range 20
L2:
  catch L1
LOOP1:
  catch MACRO3_1
  range 09 0b
  commit L3
MACRO3_1:
  catch MACRO3_2
  range 0d
  commit L3
MACRO3_2:
  range 20
L3:
  partialcommit LOOP1
L1:
  closecapture 3
  ret

MULTILINECOMMENT:
  opencapture 4
  range 2f
  range 2a
  catch L4
LOOP4:
  catch L5
  range 2a
  range 2f
  failtwice
L5:
  range 00 ff
  partialcommit LOOP4
L4:
  range 2a
  range 2f
  closecapture 4
  ret

COMMENT:
  opencapture 5
  range 2f
  range 2f
  catch L6
LOOP6:
  catch SET7_0
  range 00 09
  commit L7
SET7_0:
  range 0b ff
L7:
  partialcommit LOOP6
L6:
  range 0a
  closecapture 5
  ret

__prefix:
  catch L8
LOOP8:
  catch L10
  call MULTILINECOMMENT
  commit L9
L10:
  catch L11
  call COMMENT
  commit L9
L11:
  call SPACE
L9:
  partialcommit LOOP8
L8:
  ret

OBJECTS:
  call __prefix
  opencapture 6
  catch L13
  call SEPARATOR
  commit __NEXT__
L13:
  catch L14
LOOP14:
  call OBJECT
  call SEPARATOR
  partialcommit LOOP14
L14:
  call OBJECT
  closecapture 6
  ret

SEPARATOR:
  call __prefix
  opencapture 7
  range 23
  catch L15
LOOP15:
  catch SET16_0
  range 00 09
  commit L16
SET16_0:
  range 0b ff
L16:
  partialcommit LOOP15
L15:
  range 0a
  closecapture 7
  ret

OBJECT:
  call __prefix
  opencapture 8
  call NVPAIR
  catch L17
LOOP17:
  call NVPAIR
  partialcommit LOOP17
L17:
  closecapture 8
  ret

NVPAIR:
  call __prefix
  opencapture 9
  call NAME
  call COLON
  call VALUE
  catch L18
LOOP18:
  catch SET19_0
  range 09 09
  commit L19
SET19_0:
  range 20 20
L19:
  partialcommit LOOP18
L18:
  call NEWLINE
  closecapture 9
  ret

NAME:
  call __prefix
  opencapture 10
  call NAMEELT
  catch L20
LOOP20:
  call DOT
  call NAMEELT
  partialcommit LOOP20
L20:
  closecapture 10
  ret

NAMEELT:
  call __prefix
  opencapture 11
  catch SET22_0
  range 2d 2d
  commit L22
SET22_0:
  catch SET22_1
  range 30 39
  commit L22
SET22_1:
  catch SET22_2
  range 41 5a
  commit L22
SET22_2:
  catch SET22_3
  range 5f 5f
  commit L22
SET22_3:
  range 61 7a
L22:
  catch L21
  counter 1 63
CTR1:
  catch SET23_0
  range 2d 2d
  commit L23
SET23_0:
  catch SET23_1
  range 30 39
  commit L23
SET23_1:
  catch SET23_2
  range 41 5a
  commit L23
SET23_2:
  catch SET23_3
  range 5f 5f
  commit L23
SET23_3:
  range 61 7a
L23:
  partialcommit __NEXT__
  condjump 1 CTR1
  commit __NEXT__
L21:
  closecapture 11
  ret

DOT:
  call __prefix
  opencapture 12
  range 2e
  closecapture 12
  ret

COLON:
  call __prefix
  opencapture 13
  range 3a
  closecapture 13
  ret

VALUE:
  call __prefix
  opencapture 14
  catch L25
  call NULL
  commit L24
L25:
  catch L26
  call BOOL
  commit L24
L26:
  catch L27
  call FLOAT
  commit L24
L27:
  catch L28
  call INT
  commit L24
L28:
  catch L29
  call STRING
  commit L24
L29:
  call BLOB
L24:
  closecapture 14
  ret

NULL:
  call __prefix
  opencapture 15
  range 6e
  range 75
  range 6c
  range 6c
  closecapture 15
  ret

BOOL:
  call __prefix
  opencapture 16
  catch L32
  range 66
  range 61
  range 6c
  range 73
  range 65
  commit L31
L32:
  range 74
  range 72
  range 75
  range 65
L31:
  closecapture 16
  ret

FLOAT:
  call __prefix
  opencapture 17
  catch L34
LOOP34:
  range 30 39
L35:
  partialcommit LOOP34
L34:
  range 2e
  range 30 39
L37:
  catch L36
LOOP36:
  range 30 39
L38:
  partialcommit LOOP36
L36:
  closecapture 17
  ret

INT:
  call __prefix
  opencapture 18
  range 30 39
L40:
  catch L39
LOOP39:
  range 30 39
L41:
  partialcommit LOOP39
L39:
  closecapture 18
  ret

STRING:
  call __prefix
  opencapture 19
  range 22
  catch L42
LOOP42:
  catch L44
  range 5c
  catch SET45_0
  range 22 22
  commit L45
SET45_0:
  range 5c 5c
L45:
  commit L43
L44:
  catch L47
  range 22
  failtwice
L47:
  range 00 ff
L43:
  partialcommit LOOP42
L42:
  range 22
  closecapture 19
  ret

BLOB:
  call __prefix
  opencapture 20
  range 62
  range 36
  range 34
  range 28
  catch SET49_0
  range 09 0a
  commit L49
SET49_0:
  catch SET49_1
  range 20 20
  commit L49
SET49_1:
  catch SET49_2
  range 2b 2b
  commit L49
SET49_2:
  catch SET49_3
  range 2f 39
  commit L49
SET49_3:
  catch SET49_4
  range 41 5a
  commit L49
SET49_4:
  range 61 7a
L49:
  catch L48
LOOP48:
  catch SET50_0
  range 09 0a
  commit L50
SET50_0:
  catch SET50_1
  range 20 20
  commit L50
SET50_1:
  catch SET50_2
  range 2b 2b
  commit L50
SET50_2:
  catch SET50_3
  range 2f 39
  commit L50
SET50_3:
  catch SET50_4
  range 41 5a
  commit L50
SET50_4:
  range 61 7a
L50:
  partialcommit LOOP48
L48:
  catch L51
LOOP51:
  catch SET52_0
  range 09 0a
  commit L52
SET52_0:
  catch SET52_1
  range 20 20
  commit L52
SET52_1:
  range 3d 3d
L52:
  partialcommit LOOP51
L51:
  range 29
  closecapture 20
  ret

END:
  call __prefix
  opencapture 21
  catch L53
  range 00 ff
  failtwice
L53:
  closecapture 21
  ret

