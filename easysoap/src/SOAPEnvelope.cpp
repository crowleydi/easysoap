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
#include "SOAPEnvelope.h"
#include "SOAPNamespaces.h"
#include "SOAPPacketWriter.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SOAPEnvelope::SOAPEnvelope()
{

}

SOAPEnvelope::~SOAPEnvelope()
{

}

bool
SOAPEnvelope::WriteSOAPPacket(SOAPPacketWriter& packet) const
{
	packet.Reset();
	packet.StartNSTag(FULL_SOAP_ENV, "Envelope", "V");

	// TODO: automagically add only the tags we need...
	packet.AddXMLNS("C", FULL_SOAP_ENC);
	packet.AddXMLNS("i", FULL_SOAP_XSI);
	packet.AddXMLNS("d", FULL_SOAP_XSD);

	// TODO: allow user to set encoding style
	packet.AddNSAttr(FULL_SOAP_ENV, "encodingStyle", FULL_SOAP_ENC);

	m_header.WriteSOAPPacket(packet);
	m_body.WriteSOAPPacket(packet);

	packet.EndNSTag(FULL_SOAP_ENV, "Envelope");

	return true;
}




