#include "pipsdef.h"

/*
 * file for p4a_stubs.c
 */
/* P4A include <stdc-predef.h> starts here */
void p4a_dummy_recover();
/* Copyright (C) 1991-2014 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */




/* This header is separate from features.h so that the compiler can
   include it implicitly at the start of every compilation.  It must
   not itself include <features.h> or any other header that includes
   <features.h> because the implicit include comes before any feature
   test macros that may be defined in a source file before it first
   explicitly includes a system header.  GCC knows the name of this
   header in order to preinclude it.  */

/* glibc's intent is to support the IEC 559 math functionality, real
   and complex.  If the GCC (4.9 and later) predefined macros
   specifying compiler intent are available, use them to determine
   whether the overall intent is to support these features; otherwise,
   presume an older compiler has intent to support these features and
   define these macros by default.  */
/* wchar_t uses ISO/IEC 10646 (2nd ed., published 2011-03-15) /
   Unicode 6.0.  */


/* We do not support C11 <threads.h>.  */
/* Copyright (C) 1991-2014 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

/*
 *	ISO C99 Standard: 7.20 General utilities	<stdlib.h>
 */



/* Copyright (C) 1991-2014 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */




/* These are defined by the user (or the compiler)
   to specify the desired environment:

   __STRICT_ANSI__	ISO Standard C.
   _ISOC99_SOURCE	Extensions to ISO C89 from ISO C99.
   _ISOC11_SOURCE	Extensions to ISO C99 from ISO C11.
   _POSIX_SOURCE	IEEE Std 1003.1.
   _POSIX_C_SOURCE	If ==1, like _POSIX_SOURCE; if >=2 add IEEE Std 1003.2;
			if >=199309L, add IEEE Std 1003.1b-1993;
			if >=199506L, add IEEE Std 1003.1c-1995;
			if >=200112L, all of IEEE 1003.1-2004
			if >=200809L, all of IEEE 1003.1-2008
   _XOPEN_SOURCE	Includes POSIX and XPG things.  Set to 500 if
			Single Unix conformance is wanted, to 600 for the
			sixth revision, to 700 for the seventh revision.
   _XOPEN_SOURCE_EXTENDED XPG things and X/Open Unix extensions.
   _LARGEFILE_SOURCE	Some more functions for correct standard I/O.
   _LARGEFILE64_SOURCE	Additional functionality from LFS for large files.
   _FILE_OFFSET_BITS=N	Select default filesystem interface.
   _BSD_SOURCE		ISO C, POSIX, and 4.3BSD things.
   _SVID_SOURCE		ISO C, POSIX, and SVID things.
   _ATFILE_SOURCE	Additional *at interfaces.
   _GNU_SOURCE		All of the above, plus GNU extensions.
   _DEFAULT_SOURCE	The default set of features (taking precedence over
			__STRICT_ANSI__).
   _REENTRANT		Select additionally reentrant object.
   _THREAD_SAFE		Same as _REENTRANT, often used by other systems.
   _FORTIFY_SOURCE	If set to numeric value > 0 additional security
			measures are defined, according to level.

   The `-ansi' switch to the GNU C compiler, and standards conformance
   options such as `-std=c99', define __STRICT_ANSI__.  If none of
   these are defined, or if _DEFAULT_SOURCE is defined, the default is
   to have _SVID_SOURCE, _BSD_SOURCE, and _POSIX_SOURCE set to one and
   _POSIX_C_SOURCE set to 200809L.  If more than one of these are
   defined, they accumulate.  For example __STRICT_ANSI__,
   _POSIX_SOURCE and _POSIX_C_SOURCE together give you ISO C, 1003.1,
   and 1003.2, but nothing else.

   These are defined by this file and are used by the
   header files to decide what to declare or define:

   __USE_ISOC11		Define ISO C11 things.
   __USE_ISOC99		Define ISO C99 things.
   __USE_ISOC95		Define ISO C90 AMD1 (C95) things.
   __USE_POSIX		Define IEEE Std 1003.1 things.
   __USE_POSIX2		Define IEEE Std 1003.2 things.
   __USE_POSIX199309	Define IEEE Std 1003.1, and .1b things.
   __USE_POSIX199506	Define IEEE Std 1003.1, .1b, .1c and .1i things.
   __USE_XOPEN		Define XPG things.
   __USE_XOPEN_EXTENDED	Define X/Open Unix things.
   __USE_UNIX98		Define Single Unix V2 things.
   __USE_XOPEN2K        Define XPG6 things.
   __USE_XOPEN2KXSI     Define XPG6 XSI things.
   __USE_XOPEN2K8       Define XPG7 things.
   __USE_XOPEN2K8XSI    Define XPG7 XSI things.
   __USE_LARGEFILE	Define correct standard I/O things.
   __USE_LARGEFILE64	Define LFS things with separate names.
   __USE_FILE_OFFSET64	Define 64bit interface as default.
   __USE_BSD		Define 4.3BSD things.
   __USE_SVID		Define SVID things.
   __USE_MISC		Define things common to BSD and System V Unix.
   __USE_ATFILE		Define *at interfaces and AT_* constants for them.
   __USE_GNU		Define GNU extensions.
   __USE_REENTRANT	Define reentrant/thread-safe *_r functions.
   __USE_FORTIFY_LEVEL	Additional security measures used, according to level.

   The macros `__GNU_LIBRARY__', `__GLIBC__', and `__GLIBC_MINOR__' are
   defined by this file unconditionally.  `__GNU_LIBRARY__' is provided
   only for compatibility.  All new code should use the other symbols
   to test for features.

   All macros listed above as possibly being defined by this file are
   explicitly undefined if they are not explicitly defined.
   Feature-test macros that are not defined by the user or compiler
   but are implied by the other feature-test macros defined (or by the
   lack of any definitions) are defined by the file.  */


/* Undefine everything, so we get a clean slate.  */
/* Suppress kernel-name space pollution unless user expressedly asks
   for it.  */




/* Convenience macros to test the versions of glibc and gcc.
   Use them like this:
   #if __GNUC_PREREQ (2,8)
   ... code requiring gcc 2.8 or later ...
   #endif
   Note - they won't work for gcc1 or glibc1, since the _MINOR macros
   were not defined then.  */
/* If _GNU_SOURCE was defined by the user, turn on all the other features.  */
/* If nothing (other than _GNU_SOURCE and _DEFAULT_SOURCE) is defined,
   define _DEFAULT_SOURCE, _BSD_SOURCE and _SVID_SOURCE.  */
/* This is to enable the ISO C11 extension.  */





/* This is to enable the ISO C99 extension.  */





/* This is to enable the ISO C90 Amendment 1:1995 extension.  */





/* This is to enable compatibility for ISO C++11.

   So far g++ does not provide a macro.  Check the temporary macro for
   now, too.  */





/* If none of the ANSI/POSIX macros are defined, or if _DEFAULT_SOURCE
   is defined, use POSIX.1-2008 (or another version depending on
   _XOPEN_SOURCE).  */
/* Get definitions of __STDC_* predefined macros, if the compiler has
   not preincluded this header automatically.  */
/* Copyright (C) 1991-2014 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

/* This macro indicates that the installed library is the GNU C Library.
   For historic reasons the value now is 6 and this will stay from now
   on.  The use of this variable is deprecated.  Use __GLIBC__ and
   __GLIBC_MINOR__ now (see below) when you want to test for a specific
   GNU C library version and use the values in <gnu/lib-names.h> to get
   the sonames of the shared libraries.  */



/* Major and minor version number of the GNU C library package.  Use
   these macros to test for features in specific releases.  */






/* This is here only because every header file already includes this one.  */


/* Copyright (C) 1992-2014 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */




/* We are almost always included from features.h. */




/* The GNU libc does not support any K&R compilers or the traditional mode
   of ISO C compilers anymore.  Check for some of the combinations not
   anymore supported.  */




/* Some user header file might have defined this before.  */
/* These two macros are not used in glibc anymore.  They are kept here
   only because some other projects expect the macros to be defined.  */



/* For these things, GCC behaves the ANSI way normally,
   and the non-ANSI way under -traditional.  */




/* This is not a typedef so `const __ptr_t' does the right thing.  */




/* C++ needs to know that types and declarations are C, not C++.  */
/* The standard library needs the functions from the ISO C90 standard
   in the std namespace.  At the same time we want to be safe for
   future changes and we include the ISO C99 code in the non-standard
   namespace __c99.  The C++ wrapper header take case of adding the
   definitions to the global namespace.  */
/* For compatibility we do not add the declarations into any
   namespace.  They will end up in the global namespace which is what
   old code expects.  */
/* Fortify support.  */
/* Support for flexible arrays.  */
/* Some other non-C99 compiler.  Approximate with [1].  */






/* __asm__ ("xyz") is used throughout the headers to rename functions
   at the assembly language level.  This is wrapped by the __REDIRECT
   macro, in order to support compilers that can do this some other
   way.  When compilers don't support asm-names at all, we have to do
   preprocessor tricks instead (which don't have exactly the right
   semantics, but it's the best we can do).

   Example:
   int __REDIRECT(setpgrp, (__pid_t pid, __pid_t pgrp), setpgid); */
