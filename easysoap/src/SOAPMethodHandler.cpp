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

#include "SOAPMethodHandler.h"
#include "SOAPMethod.h"
#include "SOAPNamespaces.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SOAPMethodHandler::SOAPMethodHandler(SOAPMethod& method)
: m_method(&method)
{

}

SOAPMethodHandler::~SOAPMethodHandler()
{

}

SOAPParseEventHandler *
SOAPMethodHandler::start(const XML_Char *name, const XML_Char **attrs)
{
	m_method->Reset();
	return this;
}

SOAPParseEventHandler *
SOAPMethodHandler::startElement(const XML_Char *name, const XML_Char **attrs)
{
	// to find what type of handler we need to use,
	// we have to iterate over the attributes.  ugly.

	const XML_Char **work = attrs;
	while (*work)
	{
		const char *tag = *work++;
		const char *val = *work++;

		if (sp_strcmp(tag, FULL_SOAP_XSI PARSER_NS_SEP "type") == 0)
		{
			SOAPTypes::xsd_type type = SOAPTypes::GetXsdType(val);

			//
			// TODO:  Expand the value of the namespace identifier in the
			// value incase other namespaces (other than xsi, xsd) are used.

			if (type == SOAPTypes::soap_array ||
				// this is hack because we don't disambiguate
				(type == SOAPTypes::xsd_none && sp_strstr(val, ":Array") != 0))
			{
				m_arrayHandler.SetParameter(m_method->AddParameter());
				return m_arrayHandler.start(name, attrs);
			}

			if (type == SOAPTypes::soap_struct ||
				// this is hack because we don't disambiguate
				(type == SOAPTypes::xsd_none && sp_strstr(val, ":Struct") != 0))
			{
				m_structHandler.SetParameter(m_method->AddParameter());
				return m_structHandler.start(name, attrs);
			}

			if (type == SOAPTypes::xsd_none)
			{
				// throw after we implement more types
				return 0;
			}

			break;
		}
	}

	m_paramHandler.SetParameter(m_method->AddParameter());
	return m_paramHandler.start(name, attrs);
}

void
SOAPMethodHandler::characterData(const XML_Char *str, int len)
{
}

void
SOAPMethodHandler::endElement(const XML_Char *name)
{
}



