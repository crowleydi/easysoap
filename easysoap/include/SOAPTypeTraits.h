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


#if !defined(AFX_SOAPTYPETRAITS_H__C5FEAF2C_BF9D_4B2A_BA32_516712F68E78__INCLUDED_)
#define AFX_SOAPTYPETRAITS_H__C5FEAF2C_BF9D_4B2A_BA32_516712F68E78__INCLUDED_

template<typename T>
class SOAPTypeTraits;

class EASYSOAP_EXPORT SOAPTypeTraits<bool>
{
public:
	static void GetType(SOAPQName& type);
	static SOAPParameter& Serialize(SOAPParameter& param, bool val);
	static SOAPParameter& Serialize(SOAPParameter& param, const char *val);
	static const SOAPParameter& Deserialize(const SOAPParameter&, bool& val);
};

class EASYSOAP_EXPORT SOAPTypeTraits<char>
{
public:
	static void GetType(SOAPQName& type);
	static SOAPParameter& Serialize(SOAPParameter& param, char val);
	static const SOAPParameter& Deserialize(const SOAPParameter&, char& val);
};

class EASYSOAP_EXPORT SOAPTypeTraits<short>
{
public:
	static void GetType(SOAPQName& type);
	static SOAPParameter& Serialize(SOAPParameter& param, short val);
	static const SOAPParameter& Deserialize(const SOAPParameter&, short& val);
};

class EASYSOAP_EXPORT SOAPTypeTraits<int>
{
public:
	static void GetType(SOAPQName& type);
	static SOAPParameter& Serialize(SOAPParameter& param, int val);
	static SOAPParameter& Serialize(SOAPParameter& param, const char *val);
	static const SOAPParameter& Deserialize(const SOAPParameter&, int& val);
};

class EASYSOAP_EXPORT SOAPTypeTraits<long>
{
public:
	static void GetType(SOAPQName& type);
	static SOAPParameter& Serialize(SOAPParameter& param, long val);
	static const SOAPParameter& Deserialize(const SOAPParameter&, long& val);
};

class EASYSOAP_EXPORT SOAPTypeTraits<float>
{
public:
	static void GetType(SOAPQName& type);
	static SOAPParameter& Serialize(SOAPParameter& param, float val);
	static SOAPParameter& Serialize(SOAPParameter& param, const char *val);
	static const SOAPParameter& Deserialize(const SOAPParameter&, float& val);
};

class EASYSOAP_EXPORT SOAPTypeTraits<double>
{
public:
	static void GetType(SOAPQName& type);
	static SOAPParameter& Serialize(SOAPParameter& param, double val);
	static SOAPParameter& Serialize(SOAPParameter& param, const char *val);
	static const SOAPParameter& Deserialize(const SOAPParameter&, double& val);
};

class EASYSOAP_EXPORT SOAPTypeTraits<SOAPString>
{
public:
	static void GetType(SOAPQName& type);
	static SOAPParameter& Serialize(SOAPParameter& param, const SOAPString& val);
	static const SOAPParameter& Deserialize(const SOAPParameter&, SOAPString& val);
};

class EASYSOAP_EXPORT SOAPTypeTraits<const char *>
{
public:
	static void GetType(SOAPQName& type);
	static SOAPParameter& Serialize(SOAPParameter& param, const char *val);
	// Can't deserialize into const char *
	// and char * is just dangerous
};

#ifdef HAVE_WCHART
class EASYSOAP_EXPORT SOAPTypeTraits<const wchar_t *>
{
public:
	static void GetType(SOAPQName& type);
	static SOAPParameter& Serialize(SOAPParameter& param, const wchar_t *val);
	// Can't deserialize into const wchar_t *
	// and wchar_t * is just dangerous
};
#endif

class SOAPMapTypeTraits
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
		for (typename V::const_iterator i = val.begin(); i != val.end(); ++i)
		{
			SOAPParameter& p = param.AddParameter();
			p.SetIsStruct();
			p.AddParameter("key") << i.Key();
			p.AddParameter("value") <<i.Item();
		}
		return param;
	}

	template <typename V>
	static const SOAPParameter& Deserialize(const SOAPParameter& param, V& val)
	{
		typename V::key_type key;
		const SOAPParameter::Array& arr = param.GetArray();
		for (SOAPParameter::Array::ConstIterator i = arr.Begin(); i != arr.End(); ++i)
		{
			(*i)->GetParameter("key") >> key;
			(*i)->GetParameter("value") >> val[key];
		}
		return param;
	}
};