/* GCC has various useful declarations that can be made with the
   `__attribute__' syntax.  All of the ways we use this do fine if
   they are omitted for compilers that don't understand it. */




/* At some point during the gcc 2.96 development the `malloc' attribute
   for functions was introduced.  We don't want to use it unconditionally
   (although this would be possible) since it generates warnings.  */






/* Tell the compiler which arguments to an allocation function
   indicate the size of the allocation.  */







/* At some point during the gcc 2.96 development the `pure' attribute
   for functions was introduced.  We don't want to use it unconditionally
   (although this would be possible) since it generates warnings.  */






/* This declaration tells the compiler that the value is constant.  */






/* At some point during the gcc 3.1 development the `used' attribute
   for functions was introduced.  We don't want to use it unconditionally
   (although this would be possible) since it generates warnings.  */
/* gcc allows marking deprecated functions.  */






/* At some point during the gcc 2.8 development the `format_arg' attribute
   for functions was introduced.  We don't want to use it unconditionally
   (although this would be possible) since it generates warnings.
   If several `format_arg' attributes are given for the same function, in
   gcc-3.0 and older, all but the last one are ignored.  In newer gccs,
   all designated arguments are considered.  */






/* At some point during the gcc 2.97 development the `strfmon' format
   attribute for functions was introduced.  We don't want to use it
   unconditionally (although this would be possible) since it
   generates warnings.  */







/* The nonull function attribute allows to mark pointer parameters which
   must not be NULL.  */






/* If fortification mode, we warn about unused results of certain
   function calls which can lead to problems.  */
/* Forces a function to be always inlined.  */






/* Associate error messages with the source location of the call site rather
   than with the source location inside the function.  */
/* GCC 4.3 and above allow passing all anonymous arguments of an
   __extern_always_inline function to some other vararg function.  */





/* It is possible to compile containing GCC extensions even if GCC is
   run in pedantic mode if the uses are carefully marked using the
   `__extension__' keyword.  But this is not generally available before
   version 2.8.  */




/* __restrict is known in EGCS 1.2 and above. */




/* ISO C99 also allows to declare arrays as non-overlapping.  The syntax is
     array_name[restrict]
   GCC 3.1 supports this.  */
/* Some other non-C99 compiler.  */
/* Determine the wordsize from the preprocessor defines.  */
/* Both x86-64 and x32 use the 64-bit system call interface.  */


/* If we don't have __REDIRECT, prototypes will be missing if
   __USE_FILE_OFFSET64 but not __USE_LARGEFILE[64]. */







/* Decide whether we can define 'extern inline' functions in headers.  */







/* This is here only because every header file already includes this one.
   Get the definitions of all the appropriate `__stub_FUNCTION' symbols.
   <gnu/stubs.h> contains `#define __stub_FUNCTION' when FUNCTION is a stub
   that will always return failure (and set errno to ENOSYS).  */
/* This file is automatically generated.
   This file selects the right generated file of `__stub_FUNCTION' macros
   based on the architecture being compiled for.  */






/* This file is automatically generated.
   It defines a symbol `__stub_FUNCTION' for each function
   in the C library which is a stub, meaning it will fail
   every time called, usually setting errno to ENOSYS.  */

/* Get size_t, wchar_t and NULL from <stddef.h>.  */





/* Copyright (C) 1989-2013 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

Under Section 7 of GPL version 3, you are granted additional
permissions described in the GCC Runtime Library Exception, version
3.1, as published by the Free Software Foundation.

You should have received a copy of the GNU General Public License and
a copy of the GCC Runtime Library Exception along with this program;
see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
<http://www.gnu.org/licenses/>.  */

/*
 * ISO C Standard:  7.17  Common definitions  <stddef.h>
 */






/* Any one of these symbols __need_* means that GNU libc
   wants us just to define one data type.  So don't define
   the symbols that indicate this file's entire job has been done.  */
/* This avoids lossage on SunOS but only if stdtypes.h comes first.
   There's no way to win with the other order!  Sun lossage.  */

/* On 4.3bsd-net2, make sure ansi.h is included, so we have
   one less case to deal with in the following.  */



/* On FreeBSD 5, machine/ansi.h does not exist anymore... */




/* In 4.3bsd-net2, machine/ansi.h defines these symbols, which are
   defined if the corresponding type is *not* defined.
   FreeBSD-2.1 defines _MACHINE_ANSI_H_ instead of _ANSI_H_.
   NetBSD defines _I386_ANSI_H_ and _X86_64_ANSI_H_ instead of _ANSI_H_ */
/* Sequent's header files use _PTRDIFF_T_ in some conflicting way.
   Just ignore it.  */




/* On VxWorks, <type/vxTypesBase.h> may have defined macros like
   _TYPE_size_t which will typedef size_t.  fixincludes patched the
   vxTypesBase.h so that this macro is only defined if _GCC_SIZE_T is
   not defined, and so that defining this macro defines _GCC_SIZE_T.
   If we find that the macros are still defined at this point, we must
   invoke them so that the type is defined as expected.  */
/* In case nobody has defined these types, but we aren't running under
   GCC 2.00, make sure that __PTRDIFF_TYPE__, __SIZE_TYPE__, and
   __WCHAR_TYPE__ have reasonable values.  This can happen if the
   parts of GCC is compiled by an older compiler, that actually
   include gstddef.h, such as collect2.  */

/* Signed type of difference of two pointers.  */

/* Define this type if we are doing the whole job,
   or if we want this type in particular.  */
/* Unsigned type of `sizeof' something.  */

/* Define this type if we are doing the whole job,
   or if we want this type in particular.  */
typedef unsigned long int size_t;
/* Wide character type.
   Locale-writers should change this as necessary to
   be big enough to hold unique values not between 0 and 127,
   and not (wchar_t) -1, for each defined multibyte character.  */

/* Define this type if we are doing the whole job,
   or if we want this type in particular.  */
/* On BSD/386 1.1, at least, machine/ansi.h defines _BSD_WCHAR_T_
   instead of _WCHAR_T_, and _BSD_RUNE_T_ (which, unlike the other
   symbols in the _FOO_T_ family, stays defined even after its
   corresponding type is defined).  If we define wchar_t, then we
   must undef _WCHAR_T_; for BSD/386 1.1 (and perhaps others), if
   we undef _WCHAR_T_, then we must also define rune_t, since 
   headers like runetype.h assume that if machine/ansi.h is included,
   and _BSD_WCHAR_T_ is not defined, then rune_t is available.
   machine/ansi.h says, "Note that _WCHAR_T_ and _RUNE_T_ must be of
   the same type." */
/* FreeBSD 5 can't be handled well using "traditional" logic above
   since it no longer defines _BSD_RUNE_T_ yet still desires to export
   rune_t in some cases... */
typedef int wchar_t;
/*  In 4.3bsd-net2, leave these undefined to indicate that size_t, etc.
    are already defined.  */
/*  BSD/OS 3.1 and FreeBSD [23].x require the MACHINE_ANSI_H check here.  */
/*  NetBSD 5 requires the I386_ANSI_H and X86_64_ANSI_H checks here.  */
/* A null pointer constant.  */







/* XPG requires a few symbols from <sys/wait.h> being defined.  */
/* Definitions of flag bits for `waitpid' et al.
   Copyright (C) 1992-2014 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */






/* Bits in the third argument to `waitpid'.  */



/* Bits in the fourth argument to `waitid'.  */
/* The following values are used by the `waitid' function.  */




/* The Linux kernel defines these bare, rather than an enum,
   which causes a conflict if the include order is reversed. */






/* Wait for any child.  */
/* Wait for specified process.  */
/* Wait for members of process group.  */
typedef enum {P_ALL, P_PID, P_PGID} idtype_t;
/* Definitions of status bits for `wait' et al.
   Copyright (C) 1992-2014 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */






/* Everything extant so far uses these same bits.  */


/* If WIFEXITED(STATUS), the low-order 8 bits of the status.  */


/* If WIFSIGNALED(STATUS), the terminating signal.  */


/* If WIFSTOPPED(STATUS), the signal that stopped the child.  */


/* Nonzero if STATUS indicates normal termination.  */


/* Nonzero if STATUS indicates termination by a signal.  */



/* Nonzero if STATUS indicates the child is stopped.  */


/* Nonzero if STATUS indicates the child continued after a stop.  We only
   define this if <bits/waitflags.h> provides the WCONTINUED flag bit.  */




/* Nonzero if STATUS indicates the child dumped core.  */


/* Macros for constructing status values.  */
/* Copyright (C) 1992-2014 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */




/* Copyright (C) 1991-2014 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

/* Definitions for byte order, according to significance of bytes,
   from low addresses to high addresses.  The value is what you get by
   putting '4' in the most significant byte, '3' in the second most
   significant byte, '2' in the second least significant byte, and '1'
   in the least significant byte, and then writing down one digit for
   each byte, starting with the byte at the lowest address at the left,
   and proceeding to the byte with the highest address at the right.  */





