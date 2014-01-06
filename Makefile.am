AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = -I m4
AM_CPPFLAGS = -I@top_srcdir@/src

SUBDIRS = vobla

EXTRA_DIST = \
  AUTHORS \
  Doxyfile \
  LICENSE \
  README.md \
  contrib/cpplint.py \
  contrib/pre-commit

api:
	doxygen

distclean-local:
	rm -rf api

static:
	make -C vobla static-analytic

.PHONY: api
