This branch uses CMake for the configuration, build and installation.

    http://www.cmake.org/

I don't like the idea of requiring end users to install CMake before they
can build hemp, but I have to concede that it's a breath of fresh air after
using autotools.  I now actually feel I know what I'm doing instead of 
stumbling around blindly.  Furthermore, CMake is easy to install and makes
it easy to build and package platform-specific versions of hemp (from what
I can see - I haven't actually tried it yet).  So Linux users shouldn't be
too worried about installing it, and we can provide pre-compiled versions
for Mac/Win with nice installers so they don't have to worry themselves
about compiling it in the first place unless they really want to.

So it looks increasingly likely that I'll be settling on CMake for the 
build system and this branch will get merged back into master.

-- abw 16th Dec 2010
