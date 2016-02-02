# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([filescanner], [0.1], [user@local.com])
AM_INIT_AUTOMAKE([1.9])

AC_LANG([C++])
AC_CONFIG_SRCDIR([src/main.cc])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
#AC_PROG_CC
AC_PROG_AWK
AC_PROG_INSTALL
AC_PROG_MKDIR_P

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lssl':
AC_CHECK_LIB([ssl], [main])
AC_CHECK_LIB([magic], [main])

# Checks for command line options
AC_ARG_ENABLE([threads], [  --enable-threads   enable threads], [threads=yes], [threads=no])

# Checks for header files.
if test "x${enable_threads}" = "xyes"; then
  AC_SEARCH_LIBS([pthread_create], [pthread], [have_pthreads=yes])
  if test "x${have_pthreads}" = "xyes"; then
    AC_CHECK_HEADERS([pthread.h], [], [have_pthreads=no])
  fi
  if test "x${enable_threads}" = "xno"; then
    echo "------------------------------------------"
    echo " Unable to find pthreads on this system.  "
    echo " Building a single-threaded version.      "
    echo "------------------------------------------"
    threads=no
  else
    echo "------------------------------------------"
    echo " Found pthreads on this system.  "
    echo " Building a threaded version.      "
    echo "------------------------------------------"
    threads=yes
  fi
else
    echo "------------------------------------------"
    echo " Found pthreads on this system.  "
    echo " Building a non-threaded version.      "
    echo "------------------------------------------"
    threads=no
fi

AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h])
AC_CHECK_HEADERS([string])
AC_CHECK_HEADERS([iostream])
AC_CHECK_HEADERS([magic.h])
AC_CHECK_HEADERS([openssl/sha.h])
AC_CHECK_HEADERS([unistd.h])
AC_CHECK_HEADERS([string.h])
AC_CHECK_HEADERS([arpa/inet.h]) 
AC_CHECK_HEADERS([fcntl.h]) 
AC_CHECK_HEADERS([netdb.h]) 
AC_CHECK_HEADERS([regex.h]) 
AC_CHECK_HEADERS([netinet/in.h]) 
AC_CHECK_HEADERS([sys/mount.h]) 
AC_CHECK_HEADERS([sys/socket.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_SIZE_T

# Checks for library functions.
AC_CHECK_FUNCS([getcwd gethostbyname memset regcomp socket strstr])

AC_SUBST([AM_CXXFLAGS])
AC_SUBST([AM_LDFLAGS])

AC_CONFIG_FILES([Makefile
                 src/Makefile])

# introduce the optional configure parameter for a non-standard install prefix of XXX
AC_ARG_WITH([XXX],
    [AS_HELP_STRING([--with-XXX=prefix],
        [try this for a non-standard install prefix of the XXX library])],
    [XXXPATHSET=1],
    [XXXPATHSET=0])


AC_OUTPUT
