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

#ifdef _WIN32
#pragma warning (disable: 4786)
#endif // _WIN32

#include <vector>
#include <string>
#include <map>

#include "SOAP.h"

class EASYSOAP_EXPORT SOAPParameter
{
public:

	SOAPParameter()					{init();}
	SOAPParameter(const SOAPParameter& param);
	virtual ~SOAPParameter();

	SOAPParameter& operator=(const SOAPParameter& param);

	typedef std::vector<SOAPParameter>				Array;
	typedef std::map<std::string, SOAPParameter>	Struct;

	SOAPTypes::xsd_type GetType() const				{return m_type;}
	void	Reset()									{SetType(SOAPTypes::xsd_none);}
	void	SetType(SOAPTypes::xsd_type type)		{m_type = type;}

	void	SetName(const char *name)	{m_name = name;}
	const std::string& GetName() const	{return m_name;}

	void SetValue(int val)			{m_intval = val; m_type = SOAPTypes::xsd_int;}
	void SetValue(float val)		{m_fltval = val; m_type = SOAPTypes::xsd_float;}
	void SetValue(double val)		{m_dblval = val; m_type = SOAPTypes::xsd_double;}
	void SetValue(const char *val)	{m_strval = val; m_type = SOAPTypes::xsd_string;}
	void SetNull()					{m_type = SOAPTypes::xsd_null;}

	void SetValue(const Array& val)
	{
		GetArray() = val;
		m_type = SOAPTypes::soap_array;
	}

	void SetValue(const Struct& val)
	{
		GetStruct() = val;
		m_type = SOAPTypes::soap_struct;
	}

	int GetInt() const						{return m_intval;}
	operator int() const					{return GetInt();}

	float GetFloat() const					{return m_fltval;}
	operator float() const					{return GetFloat();}

	double GetDouble() const				{return m_dblval;}
	operator double() const					{return GetDouble();}

	std::string& GetString()				{return m_strval;}
	const std::string& GetString() const	{return m_strval;}
	operator const std::string&() const		{return GetString();}

	Array& GetArray()
	{
		if (!m_array)
			m_array = new Array();
		return *m_array;
	}
	const Array& GetArray() const	{return *m_array;}

	Struct& GetStruct()
	{
		if (!m_struct)
			m_struct = new Struct();
		return *m_struct;
	}
	const Struct& GetStruct() const	{return *m_struct;}


	SOAPParameter& AddParameter()
	{
		m_type = SOAPTypes::soap_array;
		int size = GetArray().size();
		GetArray().resize(size + 1);
		return GetArray()[size];
	}

	SOAPParameter& AddParameter(const char *name)
	{
		m_type = SOAPTypes::soap_struct;
		SOAPParameter& param = GetStruct()[name];
		param.SetName(name);
		return param;
	}

	bool WriteSOAPPacket(SOAPPacketWriter& packet) const;

private:
	const char *GetXsdString() const;
	void init()
	{
		m_type = SOAPTypes::xsd_none;
		m_intval = 0;
		m_fltval = 0.0;
		m_dblval = 0.0;
		m_struct = 0;
		m_array = 0;
	}

	static unsigned int		m_gensym;

	std::string				m_name;
	SOAPTypes::xsd_type		m_type;

	int				m_intval;
	float			m_fltval;
	double			m_dblval;
	std::string		m_strval;
	Array			*m_array;
	Struct			*m_struct;
};

#endif // !defined(AFX_SOAPPARAMETER_H__30811BAD_D6A1_4535_B256_9EEB56A84026__INCLUDED_)

