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
#include "SOAPParameter.h"
#include "SOAPNamespaces.h"
#include "SOAPPacketWriter.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

unsigned int SOAPParameter::m_gensym = 0;

SOAPParameter::~SOAPParameter()
{
	delete m_struct;
	delete m_array;
}

SOAPParameter::SOAPParameter(const SOAPParameter& param)
{
	m_name = param.m_name;
	m_type = param.m_type;

	m_intval = param.m_intval;
	m_fltval = param.m_fltval;
	m_dblval = param.m_dblval;
	m_strval = param.m_strval;

	m_array = 0;
	m_struct = 0;

	if (param.m_array)
		GetArray() = param.GetArray();
	if (param.m_struct)
		GetStruct() = param.GetStruct();
}

SOAPParameter&
SOAPParameter::operator=(const SOAPParameter& param)
{
	m_name = param.m_name;
	m_type = param.m_type;

	m_intval = param.m_intval;
	m_fltval = param.m_fltval;
	m_dblval = param.m_dblval;
	m_strval = param.m_strval;

	if (param.m_array)
		GetArray() = param.GetArray();
	if (param.m_struct)
		GetStruct() = param.GetStruct();

	return *this;
}

const char *
SOAPParameter::GetXsdString() const
{
	return SOAPTypes::GetXsdString(m_type);
}

bool
SOAPParameter::WriteSOAPPacket(SOAPPacketWriter& packet) const
{
	const char *sym = 0;
	char symbuff[64];

	if (m_type == SOAPTypes::xsd_none)
		throw SOAPException("Cannot serialize undefined parameter value");

	if (!m_name.empty())
	{
		sym = m_name.c_str();
	}
	else if (m_type == SOAPTypes::soap_array)
	{
		sym = TAG_SOAP_ENC ":Array";
	}
	else if (m_type == SOAPTypes::soap_struct)
	{
		sym = TAG_SOAP_ENC ":Struct";
	}
	else
	{
		// TODO: Move random tag generation to packet writer
		snprintf(symbuff, sizeof(symbuff), "gensym%d", ++m_gensym);
		sym = symbuff;
	}

	packet.StartTag(sym);
	packet.AddAttr(TAG_SOAP_XSI ":type", GetXsdString());

	switch (m_type)
	{
	case SOAPTypes::xsd_int:
		packet.WriteValue(m_intval);
		break;

	case SOAPTypes::xsd_float:
		packet.WriteValue(m_fltval);
		break;

	case SOAPTypes::xsd_double:
		packet.WriteValue(m_dblval);
		break;

	case SOAPTypes::xsd_string:
		packet.WriteValue(m_strval.c_str());
		break;

	case SOAPTypes::soap_array:
		{
			// TODO:  This is bogus.  We need to go through all params
			// and check their type.  If they're not all the same, use 'variant'
			// or whatever the xsd type is...
			char typebuff[128];
			if (GetArray().size() > 0)
			{
				snprintf(typebuff, sizeof(typebuff), "%s[%d]", GetArray()[0].GetXsdString(), GetArray().size());
				packet.AddAttr(TAG_SOAP_ENC ":arrayType", typebuff);
			}
			else
			{
				packet.AddAttr(TAG_SOAP_ENC ":arrayType", TAG_SOAP_XSD ":int[0]");
			}

			for (size_t i = 0; i < GetArray().size(); ++i)
				GetArray()[i].WriteSOAPPacket(packet);

		}
		break;

	case SOAPTypes::soap_struct:
		{
			for (Struct::const_iterator i = GetStruct().begin(); i != GetStruct().end(); ++i)
				i->second.WriteSOAPPacket(packet);
		}
		break;

	default:
		break;
	}

	packet.EndTag(sym);

	return true;
}