/* This file defines `__BYTE_ORDER' for the particular machine.  */
/* i386/x86_64 are little-endian.  */

/* Some machines may need to use a different endianness for floating point
   values.  */
/* Conversion interfaces.  */
/* Macros to swap the order of bytes in integer values.
   Copyright (C) 1997-2014 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */
/* Copyright (C) 1991-2014 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */
/* bits/types.h -- definitions of __*_t types underlying *_t types.
   Copyright (C) 2002-2014 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

/*
 * Never include this file directly; use <sys/types.h> instead.
 */




/* Copyright (C) 1991-2014 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */
/* Determine the wordsize from the preprocessor defines.  */
/* Both x86-64 and x32 use the 64-bit system call interface.  */

/* Convenience types.  */
typedef unsigned char __u_char;
typedef unsigned short __u_short;
typedef unsigned int __u_int;
typedef unsigned long int __u_long;

/* Fixed-size types, underlying types depend on word size and compiler.  */
typedef signed char __int8_t;
typedef unsigned char __uint8_t;
typedef signed short __int16_t;
typedef unsigned short __uint16_t;
typedef signed int __int32_t;
typedef unsigned int __uint32_t;

typedef signed long int __int64_t;
typedef unsigned long int __uint64_t;





/* quad_t is also 64 bits.  */

typedef long int __quad_t;
typedef unsigned long int __u_quad_t;






/* The machine-dependent file <bits/typesizes.h> defines __*_T_TYPE
   macros for each of the OS types we define below.  The definitions
   of those macros must use the following macros for underlying types.
   We define __S<SIZE>_TYPE and __U<SIZE>_TYPE for the signed and unsigned
   variants of each of the following integer types on this machine.

	16		-- "natural" 16-bit type (always short)
	32		-- "natural" 32-bit type (always int)
	64		-- "natural" 64-bit type (long or long long)
	LONG32		-- 32-bit type, traditionally long
	QUAD		-- 64-bit type, always long long
	WORD		-- natural type of __WORDSIZE bits (int or long)
	LONGWORD	-- type of __WORDSIZE bits, traditionally long

   We distinguish WORD/LONGWORD, 32/LONG32, and 64/QUAD so that the
   conventional uses of `long' or `long long' type modifiers match the
   types we define, even when a less-adorned type would be the same size.
   This matters for (somewhat) portably writing printf/scanf formats for
   these types, where using the appropriate l or ll format modifiers can
   make the typedefs and the formats match up across all GNU platforms.  If
   we used `long' when it's 64 bits where `long long' is expected, then the
   compiler would warn about the formats not matching the argument types,
   and the programmer changing them to shut up the compiler would break the
   program's portability.

   Here we assume what is presently the case in all the GCC configurations
   we support: long long is always 64 bits, long is always word/address size,
   and int is always 32 bits.  */
/* No need to mark the typedef with __extension__.   */




/* bits/typesizes.h -- underlying types for *_t.  Linux/x86-64 version.
   Copyright (C) 2012-2014 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */
/* See <bits/types.h> for the meaning of these macros.  This file exists so
   that <bits/types.h> need not vary across different GNU platforms.  */

/* X32 kernel interface is 64-bit.  */
/* Tell the libc code that off_t and off64_t are actually the same type
   for all ABI purposes, even if possibly expressed as different base types
   for C type-checking purposes.  */


/* Same for ino_t and ino64_t.  */



/* Number of descriptors that can fit in an `fd_set'.  */


typedef unsigned long int __dev_t;
/* Type of device numbers.  */
typedef unsigned int __uid_t;
/* Type of user identifications.  */
typedef unsigned int __gid_t;
/* Type of group identifications.  */
typedef unsigned long int __ino_t;
/* Type of file serial numbers.  */
typedef unsigned long int __ino64_t;
/* Type of file serial numbers (LFS).*/
typedef unsigned int __mode_t;
/* Type of file attribute bitmasks.  */
typedef unsigned long int __nlink_t;
/* Type of file link counts.  */
typedef long int __off_t;
/* Type of file sizes and offsets.  */
typedef long int __off64_t;
/* Type of file sizes and offsets (LFS).  */
typedef int __pid_t;
/* Type of process identifications.  */
typedef struct {
   int __val[2];
} __fsid_t;
/* Type of file system IDs.  */
typedef long int __clock_t;
/* Type of CPU usage counts.  */
typedef unsigned long int __rlim_t;
/* Type for resource measurement.  */
typedef unsigned long int __rlim64_t;
/* Type for resource measurement (LFS).  */
typedef unsigned int __id_t;
/* General type for IDs.  */
typedef long int __time_t;
/* Seconds since the Epoch.  */
typedef unsigned int __useconds_t;
/* Count of microseconds.  */
typedef long int __suseconds_t;
/* Signed count of microseconds.  */

typedef int __daddr_t;
/* The type of a disk address.  */
typedef int __key_t;
/* Type of an IPC key.  */

/* Clock ID used in clock and timer functions.  */
typedef int __clockid_t;

/* Timer ID returned by `timer_create'.  */
typedef void *__timer_t;

/* Type to represent block size.  */
typedef long int __blksize_t;

/* Types from the Large File Support interface.  */

/* Type to count number of disk blocks.  */
typedef long int __blkcnt_t;
typedef long int __blkcnt64_t;

/* Type to count file system blocks.  */
typedef unsigned long int __fsblkcnt_t;
typedef unsigned long int __fsblkcnt64_t;

/* Type to count file system nodes.  */
typedef unsigned long int __fsfilcnt_t;
typedef unsigned long int __fsfilcnt64_t;

/* Type of miscellaneous file system fields.  */
typedef long int __fsword_t;

typedef long int __ssize_t;
/* Type of a byte count, or error.  */

/* Signed long type used in system calls.  */
typedef long int __syscall_slong_t;
/* Unsigned long type used in system calls.  */
typedef unsigned long int __syscall_ulong_t;

/* These few don't really vary by system, they always correspond
   to one of the other defined types.  */
typedef __off64_t __loff_t;
/* Type of file sizes and offsets (LFS).  */
typedef __quad_t *__qaddr_t;
typedef char *__caddr_t;

/* Duplicates info from stdint.h but this is used in unistd.h.  */
typedef long int __intptr_t;

/* Duplicate info from sys/socket.h.  */
typedef unsigned int __socklen_t;
/* Determine the wordsize from the preprocessor defines.  */
/* Both x86-64 and x32 use the 64-bit system call interface.  */

/* Swap bytes in 16 bit value.  */



/* Get __bswap_16.  */
/* Macros to swap the order of bytes in 16-bit integer values.
   Copyright (C) 2012-2014 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */
static unsigned short __bswap_16(unsigned short __bsx);


/* Swap bytes in 32 bit value.  */
static unsigned int __bswap_32(unsigned int __bsx);

static __uint64_t __bswap_64(__uint64_t __bsx);
union wait {
   int w_status;
   struct {int __w_termsig:7; int __w_coredump:1; int __w_retcode:8; int :16;} __wait_terminated;
   struct {int __w_stopval:8; int __w_stopsig:8; int :16;} __wait_stopped;
};



/* Lots of hair to allow traditional BSD use of `union wait'
   as well as POSIX.1 use of `int' for the status word.  */
/* This is the type of the argument to `wait'.  The funky union
   causes redeclarations with either `int *' or `union wait *' to be
   allowed without complaint.  __WAIT_STATUS_DEFN is the type used in
   the actual function definitions.  */
/* Define the macros <sys/wait.h> also would define this way.  */

/* Returned by `div'.  */


/* Quotient.  */
/* Remainder.  */
typedef struct {
   int quot;
   int rem;
} div_t;

/* Returned by `ldiv'.  */



/* Quotient.  */
/* Remainder.  */
typedef struct {
   long int quot;
   long int rem;
} ldiv_t;






/* Returned by `lldiv'.  */


/* Quotient.  */
/* Remainder.  */
typedef struct {
   long long int quot;
   long long int rem;
} lldiv_t;





/* The largest number rand will return (same as INT_MAX).  */



/* We define these the same for all machines.
   Changes from this to the outside world should be done in `_exit'.  */




/* Maximum length of a multibyte character in the current locale.  */

size_t __ctype_get_mb_cur_max(void);



/* Convert a string to a floating-point number.  */

extern double atof(const char *__nptr);
/* Convert a string to an integer.  */

extern int atoi(const char *__nptr);
/* Convert a string to a long integer.  */

extern long int atol(const char *__nptr);




/* Convert a string to a long long integer.  */

extern long long int atoll(const char *__nptr);




/* Convert a string to a floating-point number.  */


extern double strtod(const char *__nptr, char **__endptr);




/* Likewise for `float' and `long double' sizes of floating-point numbers.  */

extern float strtof(const char *__nptr, char **__endptr);



long double strtold(const char *__nptr, char **__endptr);




/* Convert a string to a long integer.  */


extern long int strtol(const char *__nptr, char **__endptr, int __base);
/* Convert a string to an unsigned long integer.  */


