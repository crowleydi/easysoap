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


#if !defined(AFX_SOAPPARAMETER_H__30811BAD_D6A1_4535_B256_9EEB56A84026__INCLUDED_)
#define AFX_SOAPPARAMETER_H__30811BAD_D6A1_4535_B256_9EEB56A84026__INCLUDED_

#ifdef _MSC_VER
#pragma warning (disable: 4786)
#endif // _MSC_VER

#include <stdlib.h>
#include "SOAP.h"

class EASYSOAP_EXPORT SOAPParameter
{
public:

	typedef SOAPArray<SOAPParameter>				Array;
	typedef SOAPHashMap<SOAPString, SOAPParameter*>	Struct;
	typedef SOAPHashMap<SOAPString, SOAPString>		Attrs;

	SOAPParameter();
	SOAPParameter(const SOAPParameter& param);
	virtual ~SOAPParameter();

	SOAPParameter& operator=(const SOAPParameter& param);


	void	Reset();

	void SetName(const char *name);
	const SOAPString& GetName() const
	{
		return m_name;
	}

	SOAPTypes::xsd_type GetType() const;
	const char *GetTypeString() const;
	void SetType(SOAPTypes::xsd_type type);
	void SetType(const char *type);
	void SetType(const char *type, const char *ns);

	void SetValue(int val);
	void SetInteger(const char *val);
	void SetValue(float val);
	void SetFloat(const char *val);
	void SetValue(double val);
	void SetDouble(const char *val);

	void SetValue(const char *val);
	void SetNull();

	int GetInt() const						{return atoi(m_strval);}
	operator int() const					{return GetInt();}

	float GetFloat() const					{return atof(m_strval);}
	operator float() const					{return GetFloat();}

	double GetDouble() const				{return atof(m_strval);}
	operator double() const					{return GetDouble();}

	//SOAPString& GetString()					{return m_strval;}
	const SOAPString& GetString() const		{return m_strval;}
	operator const SOAPString&() const		{return GetString();}

	Array& GetArray()
	{
		return m_array;
	}

	const Array& GetArray() const
	{
		return m_array;
	}

	Struct& GetStruct()
	{
		return m_struct;
	}

	const Struct& GetStruct() const
	{
		return m_struct;
	}

	SOAPParameter& AddParameter(const char *name = "item")
	{
		size_t size = GetArray().Size();
		GetArray().Resize(size + 1);
		SOAPParameter& ret = GetArray()[size];
		ret.SetParent(this);
		ret.SetName(name);
		return ret;
	}

	const SOAPParameter *GetParameter(const char *name) const;

	void SetBaseType(SOAPTypes::xsd_type type)
	{
		m_basetype = type;
	}

	Attrs& GetAttributes() {return m_attrs;}
	const Attrs& GetAttributes() const {return m_attrs;}

	bool WriteSOAPPacket(SOAPPacketWriter& packet) const;

private:
	void SetParent(SOAPParameter *parent) {m_parent = parent;}
	void Assign(const SOAPParameter&);

	SOAPParameter	*m_parent;
	Attrs			m_attrs;
	SOAPString		m_name;

	SOAPString		m_strval;
	Array			m_array;
	Struct			m_struct;
	SOAPTypes::xsd_type m_basetype;

	static unsigned int		m_gensym;
};

inline SOAPParameter&
operator<<(SOAPParameter& param, int val)
{
	param.SetValue(val);
	return param;
}

inline SOAPParameter&
operator<<(SOAPParameter& param, float val)
{
	param.SetValue(val);
	return param;
}

inline SOAPParameter&
operator<<(SOAPParameter& param, double val)
{
	param.SetValue(val);
	return param;
}

inline SOAPParameter&
operator<<(SOAPParameter& param, const char * val)
{
	param.SetValue(val);
	return param;
}

inline const SOAPParameter&
operator>>(const SOAPParameter& param, int& val)
{
	val = param.GetInt();
	return param;
}

inline const SOAPParameter&
operator>>(const SOAPParameter& param, double& val)
{
	val = param.GetDouble();
	return param;
}

inline const SOAPParameter&
operator>>(const SOAPParameter& param, float& val)
{
	val = param.GetFloat();
	return param;
}

inline const SOAPParameter&
operator>>(const SOAPParameter& param, SOAPString& val)
{
	val = param.GetString();
	return param;
}

#endif // !defined(AFX_SOAPPARAMETER_H__30811BAD_D6A1_4535_B256_9EEB56A84026__INCLUDED_)

