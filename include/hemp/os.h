#ifndef HEMP_OS_H
#define HEMP_OS_H

#ifdef WIN32
# include <hemp/os/win32.h>
#else
# include <hemp/os/unix.h>
#endif

#endif /* HEMP_OS_H */