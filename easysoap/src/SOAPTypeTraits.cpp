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

#include <SOAP.h>
#include <SOAPParameter.h>

#ifdef STDC_HEADERS
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#endif

#ifdef HAVE_IEEEFP_H
#include <ieeefp.h>
#endif

#ifdef HAVE_FLOAT_H
#include <float.h>
#endif


const SOAPQName	xsitype("type", SOAP_XSI);

static int
sp_strtol(const char *str)
{
    char *endptr = 0;
    const char *startptr = str;
    int ret = strtol(startptr, &endptr, 10);

    if (endptr)
    {
        while (sp_isspace(*endptr))
            ++endptr;

        if (startptr == endptr || *endptr != 0)
            throw SOAPException("Could not convert string to integer: '%s'",
                    str);
    }

	if (errno == ERANGE)
		throw SOAPException("Integer %s: %s",
				(ret < 0) ? "underflow" : "overflow",
				str);

	return ret;
}


//
// TODO:  This is too tricky.  We need to find a
// portable and nice way to return NAN.
typedef union
{
	double d;
	struct
	{
		int i1;
		int i2;
	} s;
} fptricks;

static double
sp_strtod(const char *str)
{
	if (sp_strcasecmp(str, "INF") == 0)
		return HUGE_VAL;
	else if (sp_strcasecmp(str, "-INF") == 0)
		return -HUGE_VAL;
	else if (sp_strcasecmp(str, "NaN") == 0)
	{
		fptricks t;
		t.s.i1 = 0xFFFFFFFF;
		t.s.i2 = 0xFFFFFFFF;
		return t.d;
	}

	char *endptr = 0;
	const char *startptr = str;
	double ret = strtod(startptr, &endptr);

	if (endptr)
	{
		while (sp_isspace(*endptr))
			++endptr;

		if (startptr == endptr || *endptr != 0)
			throw SOAPException("Could not convert string to floating point: '%s'",
					startptr);
	}

	if (errno == ERANGE)
		throw SOAPException("Double floating-point %s: %s",
				(ret == 0.0) ? "underflow" : "overflow",
				(const char *)str);

	return ret;
}

//
//  Trait info for bool
void SOAPTypeTraits<bool>::GetType(SOAPQName& type)
{
	type.Set("boolean", SOAP_XSD);
}

void SOAPTypeTraits<bool>::Serialize(SOAPParameter& param, bool val)
{
	param.GetStringRef() = val ? "true" : "false";
}

void SOAPTypeTraits<bool>::Serialize(SOAPParameter& param, const char *val)
{
	if (!val)
		param.AddAttribute(XMLSchemaInstance::nil) = "true";
	param.GetStringRef() = val;
}

void SOAPTypeTraits<bool>::Deserialize(const SOAPParameter& param, bool& val)
{
	const SOAPString& str = param.GetString();
    if (param.IsStruct())
        throw SOAPException("Cannot convert a struct to a boolean.");

    if (param.IsNull() || str.IsEmpty())
        throw SOAPException("Cannot convert null value to a boolean.");

	if (str == "false" || str == "0")
		val = false;
	else if (str == "true" || str =="1")
		val = true;
	else
		throw SOAPException("Could not convert value to boolean: %s", (const char *)str);
}


//
//  Trait info for char
void SOAPTypeTraits<char>::GetType(SOAPQName& type)
{
	type.Set("byte", SOAP_XSD);
}

void SOAPTypeTraits<char>::Serialize(SOAPParameter& param, char val)
{
	char buffer[64];
	snprintf(buffer, sizeof(buffer), "%d", (signed char)val);
	param.GetStringRef() = buffer;
}

void SOAPTypeTraits<char>::Deserialize(const SOAPParameter& param, char& val)
{
	const SOAPString& str = param.GetString();
    if (param.IsStruct())
        throw SOAPException("Cannot convert a struct to a byte.");

    if (param.IsNull() || str.IsEmpty())
        throw SOAPException("Cannot convert null value to byte.");

	val = sp_strtol(str);
}


//
//  Trait info for short
void SOAPTypeTraits<short>::GetType(SOAPQName& type)
{
	type.Set("short", SOAP_XSD);
}

