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

const char *SOAPFault::faultactor_attr = "faultactor";
const char *SOAPFault::faultcode_attr = "faultcode";
const char *SOAPFault::faultdetail_attr = "detail";
const char *SOAPFault::faultstring_attr = "faultstring";

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
	return GetParameter(faultcode_attr);
}

const SOAPParameter*
SOAPFault::GetFaultString() const
{
	return GetParameter(faultstring_attr);
}

const SOAPParameter*
SOAPFault::GetFaultActor() const
{
	return GetParameter(faultactor_attr);
}

const SOAPParameter*
SOAPFault::GetDetail() const
{
	return GetParameter(faultdetail_attr);
}