extern unsigned long int strtoul(const char *__nptr, char **__endptr, int __base);



/* Convert a string to a quadword integer.  */



long long int strtoq(const char *__nptr, char **__endptr, int __base);
/* Convert a string to an unsigned quadword integer.  */



unsigned long long int strtouq(const char *__nptr, char **__endptr, int __base);




/* Convert a string to a quadword integer.  */



extern long long int strtoll(const char *__nptr, char **__endptr, int __base);
/* Convert a string to an unsigned quadword integer.  */



extern unsigned long long int strtoull(const char *__nptr, char **__endptr, int __base);

/* Convert N to base 64 using the digits "./0-9A-Za-z", least-significant
   digit first.  Returns a pointer to static storage overwritten by the
   next call.  */
extern char *l64a(long int __n);

/* Read a number from a string S in base 64 as above.  */

extern long int a64l(const char *__s);




/* Copyright (C) 1991-2014 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

/*
 *	POSIX Standard: 2.6 Primitive System Data Types	<sys/types.h>
 */




/* Copyright (C) 1991-2014 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */



/* bits/types.h -- definitions of __*_t types underlying *_t types.
   Copyright (C) 2002-2014 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

/*
 * Never include this file directly; use <sys/types.h> instead.
 */



typedef __u_char u_char;
typedef __u_short u_short;
typedef __u_int u_int;
typedef __u_long u_long;
typedef __quad_t quad_t;
typedef __u_quad_t u_quad_t;
typedef __fsid_t fsid_t;




typedef __loff_t loff_t;



typedef __ino_t ino_t;
typedef __dev_t dev_t;




typedef __gid_t gid_t;




typedef __mode_t mode_t;




typedef __nlink_t nlink_t;




typedef __uid_t uid_t;





typedef __off_t off_t;
typedef __pid_t pid_t;





typedef __id_t id_t;




typedef __ssize_t ssize_t;





typedef __daddr_t daddr_t;
typedef __caddr_t caddr_t;





typedef __key_t key_t;
/* Copyright (C) 1991-2014 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

/*
 *	ISO C99 Standard: 7.23 Date and time	<time.h>
 */
/* bits/types.h -- definitions of __*_t types underlying *_t types.
   Copyright (C) 2002-2014 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

/*
 * Never include this file directly; use <sys/types.h> instead.
 */


/* Returned by `clock'.  */
typedef __clock_t clock_t;



/* bits/types.h -- definitions of __*_t types underlying *_t types.
   Copyright (C) 2002-2014 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

/*
 * Never include this file directly; use <sys/types.h> instead.
 */


/* Returned by `time'.  */
typedef __time_t time_t;



/* bits/types.h -- definitions of __*_t types underlying *_t types.
   Copyright (C) 2002-2014 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

/*
 * Never include this file directly; use <sys/types.h> instead.
 */

/* Clock ID used in clock and timer functions.  */
typedef __clockid_t clockid_t;
/* bits/types.h -- definitions of __*_t types underlying *_t types.
   Copyright (C) 2002-2014 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

/*
 * Never include this file directly; use <sys/types.h> instead.
 */

/* Timer ID returned by `timer_create'.  */
typedef __timer_t timer_t;
/* Copyright (C) 1989-2013 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

Under Section 7 of GPL version 3, you are granted additional
permissions described in the GCC Runtime Library Exception, version
3.1, as published by the Free Software Foundation.

You should have received a copy of the GNU General Public License and
a copy of the GCC Runtime Library Exception along with this program;
see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
<http://www.gnu.org/licenses/>.  */

/*
 * ISO C Standard:  7.17  Common definitions  <stddef.h>
 */






/* Any one of these symbols __need_* means that GNU libc
   wants us just to define one data type.  So don't define
   the symbols that indicate this file's entire job has been done.  */
/* This avoids lossage on SunOS but only if stdtypes.h comes first.
   There's no way to win with the other order!  Sun lossage.  */

/* On 4.3bsd-net2, make sure ansi.h is included, so we have
   one less case to deal with in the following.  */



/* On FreeBSD 5, machine/ansi.h does not exist anymore... */




/* In 4.3bsd-net2, machine/ansi.h defines these symbols, which are
   defined if the corresponding type is *not* defined.
   FreeBSD-2.1 defines _MACHINE_ANSI_H_ instead of _ANSI_H_.
   NetBSD defines _I386_ANSI_H_ and _X86_64_ANSI_H_ instead of _ANSI_H_ */
/* Sequent's header files use _PTRDIFF_T_ in some conflicting way.
   Just ignore it.  */




/* On VxWorks, <type/vxTypesBase.h> may have defined macros like
   _TYPE_size_t which will typedef size_t.  fixincludes patched the
   vxTypesBase.h so that this macro is only defined if _GCC_SIZE_T is
   not defined, and so that defining this macro defines _GCC_SIZE_T.
   If we find that the macros are still defined at this point, we must
   invoke them so that the type is defined as expected.  */
/* In case nobody has defined these types, but we aren't running under
   GCC 2.00, make sure that __PTRDIFF_TYPE__, __SIZE_TYPE__, and
   __WCHAR_TYPE__ have reasonable values.  This can happen if the
   parts of GCC is compiled by an older compiler, that actually
   include gstddef.h, such as collect2.  */

/* Signed type of difference of two pointers.  */

/* Define this type if we are doing the whole job,
   or if we want this type in particular.  */
/* Unsigned type of `sizeof' something.  */

/* Define this type if we are doing the whole job,
   or if we want this type in particular.  */
/* Wide character type.
   Locale-writers should change this as necessary to
   be big enough to hold unique values not between 0 and 127,
   and not (wchar_t) -1, for each defined multibyte character.  */

/* Define this type if we are doing the whole job,
   or if we want this type in particular.  */
/*  In 4.3bsd-net2, leave these undefined to indicate that size_t, etc.
    are already defined.  */
/*  BSD/OS 3.1 and FreeBSD [23].x require the MACHINE_ANSI_H check here.  */
/*  NetBSD 5 requires the I386_ANSI_H and X86_64_ANSI_H checks here.  */
/* A null pointer constant.  */


/* Old compatibility names for C types.  */
typedef unsigned long int ulong;
typedef unsigned short ushort;
typedef unsigned int uint;


/* These size-specific names are used by some of the inet code.  */



/* These types are defined by the ISO C99 header <inttypes.h>. */


typedef char int8_t;
typedef short int16_t;
typedef int int32_t;

typedef long int int64_t;





/* But these were defined by ISO C without the first `_'.  */
typedef unsigned char u_int8_t;
typedef unsigned short u_int16_t;
typedef unsigned int u_int32_t;

typedef unsigned long int u_int64_t;




typedef int register_t;
/* In BSD <sys/types.h> is expected to define BYTE_ORDER.  */
/* Copyright (C) 1992-2014 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

/* It also defines `fd_set' and the FD_* macros for `select'.  */
/* `fd_set' type and related macros, and `select'/`pselect' declarations.
   Copyright (C) 1996-2014 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

/*	POSIX 1003.1g: 6.2 Select from File Descriptor Sets <sys/select.h>  */




/* Copyright (C) 1991-2014 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

/* Get definition of needed basic types.  */
/* bits/types.h -- definitions of __*_t types underlying *_t types.
   Copyright (C) 2002-2014 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

/*
 * Never include this file directly; use <sys/types.h> instead.
 */

/* Get __FD_* definitions.  */
/* Copyright (C) 1997-2014 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */





/* Determine the wordsize from the preprocessor defines.  */
/* Both x86-64 and x32 use the 64-bit system call interface.  */
/* We don't use `memset' because this would require a prototype and
   the array isn't too big.  */

/* Get __sigset_t.  */
/* __sig_atomic_t, __sigset_t, and related definitions.  Linux version.
   Copyright (C) 1991-2014 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */




typedef int __sig_atomic_t;
typedef struct {
   unsigned long int __val[1024/(8*sizeof(unsigned long int))];
} __sigset_t;




/* We only want to define these functions if <signal.h> was actually
   included; otherwise we were included just to define the types.  Since we
   are namespace-clean, it wouldn't hurt to define extra macros.  But
   trouble can be caused by functions being defined (e.g., any global
   register vars declared later will cause compilation errors).  */



typedef __sigset_t sigset_t;


/* Get definition of timer specification structures.  */


/* Copyright (C) 1991-2014 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

/*
 *	ISO C99 Standard: 7.23 Date and time	<time.h>
 */
/* bits/types.h -- definitions of __*_t types underlying *_t types.
   Copyright (C) 2002-2014 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

/*
 * Never include this file directly; use <sys/types.h> instead.
 */

/* POSIX.1b structure for a time value.  This is like a `struct timeval' but
   has nanoseconds instead of microseconds.  */


/* Seconds.  */
/* Nanoseconds.  */
struct timespec {
   __time_t tv_sec;
   __syscall_slong_t tv_nsec;
};

/* System-dependent timing definitions.  Linux version.
   Copyright (C) 1996-2014 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

/*
 * Never include this file directly; use <time.h> instead.
 */




