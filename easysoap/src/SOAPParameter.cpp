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

#define NULL_FLAG   1
#define ARRAY_FLAG  2
#define STRUCT_FLAG 4

unsigned int SOAPParameter::m_gensym = 0;

SOAPParameter::SOAPParameter()
: m_parent(0)
, m_flags(NULL_FLAG)
, m_outtasync(false)
{
	Reset();
}

SOAPParameter::SOAPParameter(const SOAPParameter& param)
: m_parent(0)
, m_flags(NULL_FLAG)
, m_outtasync(false)
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
	m_outtasync = true;
	m_name = param.m_name;
	m_type = param.m_type;
	m_strval = param.m_strval;
	m_flags= param.m_flags;

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
	m_type.Clear();
	m_flags = NULL_FLAG;
	m_outtasync = false;
}

void
SOAPParameter::SetName(const char *name, const char *ns)
{
	if (!ns)
		m_name = name;
	else
		m_name.Set(name, ns);

	if (m_parent)
		m_parent->m_outtasync = true;
}

void
SOAPParameter::SetType(const char *type, const char *ns)
{
	if (ns)
		m_type.Set(type, ns);
	else
		m_type.Set(type, "___easysoap_default_ns");
}

bool
SOAPParameter::IsNull() const
{
	return (m_flags & NULL_FLAG) != 0;
}

bool
SOAPParameter::IsArray() const
{
	return (m_flags & ARRAY_FLAG) != 0;
}

bool
SOAPParameter::IsStruct() const
{
	return (m_flags & STRUCT_FLAG) != 0;
}

void
SOAPParameter::SetNull(bool isnull)
{
	if (isnull)
	{
		m_flags = NULL_FLAG;
		m_strval = (const char *)0;
	}
	else
	{
		m_flags = 0;
		m_strval = "";
	}
}

void
SOAPParameter::SetIsArray()
{
	m_flags = ARRAY_FLAG;
	SetType("Array", SOAP_ENC);
}

void
SOAPParameter::SetIsStruct()
{
	m_flags = STRUCT_FLAG;
}

void
SOAPParameter::SetInt(const char *val)
{
	SetType("int");
	if (val)
	{
		m_strval = val;
		m_flags = 0;
	}
	else
		SetNull();
}

void
SOAPParameter::SetValue(const char *val)
{
	SetType("string");
	if (val)
	{
		m_strval = val;
		m_flags = 0;
	}
	else
		SetNull();
}

void
SOAPParameter::SetFloat(const char *val)
{
	SetType("float");
	if (val)
	{
		m_strval = val;
		m_flags = 0;
	}
	else
		SetNull();
}

void
SOAPParameter::SetDouble(const char *val)
{
	SetType("double");
	if (val)
	{
		m_strval = val;
		m_flags = 0;
	}
	else
		SetNull();
}

void
SOAPParameter::SetValue(int val)
{
	char buffer[64];
	snprintf(buffer, sizeof(buffer), "%d", val);
	SetInt(buffer);
}

void
SOAPParameter::SetValue(float fval)
{
	double val = fval;
	if (finite(val))
	{
		char buffer[64];
		snprintf(buffer, sizeof(buffer), "%.9G", val);
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
		snprintf(buffer, sizeof(buffer), "%.18G", val);
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

void
SOAPParameter::SetBoolean(const char *val)
{
	SetType("boolean");
	if (val)
	{
		m_strval = val;
		m_flags = 0;
	}
	else
		SetNull();
}

void
SOAPParameter::SetValue(bool val)
{
	if (val)
		SetBoolean("true");
	else
		SetBoolean("false");
}


int
SOAPParameter::GetInt() const
{
	return atoi(m_strval);
}

bool
SOAPParameter::GetBoolean() const
{
	if (sp_strcasecmp(m_strval, "true") == 0 ||
		sp_strcmp(m_strval, "1") == 0)
		return true;
	if (sp_strcasecmp(m_strval, "false") == 0 ||
		sp_strcmp(m_strval, "0") == 0)
		return false;
	throw SOAPException("Invalid value for boolean type: %s",
		(const char *)m_strval);
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
	return strtod(m_strval, 0);
}

const SOAPParameter&
SOAPParameter::GetParameter(const char *name) const
{
	CheckStructSync();
	Struct::Iterator i = m_struct.Find(name);
	if (!i)
		throw SOAPException("Could not find element by name: %s", name);
	return **i;
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

SOAPParameter&
SOAPParameter::AddParameter(const SOAPParameter& p)
{
	SOAPParameter& ret = m_array.Add(p);
	ret.SetParent(this);
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
	CheckStructSync();
	return m_struct;
}

void
SOAPParameter::CheckStructSync() const
{
	if (m_outtasync)
	{
		m_struct.Clear();
		for (Array::ConstIterator i = m_array.Begin(); i != m_array.End(); ++i)
		{
			m_struct[i->GetName().GetName()] = (SOAPParameter *)i;
		}
		m_outtasync = false;
	}
}

const SOAPQName xsitype("type", SOAP_XSI);
const SOAPQName xsinull("null", SOAP_XSI);
const SOAPQName arrayType("arrayType", SOAP_ENC);

bool
SOAPParameter::WriteSOAPPacket(SOAPPacketWriter& packet, bool writetype) const
{
	packet.StartTag(m_name);

	if (writetype)
		packet.AddAttr(xsitype, m_type);

	if (IsNull())
	{
		packet.AddAttr(xsinull, "1");
	}
	else if (IsArray())
	{
		SOAPQName atype;
		writetype = false;
		if (GetArray().Size() > 0)
		{
			atype = GetArray()[0].GetType();

			for (size_t i = 1; i < GetArray().Size(); ++i)
			{
				if (atype != GetArray()[i].GetType())
				{
					atype.Set("ur-type", SOAP_XSD);
					writetype = true;
					break;
				}
			}

		}
		else
		{
			// TODO: With no elements, we obviously can't discern the
			// type so... we probably should have a way for the user to
			// set the type to use...
			atype.Set("ur-type", SOAP_XSD);
		}

		char lenbuff[48];
		snprintf(lenbuff, sizeof(lenbuff), "[%d]", GetArray().Size());
		atype.GetName().Append(lenbuff);
		packet.AddAttr(arrayType, atype);

		for (size_t i = 0; i < GetArray().Size(); ++i)
			GetArray()[i].WriteSOAPPacket(packet, writetype);
	}
	else if (IsStruct())
	{
		for (Struct::Iterator i = GetStruct().Begin(); i != GetStruct().End(); ++i)
			(*i)->WriteSOAPPacket(packet);
	}
	else
	{
		packet.WriteValue(m_strval);
	}

	packet.EndTag(m_name);

	return true;
}



