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


#ifdef _WIN32
#pragma warning (disable: 4786)
#endif // _WIN32

#include <map>
#include <string>

#include "SOAP.h"
#include "SOAPTypes.h"
#include "SOAPNamespaces.h"

//////////////////////////////////////////////////////////////////////
// Setup the typemap
//////////////////////////////////////////////////////////////////////
typedef std::map<std::string, SOAPTypes::xsd_type> TypeMap;

static TypeMap typemap;
static int init_typemap()
{
	typemap["xsd:int"] = SOAPTypes::xsd_int;
	typemap["xsd:float"] = SOAPTypes::xsd_float;
	typemap["xsd:double"] = SOAPTypes::xsd_double;
	typemap["xsd:string"] = SOAPTypes::xsd_string;

	typemap["SOAP-ENC:SOAPStruct"] = SOAPTypes::soap_struct;
	typemap["SOAP-ENC:Array"] = SOAPTypes::soap_array;

	return 1;
}
static int didinit = init_typemap();

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SOAPTypes::SOAPTypes()
{

}

SOAPTypes::~SOAPTypes()
{

}

const char *
SOAPTypes::GetXsdString(xsd_type type)
{
	switch (type)
	{
	case xsd_int:		return TAG_SOAP_XSD ":int";
	case xsd_float:		return TAG_SOAP_XSD ":float";
	case xsd_double:	return TAG_SOAP_XSD ":double";
	case xsd_string:	return TAG_SOAP_XSD ":string";

	case soap_array:	return TAG_SOAP_ENC ":Array";
	case soap_struct:	return TAG_SOAP_ENC ":SOAPStruct";

	default:			return 0;
	}
}


SOAPTypes::xsd_type
SOAPTypes::GetXsdType(const char *str)
{
	TypeMap::iterator i = typemap.find(str);

	if (i == typemap.end())
		return xsd_none;

	return i->second;
}