/* bits/types.h -- definitions of __*_t types underlying *_t types.
   Copyright (C) 2002-2014 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

/*
 * Never include this file directly; use <sys/types.h> instead.
 */

/* A time value that is accurate to the nearest
   microsecond but also has a range of years.  */


/* Seconds.  */
/* Microseconds.  */
struct timeval {
   __time_t tv_sec;
   __suseconds_t tv_usec;
};


typedef __suseconds_t suseconds_t;




/* The fd_set member is required to be an array of longs.  */
typedef long int __fd_mask;
typedef struct {
   __fd_mask __fds_bits[1024/(8*(int) sizeof(__fd_mask))];
} fd_set;

/* Maximum number of file descriptors in `fd_set'.  */



/* Sometimes the fd_set member is assumed to have this type.  */
typedef __fd_mask fd_mask;

/* Number of bits per word of `fd_set' (some code assumes this is 32).  */




/* Access macros for `fd_set'.  */








/* Check the first NFDS descriptors each in READFDS (if not NULL) for read
   readiness, in WRITEFDS (if not NULL) for write readiness, and in EXCEPTFDS
   (if not NULL) for exceptional conditions.  If TIMEOUT is not NULL, time out
   after waiting the interval specified therein.  Returns the number of ready
   descriptors, or -1 for errors.

   This function is a cancellation point and therefore not marked with
   __THROW.  */



extern int select(int __nfds, fd_set *__readfds, fd_set *__writefds, fd_set *__exceptfds, struct timeval *__timeout);


/* Same as above only that the TIMEOUT value is given with higher
   resolution and a sigmask which is been set temporarily.  This version
   should be used.

   This function is a cancellation point and therefore not marked with
   __THROW.  */




extern int pselect(int __nfds, fd_set *__readfds, fd_set *__writefds, fd_set *__exceptfds, const struct timespec *__timeout, const __sigset_t *__sigmask);



/* Define some inlines helping to catch common problems.  */






/* BSD defines these symbols, so we follow.  */
/* Definitions of macros to access `dev_t' values.
   Copyright (C) 1996-2014 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */




/* Copyright (C) 1991-2014 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */





unsigned int gnu_dev_major(unsigned long long int __dev);


unsigned int gnu_dev_minor(unsigned long long int __dev);



unsigned long long int gnu_dev_makedev(unsigned int __major, unsigned int __minor);


/* Access the functions with their traditional names.  */





typedef __blksize_t blksize_t;



/* Types from the Large File Support interface.  */


typedef __blkcnt_t blkcnt_t;
/* Type to count number of disk blocks.  */



typedef __fsblkcnt_t fsblkcnt_t;
/* Type to count file system blocks.  */



typedef __fsfilcnt_t fsfilcnt_t;
/* Type to count file system inodes.  */
/* Now add the thread types.  */

/* Copyright (C) 2002-2014 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */




/* Determine the wordsize from the preprocessor defines.  */
/* Both x86-64 and x32 use the 64-bit system call interface.  */
/* Thread identifiers.  The structure of the attribute type is not
   exposed on purpose.  */
typedef unsigned long int pthread_t;
union pthread_attr_t {
   char __size[56];
   long int __align;
};

typedef union pthread_attr_t pthread_attr_t;
typedef struct __pthread_internal_list {
   struct __pthread_internal_list *__prev;
   struct __pthread_internal_list *__next;
} __pthread_list_t;
typedef union {
   struct __pthread_mutex_s {int __lock; unsigned int __count; int __owner; unsigned int __nusers; int __kind; short __spins; short __elision; __pthread_list_t __list;} __data;
   char __size[40];
   long int __align;
} pthread_mutex_t;
typedef union {
   char __size[4];
   int __align;
} pthread_mutexattr_t;
typedef union {
   struct {int __lock; unsigned int __futex; unsigned long long int __total_seq; unsigned long long int __wakeup_seq; unsigned long long int __woken_seq; void *__mutex; unsigned int __nwaiters; unsigned int __broadcast_seq;} __data;
   char __size[48];
   long long int __align;
} pthread_cond_t;
typedef union {
   char __size[4];
   int __align;
} pthread_condattr_t;


/* Keys for thread-specific data */
typedef unsigned int pthread_key_t;


/* Once-only execution */
typedef int pthread_once_t;
typedef union {
   struct {int __lock; unsigned int __nr_readers; unsigned int __readers_wakeup; unsigned int __writer_wakeup; unsigned int __nr_readers_queued; unsigned int __nr_writers_queued; int __writer; int __shared; unsigned long int __pad1; unsigned long int __pad2; unsigned int __flags;} __data;
   char __size[56];
   long int __align;
} pthread_rwlock_t;
typedef union {
   char __size[8];
   long int __align;
} pthread_rwlockattr_t;




/* POSIX spinlock data type.  */
typedef volatile int pthread_spinlock_t;


/* POSIX barriers data type.  The structure of the type is
   deliberately not exposed.  */
typedef union {
   char __size[32];
   long int __align;
} pthread_barrier_t;
typedef union {
   char __size[4];
   int __align;
} pthread_barrierattr_t;




/* These are the functions that actually do things.  The `random', `srandom',
   `initstate' and `setstate' functions are those from BSD Unices.
   The `rand' and `srand' functions are required by the ANSI standard.
   We provide both interfaces to the same random number generator.  */
/* Return a random long integer between 0 and RAND_MAX inclusive.  */
extern long int random(void);

/* Seed the random number generator with the given number.  */
extern void srandom(unsigned int __seed);

/* Initialize the random number generator to use state buffer STATEBUF,
   of length STATELEN, and seed it with SEED.  Optimal lengths are 8, 16,
   32, 64, 128 and 256, the bigger the better; values less than 8 will
   cause an error and values greater than 256 will be rounded down.  */

extern char *initstate(unsigned int __seed, char *__statebuf, size_t __statelen);

/* Switch the random number generator to state buffer STATEBUF,
   which should have been previously initialized by `initstate'.  */
extern char *setstate(char *__statebuf);



/* Reentrant versions of the `random' family of functions.
   These functions all use the following data structure to contain
   state, rather than global state variables.  */



/* Front pointer.  */
/* Rear pointer.  */
/* Array of state values.  */
/* Type of random number generator.  */
/* Degree of random number generator.  */
/* Distance between front and rear.  */
/* Pointer behind state table.  */
struct random_data {
   int32_t *fptr;
   int32_t *rptr;
   int32_t *state;
   int rand_type;
   int rand_deg;
   int rand_sep;
   int32_t *end_ptr;
};


int random_r(struct random_data *__buf, int32_t *__result);


int srandom_r(unsigned int __seed, struct random_data *__buf);




int initstate_r(unsigned int __seed, char *__statebuf, size_t __statelen, struct random_data *__buf);



int setstate_r(char *__statebuf, struct random_data *__buf);





/* Return a random integer between 0 and RAND_MAX inclusive.  */
extern int rand(void);
/* Seed the random number generator with the given number.  */
extern void srand(unsigned int __seed);



/* Reentrant interface according to POSIX.1.  */
int rand_r(unsigned int *__seed);




/* System V style 48-bit random number generator functions.  */

/* Return non-negative, double-precision floating-point value in [0.0,1.0).  */
extern double drand48(void);
extern double erand48(unsigned short __xsubi[3]);

/* Return non-negative, long integer in [0,2^31).  */
extern long int lrand48(void);

extern long int nrand48(unsigned short __xsubi[3]);

/* Return signed, long integers in [-2^31,2^31).  */
extern long int mrand48(void);

extern long int jrand48(unsigned short __xsubi[3]);

/* Seed random number generator.  */
extern void srand48(long int __seedval);

extern unsigned short *seed48(unsigned short __seed16v[3]);
extern void lcong48(unsigned short __param[7]);


/* Data structure for communication with thread safe versions.  This
   type is to be regarded as opaque.  It's only exported because users
   have to allocate objects of this type.  */


/* Current state.  */
/* Old state.  */
/* Additive const. in congruential formula.  */
/* Flag for initializing.  */
/* Factor in congruential
						   formula.  */
struct drand48_data {
   unsigned short __x[3];
   unsigned short __old_x[3];
   unsigned short __c;
   unsigned short __init;
   unsigned long long int __a;
};

/* Return non-negative, double-precision floating-point value in [0.0,1.0).  */

int drand48_r(struct drand48_data *__buffer, double *__result);


int erand48_r(unsigned short __xsubi[3], struct drand48_data *__buffer, double *__result);

/* Return non-negative, long integer in [0,2^31).  */


int lrand48_r(struct drand48_data *__buffer, long int *__result);



int nrand48_r(unsigned short __xsubi[3], struct drand48_data *__buffer, long int *__result);

/* Return signed, long integers in [-2^31,2^31).  */


int mrand48_r(struct drand48_data *__buffer, long int *__result);



int jrand48_r(unsigned short __xsubi[3], struct drand48_data *__buffer, long int *__result);

/* Seed random number generator.  */

