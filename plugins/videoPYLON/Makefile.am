
ACLOCAL_AMFLAGS = -I ../../m4 -I .
AM_CPPFLAGS = -I$(srcdir)/../../src

EXTRA_DIST = pylon.m4 LICENSE.txt README.txt
EXTRA_DIST += win-vs2003/videoPYLON.sln win-vs2003/videoPYLON.vcproj
EXTRA_DIST += win-vs2008/videoPYLON.sln win-vs2008/videoPYLON.vcproj

plugindir=$(libdir)/Gem

plugin_LTLIBRARIES=
dist_plugin_DATA  =

if HAVE_PYLON
plugin_LTLIBRARIES+= gem_videoPYLON.la
dist_plugin_DATA  += pylon-videoplugin.pd
endif

gem_videoPYLON_la_CXXFLAGS =
gem_videoPYLON_la_LDFLAGS  = -module -avoid-version -shared
if WINDOWS
gem_videoPYLON_la_LDFLAGS += -no-undefined
endif
gem_videoPYLON_la_LIBADD   =

# RTE
gem_videoPYLON_la_CXXFLAGS += @GEM_RTE_CFLAGS@
gem_videoPYLON_la_LDFLAGS  += @GEM_RTE_LIBS@
#gem_videoPYLON_la_CXXFLAGS += @GEM_ARCH_CXXFLAGS@
#gem_videoPYLON_la_LDFLAGS  += @GEM_ARCH_LDFLAGS@
# #gem_videoPYLON_la @MOREFLAGS@

# Dependencies
gem_videoPYLON_la_CXXFLAGS += @GEM_PYLON_CXXFLAGS@
gem_videoPYLON_la_LIBADD   += @GEM_PYLON_LIBS@

# convenience symlinks
include $(srcdir)/../symlink_ltlib.mk


### SOURCES
gem_videoPYLON_la_SOURCES= videoPYLON.cpp videoPYLON.h \
	CameraProperties.cpp CameraProperties.h \
	StreamGrabberProperties.cpp StreamGrabberProperties.h