class EASYSOAP_EXPORT SOAPTypeTraits<SOAPBase64>
{
public:
	// Encode an array as base64
	static void GetType(SOAPQName& type);
	static SOAPParameter& Serialize(SOAPParameter& param, const SOAPBase64& val);
	static const SOAPParameter& Deserialize(const SOAPParameter& param, SOAPBase64& val);
};

class EASYSOAP_EXPORT SOAPTypeTraits<SOAPHex>
{
public:
	// Encode an array as hex
	static void GetType(SOAPQName& type);
	static SOAPParameter& Serialize(SOAPParameter& param, const SOAPHex& val);
	static const SOAPParameter& Deserialize(const SOAPParameter& param, SOAPHex& val);
};

class SOAPArrayTypeTraits
{
private:
	static void parsepos(const SOAPQName& attr, const SOAPQName& val, size_t& x)
	{
		//
		// arrayType should be a value like "xsd:string[2]"
		// We use strrchr() in case it's something like
		// "xsd:string[][2]" which is certainly possible...
		const char *s = sp_strrchr(val.GetName(), '[');
		if (*s++ == '[')
		{
			char *e = 0;
			x = strtol(s, &e, 10);
			if (*e == ']' && e[1] == 0)
				return;
		}
		throw SOAPException("Invalid value for array encoding tag '%s': %s",
			(const char *)attr.GetName(), (const char *)val.GetName());
	}

public:
	static void GetType(SOAPQName& type)
	{
		type = SOAPEnc::Array;
	}

	template <typename V>
	static SOAPParameter& Serialize(SOAPParameter& param, const V& val)
	{
		//
		// Add SOAP-ENC:arrayType attribute
		char buffer[32];

		SOAPQName& atype = param.AddAttribute(SOAPEnc::arrayType);
		SOAPTypeTraits<typename V::value_type>::GetType(atype);
		snprintf(buffer, sizeof(buffer), "[%d]", val.size());
		atype.GetName().Append(buffer);

		//
		// Serialize the array values
		param.SetIsStruct();
		for (typename V::const_iterator i = val.begin(); i != val.end(); ++i)
			SOAPTypeTraits<typename V::value_type>::Serialize(param.AddParameter(), *i);
		return param;
	}

	template <typename V>
	static const SOAPParameter& Deserialize(const SOAPParameter& param, V& val)
	{
		const SOAPParameter::Array& arr = param.GetArray();
		//
		// parse arrayType attribute
		SOAPParameter::Attrs::Iterator attr = param.GetAttributes().Find(SOAPEnc::arrayType);
		if (!attr)
			throw SOAPException("Cannot de-serialize array without arrayType attribute.");

		size_t numvals;
		parsepos(SOAPEnc::arrayType, *attr, numvals);

		size_t pos = 0;

		//
		// parse offset attribute if present
		bool haveoffset = false;
		attr = param.GetAttributes().Find(SOAPEnc::offset);
		if (attr)
		{
			parsepos(SOAPEnc::offset, *attr, pos);
			haveoffset = true;
		}

		if (arr.Size() > numvals)
				throw SOAPException("Error de-serializing array.  Too many values.");

		val.resize(numvals);
		for (SOAPParameter::Array::ConstIterator i = arr.Begin(); i != arr.End(); ++i)
		{
			//
			// parse position attribute if present
			attr = (*i)->GetAttributes().Find(SOAPEnc::position);
			if (attr)
			{
				if (haveoffset)
					throw SOAPException("Cannot de-serialize array with position attribute and offset attribute.");
				parsepos(SOAPEnc::position, *attr, pos);
			}
			SOAPTypeTraits<typename V::value_type>::Deserialize(**i, val[pos++]);
		}
		return param;
	}
};

class SOAPTypeTraits< SOAPArray<bool> > : public SOAPArrayTypeTraits
{
};

class SOAPTypeTraits< SOAPArray<short> > : public SOAPArrayTypeTraits
{
};

class SOAPTypeTraits< SOAPArray<int> > : public SOAPArrayTypeTraits
{
};

class SOAPTypeTraits< SOAPArray<long> > : public SOAPArrayTypeTraits
{
};

class SOAPTypeTraits< SOAPArray<float> > : public SOAPArrayTypeTraits
{
};

class SOAPTypeTraits< SOAPArray<double> > : public SOAPArrayTypeTraits
{
};

class SOAPTypeTraits< SOAPArray<SOAPString> > : public SOAPArrayTypeTraits
{
};