int srand48_r(long int __seedval, struct drand48_data *__buffer);


int seed48_r(unsigned short __seed16v[3], struct drand48_data *__buffer);



int lcong48_r(unsigned short __param[7], struct drand48_data *__buffer);








/* Allocate SIZE bytes of memory.  */
extern void *malloc(size_t __size);
/* Allocate NMEMB elements of SIZE bytes each, all initialized to 0.  */

extern void *calloc(size_t __nmemb, size_t __size);





/* Re-allocate the previously allocated block
   in PTR, making the new block SIZE bytes long.  */
/* __attribute_malloc__ is not used, because if realloc returns
   the same pointer that was passed to it, aliasing needs to be allowed
   between objects pointed by the old and new pointers.  */

extern void *realloc(void *__ptr, size_t __size);
/* Free a block allocated by `malloc', `realloc' or `calloc'.  */
extern void free(void *__ptr);



/* Free a block.  An alias for `free'.	(Sun Unices).  */
void cfree(void *__ptr);



/* Copyright (C) 1992-2014 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */




/* Copyright (C) 1991-2014 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */


/* Copyright (C) 1989-2013 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

Under Section 7 of GPL version 3, you are granted additional
permissions described in the GCC Runtime Library Exception, version
3.1, as published by the Free Software Foundation.

You should have received a copy of the GNU General Public License and
a copy of the GCC Runtime Library Exception along with this program;
see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
<http://www.gnu.org/licenses/>.  */

/*
 * ISO C Standard:  7.17  Common definitions  <stddef.h>
 */






/* Any one of these symbols __need_* means that GNU libc
   wants us just to define one data type.  So don't define
   the symbols that indicate this file's entire job has been done.  */
/* This avoids lossage on SunOS but only if stdtypes.h comes first.
   There's no way to win with the other order!  Sun lossage.  */

/* On 4.3bsd-net2, make sure ansi.h is included, so we have
   one less case to deal with in the following.  */



/* On FreeBSD 5, machine/ansi.h does not exist anymore... */




/* In 4.3bsd-net2, machine/ansi.h defines these symbols, which are
   defined if the corresponding type is *not* defined.
   FreeBSD-2.1 defines _MACHINE_ANSI_H_ instead of _ANSI_H_.
   NetBSD defines _I386_ANSI_H_ and _X86_64_ANSI_H_ instead of _ANSI_H_ */
/* Sequent's header files use _PTRDIFF_T_ in some conflicting way.
   Just ignore it.  */




/* On VxWorks, <type/vxTypesBase.h> may have defined macros like
   _TYPE_size_t which will typedef size_t.  fixincludes patched the
   vxTypesBase.h so that this macro is only defined if _GCC_SIZE_T is
   not defined, and so that defining this macro defines _GCC_SIZE_T.
   If we find that the macros are still defined at this point, we must
   invoke them so that the type is defined as expected.  */
/* In case nobody has defined these types, but we aren't running under
   GCC 2.00, make sure that __PTRDIFF_TYPE__, __SIZE_TYPE__, and
   __WCHAR_TYPE__ have reasonable values.  This can happen if the
   parts of GCC is compiled by an older compiler, that actually
   include gstddef.h, such as collect2.  */

/* Signed type of difference of two pointers.  */

/* Define this type if we are doing the whole job,
   or if we want this type in particular.  */
/* Unsigned type of `sizeof' something.  */

/* Define this type if we are doing the whole job,
   or if we want this type in particular.  */
/* Wide character type.
   Locale-writers should change this as necessary to
   be big enough to hold unique values not between 0 and 127,
   and not (wchar_t) -1, for each defined multibyte character.  */

/* Define this type if we are doing the whole job,
   or if we want this type in particular.  */
/*  In 4.3bsd-net2, leave these undefined to indicate that size_t, etc.
    are already defined.  */
/*  BSD/OS 3.1 and FreeBSD [23].x require the MACHINE_ANSI_H check here.  */
/*  NetBSD 5 requires the I386_ANSI_H and X86_64_ANSI_H checks here.  */
/* A null pointer constant.  */



/* Remove any previous definitions.  */


/* Allocate a block that will be freed when the calling function exits.  */
extern void *alloca(size_t __size);










/* Allocate SIZE bytes on a page boundary.  The storage cannot be freed.  */
extern void *valloc(size_t __size);



/* Allocate memory of SIZE bytes with an alignment of ALIGNMENT.  */

extern int posix_memalign(void **__memptr, size_t __alignment, size_t __size);

/* Abort execution and generate a core-dump.  */
extern void abort(void);


/* Register a function to be called when `exit' is called.  */
extern int atexit(void (*__func)(void));



/* Register a function to be called with the status
   given to `exit' and the given argument.  */

int on_exit(void (*__func)(int __status, void *__arg), void *__arg);



/* Call all functions registered with `atexit' and `on_exit',
   in the reverse of the order in which they were registered,
   perform stdio cleanup, and terminate program execution with STATUS.  */
extern void exit(int __status);











/* Terminate the program with STATUS without calling any of the
   functions registered with `atexit' or `on_exit'.  */
void _Exit(int __status);





/* Return the value of envariable NAME, or NULL if it doesn't exist.  */
extern char *getenv(const char *__name);

/* The SVID says this is in <stdio.h>, but this seems a better place.	*/
/* Put STRING, which is of the form "NAME=VALUE", in the environment.
   If there is no `=', remove NAME from the environment.  */
extern int putenv(char *__string);



/* Set NAME to VALUE in the environment.
   If REPLACE is nonzero, overwrite an existing value.  */

int setenv(const char *__name, const char *__value, int __replace);

/* Remove the variable NAME from the environment.  */
int unsetenv(const char *__name);



/* The `clearenv' was planned to be added to POSIX.1 but probably
   never made it.  Nevertheless the POSIX.9 standard (POSIX bindings
   for Fortran 77) requires this function.  */
int clearenv(void);





/* Generate a unique temporary file name from TEMPLATE.
   The last six characters of TEMPLATE must be "XXXXXX";
   they are replaced with a string that makes the file name unique.
   Always returns TEMPLATE, it's either a temporary file name or a null
   string if it cannot get a unique file name.  */
extern char *mktemp(char *__template);




/* Generate a unique temporary file name from TEMPLATE.
   The last six characters of TEMPLATE must be "XXXXXX";
   they are replaced with a string that makes the filename unique.
   Returns a file descriptor open on the file for reading and writing,
   or -1 if it cannot create a uniquely-named file.

   This function is a possible cancellation point and therefore not
   marked with __THROW.  */

extern int mkstemp(char *__template);
/* Similar to mkstemp, but the template can have a suffix after the
   XXXXXX.  The length of the suffix is specified in the second
   parameter.

   This function is a possible cancellation point and therefore not
   marked with __THROW.  */

int mkstemps(char *__template, int __suffixlen);
/* Create a unique temporary directory from TEMPLATE.
   The last six characters of TEMPLATE must be "XXXXXX";
   they are replaced with a string that makes the directory name unique.
   Returns TEMPLATE, or a null pointer if it cannot get a unique name.
   The directory is created mode 700.  */
char *mkdtemp(char *__template);

/* Execute the given line as a shell command.

   This function is a cancellation point and therefore not marked with
   __THROW.  */
extern int system(const char *__command);

/* Return the canonical absolute name of file NAME.  If RESOLVED is
   null, the result is malloc'd; otherwise, if the canonical name is
   PATH_MAX chars or more, returns null with `errno' set to
   ENAMETOOLONG; if the name fits in fewer than PATH_MAX chars,
   returns the name in RESOLVED.  */

extern char *realpath(const char *__name, char *__resolved);



/* Shorthand for type of comparison functions.  */


typedef int (*__compar_fn_t)(void *, void *);

/* Do a binary search for KEY in BASE, which consists of NMEMB elements
   of SIZE bytes each, using COMPAR to perform the comparisons.  */


extern void *bsearch(const void *__key, const void *__base, size_t __nmemb, size_t __size, __compar_fn_t __compar);





/* Sort NMEMB elements of BASE, of SIZE bytes each,
   using COMPAR to perform the comparisons.  */

extern void qsort(void *__base, size_t __nmemb, size_t __size, __compar_fn_t __compar);







/* Return the absolute value of X.  */
extern int abs(int __x);
extern long int labs(long int __x);




extern long long int llabs(long long int __x);




/* Return the `div_t', `ldiv_t' or `lldiv_t' representation
   of the value of NUMER over DENOM. */
/* GCC may have built-ins for these someday.  */

extern div_t div(int __numer, int __denom);

extern ldiv_t ldiv(long int __numer, long int __denom);






extern lldiv_t lldiv(long long int __numer, long long int __denom);






/* Convert floating point numbers to strings.  The returned values are
   valid only until another call to the same function.  */

/* Convert VALUE to a string with NDIGIT digits and return a pointer to
   this.  Set *DECPT with the position of the decimal character and *SIGN
   with the sign of the number.  */

extern char *ecvt(double __value, int __ndigit, int *__decpt, int *__sign);

