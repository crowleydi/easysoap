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

//
//  The struct used for interop tests and is shared by
//  the interopclient and interopserver programs.
//

struct SOAPStruct
{
	SOAPString	varString;
	int			varInt;
	float		varFloat;

	bool operator==(const SOAPStruct& other) const
	{
		return varInt == other.varInt &&
			varFloat == other.varFloat &&
			varString == other.varString;
	}

	bool operator!=(const SOAPStruct& other) const
	{
		return varInt != other.varInt ||
			varFloat != other.varFloat ||
			varString != other.varString;
	}

	static const char *soap_name;
	static const char *soap_namespace;
};

struct SOAPStructStruct
{
	SOAPString	varString;
	int			varInt;
	float		varFloat;
	SOAPStruct	varStruct;

	bool operator==(const SOAPStructStruct& other) const
	{
		return varString == other.varString &&
			varInt == other.varInt &&
			varFloat == other.varFloat &&
			varStruct == other.varStruct;
	}

	bool operator!=(const SOAPStructStruct& other) const
	{
		return varString != other.varString ||
			varInt != other.varInt ||
			varFloat != other.varFloat ||
			varStruct != other.varStruct;
	}

	static const char *soap_name;
	static const char *soap_namespace;
};

struct SOAPArrayStruct
{
	SOAPString				varString;
	int						varInt;
	float					varFloat;
	SOAPArray<SOAPString>	varArray;

	bool operator==(const SOAPArrayStruct& other) const
	{
		return varString == other.varString &&
			varInt == other.varInt &&
			varFloat == other.varFloat &&
			varArray == other.varArray;
	}

	bool operator!=(const SOAPArrayStruct& other) const
	{
		return varString != other.varString ||
			varInt != other.varInt ||
			varFloat != other.varFloat ||
			varArray != other.varArray;
	}

	static const char *soap_name;
	static const char *soap_namespace;
};

const char *SOAPStruct::soap_name = "SOAPStruct";
const char *SOAPStruct::soap_namespace = "http://soapinterop.org/xsd";
const char *SOAPStructStruct::soap_name = "SOAPStructStruct";
const char *SOAPStructStruct::soap_namespace = "http://soapinterop.org/xsd";
const char *SOAPArrayStruct::soap_name = "SOAPArrayStruct";
const char *SOAPArrayStruct::soap_namespace = "http://soapinterop.org/xsd";


//
//  Define how we serialize the structs
inline SOAPParameter&
operator<<(SOAPParameter& param, const SOAPStruct& val)
{
	param.SetType(val.soap_name, val.soap_namespace);
	param.SetIsStruct();

	param.AddParameter("varString") << val.varString;
	param.AddParameter("varInt") << val.varInt;
	param.AddParameter("varFloat") << val.varFloat;

	return param;
}

inline SOAPParameter&
operator<<(SOAPParameter& param, const SOAPStructStruct& val)
{
	param.SetType(val.soap_name, val.soap_namespace);
	param.SetIsStruct();

	param.AddParameter("varString") << val.varString;
	param.AddParameter("varInt") << val.varInt;
	param.AddParameter("varFloat") << val.varFloat;
	param.AddParameter("varStruct") << val.varStruct;

	return param;
}

inline SOAPParameter&
operator<<(SOAPParameter& param, const SOAPArrayStruct& val)
{
	param.SetType(val.soap_name, val.soap_namespace);
	param.SetIsStruct();

	param.AddParameter("varString") << val.varString;
	param.AddParameter("varInt") << val.varInt;
	param.AddParameter("varFloat") << val.varFloat;
	param.AddParameter("varArray") << val.varArray;

	return param;
}

//
// Define how we de-serialize the structs
inline const SOAPParameter&
operator>>(const SOAPParameter& param, SOAPStruct& val)
{
	param.GetParameter("varString") >> val.varString;
	param.GetParameter("varInt") >> val.varInt;
	param.GetParameter("varFloat") >> val.varFloat;

	return param;
}

inline const SOAPParameter&
operator>>(const SOAPParameter& param, SOAPStructStruct& val)
{
	param.GetParameter("varString") >> val.varString;
	param.GetParameter("varInt") >> val.varInt;
	param.GetParameter("varFloat") >> val.varFloat;
	param.GetParameter("varStruct") >> val.varStruct;

	return param;
}

inline const SOAPParameter&
operator>>(const SOAPParameter& param, SOAPArrayStruct& val)
{
	param.GetParameter("varString") >> val.varString;
	param.GetParameter("varInt") >> val.varInt;
	param.GetParameter("varFloat") >> val.varFloat;
	param.GetParameter("varArray") >> val.varArray;

	return param;
}


