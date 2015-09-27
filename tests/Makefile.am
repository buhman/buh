AM_CFLAGS = \
	-I ../include \
	$(LIBCHECK_CFLAGS)

AM_LDFLAGS = \
	$(LIBCHECK_LIBS)

TESTS = \
	check_buh_hash

check_PROGRAMS = \
	check_buh_hash

check_buh_hash_SOURCES = check_buh_hash.c
check_buh_hash_LDADD = $(top_builddir)/lib/libbuh.la
