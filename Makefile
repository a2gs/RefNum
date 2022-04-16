# Andre Augusto Giannotti Scota (https://sites.google.com/view/a2gs/)
# C flags:
CFLAGS_OPTIMIZATION = -g
#CFLAGS_OPTIMIZATION = -O3
CFLAGS_VERSION = -std=c11
CFLAGS_WARNINGS = -Wall -Wextra -Wno-unused-parameter -Wno-unused-but-set-parameter -Wformat-truncation=0
CFLAGS_DEFINES = -D_XOPEN_SOURCE=700 -D_POSIX_C_SOURCE=200809L -D_POSIX_SOURCE=1 -D_DEFAULT_SOURCE=1 -D_GNU_SOURCE=1 -DBTAVL_TRANSVERSAL=1 -DBTAVL_DEBUG
CFLAGS = $(CFLAGS_OPTIMIZATION) $(CFLAGS_VERSION) $(CFLAGS_WARNINGS) $(CFLAGS_DEFINES)

# System shell utilities
CC = gcc
RM = rm -fr
CP = cp
AR = ar
RANLIB = ranlib
CPPCHECK = cppcheck

INCLUDEPATH = -I./
LIBS = -lm -lpthread
LIBPATH = -L./

all: clean librefnum rncmd rnsrv

librefnum:
	@echo
	@echo "=== librefnum ================="
	$(CC) -o refnum.o -c refnum.c $(CFLAGS) $(LIBS)
	$(AR) rc librefnum.a refnum.o
	$(RANLIB) librefnum.a
	-$(RM) refnum.o

rncmd: librefnum
	@echo
	@echo "=== Compiling cmdtool =============="
	$(CC) -o rncmd rncmd.c -lrefnum $(CFLAGS) $(INCLUDEPATH) $(LIBPATH) $(LIBS)

rnsrv: librefnum
	@echo
	@echo "=== Compiling server =============="
	$(CC) -o rnsrv rnsrv.c -lrefnum $(CFLAGS) $(INCLUDEPATH) $(LIBPATH) $(LIBS)

clean:
	@echo
	@echo "=== clean_data =============="
	-$(RM) rncmd rnsrv librefnum.a core
