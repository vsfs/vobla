#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.61])
AC_INIT([vobla], [0.0.1], [eddyxu@gmail.com])
AM_INIT_AUTOMAKE
AC_LIBTOOL_DLOPEN
AC_PROG_LIBTOOL
AC_CONFIG_SRCDIR([vobla/status.cpp])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_LANG([C++])

CFLAGS="-Wall -Wextra -Werror -pedantic -Wunused-value $CFLAGS"
CXXFLAGS="$CFLAGS $CXXFLAGS"

# Checks for libraries.
AC_CHECK_LIB([glog], [openlog], [], [AC_MSG_ERROR(google-glog was not found.)])
AC_CHECK_LIB([crypto], [MD5_Init], [], [AC_MSG_ERROR([libcrypto was not found.])])

# Checks for header files.
AC_CHECK_HEADERS([sys/time.h fcntl.h stddef.h sys/ioctl.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_INT8_T
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_UINT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_OFF_T
AC_TYPE_PID_T

AC_CHECK_HEADER_STDBOOL
AX_CXX_COMPILE_STDCXX_11(noext)
if test "$ac_cv_cxx_compile_cxx11" = no ; then
	AC_MSG_ERROR("It requires C++ 11 to build.!")
fi

# Checks for library functions.
AC_CHECK_FUNCS([gettimeofday strerror memset strrchr])
AC_FUNC_ERROR_AT_LINE

AC_ARG_ENABLE([gcov],
		      [AS_HELP_STRING([--enable-gcov],
				  [enable coverage test (default: no).])],
			  [gcov_enabled=${enableval}], [gcov_enabled=no])
if test "x${gcov_enabled}" = xyes; then
  CFLAGS = "$CFLAGS -fprofile-arcs -ftest-coverage --coverage"
  CXXFLAGS="$CXXFLAGS -fprofile-arcs -ftest-coverage --coverage"
  LDFLAGS="$LDFLAGS -fprofile-arcs"
  LIBS="$LIBS -lgcov"
fi

AC_SUBST([AM_CXXFLAGS], ["$CXXFLAGS"])
AC_SUBST(LDFLAGS, "$LDFLAGS")
AC_SUBST(LIBS, "$LIBS")
AC_CONFIG_FILES([Makefile
		         vobla/Makefile])
AC_OUTPUT
