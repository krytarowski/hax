#
# This Makefile is for use when distributing Hax to the outside world.
#
# Some changes you may wish to make here:
#
# 1. To compile for non-UNIX systems (so that only the non-UNIX-specific
# commands are available), change the OBJS line below so it doesn't
# include ${UNIX_OBJS}.  Also, add the switch "-DHAX_GENERIC_ONLY" to
# CFLAGS.  Lastly, you'll have to provide your own replacement for the
# "Hax_Panic" procedure (see haxPanic.c for what the current one does).

# 2. To compile Hax with Software Floats, add the switch
# "-DHAX_SOFTFLOAT" to CFLAGS.

# 3. If you want to put Hax-related information in non-standard places,
# change the following definitions below to reflect where you want
# things (all must be specified as full rooted path names):
#
#    PREFIX		Top-level directory in which to install;  contains
#			each of the other directories below.
#    HAX_LIBRARY	Directory in which to install the library of Hax
#			scripts.  Note: if the HAX_LIBRARY environment
#			variable is specified at run-time then Hax looks
#			there rather than in the place specified here.
#    LIB_DIR		Directory in which to install the archive libhax.a
#    INCLUDE_DIR	Directory in which to install include files.
#    MAN_DIR		Directory in which to install manual pages.
#    MAN3_DIR		Directory in which to install manual entries for
#			library procedures such as Hax_Eval.
#    MANN_DIR		Directory in which to install manual entries for
#			miscellaneous things such as the Hax overview
#			manual entry.
#

# 3. If you want to alter the default build programs and flags, change:
#
#    CC			Path to cc(1).
#    AR			Path to ar(1).
#    RANLIB		Path to ranlib(1).
#    RANLIB		Path to ranlib(1).
#
#    CFLAGS		Flags passed to a compiler.
#    LDFLAGS		Flags passed to a linker.
#

.POSIX:
.SUFFIXES:
AR	= ar
RANLIB	= ranlib
CC	= cc
CFLAGS	= -DHAX_SOFTFLOAT
LDFLAGS	=

PREFIX ?=	/usr/local
HAX_LIBRARY ?=	lib/hax
BIN_DIR ?=	bin
LIB_DIR ?=	lib
INCLUDE_DIR ?=	include
MAN_DIR ?=	man
MAN3_DIR ?=	$(MAN_DIR)/man3
MANN_DIR ?=	$(MAN_DIR)/mann

all: libhax.a haxsh

GENERIC_OBJS =	haxRegexp.o haxAssem.o haxBasic.o haxCkalloc.o \
	haxCmdAH.o haxCmdIL.o haxCmdMZ.o haxExpr.o haxGet.o \
	haxHash.o haxHistory.o haxParse.o haxProc.o haxUtil.o \
	haxVar.o haxPanic.o

UNIX_OBJS = haxEnv.o haxGlob.o haxUnixAZ.o haxUnixStr.o haxUnixUtil.o

SOFTFP_OBJS = softfp/haxDoubleEq.o softfp/haxDoubleNeq.o softfp/Hax_strtod.o \
	softfp/haxLongLongToDouble.o softfp/haxDoubleMul.o \
	softfp/haxDoubleDiv.o softfp/haxDoubleAdd.o softfp/haxDoubleSub.o \
	softfp/haxDoubleLt.o softfp/haxDoubleGt.o softfp/haxDoubleLe.o \
	softfp/haxDoubleGe.o softfp/Hax_s_normRoundPackToF64.o \
	softfp/Hax_s_normRoundPackToF64.o softfp/Hax_s_normSubnormalF64Sig.o \
	softfp/Hax_s_roundPackToF32.o softfp/Hax_s_countLeadingZeros64.o \
	softfp/Hax_s_propagateNaNF64UI.o softfp/Hax_s_roundPackToF64.o \
	softfp/Hax_s_mul64To128M.o softfp/Hax_softfloat_raiseFlags.o \
	softfp/Hax_softfloat_state.o softfp/Hax_s_countLeadingZeros8.o \
	softfp/Hax_s_approxRecip32_1.o softfp/Hax_s_subMagsF64.o \
	softfp/Hax_s_approxRecip_1Ks.o softfp/Hax_s_addMagsF64.o \
	softfp/haxDoubleToLongLong.o

COMPAT_OBJS =

OBJS = $(GENERIC_OBJS) $(UNIX_OBJS) $(SOFTFP_OBJS) $(COMPAT_OBJS)

libhax.a: $(OBJS)
	$(AR) cr $@ $(OBJS)
	$(RANLIB) $@

haxsh: haxsh.o libhax.a
	$(CC) $(LDFLAGS) -o $@ haxsh.o libhax.a

install: libhax.a
	install -d $(DESTDIR)$(PREFIX)/$(BIN_DIR)
	install -d $(DESTDIR)$(PREFIX)/$(LIB_DIR)
	install -d $(DESTDIR)$(PREFIX)/$(HAX_LIBRARY)
	install -d $(DESTDIR)$(PREFIX)/$(INCLUDE_DIR)
	install -d $(DESTDIR)$(PREFIX)/$(MAN3_DIR)
	install -d $(DESTDIR)$(PREFIX)/$(MANN_DIR)

	install haxsh $(DESTDIR)$(PREFIX)/$(BIN_DIR)

	cd library; for i in *.tcl; do \
		install $$i $(DESTDIR)$(PREFIX)/$(HAX_LIBRARY); \
	done

	install libhax.a $(DESTDIR)$(PREFIX)/$(LIB_DIR)

	install hax.h $(DESTDIR)$(PREFIX)/$(INCLUDE_DIR)
	install haxHash.h $(DESTDIR)$(PREFIX)/$(INCLUDE_DIR)

	cd doc; for i in *.3; do \
		sed -e '/man\.macros/r man.macros' -e '/man\.macros/d' \
			$$i > Hax_$$i; \
		install Hax_$$i $(DESTDIR)$(PREFIX)/$(MAN3_DIR); \
		rm -f Hax_$$i; \
	done

	cd doc; \
		sed -e '/man\.macros/r man.macros' \
		    -e '/man\.macros/d' Hax.n > Hax.n.tmp; \
		install Hax.n.tmp $(DESTDIR)$(PREFIX)/$(MANN_DIR)/Hax.n; \
		rm -f Hax.n.tmp

	cd doc; \
		sed -e '/man\.macros/r man.macros' \
		    -e '/man\.macros/d' library.n > Hax_library.n; \
		install Hax_library.n $(DESTDIR)$(PREFIX)/$(MANN_DIR); \
		rm -f Hax_library.n

test: haxsh
	( echo cd tests ; echo source all ) | ./haxsh

clean:
	rm -f $(OBJS) libhax.a haxsh.o haxsh

$(OBJS): hax.h haxHash.h haxInt.h
$(UNIX_OJBS): haxUnix.h

.SUFFIXES: .c .o
.c.o:
	$(CC) $(CFLAGS) -I. -DHAX_LIBRARY=\"$(PREFIX)/$(HAX_LIBRARY)\" -o $@ -c $<
