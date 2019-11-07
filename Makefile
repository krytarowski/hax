#
# This Makefile is for use when distributing Hax to the outside world.
#
# Some changes you may wish to make here:
#
# 1. To compile for non-UNIX systems (so that only the non-UNIX-specific
# commands are available), change the OBJS line below so it doesn't
# include ${UNIX_OBJS}.  Also, add the switch "-DHAX_GENERIC_ONLY" to
# CFLAGS.  Lastly, you'll have to provide your own replacement for the
# "panic" procedure (see panic.c for what the current one does).

# 2. If you want to put Hax-related information in non-standard places,
# change the following definitions below to reflect where you want
# things (all must be specified as full rooted path names):
#
#    INSTALL_DIR	Top-level directory in which to install;  contains
#			each of the other directories below.
#    HAX_LIBRARY	Directory in which to install the library of Hax
#			scripts.  Note: if the HAX_LIBRARY environment
#			variable is specified at run-time then Hax looks
#			there rather than in the place specified here.
#    LIB_DIR		Directory in which to install the archive libhax.a
#    INCLUDE_DIR	Directory in which to install include files.
#    MAN3_DIR		Directory in which to install manual entries for
#			library procedures such as Hax_Eval.
#    MANN_DIR		Directory in which to install manual entries for
#			miscellaneous things such as the Hax overview
#			manual entry.
#    RANLIB		If you're using a System-V-based UNIX that doesn't
#			have ranlib, change this definition to "echo" or
#			something else harmless.

INSTALL_DIR =	/usr/local
HAX_LIBRARY =	$(INSTALL_DIR)/lib/hax
LIB_DIR =	$(INSTALL_DIR)/lib
INCLUDE_DIR =	$(INSTALL_DIR)/include
MAN3_DIR =	$(INSTALL_DIR)/man/man3
MANN_DIR =	$(INSTALL_DIR)/man/mann
RANLIB =	ranlib

CC =		cc
CFLAGS =	-O -I. -DHAX_LIBRARY=\"${HAX_LIBRARY}\"

GENERIC_OBJS =	haxRegexp.o haxAssem.o haxBasic.o haxCkalloc.o \
	haxCmdAH.o haxCmdIL.o haxCmdMZ.o haxExpr.o haxGet.o \
	haxHash.o haxHistory.o haxParse.o haxProc.o haxUtil.o \
	haxVar.o

UNIX_OBJS = panic.o haxEnv.o haxGlob.o haxUnixAZ.o haxUnixStr.o \
	haxUnixUtil.o 

COMPAT_OBJS =

OBJS = ${GENERIC_OBJS} ${UNIX_OBJS} ${COMPAT_OBJS}

all: libhax.a

libhax.a: ${OBJS}
	rm -f libhax.a
	ar cr libhax.a ${OBJS}
	$(RANLIB) libhax.a

install: libhax.a
	-if [ ! -d $(LIB_DIR) ] ; then mkdir -p $(LIB_DIR); fi
	-if [ ! -d $(HAX_LIBRARY) ] ; then mkdir -p $(HAX_LIBRARY); fi
	-if [ ! -d $(INCLUDE_DIR) ] ; then mkdir -p $(INCLUDE_DIR); fi
	-if [ ! -d $(MAN3_DIR) ] ; then mkdir -p $(MAN3_DIR); fi
	-if [ ! -d $(MANN_DIR) ] ; then mkdir -p $(MANN_DIR); fi
	rm -rf $(HAX_LIBRARY)/*
	for i in library/*.hax library/haxIndex; \
	    do \
	    cp $$i $(HAX_LIBRARY);  \
	    done
	rm -f $(LIB_DIR)/libhax.a
	cp libhax.a $(LIB_DIR)
	$(RANLIB) $(LIB_DIR)/libhax.a
	rm -f $(INCLUDE_DIR)/hax.h $(INCLUDE_DIR)/haxHash.h
	cp hax.h $(INCLUDE_DIR)
	cp haxHash.h $(INCLUDE_DIR)
	cd doc; for i in *.3; \
	    do \
	    rm -f $(MAN3_DIR)/$$i; \
	    sed -e '/man\.macros/r man.macros' -e '/man\.macros/d' \
		    $$i > $(MAN3_DIR)/$$i; \
	    done; cd ..
	cd doc; for i in *.n; \
	    do \
	    rm -f $(MANN_DIR)/$$i; \
	    sed -e '/man\.macros/r man.macros' -e '/man\.macros/d' \
		    $$i > $(MANN_DIR)/$$i; \
	    done; cd ..

haxTest: haxTest.o libhax.a
	${CC} ${CFLAGS} haxTest.o libhax.a -o haxTest

test: haxTest
	( echo cd tests ; echo source all ) | ./haxTest

clean:
	rm -f ${OBJS} libhax.a haxTest.o haxTest

${OBJS}: hax.h haxHash.h haxInt.h
${UNIX_OBJS}: haxUnix.h
