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
 *
 * $Id$
 */

#ifndef _SOAPSTL_H_
#define _SOAPSTL_H_

//
//
//  This file provides serialization mechanisms
//  for some of the common STL classes
//
//
//  Because many compilers (MSVC 6 included) do not
//  support partial template specialization, you
//  will have to make some simple class declarations
//  so the compiler can figure out how to serialze
//  and deserialize your arrays and maps.
//
//  Example for a std::vector of MyStruct:
//
//  class SOAPTypeTraits<std::vector<MyStruct> >
//   : public SOAPArrayTypeTrais
//   {
//   };
//
//  Example for a std::map:
//
//  class SOAPTypeTraits<std::map<std::string,MyStruct> >
//   : public SOAPSTLMapTypeTrais
//   {
//   };
//

#include <string>
#include <vector>
#include <map>

class SOAPTypeTraits<std::string>
{
public:
	static void GetType(SOAPQName& type)
	{
		type.Set("string", SOAP_XSD);
	}

	static SOAPParameter& Serialize(SOAPParameter& param, const std::string& val)
	{
		param.SetValue(val.c_str());
		return param;
	}

	static const SOAPParameter& Deserialize(const SOAPParameter& param, std::string& val)
	{
		val = (const char *)param.GetString();
		return param;
	}
};

class SOAPTypeTraits< std::vector<bool> > : public SOAPArrayTypeTraits
{
};

class SOAPTypeTraits< std::vector<short> > : public SOAPArrayTypeTraits
{
};

class SOAPTypeTraits< std::vector<int> > : public SOAPArrayTypeTraits
{
};

class SOAPTypeTraits< std::vector<long> > : public SOAPArrayTypeTraits
{
};

class SOAPTypeTraits< std::vector<float> > : public SOAPArrayTypeTraits
{
};

class SOAPTypeTraits< std::vector<double> > : public SOAPArrayTypeTraits
{
};

class SOAPTypeTraits< std::vector<std::string> > : public SOAPArrayTypeTraits
{
};

class SOAPSTLMapTypeTraits
{
public:
	static void GetType(SOAPQName& type)
	{
		type.Set("Map", "http://xml.apache.org/xml-soap");
	}

	template <typename V>
	static SOAPParameter& Serialize(SOAPParameter& param, const V& val)
	{
		param.SetIsStruct();
		for (MEMBER_TYPE(V::const_iterator) i = val.begin(); i != val.end(); ++i)
		{
			SOAPParameter& p = param.AddParameter();
			p.SetIsStruct();
			p.AddParameter("key") << i->first;
			p.AddParameter("value") << i->second;
		}
		return param;
	}

	template <typename V>
	static const SOAPParameter& Deserialize(const SOAPParameter& param, V& val)
	{
		MEMBER_TYPE(V::key_type) key;
		const SOAPParameter::Array& arr = param.GetArray();
		for (SOAPParameter::Array::ConstIterator i = arr.Begin(); i != arr.End(); ++i)
		{
			(*i)->GetParameter("key") >> key;
			(*i)->GetParameter("value") >> val[key];
		}
		return param;
	}
};

#endif // _SOAPSTL_H_
