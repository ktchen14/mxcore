AC_PREREQ([2.67])

AC_INIT([Vector], [0.1.0], [ktchen14@gmail.com], [vector],
        [https://github.com/ktchen14/vector])
AC_CONFIG_SRCDIR([source/vector.c])
AC_CONFIG_AUX_DIR([tool])
AC_CONFIG_MACRO_DIRS([tool])

AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects no-dist-gzip dist-xz])

AM_PROG_AR
LT_INIT
AC_PROG_CC

PKG_INSTALLDIR
AC_SUBST([PACKAGE_DESCRIPTION], ["An unobtrusive vector implementation in C"])
AC_CONFIG_FILES([data/vector.pc])

AC_OUTPUT([Makefile header/Makefile])