/* Convert VALUE to a string rounded to NDIGIT decimal digits.  Set *DECPT
   with the position of the decimal character and *SIGN with the sign of
   the number.  */

extern char *fcvt(double __value, int __ndigit, int *__decpt, int *__sign);

/* If possible convert VALUE to a string with NDIGIT significant digits.
   Otherwise use exponential representation.  The resulting string will
   be written to BUF.  */

extern char *gcvt(double __value, int __ndigit, char *__buf);



/* Long double versions of above functions.  */


extern char *qecvt(long double __value, int __ndigit, int *__decpt, int *__sign);


extern char *qfcvt(long double __value, int __ndigit, int *__decpt, int *__sign);

extern char *qgcvt(long double __value, int __ndigit, char *__buf);


/* Reentrant version of the functions above which provide their own
   buffers.  */


int ecvt_r(double __value, int __ndigit, int *__decpt, int *__sign, char *__buf, size_t __len);


int fcvt_r(double __value, int __ndigit, int *__decpt, int *__sign, char *__buf, size_t __len);




int qecvt_r(long double __value, int __ndigit, int *__decpt, int *__sign, char *__buf, size_t __len);



int qfcvt_r(long double __value, int __ndigit, int *__decpt, int *__sign, char *__buf, size_t __len);




/* Return the length of the multibyte character
   in S, which is no longer than N.  */
extern int mblen(const char *__s, size_t __n);
/* Return the length of the given multibyte character,
   putting its `wchar_t' representation in *PWC.  */

extern int mbtowc(wchar_t *__pwc, const char *__s, size_t __n);
/* Put the multibyte character represented
   by WCHAR in S, returning its length.  */
extern int wctomb(char *__s, wchar_t __wchar);


/* Convert a multibyte string to a wide char string.  */

extern size_t mbstowcs(wchar_t *__pwcs, const char *__s, size_t __n);
/* Convert a wide char string to multibyte string.  */


extern size_t wcstombs(char *__s, const wchar_t *__pwcs, size_t __n);




/* Determine whether the string value of RESPONSE matches the affirmation
   or negative response expression as specified by the LC_MESSAGES category
   in the program's current locale.  Returns 1 if affirmative, 0 if
   negative, and -1 if not matching.  */
int rpmatch(const char *__response);




/* Parse comma separated suboption from *OPTIONP and match against
   strings in TOKENS.  If found return index and set *VALUEP to
   optional value introduced by an equal sign.  If the suboption is
   not part of TOKENS return in *VALUEP beginning of unknown
   suboption.  On exit *OPTIONP is set to the beginning of the next
   token or at the terminating NUL character.  */



extern int getsubopt(char **__optionp, char ** const __tokens, char **__valuep);
/* X/Open pseudo terminal handling.  */
/* Put the 1 minute, 5 minute and 15 minute load averages into the first
   NELEM elements of LOADAVG.  Return the number written (never more than
   three, but may be less than NELEM), or -1 if an error occurred.  */

int getloadavg(double __loadavg[], int __nelem);


/* Floating-point inline functions for stdlib.h.
   Copyright (C) 2012-2014 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

/* Define some macros helping to catch buffer overflows.  */


/** @defgroup p4a_accel_stubs Equivalent stubs of Par4All runtime to have
    PIPS analyzis happy

    The main idea is that PIPS is interprocedural and needs to know
    everything about the program to be analysed. Since the generated
    function calls are in the PIPS output, they need to be analyzed and so
    need to be in the PIPS input.

    Since CUDA runtime is not C or Fortran, it cannot be analyzed by
    PIPS. So, in the entry, we give to PIPS equivalent functions to eat
    (these stubs functions). Of course it is useless to have a functional
    emulation of each accelerator element processors :-), so just have a
    functional equivalent with the same behaviour on the memory (store
    effects in the PIPS jargon).

  @{
 */


/** @defgroup p4a_accel_C_stubs Equivalent stubs in C of Par4All runtime to have
    PIPS analyzes happy

    TODO: Well, all these stubs should be sourced from the OpenMP
    implementation from p4a_accel.c instead of being pasted here

    @{
*/


/** Stub for P4A_accel_kernel_wrapper #define in p4a_accel, which has to be
 * parsed as a typedef in pips
 */
typedef void P4A_accel_kernel_wrapper;
typedef void P4A_accel_kernel;


/** Stub for copying a scalar from the hardware accelerator memory to
    the host.

    @param[in] element_size is the size of one element of the array in
    byte

    @param[out] host_address point to the element on the host to write
    into

    @param[in] accel_address refer to the compact memory area to read
    data. In the general case, accel_address may be seen as a unique idea (FIFO)
    and not some address in some memory space.
*/
void P4A_copy_from_accel(size_t element_size, void *host_address, const void *accel_address);



/** Stub for copying a scalar from the host to a memory zone in the
    hardware accelerator.
*/
void P4A_copy_to_accel(size_t element_size, const void *host_address, void *accel_address);


/** Stub for copying memory from the host to the hardware accelerator.

    Since it is a stub so that PIPS can understand it, use simple
    implementation with standard memory copy operations

    Do not change the place of the pointers in the API. The host address
    is always in the first position...

    This function could be quite simpler but is designed by symmetry with
    other functions.

    @param[in] element_size is the size of one element of the array in
    byte

    @param[in] d1_size is the number of elements in the array. It is not
    used but here for symmetry with functions of higher dimensionality

    @param[in] d1_size is the number of elements in the array. It is not
    used but here for symmetry with functions of higher dimensionality

    @param[in] d1_block_size is the number of element to transfer

    @param[in] d1_offset is element order to start the transfer to

    @param[out] host_address point to the array on the host to write into

    @param[in] accel_address refer to the compact memory area to read
    data. In the general case, accel_address may be seen as a unique idea (FIFO)
    and not some address in some memory space.
*/
void P4A_copy_from_accel_1d(size_t element_size, size_t d1_size, size_t d1_block_size, size_t d1_offset, void *host_address, const void *accel_address);



/** Stub for copying a 1D memory zone from the hardware accelerator to the
    host.

    Do not change the place of the pointers in the API. The host address
    is always in the first position...

    This function could be quite simpler but is designed by symmetry with
    other functions.

    @param[in] element_size is the size of one element of the array in
    byte

    @param[in] d1_size is the number of elements in the array. It is not
    used but here for symmetry with functions of higher dimensionality

    @param[in] d1_size is the number of elements in the array. It is not
    used but here for symmetry with functions of higher dimensionality

    @param[in] d1_block_size is the number of element to transfer

    @param[in] d1_offset is element order to start the transfer from

    @param[in] host_address point to the array on the host to read

    @param[out] accel_address refer to the compact memory area to write
    data. In the general case, accel_address may be seen as a unique idea
    (FIFO) and not some address in some memory space.
*/
void P4A_copy_to_accel_1d(size_t element_size, size_t d1_size, size_t d1_block_size, size_t d1_offset, const void *host_address, void *accel_address);



/** Stub for copying a 2D memory zone from the host to a compact memory
    zone in the hardware accelerator.
*/
void P4A_copy_to_accel_2d(size_t element_size, size_t d1_size, size_t d2_size, size_t d1_block_size, size_t d2_block_size, size_t d1_offset, size_t d2_offset, const void *host_address, void *accel_address);



/** Stub for copying memory from the hardware accelerator to a 2D array in
    the host.
*/
void P4A_copy_from_accel_2d(size_t element_size, size_t d1_size, size_t d2_size, size_t d1_block_size, size_t d2_block_size, size_t d1_offset, size_t d2_offset, void *host_address, const void *accel_address);



/** Stub for copying memory from the hardware accelerator to a 3D array in
    the host.
*/
void P4A_copy_from_accel_3d(size_t element_size, size_t d1_size, size_t d2_size, size_t d3_size, size_t d1_block_size, size_t d2_block_size, size_t d3_block_size, size_t d1_offset, size_t d2_offset, size_t d3_offset, void *host_address, const void *accel_address);



/** Stub for copying a 3D memory zone from the host to a compact memory
    zone in the hardware accelerator.
*/
void P4A_copy_to_accel_3d(size_t element_size, size_t d1_size, size_t d2_size, size_t d3_size, size_t d1_block_size, size_t d2_block_size, size_t d3_block_size, size_t d1_offset, size_t d2_offset, size_t d3_offset, const void *host_address, void *accel_address);



/** Stub for copying memory from the hardware accelerator to a 4D array in
    the host.
*/
void P4A_copy_from_accel_4d(size_t element_size, size_t d1_size, size_t d2_size, size_t d3_size, size_t d4_size, size_t d1_block_size, size_t d2_block_size, size_t d3_block_size, size_t d4_block_size, size_t d1_offset, size_t d2_offset, size_t d3_offset, size_t d4_offset, void *host_address, const void *accel_address);



