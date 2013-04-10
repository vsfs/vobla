# vim: ts=8:st=8:noexpandtab
AM_LDFLAGS = -lpthread

SUBDIRS = .
noinst_LIBRARIES = libvobla.a
libvobla_a_SOURCES = \
		     status.h status.cpp

TESTS =

check_PROGRAMS = $(TESTS)

LDADD = libvobla.a  $(GTEST_LIBS)
