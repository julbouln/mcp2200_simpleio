# this is example-file: tools/Makefile.am

# additional include paths necessary to compile the C++ programs
AM_CXXFLAGS = -I$(top_srcdir)/src @AM_CXXFLAGS@

###############################################################################
# THE PROGRAMS TO BUILD
###############################################################################

# the program to build (the names of the final binaries)
bin_PROGRAMS = mcp2200_config

# list of sources for the 'mcp2200_config' binary
mcp2200_config_SOURCES =    \
                        mcp2200_config.cpp

# the additional libraries needed to link mcp2200_config
mcp2200_config_LDADD =  $(top_builddir)/src/libmcp2200_simpleio.a $(AM_LDFLAGS)

###############################################################################