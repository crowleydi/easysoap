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


#ifndef __SOAPNamesSpaces_h__
#define __SOAPNamesSpaces_h__

#define SOAP_ENV "http://schemas.xmlsoap.org/soap/envelope/"
#define SOAP_ENC "http://schemas.xmlsoap.org/soap/encoding/"
#define SOAP_ACTOR_NEXT "http://schemas.xmlsoap.org/soap/actor/next"

#define SOAP_XSI_1999 "http://www.w3.org/1999/XMLSchema-instance"
#define SOAP_XSD_1999 "http://www.w3.org/1999/XMLSchema"
#define SOAP_XSI_2001 "http://www.w3.org/2001/XMLSchema-instance"
#define SOAP_XSD_2001 "http://www.w3.org/2001/XMLSchema"
#define SOAP_XSI SOAP_XSI_2001
#define SOAP_XSD SOAP_XSD_2001

class EASYSOAP_EXPORT SOAPEnc
{
public:
	//
	// for locating the document root
	static const SOAPQName root;
	//
	// For array encodings.
	static const SOAPQName Array;
	static const SOAPQName arrayType;
	static const SOAPQName offset;
	static const SOAPQName position;
	//
	// This was for before there was a base64
	// type in the XML Schema
	static const SOAPQName base64;
};

class EASYSOAP_EXPORT SOAPEnv
{
public:
	static const SOAPQName Body;
	static const SOAPQName Header;
	static const SOAPQName Envelope;
	static const SOAPQName Fault;

	static const SOAPQName encodingStyle;
	//
	// For headers
	static const SOAPQName mustUnderstand;
	static const SOAPQName actor;
};

class EASYSOAP_EXPORT XMLSchemaInstance
{
public:
	static const SOAPQName type;
	static const SOAPQName nil;
};

#endif // __SOAPNamesSpaces_h__