class SOAP2DArrayTypeTraits
{
private:
	static void parse2Dpos(const SOAPQName& attr, const SOAPQName& val, size_t& x, size_t& y)
	{
		//
		// arrayType should be a value like "xsd:string[2,3]"
		// We use strrchr() in case it's something like
		// "xsd:string[][2,3]" which is certainly possible...
		const char *s = sp_strrchr(val.GetName(), '[');
		if (*s++ == '[')
		{
			char *e = 0;
			x = strtol(s, &e, 10);
			if (*e++ == ',')
			{
				s = e;
				y = strtol(s, &e, 10);
				if (*e == ']' && e[1] == 0)
					return;
			}
		}
		throw SOAPException("Invalid 2D value for array encoding tag '%s': %s",
			(const char *)attr.GetName(), (const char *)val.GetName());
	}

public:
	static void GetType(SOAPQName& type)
	{
		type = SOAPEnc::Array;
	}

	template<typename V>
	static SOAPParameter& Serialize(SOAPParameter& param, const V& val)
	{
		//
		// Add SOAP-ENC:arrayType attribute
		char buffer[64];

		SOAPQName& atype = param.AddAttribute(SOAPEnc::arrayType);
		SOAPTypeTraits<typename V::value_type>::GetType(atype);
		snprintf(buffer, sizeof(buffer), "[%d,%d]", val.GetNumRows(), val.GetNumCols());
		atype.GetName().Append(buffer);

		//
		// Serialize the array values
		param.SetIsStruct();
		for (size_t r = 0; r < val.GetNumRows(); ++r)
			for (size_t c = 0; c < val.GetNumCols(); ++c)
				SOAPTypeTraits<typename V::value_type>::Serialize(param.AddParameter(), val[r][c]);
		return param;
	}

	template<typename V>
	static const SOAPParameter& Deserialize(const SOAPParameter& param, V& val)
	{
		const SOAPParameter::Array& arr = param.GetArray();

		//
		// parse arrayType attribute
		SOAPParameter::Attrs::Iterator attr = param.GetAttributes().Find(SOAPEnc::arrayType);
		if (!attr)
			throw SOAPException("Cannot de-serialize 2D-array without arrayType attribute.");

		size_t numrows;
		size_t numcols;
		parse2Dpos(SOAPEnc::arrayType, *attr, numrows, numcols);

		size_t row = 0;
		size_t col = 0;

		//
		// parse offset attribute if present
		bool haveoffset = false;
		attr = param.GetAttributes().Find(SOAPEnc::offset);
		if (attr)
		{
			parse2Dpos(SOAPEnc::offset, *attr, row, col);
			haveoffset = true;
		}

		if (arr.Size() > numrows * numcols)
				throw SOAPException("Error de-serializing 2D array.  Too many values.");

		val.resize(numrows, numcols);
		for (SOAPParameter::Array::ConstIterator i = arr.Begin(); i != arr.End(); ++i)
		{
			//
			// parse position attribute if present
			attr = (*i)->GetAttributes().Find(SOAPEnc::position);
			if (attr)
			{
				if (haveoffset)
					throw SOAPException("Cannot de-serialize array with position attribute and offset attribute.");
				parse2Dpos(SOAPEnc::position, *attr, row, col);
			}
			SOAPTypeTraits<typename V::value_type>::Deserialize(**i, val[row][col]);
			if (++col == numcols)
			{
				++row;
				col = 0;
			}
		}
		return param;
	}
};

class SOAPTypeTraits< SOAP2DArray<bool> > : public SOAP2DArrayTypeTraits
{
};

class SOAPTypeTraits< SOAP2DArray<char> > : public SOAP2DArrayTypeTraits
{
};

class SOAPTypeTraits< SOAP2DArray<int> > : public SOAP2DArrayTypeTraits
{
};

class SOAPTypeTraits< SOAP2DArray<long> > : public SOAP2DArrayTypeTraits
{
};

class SOAPTypeTraits< SOAP2DArray<float> > : public SOAP2DArrayTypeTraits
{
};

class SOAPTypeTraits< SOAP2DArray<double> > : public SOAP2DArrayTypeTraits
{
};

class SOAPTypeTraits< SOAP2DArray<SOAPString> > : public SOAP2DArrayTypeTraits
{
};


#endif // !defined(AFX_SOAPTYPETRAITS_H__C5FEAF2C_BF9D_4B2A_BA32_516712F68E78__INCLUDED_)
