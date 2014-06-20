# this is example-file: configure.ac

# initial information about the project
AC_INIT([mcp2200_simpleio],[1.0],[jboulnois@gmail.com])

# Checks for programs

# check for C++ preprocessor and compiler and the library compiler
AC_PROG_CXXCPP
AC_PROG_CXX
AC_PROG_RANLIB

# automake initialisation and check for minimal automake API version 1.9
AM_INIT_AUTOMAKE([1.9])

# use the C++ compiler for the following checks
AC_LANG([C++])

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([string])
AC_CHECK_HEADERS([iostream])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T


##########################################################################
# check for hidapi library
##########################################################################

# store current *FLAGS and merge with AM_*FLAGS for compilation and linker check   
OLD_CXXFLAGS=$CXXFLAGS;
OLD_LDFLAGS=$LDFLAGS;
CXXFLAGS="$AM_CXXFLAGS $CXXFLAGS"
LDFLAGS="$AM_LDFLAGS $LDFLAGS"

# ensure the library to check for is covered by the LIBS variable
OLD_LIBS=$LIBS
LIBS="$LIBS -lhidapi"

# check for hidapi library headers   
AC_MSG_CHECKING([for the hidapi library headers])
# try to compile a file that includes a header of the library hidapi
AC_COMPILE_IFELSE([[#include <hidapi/hidapi.h>]],
    [AC_MSG_RESULT([found])
        # try to link the function 'hid_enumerate' out of library hidapi
        AC_MSG_CHECKING([whether the hidapi library can be linked])
        AC_LINK_IFELSE(
            [AC_LANG_PROGRAM([[#include <hidapi/hidapi.h>]],
                [[hid_enumerate(0x0, 0x0);]])],
            [AC_MSG_RESULT([yes])
                FOUND_HIDAPI=1;],
            [AC_MSG_RESULT([no])
                LIBS=$OLD_LIBS; dnl reset to old value since hidapi was not found
                FOUND_HIDAPI=0;])],
    [AC_MSG_RESULT([not found])
        FOUND_HIDAPI=0;])

# reset original *FLAGS
CXXFLAGS=$OLD_CXXFLAGS
LDFLAGS=$OLD_LDFLAGS

# handle check results
if test $FOUND_HIDAPI != 1; then
    AC_MSG_NOTICE([])
    AC_MSG_NOTICE([The hidapi library was not found!])
    if test  $HIDAPIPATHSET = 1 ; then
        AC_MSG_NOTICE([ The install prefix '$with_hidapi' for the XXX library was set.])
        AC_MSG_NOTICE([ --> Maybe wrong ???])
    else
        AC_MSG_NOTICE([ No non-standard install prefix was set.])
        AC_MSG_NOTICE([ --> You might want to use '--with-hidapi=PREFIX' ?!?])
    fi
    AC_MSG_NOTICE([])
    AC_MSG_ERROR([ hidapi library is an essential dependency : cannot build and stop here !])
fi

##########################################################################

# distribute additional compiler and linker flags
# --> set these variables instead of CXXFLAGS or LDFLAGS
AC_SUBST([AM_CXXFLAGS])
AC_SUBST([AM_LDFLAGS])

# files to generate via autotools (.am or .in source files)
AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([src/Makefile])
AC_CONFIG_FILES([tools/Makefile])

# generate the final Makefile etc.
AC_OUTPUT
