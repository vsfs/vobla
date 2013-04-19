# vim: ts=8:st=8:noexpandtab
AM_LDFLAGS = -lpthread -L@top_srcdir@/test/gtest-1.6.0/lib
AM_CXXFLAGS = -I@top_srcdir@/test/gtest-1.6.0/include

SUBDIRS = .
noinst_LIBRARIES = libvobla.a
libvobla_a_SOURCES = \
		     stl_util.h \
		     clock.h clock.cpp \
		     hash.h hash.cpp \
		     macros.h \
		     map_util.h \
		     range.h \
		     status.h status.cpp \
		     timer.h timer.cpp \
		     traits.h traits.cpp

TESTS = \
	hash_test \
	map_util_test \
	range_test \
	status_test \
	traits_test

check_PROGRAMS = $(TESTS)

LDADD = -lgtest -lgtest_main libvobla.a
hash_test_SOURCES = hash.h hash.cpp hash_test.cpp
map_util_test_SOURCES = map_util.h map_util_test.cpp
range_test_SOURCES = range_test.cpp range.h
status_test_SOURCES = status_test.cpp status.h status.cpp
traits_test_SOURCES = traits_test.cpp traits.h traits.cpp
