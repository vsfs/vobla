# vim: ts=8:st=8:noexpandtab
AM_LDFLAGS = -lpthread -L@top_srcdir@/test/gtest-1.6.0/lib
AM_CXXFLAGS = -I@top_srcdir@/test/gtest-1.6.0/include

CLEANFILES = *.gcov *.gcno *.gcda

SUBDIRS = .
lib_LTLIBRARIES = libvobla.la

voblaincludedir = $(includedir)/vobla

nobase_voblainclude_HEADERS = \
  clock.h \
  hash.h \
  lru_cache.h \
  macros.h \
  map_util.h \
  range.h \
  status.h \
  sysinfo.h \
  thread_pool.h \
  timer.h \
  traits.h \
  unique_resource.h

libvobla_la_SOURCES = \
		     clock.h clock.cpp \
		     hash.h hash.cpp \
		     lru_cache.h \
		     macros.h \
		     map_util.h \
		     range.h \
		     status.h status.cpp \
		     stl_util.h \
		     sysinfo.h sysinfo.cpp \
		     thread_pool.h thread_pool.cpp \
		     timer.h timer.cpp \
		     traits.h traits.cpp \
		     unique_resource.h

TESTS = \
	hash_test \
	lru_cache_test \
	map_util_test \
	range_test \
	status_test \
	thread_pool_test \
	traits_test \
	unique_resource_test

check_PROGRAMS = $(TESTS)

LDADD = -lgtest -lgtest_main libvobla.la
hash_test_SOURCES = hash_test.cpp
lru_cache_test_SOURCES = lru_cache_test.cpp
map_util_test_SOURCES = map_util_test.cpp
range_test_SOURCES = range_test.cpp
status_test_SOURCES = status_test.cpp
thread_pool_test_SOURCES = thread_pool_test.cpp
traits_test_SOURCES = traits_test.cpp
unique_resource_test_SOURCES = unique_resource_test.cpp
