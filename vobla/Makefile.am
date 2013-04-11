# vim: ts=8:st=8:noexpandtab
AM_LDFLAGS = -lpthread

SUBDIRS = .
noinst_LIBRARIES = libvobla.a
libvobla_a_SOURCES = \
		     clock.h clock.cpp \
		     status.h status.cpp \
		     traits.h traits.cpp

TESTS = \
	traits_test

check_PROGRAMS = $(TESTS)

LDADD = -lgtest -lgtest_main libvobla.a
traits_test_SOURCES = traits_test.cpp traits.h traits.cpp
