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

#include "SOAP.h"
#include "SOAPEnvelope.h"
#include "SOAPNamespaces.h"
#include "SOAPPacketWriter.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

const char *SOAPEnvelope::SOAP_ENV_ns = FULL_SOAP_ENV;
const char *SOAPEnvelope::SOAP_ENC_ns = FULL_SOAP_ENC;
const char *SOAPEnvelope::SOAP_xsi_ns = FULL_SOAP_XSI;
const char *SOAPEnvelope::SOAP_xsd_ns = FULL_SOAP_XSD;

const char *SOAPEnvelope::write_tag = TAG_SOAP_ENV ":Envelope";

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
	packet.StartTag(write_tag);

	// TODO: automagically add only the tags we need...
	packet.AddAttr("xmlns:" TAG_SOAP_ENC, SOAP_ENC_ns);
	packet.AddAttr("xmlns:" TAG_SOAP_ENV, SOAP_ENV_ns);
	packet.AddAttr("xmlns:" TAG_SOAP_XSI, SOAP_xsi_ns);
	packet.AddAttr("xmlns:" TAG_SOAP_XSD, SOAP_xsd_ns);

	// Some services seem to need this:
	packet.AddAttr("SOAP-ENV:encodingStyle", SOAP_ENC_ns);

	m_header.WriteSOAPPacket(packet);
	m_body.WriteSOAPPacket(packet);

	packet.EndTag(write_tag);
	return true;
}




