SET( CMAKE_C_FLAGS_MAINTAINER "-Wall" CACHE STRING
    "Flags used by the C compiler during maintainer builds."
    FORCE )

SET( CMAKE_EXE_LINKER_FLAGS_MAINTAINER
    "-Wl" CACHE STRING
    "Flags used for linking binaries during maintainer builds."
    FORCE )

SET( CMAKE_SHARED_LINKER_FLAGS_MAINTAINER
    "-Wl" CACHE STRING
    "Flags used by the shared libraries linker during maintainer builds."
    FORCE )

MARK_AS_ADVANCED(
    CMAKE_C_FLAGS_MAINTAINER
    CMAKE_EXE_LINKER_FLAGS_MAINTAINER
    CMAKE_SHARED_LINKER_FLAGS_MAINTAINER )

SET( CMAKE_BUILD_TYPE "${CMAKE_BUILD_TYPE}" CACHE STRING
    "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel Maintainer."
    FORCE )


