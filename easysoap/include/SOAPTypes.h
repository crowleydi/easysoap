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
 */


#if !defined(AFX_SOAPTYPES_H__F9D6AD01_BA1F_43D4_A5E4_FC254EB9FD7A__INCLUDED_)
#define AFX_SOAPTYPES_H__F9D6AD01_BA1F_43D4_A5E4_FC254EB9FD7A__INCLUDED_

#include "SOAP.h"

class EASYSOAP_EXPORT SOAPTypes  
{

public:
	SOAPTypes();
	~SOAPTypes();

	typedef enum {
		xsd_none,

		xsd_int,		// 32-bit signed integer
		xsd_integer,	// infinite precision signed integer, -inf..+inf
		xsd_float,		// 32-bit floating point
		xsd_double,		// 64-bit floating point
		xsd_string,		// a string
		xsd_base64,		// binary encoded into base64

		soap_array,
		soap_struct,

		unknown			// for custom types
	} xsd_type;

	static xsd_type GetXsdType(const char *str);
	static const char *GetXsdString(xsd_type type);

};

#endif // !defined(AFX_SOAPTYPES_H__F9D6AD01_BA1F_43D4_A5E4_FC254EB9FD7A__INCLUDED_)

