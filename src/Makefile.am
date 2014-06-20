# this is example-file: src/mylib/Makefile.am

# additional include paths necessary to compile the C++ library
AM_CXXFLAGS = -I$(top_srcdir) @AM_CXXFLAGS@

###############################################################################
# THE LIBRARIES TO BUILD
###############################################################################

# the library names to build (note we are building static libs only)
lib_LIBRARIES = libmcp2200_simpleio.a

# where to install the headers on the system
libmcp2200_simpleio_adir = $(includedir)

# the list of header files that belong to the library (to be installed later)
libmcp2200_simpleio_a_HEADERS =    \
                        mcp2200_simpleio.h 

# the sources to add to the library and to add to the source distribution
libmcp2200_simpleio_a_SOURCES =    \
                        $(libmcp2200_simpleio_a_HEADERS) \
                        mcp2200_simpleio.cpp 
						
###############################################################################
