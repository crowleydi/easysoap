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


#if !defined(AFX_SOAPDATA_H__EF876A19_BB53_4DAD_B71D_C892E4D24D5A__INCLUDED_)
#define AFX_SOAPDATA_H__EF876A19_BB53_4DAD_B71D_C892E4D24D5A__INCLUDED_

#include "SOAP.h"
#include "SOAPArray.h"

class EASYSOAP_EXPORT SOAPData
{
public:
	SOAPData();
	SOAPData(const SOAPData& data);

	virtual ~SOAPData();

	SOAPData& operator=(const SOAPData& data);


	const SOAPString& GetType()	const
	{
		return m_type;
	}

	SOAPData& SetType(const char *type)
	{
		m_type = type;
		return *this;
	}

	const SOAPString& GetName() const
	{
		return m_name;
	}

	SOAPData& SetName(const char *name)
	{
		m_name = name;
		if (m_parent)
			m_parent->m_datamap[name] = this;
		return *this;
	}

	void SetValue(int val);
	void SetValue(float val);
	void SetValue(double val);
	void SetValue(const char *val);

	void SetInteger(const char *val);
	void SetFloat(const char *val);
	void SetDouble(const char *val);
	void SetString(const char *val);

	SOAPData& AddData(const char *name = 0);

private:

	SOAPData	*m_parent;
	SOAPString	m_name;
	SOAPString	m_type;
	SOAPString	m_value;

	SOAPHashMap<SOAPString, SOAPString>	m_attrs;
	SOAPArray<SOAPData>					m_data;
	SOAPHashMap<SOAPString, SOAPData *>	m_datamap;
};

#endif // !defined(AFX_SOAPDATA_H__EF876A19_BB53_4DAD_B71D_C892E4D24D5A__INCLUDED_)
