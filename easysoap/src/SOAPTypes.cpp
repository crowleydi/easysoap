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


#ifdef _MSC_VER
#pragma warning (disable: 4786)
#endif // _MSC_VER

#include "SOAP.h"
#include "SOAPTypes.h"
#include "SOAPNamespaces.h"

//////////////////////////////////////////////////////////////////////
// Setup the typemap
//////////////////////////////////////////////////////////////////////
typedef SOAPHashMap<SOAPString, SOAPTypes::xsd_type> TypeMap;

static TypeMap *__es_typemap = 0;
static TypeMap& GetTypeMap()
{
	if (!__es_typemap)
	{
		__es_typemap = new TypeMap();
		TypeMap& typemap = *__es_typemap;

		typemap[TAG_SOAP_XSD ":int"] = SOAPTypes::xsd_int;
		typemap[TAG_SOAP_XSD ":integer"] = SOAPTypes::xsd_integer;
		typemap[TAG_SOAP_XSD ":float"] = SOAPTypes::xsd_float;
		typemap[TAG_SOAP_XSD ":double"] = SOAPTypes::xsd_double;
		typemap[TAG_SOAP_XSD ":string"] = SOAPTypes::xsd_string;
		typemap[TAG_SOAP_XSD ":base64"] = SOAPTypes::xsd_base64;

		typemap[FULL_SOAP_XSD_2001 PARSER_NS_SEP "int"] = SOAPTypes::xsd_int;
		typemap[FULL_SOAP_XSD_2001 PARSER_NS_SEP "integer"] = SOAPTypes::xsd_integer;
		typemap[FULL_SOAP_XSD_2001 PARSER_NS_SEP "float"] = SOAPTypes::xsd_float;
		typemap[FULL_SOAP_XSD_2001 PARSER_NS_SEP "double"] = SOAPTypes::xsd_double;
		typemap[FULL_SOAP_XSD_2001 PARSER_NS_SEP "string"] = SOAPTypes::xsd_string;

		typemap[FULL_SOAP_XSD_1999 PARSER_NS_SEP "int"] = SOAPTypes::xsd_int;
		typemap[FULL_SOAP_XSD_1999 PARSER_NS_SEP "integer"] = SOAPTypes::xsd_integer;
		typemap[FULL_SOAP_XSD_1999 PARSER_NS_SEP "float"] = SOAPTypes::xsd_float;
		typemap[FULL_SOAP_XSD_1999 PARSER_NS_SEP "double"] = SOAPTypes::xsd_double;
		typemap[FULL_SOAP_XSD_1999 PARSER_NS_SEP "string"] = SOAPTypes::xsd_string;

		typemap["SOAP-ENC:SOAPStruct"] = SOAPTypes::soap_struct;
		typemap["SOAP-ENC:Array"] = SOAPTypes::soap_array;

		typemap[FULL_SOAP_ENC PARSER_NS_SEP "SOAPStruct"] = SOAPTypes::soap_struct;
		typemap[FULL_SOAP_ENC PARSER_NS_SEP "Array"] = SOAPTypes::soap_array;
	}
	return *__es_typemap;
}

static class gc_es_typemap
{
public:
	~gc_es_typemap() {delete __es_typemap;}
} _gc_es_typemap;

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
	case xsd_integer:	return TAG_SOAP_XSD ":integer";
	case xsd_float:		return TAG_SOAP_XSD ":float";
	case xsd_double:	return TAG_SOAP_XSD ":double";
	case xsd_string:	return TAG_SOAP_XSD ":string";
	case xsd_base64:	return TAG_SOAP_XSD ":base64";

	case soap_array:	return TAG_SOAP_ENC ":Array";
	case soap_struct:	return TAG_SOAP_ENC ":SOAPStruct";

	default:			return 0;
	}
}


SOAPTypes::xsd_type
SOAPTypes::GetXsdType(const char *str)
{
	if (!str)
		return xsd_none;

	TypeMap::Iterator i = GetTypeMap().Find(str);

	if (i == GetTypeMap().End())
		return unknown;

	return *i;
}