void SOAPTypeTraits<short>::Serialize(SOAPParameter& param, short val)
{
	char buffer[64];
	snprintf(buffer, sizeof(buffer), "%d", val);
	param.GetStringRef() = buffer;
}

void SOAPTypeTraits<short>::Deserialize(const SOAPParameter& param, short& val)
{
	const SOAPString& str = param.GetString();
    if (param.IsStruct())
        throw SOAPException("Cannot convert a struct to a short.");

    if (param.IsNull() || str.IsEmpty())
        throw SOAPException("Cannot convert null value to short.");

	val = sp_strtol(str);
}


//
//  Trait info for int
void SOAPTypeTraits<int>::GetType(SOAPQName& type)
{
	type.Set("int", SOAP_XSD);
}

void SOAPTypeTraits<int>::Serialize(SOAPParameter& param, int val)
{
	char buffer[64];
	snprintf(buffer, sizeof(buffer), "%d", val);
	Serialize(param, buffer);
}

void SOAPTypeTraits<int>::Serialize(SOAPParameter& param, const char *val)
{
	if (!val)
		param.AddAttribute(XMLSchemaInstance::nil) = "true";
	param.GetStringRef() = val;
}

void SOAPTypeTraits<int>::Deserialize(const SOAPParameter& param, int& val)
{
	const SOAPString& str = param.GetString();
    if (param.IsStruct())
        throw SOAPException("Cannot convert a struct to an integer.");

    if (param.IsNull() || str.IsEmpty())
        throw SOAPException("Cannot convert null value to integer.");

	val = sp_strtol(str);
}


//
//  Trait info for float
void SOAPTypeTraits<float>::GetType(SOAPQName& type)
{
	type.Set("float", SOAP_XSD);
}

void SOAPTypeTraits<float>::Serialize(SOAPParameter& param, float val)
{
    double dval = val;
    if (finite(dval))
    {
        char buffer[64];
        snprintf(buffer, sizeof(buffer), "%.9G", dval);
        Serialize(param, buffer);
    }
    else
    {
        if (isnan(dval))
            Serialize(param, "NaN");
        else if (dval > 0.0)
            Serialize(param, "INF");
        else
            Serialize(param, "-INF");
    }
}

void SOAPTypeTraits<float>::Serialize(SOAPParameter& param, const char *val)
{
	if (!val)
		param.AddAttribute(XMLSchemaInstance::nil) = "true";
	param.GetStringRef() = val;
}

void SOAPTypeTraits<float>::Deserialize(const SOAPParameter& param, float& val)
{
	if (param.IsStruct())
		throw SOAPException("Cannot convert a struct to a float.");

	if (param.IsNull())
		throw SOAPException("Cannot convert null value to float.");

	double dret = sp_strtod(param.GetStringRef());
	float ret = dret;

	if (ret == HUGE_VAL && dret != HUGE_VAL ||
        ret == -HUGE_VAL && dret != -HUGE_VAL)
        throw SOAPException("Floating-point overflow: %s", (const char *)param.GetStringRef());
    if (ret == 0.0 && dret != 0.0)
        throw SOAPException("Floating-point underflow: %s", (const char *)param.GetStringRef());

	val = ret;
}

//
//  Trait info for double

void SOAPTypeTraits<double>::GetType(SOAPQName& type)
{
	type.Set("double", SOAP_XSD);
}

void SOAPTypeTraits<double>::Serialize(SOAPParameter& param, double val)
{
    if (finite(val))
    {
        char buffer[64];
        snprintf(buffer, sizeof(buffer), "%.18G", val);
        Serialize(param, buffer);
    }
    else
    {
        if (isnan(val))
            Serialize(param, "NaN");
        else if (val > 0.0)
            Serialize(param, "+INF");
        else
            Serialize(param, "-INF");
    }
}

void SOAPTypeTraits<double>::Serialize(SOAPParameter& param, const char *val)
{
	if (!val)
		param.AddAttribute(XMLSchemaInstance::nil) = "true";
	param.GetStringRef() = val;
}

void SOAPTypeTraits<double>::Deserialize(const SOAPParameter& param, double& val)
{
	if (param.IsStruct())
		throw SOAPException("Cannot convert a struct to a double.");

	if (param.IsNull())
		throw SOAPException("Cannot convert null value to double.");

	val = sp_strtod(param.GetStringRef());
}

