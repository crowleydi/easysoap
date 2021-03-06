/* 
 * EasySoap++ - A C++ library for SOAP (Simple Object Access Protocol)
 * Copyright (C) 2001 David Crowley; SciTegic, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * $Id$
 */

/*
 * This file was taken from STL port 4.0 and hacked up
 * a bit.
 */

/*
 * Copyright (c) 1997
 * Moscow Center for SPARC Technology
 *
 * Copyright (c) 1999 
 * Boris Fomitchev
 *
 * This material is provided "as is", with absolutely no warranty expressed
 * or implied. Any use is at your own risk.
 *
 * Permission to use or copy this software for any purpose is hereby granted 
 * without fee, provided the above notices are retained on all copies.
 * Permission to modify the code and to distribute modified code is granted,
 * provided the above notices are retained, and a notice that the code was
 * modified is included with the above copyright notice.
 *
 */

/*
 * Purpose of this file :
 *
 * To hold COMPILER-SPECIFIC portion of Easysoap++ settings.
 * In general, user should not edit this file unless 
 * using the compiler not recognized below.
 *
 * If your compiler is not being recognized yet, 
 * good luck.
 */

#ifndef __ES_COMP_H
# define __ES_COMP_H

/* distinguish real MSC from Metrowerks and Intel */
#if defined(_MSC_VER) && !defined(__MWERKS__) && !defined (__ICL) && !defined (__COMO__)
/* Microsoft Visual C++ 4.0, 4.1, 4.2, 5.0 */
#  include "es_msvc.h"
# elif defined (__MWERKS__)
/* Metrowerks CodeWarrior */
#  include "es_mwerks.h"
# elif defined(__BORLANDC__)
/* Borland */
#  include "es_borl.h"
# else
#  include "es_config.h"
#endif

#if 0

#if 0
# elif defined(__sgi) && !defined(__GNUC__)
#  include <es_sgi.h>
# elif (defined(__OS400__))
/* AS/400 C++ */
#  include <es_as400.h>
# elif ( defined (__xlC__) && __xlC__ < 0x400 ) || (defined(__MVS__) && defined ( __IBMCPP__ ) && (__IBMCPP__ < 23000 )) || ( defined (  __IBMCPP__ ) && (  __IBMCPP__ < 400 ) && !defined(__OS400__) )
/* AIX xlC 3.1 , 3.0.1 ==0x301
 * Visual Age C++ 3.x
 * OS-390 C++
 */
#  include <es_ibm.h>
# elif defined ( __BORLANDC__ )
/* Borland C++ ( 4.x - 5.x ) */
#  include <es_bc.h>
# elif defined(__SUNPRO_CC)
/* SUN CC 4.0.1-5.0  */
#  include <es_sunpro.h>
# elif defined (__WATCOM_CPLUSPLUS__)
/* Watcom C++ */
#  include <es_watcom.h>
# elif defined(__COMO__) || defined (__COMO_VERSION_)
#  include <es_como.h>
# elif defined (__SC__) && (__SC__ < 0x800)
/* Symantec 7.5 */
#  include <es_symantec.h>
# elif defined (__MRC__) || (defined (__SC__) && (__SC__ >= 0x882))
/* Apple MPW SCpp 8.8.2  
 * Apple MPW MrCpp 4.1.0 */
#  include <es_apple.h>
# elif defined(__hpux)
/* HP compilers */
#  include <es_hpacc.h>
# elif defined(__ICL)
/* Intel reference compiler for Win */
#  include <es_intel.h>
/* SCO UDK 7 compiler (UnixWare 7x, OSR 5, UnixWare 2x) */
# elif defined(__USLC__)
#  include <es_sco.h>
/* Apogee 4.x */
# elif defined (__APOGEE__)
#  include <es_apcc.h>
# elif defined (__KCC)
#  include <es_kai.h>
# elif defined (__DECCXX)
#  ifdef __vms
#    include <es_dec_vms.h>
#  else
#    include <es_dec.h>
#  endif
# elif defined (__MLCCPP__)
#  include <es_mlc.h>
# else
/* Unable to identify the compiler, issue error diagnostic.
 * Edit <es_mycomp.h> to set STLport up for your compiler. */
#  include <es_mycomp.h>
# endif /* end of compiler choice */
# undef __GIVE_UP_WITH_STL

#endif
#endif

