# Microsoft Visual C++ generated build script - Do not modify

PROJ = RENDER
DEBUG = 1
PROGTYPE = 6
CALLER = 
ARGS = 
DLLS = 
D_RCDEFINES = -d_DEBUG
R_RCDEFINES = -dNDEBUG
ORIGIN = MSVC
ORIGIN_VER = 1.00
PROJPATH = C:\CODE\MSVC\RENDER\
USEMFC = 0
CC = cl
CPP = cl
CXX = cl
CCREATEPCHFLAG = 
CPPCREATEPCHFLAG = 
CUSEPCHFLAG = 
CPPUSEPCHFLAG = 
FIRSTC =             
FIRSTCPP = ANIM_SYS.CPP
RC = rc
CFLAGS_D_DEXE = /nologo /G3 /FPi87 /W3 /Zi /AM /Od /Ob2 /OV9 /D "_DEBUG" /D "_DOS" /FR /Fd"RENDER.PDB"
CFLAGS_R_DEXE = /nologo /Gs /G2 /W3 /AM /Ox /D "NDEBUG" /D "_DOS" /FR 
LFLAGS_D_DEXE = /NOLOGO /NOI /STACK:5120 /ONERROR:NOEXE /CO 
LFLAGS_R_DEXE = /NOLOGO /NOI /STACK:5120 /ONERROR:NOEXE 
LIBS_D_DEXE = oldnames mlibc7 
LIBS_R_DEXE = oldnames mlibce 
RCFLAGS = /nologo
RESFLAGS = /nologo
RUNFLAGS = 
OBJS_EXT = 
LIBS_EXT = MODEX.LIB 
!if "$(DEBUG)" == "1"
CFLAGS = $(CFLAGS_D_DEXE)
LFLAGS = $(LFLAGS_D_DEXE)
LIBS = $(LIBS_D_DEXE)
MAPFILE = nul
RCDEFINES = $(D_RCDEFINES)
!else
CFLAGS = $(CFLAGS_R_DEXE)
LFLAGS = $(LFLAGS_R_DEXE)
LIBS = $(LIBS_R_DEXE)
MAPFILE = nul
RCDEFINES = $(R_RCDEFINES)
!endif
!if [if exist MSVC.BND del MSVC.BND]
!endif
SBRS = ANIM_SYS.SBR \
		KEYBOARD.SBR \
		PHONG.SBR \
		VECTOR.SBR \
		FACE.SBR \
		RIPPER.SBR


MODEX_DEP = 

ANIM_SYS_DEP = c:\code\msvc\render\anim_sys.h \
	c:\code\msvc\render\modex.h


KEYBOARD_DEP = c:\code\msvc\render\keyboard.h


PHONG_DEP = c:\code\msvc\render\modex.h \
	c:\code\msvc\render\anim_sys.h \
	c:\code\msvc\render\vector.h \
	c:\code\msvc\render\face.h \
	c:\code\msvc\render\keyboard.h


VECTOR_DEP = c:\code\msvc\render\vector.h \
	c:\code\msvc\render\anim_sys.h \
	c:\code\msvc\render\modex.h


FACE_DEP = c:\code\msvc\render\face.h \
	c:\code\msvc\render\vector.h \
	c:\code\msvc\render\anim_sys.h \
	c:\code\msvc\render\modex.h


RIPPER_DEP = 

all:	$(PROJ).EXE $(PROJ).BSC

ANIM_SYS.OBJ:	ANIM_SYS.CPP $(ANIM_SYS_DEP)
	$(CPP) $(CFLAGS) $(CPPCREATEPCHFLAG) /c ANIM_SYS.CPP

KEYBOARD.OBJ:	KEYBOARD.CPP $(KEYBOARD_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c KEYBOARD.CPP

PHONG.OBJ:	PHONG.CPP $(PHONG_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c PHONG.CPP

VECTOR.OBJ:	VECTOR.CPP $(VECTOR_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c VECTOR.CPP

FACE.OBJ:	FACE.CPP $(FACE_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c FACE.CPP

RIPPER.OBJ:	RIPPER.CPP $(RIPPER_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c RIPPER.CPP

$(PROJ).EXE::	ANIM_SYS.OBJ KEYBOARD.OBJ PHONG.OBJ VECTOR.OBJ FACE.OBJ RIPPER.OBJ $(OBJS_EXT) $(DEFFILE)
	echo >NUL @<<$(PROJ).CRF
ANIM_SYS.OBJ +
KEYBOARD.OBJ +
PHONG.OBJ +
VECTOR.OBJ +
FACE.OBJ +
RIPPER.OBJ +
$(OBJS_EXT)
$(PROJ).EXE
$(MAPFILE)
d:\msvc\lib\+
c:\apps\msvc\lib\+
c:\apps\msvc\mfc\lib\+
MODEX.LIB+
$(LIBS)
$(DEFFILE);
<<
	link $(LFLAGS) @$(PROJ).CRF

run: $(PROJ).EXE
	$(PROJ) $(RUNFLAGS)


$(PROJ).BSC: $(SBRS)
	bscmake @<<
/o$@ $(SBRS)
<<