//
//  Trait info for const char *
void SOAPTypeTraits<const char *>::GetType(SOAPQName& type)
{
	type.Set("string", SOAP_XSD);
}

void SOAPTypeTraits<const char *>::Serialize(SOAPParameter& param, const char *val)
{
	if (!val)
		param.AddAttribute(XMLSchemaInstance::nil) = "true";
	param.GetStringRef() = val;
}


#ifdef HAVE_WCHART
//
//  Trait info for const wchar_t *
void SOAPTypeTraits<const wchar_t *>::GetType(SOAPQName& type)
{
	type.Set("string", SOAP_XSD);
}

void SOAPTypeTraits<const wchar_t *>::Serialize(SOAPParameter& param, const wchar_t * val)
{
	if (!val)
		param.AddAttribute(XMLSchemaInstance::nil) = "true";
	param.GetStringRef() = val;
}
#endif // HAVE_WCHART


//
//  Trait info for SOAPString
void SOAPTypeTraits<SOAPString>::GetType(SOAPQName& type)
{
	type.Set("string", SOAP_XSD);
}

void SOAPTypeTraits<SOAPString>::Serialize(SOAPParameter& param, const SOAPString& val)
{
	if (!val)
		param.AddAttribute(XMLSchemaInstance::nil) = "true";
	param.GetStringRef() = val;
}

void SOAPTypeTraits<SOAPString>::Deserialize(const SOAPParameter& param, SOAPString& val)
{
	if (param.IsNull())
		val = (const char *)0;
	else
		val = param.GetStringRef();
}

//
// Traits to base64 encode char arrays
void SOAPTypeTraits<SOAPBase64>::GetType(SOAPQName& type)
{
	type.Set("base64Binary", SOAP_XSD);
}

void SOAPTypeTraits<SOAPBase64>::Serialize(SOAPParameter& param, const SOAPBase64& val)
{
	const SOAPArray<char> *arr = val.m_arr ? val.m_arr : val.m_carr;
	SOAPBase64::Encode(*arr, param.GetStringRef());
}

void SOAPTypeTraits<SOAPBase64>::Deserialize(const SOAPParameter& param, SOAPBase64& val)
{
	SOAPBase64::Decode(param.GetStringRef(), *val.m_arr);
}


//
// Traits to hex encode char arrays
void SOAPTypeTraits<SOAPHex>::GetType(SOAPQName& type)
{
	type.Set("hexBinary", SOAP_XSD);
}

void SOAPTypeTraits<SOAPHex>::Serialize(SOAPParameter& param, const SOAPHex& val)
{
	const SOAPArray<char> *arr = val.m_arr ? val.m_arr : val.m_carr;
	SOAPHex::Encode(*arr, param.GetStringRef());
}

void SOAPTypeTraits<SOAPHex>::Deserialize(const SOAPParameter& param, SOAPHex& val)
{
	SOAPHex::Decode(param.GetStringRef(), *val.m_arr);
}


#if 0
template<typename K, typename I, typename H, typename E>
inline SOAPParameter&
operator<<(SOAPParameter& param, const SOAPHashMap<K,I,H,E>& hash)
{
    param.SetIsStruct();
    param.SetType("Map", "http://xml.apache.org/xml-soap");
    for (SOAPHashMap<K,I>::Iterator i = hash.Begin(); i != hash.End(); ++i)
    {
        SOAPParameter& p = param.AddParameter();
        p.SetIsStruct();
        param.SetType("MapItem", "http://xml.apache.org/xml-soap");
        p.AddParameter("key") << i.Key();
        p.AddParameter("value") <<i.Item();
    }
    return param;
}


template<typename K, typename I, typename H, typename E>
inline const SOAPParameter&
operator>>(const SOAPParameter& param, SOAPHashMap<K,I,H,E>& hash)
{
    K key;
    const SOAPParameter::Array& arr = param.GetArray();
    for (SOAPParameter::Array::ConstIterator i = arr.Begin(); i != arr.End(); ++i)
    {
        (*i)->GetParameter("key") >> key;
        (*i)->GetParameter("value") >> hash[key];
    }
    return param;
}
#endif
