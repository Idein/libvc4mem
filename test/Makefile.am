AM_CFLAGS = -pipe -O2 -g -W -Wall -Wextra -I$(top_srcdir)/include

check_PROGRAMS = speed_cached_noncached

nodist_speed_cached_noncached_SOURCES = speed_cached_noncached.c
speed_cached_noncached_LDADD = $(top_builddir)/src/.libs/libvc4mem.a
