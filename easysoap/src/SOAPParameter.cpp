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
static const char *SOAP_xsi_type = TAG_SOAP_XSI ":type";

SOAPParameter::SOAPParameter()
: m_parent(0)
, m_basetype(SOAPTypes::xsd_none)
{
	Reset();
}

SOAPParameter::~SOAPParameter()
{
}

void
SOAPParameter::Assign(const SOAPParameter& param)
{
	m_basetype = param.m_basetype;
	m_name = param.m_name;
	m_attrs = param.m_attrs;
	m_strval = param.m_strval;

	const Array& params = param.GetArray();
	for (int i = 0; i < params.Size(); ++i)
		AddParameter(params[i].GetName()) = params[i];
}

SOAPParameter::SOAPParameter(const SOAPParameter& param)
: m_parent(0)
{
	Assign(param);
}

SOAPParameter&
SOAPParameter::operator=(const SOAPParameter& param)
{
	Reset();
	Assign(param);
	return *this;
}

void
SOAPParameter::Reset()
{
	m_array.Resize(0);
	m_struct.Clear();
	m_attrs.Clear();
	SetType(SOAPTypes::xsd_none);
}

void
SOAPParameter::SetName(const char *name)
{
	if (m_parent && m_name.Length() > 0)
		m_parent->m_struct.Remove(m_name);

	m_name = name;

	if (m_parent)
		m_parent->m_struct[m_name] = this;
}

void
SOAPParameter::SetType(const char *type)
{
	m_attrs[SOAP_xsi_type] = type;
}

void
SOAPParameter::SetType(const char *type, const char *ns)
{
	char nsbuff[64];
	char typebuff[256];

	unsigned int symnum = ++m_gensym;
	snprintf(nsbuff, sizeof(nsbuff), "xmlns:ns%d",symnum);
	snprintf(typebuff, sizeof(typebuff), "ns%d:%s",symnum, type);

	SOAPParameter *param = this;
	while (param->m_parent)
		param = param->m_parent;
	param->m_attrs[nsbuff] = ns;
	m_attrs[SOAP_xsi_type] = typebuff;
}

void
SOAPParameter::SetType(SOAPTypes::xsd_type type)
{
	SetType(SOAPTypes::GetXsdString(type));
}

SOAPTypes::xsd_type
SOAPParameter::GetType() const
{
	if (m_basetype != SOAPTypes::xsd_none)
		return m_basetype;

	Attrs::Iterator i = m_attrs.Find(SOAP_xsi_type);
	if (i)
		return SOAPTypes::GetXsdType(*i);
	return SOAPTypes::xsd_none;
}

const char *
SOAPParameter::GetTypeString() const
{
	Attrs::Iterator i = m_attrs.Find(SOAP_xsi_type);
	if (i)
		return *i;
	return 0;
}

void
SOAPParameter::SetNull()
{
	//SetType(SOAPTypes::xsd_null);
}

void
SOAPParameter::SetInteger(const char *val)
{
	SetType(SOAPTypes::xsd_int);
	m_strval = val;
}

void
SOAPParameter::SetValue(const char *val)
{
	SetType(SOAPTypes::xsd_string);
	m_strval = val;
}

void
SOAPParameter::SetValue(int val)
{
	char buffer[64];
	snprintf(buffer, sizeof(buffer), "%d", val);
	SetInteger(buffer);
}

void
SOAPParameter::SetFloat(const char *val)
{
	SetType(SOAPTypes::xsd_float);
	m_strval = val;
}

void
SOAPParameter::SetValue(float val)
{
	char buffer[64];
	snprintf(buffer, sizeof(buffer), "%f", val);
	SetFloat(buffer);
}

void
SOAPParameter::SetDouble(const char *val)
{
	SetType(SOAPTypes::xsd_double);
	m_strval = val;
}

void
SOAPParameter::SetValue(double val)
{
	char buffer[64];
	snprintf(buffer, sizeof(buffer), "%f", val);
	SetDouble(buffer);
}

const SOAPParameter *
SOAPParameter::GetParameter(const char *name) const
{
	Struct::Iterator i = m_struct.Find(name);
	if (i)
		return *i;
	return 0;
}


bool
SOAPParameter::WriteSOAPPacket(SOAPPacketWriter& packet) const
{
	const char *sym = 0;
	char symbuff[64];

	//if (m_type == SOAPTypes::xsd_none)
		//throw SOAPException("Cannot serialize undefined parameter value");

	if (m_name.Length() > 0)
	{
		sym = m_name;
	}
	/*
	else if (m_type == SOAPTypes::soap_array)
	{
		sym = TAG_SOAP_ENC ":Array";
	}
	else if (m_type == SOAPTypes::soap_struct)
	{
		sym = TAG_SOAP_ENC ":Struct";
	}*/
	else
	{
		// TODO: Move random tag generation to packet writer
		snprintf(symbuff, sizeof(symbuff), "gensym%d", ++m_gensym);
		sym = symbuff;
	}

	packet.StartTag(sym);

	for (Attrs::Iterator ai = m_attrs.Begin(); ai != m_attrs.End(); ++ai)
		packet.AddAttr(ai.Key(), ai.Item());

	switch (GetType())
	{
	case SOAPTypes::soap_array:
		{
			// TODO:  This is bogus.  We need to go through all params
			// and check their type.  If they're not all the same, use 'anyType'
			char typebuff[128];
			if (GetArray().Size() > 0)
			{
				snprintf(typebuff, sizeof(typebuff), "%s[%d]", GetArray()[0].GetTypeString(), GetArray().Size());
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
				(*i)->WriteSOAPPacket(packet);
		}
		break;

	default:
		packet.WriteValue(m_strval);
		break;
	}

	packet.EndTag(sym);

	return true;
}



