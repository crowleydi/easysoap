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

#include <float.h>
#include <math.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

unsigned int SOAPParameter::m_gensym = 0;
static const char *SOAP_xsi_type = TAG_SOAP_XSI ":type";

SOAPParameter::SOAPParameter()
: m_parent(0)
, m_isnull(true)
, m_outtasync(false)
, m_basetype(SOAPTypes::xsd_none)
{
	Reset();
}

SOAPParameter::SOAPParameter(const SOAPParameter& param)
: m_parent(0)
, m_isnull(true)
, m_basetype(SOAPTypes::xsd_none)
{
	Assign(param);
}

SOAPParameter::~SOAPParameter()
{
	Reset();
}

void
SOAPParameter::Assign(const SOAPParameter& param)
{
	Reset();
	m_basetype = param.m_basetype;
	SetName(param.m_name);
	m_attrs = param.m_attrs;
	m_strval = param.m_strval;

	const Array& params = param.GetArray();
	m_array.Resize(params.Size());
	for (size_t i = 0; i < params.Size(); ++i)
	{
		SOAPParameter& p = m_array[i];
		p.SetParent(this);
		p = params[i];
	}
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
	SetNull();
}

void
SOAPParameter::SetName(const char *name)
{
	m_name = name;

	if (m_parent)
		m_parent->m_outtasync = true;
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
	m_isnull = true;
	m_strval = "";
}

void
SOAPParameter::SetInteger(const char *val)
{
	SetType(SOAPTypes::xsd_int);
	m_strval = val;
	m_isnull = false;
}

void
SOAPParameter::SetValue(const char *val)
{
	SetType(SOAPTypes::xsd_string);
	m_strval = val;
	m_isnull = false;
}

void
SOAPParameter::SetFloat(const char *val)
{
	SetType(SOAPTypes::xsd_float);
	m_strval = val;
	m_isnull = false;
}

void
SOAPParameter::SetDouble(const char *val)
{
	SetType(SOAPTypes::xsd_double);
	m_strval = val;
	m_isnull = false;
}

void
SOAPParameter::SetValue(int val)
{
	char buffer[64];
	snprintf(buffer, sizeof(buffer), "%d", val);
	SetInteger(buffer);
}

void
SOAPParameter::SetValue(float fval)
{
	double val = fval;
	if (finite(val))
	{
		char buffer[64];
		snprintf(buffer, sizeof(buffer), "%g", val);
		SetFloat(buffer);
	}
	else
	{
		if (isnan(val))
			SetFloat("NaN");
		else if (val > 0.0)
			SetFloat("INF");
		else
			SetFloat("-INF");
	}
}

void
SOAPParameter::SetValue(double val)
{
	if (finite(val))
	{
		char buffer[64];
		snprintf(buffer, sizeof(buffer), "%g", val);
		SetDouble(buffer);
	}
	else
	{
		if (isnan(val))
			SetDouble("NaN");
		else if (val > 0.0)
			SetDouble("+INF");
		else
			SetDouble("-INF");
	}
}

int
SOAPParameter::GetInt() const
{
	return atoi(m_strval);
}

float
SOAPParameter::GetFloat() const
{
	return GetDouble();
}

double
SOAPParameter::GetDouble() const
{
	if (m_strval == "INF")
		return HUGE_VAL;
	else if (m_strval == "-INF")
		return -HUGE_VAL;
	return atof(m_strval);
}

const SOAPParameter *
SOAPParameter::GetParameter(const char *name) const
{
	SOAPParameter *ncthis = const_cast<SOAPParameter *>(this);
	ncthis->CheckStructSync();
	Struct::Iterator i = m_struct.Find(name);
	if (i)
		return *i;
	return 0;
}


SOAPParameter&
SOAPParameter::AddParameter(const char *name)
{
	SOAPParameter& ret = m_array.Add();
	ret.SetParent(this);
	ret.SetName(name);
	m_outtasync = true;

	return ret;
}

SOAPParameter::Struct&
SOAPParameter::GetStruct()
{
	CheckStructSync();
	return m_struct;
}

const SOAPParameter::Struct&
SOAPParameter::GetStruct() const
{
	SOAPParameter *ncthis = const_cast<SOAPParameter *>(this);
	ncthis->CheckStructSync();
	return m_struct;
}

void
SOAPParameter::CheckStructSync()
{
	if (m_outtasync)
	{
		m_struct.Clear();
		for (Array::Iterator i = m_array.Begin(); i != m_array.End(); ++i)
		{
			m_struct[i->GetName()] = i;
		}
		m_outtasync = false;
	}
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



