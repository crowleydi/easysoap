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

#include "SOAP.h"
#include "SOAPBase64.h"
#include "SOAPNamespaces.h"

class SOAPParameterHandler;

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

	const SOAPString& GetType() const;
	const SOAPString& GetTypeNamespace() const;
	void SetType(const char *type, const char *ns = 0);

	void SetValue(bool val);
	void SetBoolean(const char *val);
	void SetValue(int val);
	void SetInt(const char *val);
	void SetValue(float val);
	void SetFloat(const char *val);
	void SetValue(double val);
	void SetDouble(const char *val);

	void SetValue(const char *val);
	bool GetBoolean() const;

	int GetInt() const;
	operator int() const					{return GetInt();}

	float GetFloat() const;
	operator float() const					{return GetFloat();}

	double GetDouble() const;
	operator double() const					{return GetDouble();}

	SOAPString& GetString()					{return m_strval;}
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

	Struct& GetStruct();
	const Struct& GetStruct() const;

	SOAPParameter& AddParameter(const char *name = "item");
	const SOAPParameter *GetParameter(const char *name) const;

	bool WriteSOAPPacket(SOAPPacketWriter& packet) const;

	void SetIsArray();
	void SetIsStruct();
	void SetNull(bool isnull = true);
	bool IsNull() const;
	bool IsStruct() const;
	bool IsArray() const;

private:
	void SetParent(SOAPParameter *parent) {m_parent = parent;}
	void Assign(const SOAPParameter&);
	void CheckStructSync() const;

	friend class SOAPParameterHandler;

	SOAPParameter	*m_parent;
	SOAPString		m_name;
	SOAPString		m_type;
	SOAPString		m_typens;

	int				m_flags;

	SOAPString		m_strval;
	Array			m_array;
	mutable Struct	m_struct;
	mutable bool	m_outtasync;

	static unsigned int		m_gensym;
};

inline SOAPParameter&
operator<<(SOAPParameter& param, bool val)
{
	param.SetValue(val);
	return param;
}

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
operator>>(const SOAPParameter& param, bool& val)
{
	val = param.GetBoolean();
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

template<typename T>
inline const SOAPParameter&
operator<<(SOAPParameter& param, const SOAPArray<T>& val)
{
	param.SetIsArray();
	for (SOAPArray<T>::ConstIterator i = val.Begin(); i != val.End(); ++i)
		param.AddParameter() << *i;
	return param;
}

template <typename T>
inline const SOAPParameter&
operator>>(const SOAPParameter& param, SOAPArray<T>& val)
{
	val.Resize(0);
	for (SOAPParameter::Array::ConstIterator i = param.GetArray().Begin();
		i != param.GetArray().End(); ++i)
		*i >> val.Add();
	return param;
}


inline SOAPParameter&
operator<<(SOAPParameter& param, short val)
{
	param.SetValue((int)val);
	param.SetType("short");
	return param;
}

inline const SOAPParameter&
operator>>(const SOAPParameter& param, short& val)
{
	val = param.GetInt();
	return param;
}

inline SOAPParameter&
operator<<(SOAPParameter& param, char val)
{
	param.SetValue((int)val);
	param.SetType("byte");
	return param;
}

inline const SOAPParameter&
operator>>(const SOAPParameter& param, char& val)
{
	val = param.GetInt();
	return param;
}

//
// Specialize to automatically use base64 encoding
inline const SOAPParameter&
operator<<(SOAPParameter& param, const SOAPArray<char>& val)
{
	param.SetValue(""); // coerce it to a string
	SOAPBase64::Encode(val, param.GetString());
	param.SetType("base64", SOAP_ENC);
	// Or, you could use this
	// param.SetType("base64Binary", SOAP_XSD_2001);
	return param;
}

//
// Specialize to automatically convert base64 encoding
inline const SOAPParameter&
operator>>(const SOAPParameter& param, SOAPArray<char>& val)
{
	val.Resize(0);
	if (param.IsArray())
	{
		for (SOAPParameter::Array::ConstIterator i = param.GetArray().Begin();
			i != param.GetArray().End(); ++i)
			*i >> val.Add();
	}
	else
	{
		//
		// Could also check for Hex encoding.
		SOAPBase64::Decode(param.GetString(), val);
	}
	return param;
}

#endif // !defined(AFX_SOAPPARAMETER_H__30811BAD_D6A1_4535_B256_9EEB56A84026__INCLUDED_)

