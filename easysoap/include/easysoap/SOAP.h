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


#ifndef __SOAP_h__
#define __SOAP_h__

#define EASYSOAP_STRING			"EasySoap++"
#define EASYSOAP_HOMEPAGE		"http://easysoap.sourceforge.net/"
#define EASYSOAP_VERSION_STRING	"0.5"
#define EASYSOAP_VERSION		50

#include <easysoap/es_comp.h>

#include <easysoap/SOAPUtil.h>
#include <easysoap/SOAPHashMap.h>
#include <easysoap/SOAPArray.h>
#include <easysoap/SOAPString.h>
#include <easysoap/SOAPQName.h>
#include <easysoap/SOAPException.h>

class SOAPPacketWriter;

#include <easysoap/SOAPParseEventHandler.h>
#include <easysoap/SOAPProxy.h>

#endif // __SOAP_h__

