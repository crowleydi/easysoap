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

struct SOAPInteropStruct
{
	SOAPInteropStruct()
	{
		varInt = 0;
		varFloat = 0.0;
	}

	SOAPInteropStruct(const char *str, int i, float f)
	{
		varString = str;
		varInt = i;
		varFloat = f;
	}

	bool operator==(const SOAPInteropStruct& other) const
	{
		return varInt == other.varInt &&
			varFloat == other.varFloat &&
			varString == other.varString;
	}

	bool operator!=(const SOAPInteropStruct& other) const
	{
		return varInt != other.varInt ||
			varFloat != other.varFloat ||
			varString != other.varString;
	}

	SOAPString	varString;
	int			varInt;
	float		varFloat;

	static const char *soap_name;
	static const char *soap_namespace;
};
const char *SOAPInteropStruct::soap_name = "SOAPStruct";
const char *SOAPInteropStruct::soap_namespace = "http://soapinterop.org/xsd";


//
//  Define how we serialize the struct
inline SOAPParameter&
operator<<(SOAPParameter& param, const SOAPInteropStruct& val)
{
	param.SetType(val.soap_name, val.soap_namespace);
	param.SetIsStruct();

	param.AddParameter("varString") << val.varString;
	param.AddParameter("varInt") << val.varInt;
	param.AddParameter("varFloat") << val.varFloat;

	return param;
}

//
// Define how we de-serialize the struct
inline const SOAPParameter&
operator>>(const SOAPParameter& param, SOAPInteropStruct& val)
{
	// We should probably confirm the types are
	// correct...
	param.GetParameter("varString") >> val.varString;
	param.GetParameter("varInt") >> val.varInt;
	param.GetParameter("varFloat") >> val.varFloat;

	return param;
}