/** Stub for copying a 4D memory zone from the host to a compact memory
    zone in the hardware accelerator.
*/
void P4A_copy_to_accel_4d(size_t element_size, size_t d1_size, size_t d2_size, size_t d3_size, size_t d4_size, size_t d1_block_size, size_t d2_block_size, size_t d3_block_size, size_t d4_block_size, size_t d1_offset, size_t d2_offset, size_t d3_offset, size_t d4_offset, const void *host_address, void *accel_address);


/** Stub for allocating memory on the hardware accelerator.

    @param[out] address is the address of a variable that is updated by
    this macro to contains the address of the allocated memory block

    @param[in] size is the size to allocate in bytes
*/
void P4A_accel_malloc(void **address, size_t size);



/** Stub for freeing memory on the hardware accelerator.

    @param[in] address is the address of a previously allocated memory
    zone on the hardware accelerator
*/
void P4A_accel_free(void *address);



/** Stubs for P4A runtime */
void P4A_runtime_copy_to_accel(void *host_ptr, size_t size);

void P4A_runtime_copy_from_accel(void *host_ptr, size_t size);

void *P4A_runtime_host_ptr_to_accel_ptr(void *host_ptr, size_t size);
static unsigned short __bswap_16(unsigned short __bsx)
{
   return (unsigned short) (__bsx>>8&0xff|(__bsx&0xff)<<8);
}
static unsigned int __bswap_32(unsigned int __bsx)
{
   return (__bsx&0xff000000)>>24|(__bsx&0x00ff0000)>>8|(__bsx&0x0000ff00)<<8|(__bsx&0x000000ff)<<24;
}
static __uint64_t __bswap_64(__uint64_t __bsx)
{
   return (__bsx&0xff00000000000000ull)>>56|(__bsx&0x00ff000000000000ull)>>40|(__bsx&0x0000ff0000000000ull)>>24|(__bsx&0x000000ff00000000ull)>>8|(__bsx&0x00000000ff000000ull)<<8|(__bsx&0x0000000000ff0000ull)<<24|(__bsx&0x000000000000ff00ull)<<40|(__bsx&0x00000000000000ffull)<<56;
}
void P4A_copy_from_accel(size_t element_size, void *host_address, const void *accel_address)
{
   size_t i;
   char *cdest = host_address;
   const char *csrc = accel_address;
   for(i = 0; i < element_size; i += 1)
      cdest[i] = csrc[i];
}
void P4A_copy_to_accel(size_t element_size, const void *host_address, void *accel_address)
{
   size_t i;
   char *cdest = accel_address;
   const char *csrc = host_address;
   for(i = 0; i < element_size; i += 1)
      cdest[i] = csrc[i];
}
void P4A_copy_from_accel_1d(size_t element_size, size_t d1_size, size_t d1_block_size, size_t d1_offset, void *host_address, const void *accel_address)
{
   size_t i;
   char *cdest = d1_offset*element_size+(char *) host_address;
   const char *csrc = accel_address;
   for(i = 0; i < d1_block_size*element_size-1+1; i += 1)
      cdest[i] = csrc[i];
}
void P4A_copy_to_accel_1d(size_t element_size, size_t d1_size, size_t d1_block_size, size_t d1_offset, const void *host_address, void *accel_address)
{
   size_t i;
   char *cdest = accel_address;
   const char *csrc = d1_offset*element_size+(char *) host_address;
   for(i = 0; i < d1_block_size*element_size-1+1; i += 1)
      cdest[i] = csrc[i];
}
void P4A_copy_to_accel_2d(size_t element_size, size_t d1_size, size_t d2_size, size_t d1_block_size, size_t d2_block_size, size_t d1_offset, size_t d2_offset, const void *host_address, void *accel_address)
{
   size_t i, j;
   char *cdest = (char *) accel_address;
   const char *csrc = d2_offset*element_size+(char *) host_address;
   for(i = 0; i < d1_block_size; i += 1)
      for(j = 0; j < d2_block_size*element_size-1+1; j += 1)
         cdest[i*element_size*d2_block_size+j] = csrc[(i+d1_offset)*element_size*d2_size+j];
}
void P4A_copy_from_accel_2d(size_t element_size, size_t d1_size, size_t d2_size, size_t d1_block_size, size_t d2_block_size, size_t d1_offset, size_t d2_offset, void *host_address, const void *accel_address)
{
   size_t i, j;
   char *cdest = d2_offset*element_size+(char *) host_address;
   char *csrc = (char *) accel_address;
   for(i = 0; i < d1_block_size; i += 1)
      for(j = 0; j < d2_block_size*element_size-1+1; j += 1)
         cdest[(i+d1_offset)*element_size*d2_size+j] = csrc[i*element_size*d2_block_size+j];
}
void P4A_copy_from_accel_3d(size_t element_size, size_t d1_size, size_t d2_size, size_t d3_size, size_t d1_block_size, size_t d2_block_size, size_t d3_block_size, size_t d1_offset, size_t d2_offset, size_t d3_offset, void *host_address, const void *accel_address)
{
   size_t i, j, k;
   char *cdest = d3_offset*element_size+(char *) host_address;
   const char *csrc = (char *) accel_address;
   for(i = 0; i < d1_block_size; i += 1)
      for(j = 0; j < d2_block_size; j += 1)
         for(k = 0; k < d3_block_size*element_size-1+1; k += 1)
            cdest[((i+d1_offset)*d2_block_size+j+d2_offset)*element_size*d3_size+k] = csrc[(i*d2_block_size+j)*d3_block_size*element_size+k];
}
void P4A_copy_to_accel_3d(size_t element_size, size_t d1_size, size_t d2_size, size_t d3_size, size_t d1_block_size, size_t d2_block_size, size_t d3_block_size, size_t d1_offset, size_t d2_offset, size_t d3_offset, const void *host_address, void *accel_address)
{
   size_t i, j, k;
   char *cdest = (char *) accel_address;
   const char *csrc = d3_offset*element_size+(char *) host_address;
   for(i = 0; i < d1_block_size; i += 1)
      for(j = 0; j < d2_block_size; j += 1)
         for(k = 0; k < d3_block_size*element_size-1+1; k += 1)
            cdest[(i*d2_block_size+j)*d3_block_size*element_size+k] = csrc[((i+d1_offset)*d2_block_size+j+d2_offset)*element_size*d3_size+k];
}
void P4A_copy_from_accel_4d(size_t element_size, size_t d1_size, size_t d2_size, size_t d3_size, size_t d4_size, size_t d1_block_size, size_t d2_block_size, size_t d3_block_size, size_t d4_block_size, size_t d1_offset, size_t d2_offset, size_t d3_offset, size_t d4_offset, void *host_address, const void *accel_address)
{
   size_t i, j, k, l;
   char *cdest = (char *) host_address;
   const char *csrc = (char *) accel_address;
   for(i = 0; i < d1_block_size; i += 1)
      for(j = 0; j < d2_block_size; j += 1)
         for(k = 0; k < d3_block_size; k += 1)
            for(l = 0; l < d4_block_size; l += 1) {
               int h_index = (i+d1_offset)*d2_size*d3_size*d4_size+(j+d2_offset)*d3_size*d4_size+(k+d3_offset)*d4_size+(l+d4_offset);
               
               
               
               int a_index = i*d2_block_size*d3_block_size*d4_block_size+j*d3_block_size*d4_block_size+k*d4_block_size+l;
               cdest[h_index] = csrc[a_index];
            }
}
void P4A_copy_to_accel_4d(size_t element_size, size_t d1_size, size_t d2_size, size_t d3_size, size_t d4_size, size_t d1_block_size, size_t d2_block_size, size_t d3_block_size, size_t d4_block_size, size_t d1_offset, size_t d2_offset, size_t d3_offset, size_t d4_offset, const void *host_address, void *accel_address)
{
   size_t i, j, k, l;
   char *cdest = (char *) accel_address;
   const char *csrc = (char *) host_address;
   for(i = 0; i < d1_block_size; i += 1)
      for(j = 0; j < d2_block_size; j += 1)
         for(k = 0; k < d3_block_size; k += 1)
            for(l = 0; l < d4_block_size; l += 1) {
               int h_index = (i+d1_offset)*d2_size*d3_size*d4_size+(j+d2_offset)*d3_size*d4_size+(k+d3_offset)*d4_size+(l+d4_offset);
               
               
               
               int a_index = i*d2_block_size*d3_block_size*d4_block_size+j*d3_block_size*d4_block_size+k*d4_block_size+l;
               cdest[a_index] = csrc[h_index];
            }
}
void P4A_accel_malloc(void **address, size_t size)
{
   *address = malloc(size);
}
void P4A_accel_free(void *address)
{
   free(address);
}
void P4A_runtime_copy_to_accel(void *host_ptr, size_t size)
{
   P4A_copy_to_accel(size, host_ptr, (void *) 0);
}
void P4A_runtime_copy_from_accel(void *host_ptr, size_t size)
{
   P4A_copy_from_accel(size, host_ptr, (void *) 0);
}
void * P4A_runtime_host_ptr_to_accel_ptr(void *host_ptr, size_t size)
{
   void *accel_ptr;
   malloc(size);
   P4A_accel_malloc(&accel_ptr, size);
   return accel_ptr;
}
