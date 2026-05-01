-- GPEG compiler, release 0.5.7

  call TOP
  end 0

TOP:
  call JSON
  ret

UTF8CHAR:
  opencapture 1
  range 00 ff
  closecapture 1
  ret

SPACE:
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
  ret

MULTILINECOMMENT:
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
  ret

COMMENT:
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

END:
  call __prefix
  catch L13
  range 00 ff
  failtwice
L13:
  ret

JSON:
  call __prefix
  call VALUE
  call END
  ret

HASH:
  call __prefix
  opencapture 2
  call CBOPEN
  call OPTHASHELTS
  call CBCLOSE
  closecapture 2
  ret

OPTHASHELTS:
  call __prefix
  catch L14
  call HASHELTS
  commit __NEXT__
L14:
  ret

HASHELTS:
  call __prefix
  call HASHELT
  catch L15
LOOP15:
  call COMMA
  call HASHELT
  partialcommit LOOP15
L15:
  ret

HASHELT:
  call __prefix
  opencapture 3
  call NAME
  call COLON
  call VALUE
  closecapture 3
  ret

ARRAY:
  call __prefix
  opencapture 4
  call ABOPEN
  call OPTARRAYELTS
  call ABCLOSE
  closecapture 4
  ret

OPTARRAYELTS:
  call __prefix
  catch L16
  call ARRAYELTS
  commit __NEXT__
L16:
  ret

ARRAYELTS:
  call __prefix
  call VALUE
  catch L17
LOOP17:
  call COMMA
  call VALUE
  partialcommit LOOP17
L17:
  ret

VALUE:
  call __prefix
  catch L19
  call BIGSTRING
  commit L18
L19:
  catch L20
  call STRING
  commit L18
L20:
  catch L21
  call FLOAT
  commit L18
L21:
  catch L22
  call INT
  commit L18
L22:
  catch L23
  call BOOL
  commit L18
L23:
  catch L24
  call NULL
  commit L18
L24:
  catch L25
  call HASH
  commit L18
L25:
  call ARRAY
L18:
  ret

BIGSTRING:
  call __prefix
  range 22
  range 22
  range 22
  opencapture 5
  catch L27
LOOP27:
  catch L28
  range 22
  range 22
  range 22
  failtwice
L28:
  call UTF8CHAR
  partialcommit LOOP27
L27:
  closecapture 5
  range 22
  range 22
  range 22
  ret

NAME:
  call __prefix
  call STRING
  ret

STRING:
  call __prefix
  range 22
  opencapture 6
  catch L29
LOOP29:
  catch L31
  range 5c
  opencapture 7
  catch L33
  catch SET34_0
  range 22 22
  commit L34
SET34_0:
  catch SET34_1
  range 5c 5c
  commit L34
SET34_1:
  catch SET34_2
  range 6e 6e
  commit L34
SET34_2:
  catch SET34_3
  range 72 72
  commit L34
SET34_3:
  range 74 74
L34:
  commit L32
L33:
  range 75
  counter 0 4
CTR0:
  catch SET36_0
  range 30 39
  commit L36
SET36_0:
  catch SET36_1
  range 41 46
  commit L36
SET36_1:
  range 61 66
L36:
  condjump 0 CTR0
L32:
  closecapture 7
  commit L30
L31:
  catch L38
  range 22
  failtwice
L38:
  call UTF8CHAR
L30:
  partialcommit LOOP29
L29:
  closecapture 6
  range 22
  ret

INT:
  call __prefix
  opencapture 8
  catch L39
  range 2d
  commit __NEXT__
L39:
  range 30 39
L41:
  catch L40
LOOP40:
  range 30 39
L42:
  partialcommit LOOP40
L40:
  closecapture 8
  ret

FLOAT:
  call __prefix
  opencapture 9
  catch L43
  range 2d
  commit __NEXT__
L43:
  catch L44
LOOP44:
  range 30 39
L45:
  partialcommit LOOP44
L44:
  range 2e
  range 30 39
L47:
  catch L46
LOOP46:
  range 30 39
L48:
  partialcommit LOOP46
L46:
  closecapture 9
  ret

BOOL:
  call __prefix
  opencapture 10
  catch L50
  range 74
  range 72
  range 75
  range 65
  commit L49
L50:
  range 66
  range 61
  range 6c
  range 73
  range 65
L49:
  closecapture 10
  ret

NULL:
  call __prefix
  opencapture 11
  range 6e
  range 75
  range 6c
  range 6c
  closecapture 11
  ret

CBOPEN:
  call __prefix
  range 7b
  ret

CBCLOSE:
  call __prefix
  range 7d
  ret

ABOPEN:
  call __prefix
  range 5b
  ret

ABCLOSE:
  call __prefix
  range 5d
  ret

COMMA:
  call __prefix
  range 2c
  ret

COLON:
  call __prefix
  range 3a
  ret

