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

template<>
class EASYSOAP_EXPORT SOAPTypeTraits<bool>
{
public:
	void GetType(SOAPQName& type);
	void Serialize(SOAPParameter& param, bool val);
	void Serialize(SOAPParameter& param, const char *val);
	void Deserialize(const SOAPParameter&, bool& val);
};

template<>
class EASYSOAP_EXPORT SOAPTypeTraits<char>
{
public:
	void GetType(SOAPQName& type);
	void Serialize(SOAPParameter& param, char val);
	void Deserialize(const SOAPParameter&, char& val);
};

template<>
class EASYSOAP_EXPORT SOAPTypeTraits<unsigned char>
{
public:
	void GetType(SOAPQName& type);
	void Serialize(SOAPParameter& param, unsigned char val);
	void Deserialize(const SOAPParameter&, unsigned char& val);
};

template<>
class EASYSOAP_EXPORT SOAPTypeTraits<short>
{
public:
	void GetType(SOAPQName& type);
	void Serialize(SOAPParameter& param, short val);
	void Deserialize(const SOAPParameter&, short& val);
};

template<>
class EASYSOAP_EXPORT SOAPTypeTraits<int>
{
public:
	void GetType(SOAPQName& type);
	void Serialize(SOAPParameter& param, int val);
	void Serialize(SOAPParameter& param, const char *val);
	void Deserialize(const SOAPParameter&, int& val);
};

template<>
class EASYSOAP_EXPORT SOAPTypeTraits<float>
{
public:
	void GetType(SOAPQName& type);
	void Serialize(SOAPParameter& param, float val);
	void Serialize(SOAPParameter& param, const char *val);
	void Deserialize(const SOAPParameter&, float& val);
};

template<>
class EASYSOAP_EXPORT SOAPTypeTraits<double>
{
public:
	void GetType(SOAPQName& type);
	void Serialize(SOAPParameter& param, double val);
	void Serialize(SOAPParameter& param, const char *val);
	void Deserialize(const SOAPParameter&, double& val);
};

template<>
class EASYSOAP_EXPORT SOAPTypeTraits<SOAPString>
{
public:
	void GetType(SOAPQName& type);
	void Serialize(SOAPParameter& param, const SOAPString& val);
	void Deserialize(const SOAPParameter&, SOAPString& val);
};

template<>
class EASYSOAP_EXPORT SOAPTypeTraits<const char *>
{
public:
	void GetType(SOAPQName& type);
	void Serialize(SOAPParameter& param, const char *val);
	// Deserialize is dangerous...
};

#ifdef HAVE_WCHART
template<>
class EASYSOAP_EXPORT SOAPTypeTraits<const wchar_t *>
{
public:
	void GetType(SOAPQName& type);
	void Serialize(SOAPParameter& param, const wchar_t *val);
	// Deserialize is dangerous...
};
#endif

class SOAPMapTypeTraits
{
public:
	void GetType(SOAPQName& type)
	{
		type.Set("Map", "http://xml.apache.org/xml-soap");
	}

	template<typename K, typename I, typename H, typename E>
	void Serialize(SOAPParameter& param, const SOAPHashMap<K,I,H,E>& val)
	{
		param.SetIsStruct();
		for (SOAPHashMap<K,I,H,E>::Iterator i = val.Begin(); i != val.End(); ++i)
		{
			SOAPParameter& p = param.AddParameter();
			p.SetIsStruct();
			p.AddParameter("key") << i.Key();
			p.AddParameter("value") <<i.Item();
		}
	}

	template<typename K, typename I, typename H, typename E>
	void Deserialize(const SOAPParameter& param, SOAPHashMap<K,I,H,E>& val)
	{
		K key;
		const SOAPParameter::Array& arr = param.GetArray();
		for (SOAPParameter::Array::ConstIterator i = arr.Begin(); i != arr.End(); ++i)
		{
			(*i)->GetParameter("key") >> key;
			(*i)->GetParameter("value") >> val[key];
		}
	}
};

class EASYSOAP_EXPORT SOAPTypeTraits<SOAPBase64>
{
public:
	// Encode an array as base64
	void GetType(SOAPQName& type);
	void Serialize(SOAPParameter& param, const SOAPBase64& val);
	void Deserialize(const SOAPParameter& param, SOAPBase64& val);
};

class EASYSOAP_EXPORT SOAPTypeTraits<SOAPHex>
{
public:
	// Encode an array as hex
	void GetType(SOAPQName& type);
	void Serialize(SOAPParameter& param, const SOAPHex& val);
	void Deserialize(const SOAPParameter& param, SOAPHex& val);
};

class SOAPArrayTypeTraits
{
private:
	void parsepos(const SOAPQName& attr, const SOAPQName& val, size_t& x)
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
	void GetType(SOAPQName& type)
	{
		type = SOAPEnc::Array;
	}

	template<typename T>
	void Serialize(SOAPParameter& param, const SOAPArray<T>& val)
	{
		//
		// Add SOAP-ENC:arrayType attribute
		char buffer[32];
		SOAPTypeTraits<T> traits;

		SOAPQName& atype = param.AddAttribute(SOAPEnc::arrayType);
		traits.GetType(atype);
		snprintf(buffer, sizeof(buffer), "[%d]", val.Size());
		atype.GetName().Append(buffer);

		//
		// Serialize the array values
		param.SetIsStruct();
		for (SOAPArray<T>::ConstIterator i = val.Begin(); i != val.End(); ++i)
			traits.Serialize(param.AddParameter(), *i);
	}

	template<typename T>
	void Deserialize(const SOAPParameter& param, SOAPArray<T>& val)
	{
		SOAPTypeTraits<T> traits;
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

		val.Resize(numvals);
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
			traits.Deserialize(**i, val[pos++]);
		}
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
	void parse2Dpos(const SOAPQName& attr, const SOAPQName& val, size_t& x, size_t& y)
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
	void GetType(SOAPQName& type)
	{
		type = SOAPEnc::Array;
	}

	template<typename T>
	void Serialize(SOAPParameter& param, const SOAP2DArray<T>& val)
	{
		//
		// Add SOAP-ENC:arrayType attribute
		char buffer[64];
		SOAPTypeTraits<T> traits;

		SOAPQName& atype = param.AddAttribute(SOAPEnc::arrayType);
		traits.GetType(atype);
		snprintf(buffer, sizeof(buffer), "[%d,%d]", val.GetNumRows(), val.GetNumCols());
		atype.GetName().Append(buffer);

		//
		// Serialize the array values
		param.SetIsStruct();
		for (size_t r = 0; r < val.GetNumRows(); ++r)
			for (size_t c = 0; c < val.GetNumCols(); ++c)
				traits.Serialize(param.AddParameter(), val[r][c]);
	}

	template<typename T>
	void Deserialize(const SOAPParameter& param, SOAP2DArray<T>& val)
	{
		SOAPTypeTraits<T> traits;
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

		val.Resize(numrows, numcols);
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
			traits.Deserialize(**i, val[row][col]);
			if (++col == numcols)
			{
				++row;
				col = 0;
			}
		}
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
