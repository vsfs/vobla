# vim: ts=8:st=8:noexpandtab
AM_LDFLAGS = -lpthread \
	     -L@top_srcdir@/test/gmock-1.7.0/lib \
	     -L@top_srcdir@/test/gmock-1.7.0/gtest/lib \
	     $(BOOST_FILESYSTEM_LIB) $(BOOST_SYSTEM_LIB)

AM_CXXFLAGS = -isystem @top_srcdir@/test/gmock-1.7.0/include \
	      -isystem @top_srcdir@/test/gmock-1.7.0/gtest/include

CLEANFILES = *.gcov *.gcno *.gcda

SUBDIRS = .
lib_LTLIBRARIES = libvobla.la

voblaincludedir = $(includedir)/vobla

nobase_voblainclude_HEADERS = \
  clock.h \
  consistent_hash_map.h \
  file.h \
  hash.h \
  lru_cache.h \
  macros.h \
  map_util.h \
  range.h \
  status.h \
  string_util.h \
  sysinfo.h \
  thread_pool.h \
  timer.h \
  traits.h \
  unique_resource.h

libvobla_la_LDFLAGS = $(BOOST_FILESYSTEM_LIB) $(BOOST_SYSTEM_LIB) $(LDFLAGS)
libvobla_la_CXXFLAGS = $(CXXFLAGS)
libvobla_la_SOURCES = \
  clock.h clock.cpp \
  file.h file.cpp \
  hash.h hash.cpp \
  lru_cache.h \
  macros.h \
  map_util.h \
  range.h \
  status.h status.cpp \
  stl_util.h \
  string_util.h string_util.cpp \
  sysinfo.h sysinfo.cpp \
  thread_pool.h thread_pool.cpp \
  timer.h timer.cpp \
  traits.h traits.cpp \
  unique_resource.h

analyze_srcs = $(filter %.cpp, $(libvobla_la_SOURCES))
analyze_plists = $(analyze_srcs:%.cpp=%.plist)

MOSTLYCLEANFILES = $(analyze_plists)

$(analyze_plists): %.plist: %.cpp
	@echo "  Static analyzing" $@
	@clang -I.. ${CXXFLAGS} ${LDFLAGS} --analyze $< -o $@
	@oclint -fatal-assembler-warnings ${@:.plist=.cpp} -- -I.. ${CXXFLAGS}

static-analysis: $(analyze_plists)

TESTS = \
  consistent_hash_map_test \
  file_test \
  hash_test \
  lru_cache_test \
  map_util_test \
  range_test \
  status_test \
  string_util_test \
  thread_pool_test \
  timer_test \
  traits_test \
  unique_resource_test

check_PROGRAMS = $(TESTS)

LDADD = -lgtest -lgtest_main -lgmock libvobla.la
consistent_hash_map_test_SOURCES = consistent_hash_map_test.cpp
file_test_SOURCES = file_test.cpp
hash_test_SOURCES = hash_test.cpp
lru_cache_test_SOURCES = lru_cache_test.cpp
map_util_test_SOURCES = map_util_test.cpp
range_test_SOURCES = range_test.cpp
status_test_SOURCES = status_test.cpp
string_util_test_SOURCES = string_util_test.cpp
thread_pool_test_SOURCES = thread_pool_test.cpp
timer_test_SOURCES = timer_test.cpp
traits_test_SOURCES = traits_test.cpp
unique_resource_test_SOURCES = unique_resource_test.cpp

.PHONY: static-analysis
