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


#if !defined(AFX_SOAPMETHOD_H__B3726AD6_5844_4059_8ECD_36A553A918AD__INCLUDED_)
#define AFX_SOAPMETHOD_H__B3726AD6_5844_4059_8ECD_36A553A918AD__INCLUDED_

#include "SOAP.h"
#include "SOAPParameter.h"

class EASYSOAP_EXPORT SOAPMethod
{
public:
	SOAPMethod(const char *name = 0, const char *ns = 0);
	virtual ~SOAPMethod();

	SOAPMethod& SetName(const char *name, const char *ns = 0)
	{
		if (name) m_name = name;
		if (ns) m_namespace = ns;
		return *this;
	}

	SOAPMethod& SetNamespace(const char *ns)
	{
		m_namespace = ns;
		return *this;
	}

	const SOAPString& GetName() const
	{
		return m_name;
	}

	const SOAPString& GetNamespace() const
	{
		return m_namespace;
	}

	void Reset();

	size_t GetNumParameters() const
	{
		return m_params.Size();
	}

	SOAPParameter& AddParameter()
	{
		size_t size = m_params.Size();
		m_params.Resize(size + 1);
		return m_params[size];
	}

	SOAPParameter& AddParameter(const char *name)
	{
		size_t size = m_params.Size();
		m_params.Resize(size + 1);
		m_params[size].SetName(name);
		return m_params[size];
	}

	SOAPParameter& AddParameter(const SOAPParameter& param)
	{
		return m_params.Add(param);
	}

	const SOAPParameter& GetParameter(size_t i) const
	{
		return m_params[i];
	}

	bool WriteSOAPPacket(SOAPPacketWriter& packet) const;
private:

	SOAPParameter::Array	m_params;
	SOAPString				m_name;
	SOAPString				m_namespace;
};

#endif // !defined(AFX_SOAPMETHOD_H__B3726AD6_5844_4059_8ECD_36A553A918AD__INCLUDED_)

