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
}

SOAPParameter::SOAPParameter(const SOAPParameter& param)
{
	m_name = param.m_name;
	m_type = param.m_type;

	m_strval = param.m_strval;
	m_struct = 0;

	GetArray() = param.GetArray();
	if (param.m_struct)
		GetStruct() = param.GetStruct();
}

SOAPParameter&
SOAPParameter::operator=(const SOAPParameter& param)
{
	m_name = param.m_name;
	m_type = param.m_type;

	m_strval = param.m_strval;

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

void
SOAPParameter::Reset()
{
	SetType(SOAPTypes::xsd_none);
	m_array.Resize(0);
	if (m_struct)
		m_struct->Clear();
}

void
SOAPParameter::SetInteger(const char *val)
{
	m_strval = val;
	m_type = SOAPTypes::xsd_int;
}

void
SOAPParameter::SetValue(int val)
{
	char buffer[32];
	snprintf(buffer, sizeof(buffer), "%d", val);
	SetInteger(buffer);
}

void
SOAPParameter::SetFloat(const char *val)
{
	m_strval = val;
	m_type = SOAPTypes::xsd_float;
}

void
SOAPParameter::SetValue(float val)
{
	char buffer[32];
	snprintf(buffer, sizeof(buffer), "%f", val);
	SetFloat(buffer);
}

void
SOAPParameter::SetDouble(const char *val)
{
	m_strval = val;
	m_type = SOAPTypes::xsd_double;
}

void
SOAPParameter::SetValue(double val)
{
	char buffer[32];
	snprintf(buffer, sizeof(buffer), "%f", val);
	SetDouble(buffer);
}

bool
SOAPParameter::WriteSOAPPacket(SOAPPacketWriter& packet) const
{
	const char *sym = 0;
	char symbuff[64];

	if (m_type == SOAPTypes::xsd_none)
		throw SOAPException("Cannot serialize undefined parameter value");

	if (m_name.Length() > 0)
	{
		sym = m_name;
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
	case SOAPTypes::xsd_float:
	case SOAPTypes::xsd_double:
	case SOAPTypes::xsd_string:
		packet.WriteValue(m_strval);
		break;

	case SOAPTypes::soap_array:
		{
			// TODO:  This is bogus.  We need to go through all params
			// and check their type.  If they're not all the same, use 'variant'
			// or whatever the xsd type is...
			char typebuff[128];
			if (GetArray().Size() > 0)
			{
				snprintf(typebuff, sizeof(typebuff), "%s[%d]", GetArray()[0].GetXsdString(), GetArray().Size());
				packet.AddAttr(TAG_SOAP_ENC ":arrayType", typebuff);
			}
			else
			{
				packet.AddAttr(TAG_SOAP_ENC ":arrayType", TAG_SOAP_XSD ":int[0]");
			}

			for (size_t i = 0; i < GetArray().Size(); ++i)
				GetArray()[i].WriteSOAPPacket(packet);

		}
		break;

	case SOAPTypes::soap_struct:
		{
			for (Struct::Iterator i = GetStruct().Begin(); i != GetStruct().End(); ++i)
				i->WriteSOAPPacket(packet);
		}
		break;

	default:
		break;
	}

	packet.EndTag(sym);

	return true;
}



