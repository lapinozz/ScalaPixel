/*
 * Public domain
 * dirent.h compatibility shim
 */

#ifndef LIBCRYPTOCOMPAT_DIRENT_H
#define LIBCRYPTOCOMPAT_DIRENT_H

#ifdef _MSC_VER
#include <windows.h>
#include "dirent_window.h"
#else
#include <dirent.h>
#endif

#endif
