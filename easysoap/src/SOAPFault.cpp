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
#include "SOAPFault.h"
#include "SOAPNamespaces.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

const char *SOAPFault::faultactor_attr = FULL_SOAP_ENV PARSER_NS_SEP "faultactor";
const char *SOAPFault::faultcode_attr = FULL_SOAP_ENV PARSER_NS_SEP "faultcode";
const char *SOAPFault::faultdetail_attr = FULL_SOAP_ENV PARSER_NS_SEP "detail";
const char *SOAPFault::faultstring_attr = FULL_SOAP_ENV PARSER_NS_SEP "faultstring";

SOAPFault::SOAPFault()
{

}

SOAPFault::~SOAPFault()
{

}

bool
SOAPFault::WriteSOAPPacket(SOAPPacketWriter& packet) const
{
	return true;
}

const SOAPParameter*
SOAPFault::GetFaultCode() const
{
	Struct::Iterator i = m_params.Find(faultcode_attr);
	if (i)
		return *i;
	return 0;
}

const SOAPParameter*
SOAPFault::GetFaultString() const
{	Struct::Iterator i = m_params.Find(faultstring_attr);
	if (i)
		return *i;
	return 0;
}

const SOAPParameter*
SOAPFault::GetFaultActor() const
{
	Struct::Iterator i = m_params.Find(faultactor_attr);
	if (i)
		return *i;
	return 0;
}

const SOAPParameter*
SOAPFault::GetDetail() const
{
	Struct::Iterator i = m_params.Find(faultdetail_attr);
	if (i)
		return *i;
	return 0;
}
