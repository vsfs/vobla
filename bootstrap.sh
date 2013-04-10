#!/bin/sh -
#
# Re-generate ./configure and Makefile.in files.
#
# Copyright 2013 (c) Lei Xu <eddyxu@gmail.com>

cp contrib/pre-commit .git/hooks
case `uname` in
	Darwin*) glibtoolize --copy ;;  # For MacOSX
    *) libtoolize --copy ;; # Otherwise.
esac

autoheader
aclocal -I m4 --install
autoconf

automake --foreign --add-missing --force-missing --copy
