# vim: ts=8:st=8:noexpandtab
AM_LDFLAGS = -lpthread -L@top_srcdir@/test/gtest-1.6.0/lib
AM_CXXFLAGS = -I@top_srcdir@/test/gtest-1.6.0/include

CLEANFILES = *.gcov *.gcno *.gcda

SUBDIRS = .
noinst_LIBRARIES = libvobla.a
libvobla_a_SOURCES = \
		     clock.h clock.cpp \
		     consistent_hash_map.h \
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
		     traits.h traits.cpp

TESTS = \
	consistent_hash_map_test \
	hash_test \
	lru_cache_test \
	map_util_test \
	range_test \
	status_test \
	thread_pool_test \
	traits_test

check_PROGRAMS = $(TESTS)

LDADD = -lgtest -lgtest_main libvobla.a
consistent_hash_map_test_SOURCES = consistent_hash_map.h consistent_hash_map_test.cpp
hash_test_SOURCES = hash.h hash.cpp hash_test.cpp
lru_cache_test_SOURCES = lru_cache.h lru_cache_test.cpp
map_util_test_SOURCES = map_util.h map_util_test.cpp
range_test_SOURCES = range_test.cpp range.h
status_test_SOURCES = status_test.cpp status.h status.cpp
thread_pool_test_SOURCES = thread_pool_test.cpp thread_pool.h thread_pool.cpp
traits_test_SOURCES = traits_test.cpp traits.h traits.cpp
