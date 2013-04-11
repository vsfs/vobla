# vim: ts=8:st=8:noexpandtab
AM_LDFLAGS = -lpthread -L@top_srcdir@/test/gtest-1.6.0/lib
AM_CXXFLAGS = -I@top_srcdir@/test/gtest-1.6.0/include

SUBDIRS = .
noinst_LIBRARIES = libvobla.a
libvobla_a_SOURCES = \
		     clock.h clock.cpp \
		     hash.h hash.cpp \
		     macors.h \
		     status.h status.cpp \
		     timer.h timer.cpp \
		     traits.h traits.cpp

TESTS = \
	hash_test \
	traits_test

check_PROGRAMS = $(TESTS)

LDADD = -lgtest -lgtest_main libvobla.a
hash_test_SOURCES = hash.h hash.cpp hash_test.cpp
traits_test_SOURCES = traits_test.cpp traits.h traits.cpp
