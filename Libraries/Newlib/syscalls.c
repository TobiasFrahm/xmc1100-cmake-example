/**
 * @file syscalls.c
 * @brief    Newlib stubs
 * @version  V1.8
 * @date 2019-09-21
 *
 * @cond
 *****************************************************************************
 * Copyright (c) 2015-2020, Infineon Technologies AG
 * All rights reserved.
 *
 * Boost Software License - Version 1.0 - August 17th, 2003
 *
 * Permission is hereby granted, free of charge, to any person or organization
 * obtaining a copy of the software and accompanying documentation covered by
 * this license (the "Software") to use, reproduce, display, distribute,
 * execute, and transmit the Software, and to prepare derivative works of the
 * Software, and to permit third-parties to whom the Software is furnished to
 * do so, all subject to the following:
 *
 * The copyright notices in the Software and this entire statement, including
 * the above license grant, this restriction and the following disclaimer,
 * must be included in all copies of the Software, in whole or in part, and
 * all derivative works of the Software, unless such copies or derivative
 * works are solely in the form of machine-executable object code generated by
 * a source language processor.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * To improve the quality of the software, users are encouraged to share
 * modifications, enhancements or bug fixes with Infineon Technologies AG
 * at XMCSupport@infineon.com.
 *****************************************************************************
 *
 * *************************** Change history ********************************
 * V0.1                  : Initial version
 * V0.2                  : Label updates
 * V1.0                  : Made _sbrk device agnostic
 * V1.1                  : C++ support
 * V1.2                  : Restored compatibility with old project files
 * V1.3    Jan 2014, PKB : Encapsulating everything in this file for use only with GCC
 * V1.4 11 Dec 2015, JFT : Fix heap overflow
 * V1.5 09 Mar 2016, JFT : Add dso_handle to support destructors call at exit 
 * V1.6 20 Apr 2017, JFT : Foward declaration of __sbrk to fix link time optimization (-flto) compilation errors
 * V1.7 21 Sep 2019, JFT : Fixed C++ compilation issues
 * V1.8 11 Nov 2020, JFT : Fix compilation with ARMCC
 * @endcond
 */

/*
 * This file contains stubs for standard C library functionality that must
 * typically be provided for the underlying platform.
 *
 */
#if defined ( __GNUC__ ) && !defined (__ARMCC_VERSION)

#include <stdint.h>
#include <errno.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward prototypes.  */
caddr_t _sbrk(int nbytes) __attribute__((externally_visible));
void _init(void) __attribute__((externally_visible));

#ifdef __cplusplus
}
#endif

/* c++ destructor dynamic shared object needed if -fuse-cxa-atexit is used*/
void *__dso_handle __attribute__ ((weak));

// defined in linker script
extern caddr_t Heap_Bank1_Start;
extern caddr_t Heap_Bank1_End;
caddr_t _sbrk(int nbytes)
{
  static caddr_t heap_ptr = NULL;
  caddr_t base;

  if (heap_ptr == NULL) {
    heap_ptr = (caddr_t)&Heap_Bank1_Start;
  }

  base = heap_ptr;

  /* heap word alignment */
  nbytes = (nbytes + 3) & ~0x3U;
  if ((caddr_t)&Heap_Bank1_End > (heap_ptr + nbytes))
  {
    heap_ptr += nbytes;
    return (base);
  }
  else
  {
    /* Heap overflow */
    errno = ENOMEM;
    return ((caddr_t)-1);
  }
}

/* Init */
void _init(void)
{}

#endif /* __GNUC__ */
