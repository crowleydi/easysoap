
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
// This is a test harness for running interop tests
// with Easysoap++.  For more information on interop
// testing, see:
//
//    http://www.xmethods.net/ilab/ilab.html
//

#ifdef _WIN32
#pragma warning (disable: 4786)
#endif // _WIN32

#include <iostream>
#include <math.h>
#include "SOAP.h"
#include "SOAPDebugger.h"


//
// The main library doesn't include iostream,
// so we define an overload to write out
// a SOAPString here....
inline std::ostream&
operator<<(std::ostream& os, const SOAPString& str)
{
	const char *s = str;
	return os << (s ? s : "(null)");
}

void
SetTraceFile(const char *server, const char *test)
{
	char buffer[256];
	snprintf(buffer, sizeof(buffer), "%s\\%s.txt", server, test);
	SOAPDebugger::SetFile(buffer);
}

//
//  The struct used for interop tests
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

	static const char *soap_namespace;
};
const char *SOAPInteropStruct::soap_namespace = "http://soapinterop.org/xsd";


//
//  Define how we serialize the struct
SOAPParameter&
operator<<(SOAPParameter& param, const SOAPInteropStruct& val)
{
	param.SetType("SOAPStruct", SOAPInteropStruct::soap_namespace);
	param.SetIsStruct();

	param.AddParameter("varString") << val.varString;
	param.AddParameter("varInt") << val.varInt;
	param.AddParameter("varFloat") << val.varFloat;

	return param;
}

//
// Little helper function
const SOAPParameter&
SafeGetParameter(const SOAPParameter& param, const char *pname)
{
	const SOAPParameter *p = 0;
	if (!(p = param.GetParameter(pname)))
		throw SOAPException("Invalid struct, member '%s' is missing", pname);
	return *p;
}

//
// Define how we de-serialize the struct
const SOAPParameter&
operator>>(const SOAPParameter& param, SOAPInteropStruct& val)
{
	// We should probably confirm the type is
	// correct...
	SafeGetParameter(param, "varString") >> val.varString;
	SafeGetParameter(param, "varInt") >> val.varInt;
	SafeGetParameter(param, "varFloat") >> val.varFloat;

	return param;
}

bool
almostequal(float a, float b)
{
	return (fabs(a - b) <= fabs(a) * 0.0000001);
}

bool
almostequal(const SOAPArray<float>& a, const SOAPArray<float>& b)
{
	if (a.Size() != b.Size())
		return false;
	bool retval = true;
	for (size_t i = 0; i < a.Size(); ++i)
	{
		if (a[i] != b[i])
		{
			std::cout << "Checking almost equal "
				<< a[i] << " " << b[i];
			if (!almostequal(a[i], b[i]))
			{
				retval = false;
				std::cout << " NOPE, diff=" << (a[i] - b[i]) << std::endl;
			}
			else
				std::cout << " OKAY" << std::endl;
		}
	}
	return retval;
}

bool
TestEchoVoid(SOAPProxy& proxy,
			const char *uri,
			const char *soapAction,
			bool appendMethod)
{
	try
	{
		SOAPMethod method("echoVoid", uri, soapAction, appendMethod);
		std::cout << "Testing " << method.GetName() << ": ";

		const SOAPResponse& response = proxy.Execute(method);
		if (response.GetBody().GetMethod().GetNumParameters() != 0)
			throw SOAPException("Received unexpected return values.");

		std::cout << "PASS" << std::endl;
		return false;
	}
	catch (SOAPException& sex)
	{
		std::cout << "FAILED: " << sex.What() << std::endl;
	}
	catch (...)
	{
		std::cout << "FAILED (badly)" << std::endl;
	}
	return true;
}

bool
TestEchoString(SOAPProxy& proxy,
			const char *uri,
			const char *soapAction,
			bool appendMethod, const char *value)
{
	try
	{
		SOAPString inputValue = value;

		SOAPMethod method("echoString", uri, soapAction, appendMethod);
		method.AddParameter("inputString") << inputValue;

		std::cout << "Testing " << method.GetName() << ": ";

		const SOAPResponse& response = proxy.Execute(method);
		SOAPString outputValue;
		response.GetReturnValue() >> outputValue;
		if (inputValue != outputValue)
			throw SOAPException("Values are not equal");

		std::cout << "PASS" << std::endl;
		return false;
	}
	catch (SOAPException& sex)
	{
		std::cout << "FAILED: " << sex.What() << std::endl;
	}
	catch (...)
	{
		std::cout << "FAILED (badly)" << std::endl;
	}
	return true;
}

bool
TestEchoInteger(SOAPProxy& proxy,
			const char *uri,
			const char *soapAction,
			bool appendMethod, int value)
{
	try
	{
		int inputValue = value;

		SOAPMethod method("echoInteger", uri, soapAction, appendMethod);
		method.AddParameter("inputInteger") << inputValue;

		std::cout << "Testing " << method.GetName() << ": ";

		const SOAPResponse& response = proxy.Execute(method);
		int outputValue = 0;
		response.GetReturnValue() >> outputValue;
		if (inputValue != outputValue)
			throw SOAPException("Values are not equal");

		std::cout << "PASS" << std::endl;
		return false;
	}
	catch (SOAPException& sex)
	{
		std::cout << "FAILED: " << sex.What() << std::endl;
	}
	catch (...)
	{
		std::cout << "FAILED (badly)" << std::endl;
	}
	return true;
}

bool
TestEchoIntegerInvalid(SOAPProxy& proxy,
			const char *uri,
			const char *soapAction,
			bool appendMethod, const char *value)
{
	try
	{
		SOAPString outputValue;

		SOAPMethod method("echoInteger", uri, soapAction, appendMethod);
		method.AddParameter("inputInteger").SetInt(value);

		std::cout << "Testing " << method.GetName() << ": ";

		const SOAPResponse& response = proxy.Execute(method);
		response.GetReturnValue() >> outputValue;

		std::cout << "FAILED: " << outputValue << std::endl;
		return false;
	}
	catch (SOAPException& sex)
	{
		std::cout << "PASS: " << sex.What() << std::endl;
	}
	catch (...)
	{
		std::cout << "FAILED (badly)" << std::endl;
	}
	return true;
}

bool
TestEchoFloat(SOAPProxy& proxy,
			const char *uri,
			const char *soapAction,
			bool appendMethod, float value)
{
	try
	{
		float inputValue = value;

		SOAPMethod method("echoFloat", uri, soapAction, appendMethod);
		SOAPParameter& inputParam = method.AddParameter("inputFloat");
		inputParam << inputValue;

		std::cout << "Testing " << method.GetName() << ": ";

		const SOAPResponse& response = proxy.Execute(method);
		const SOAPParameter& outputParam = response.GetReturnValue();
		float outputValue = 0;
		outputParam >> outputValue;

		if (inputValue == outputValue)
			std::cout << "PASS exact" << std::endl;
		else if (almostequal(inputValue, outputValue))
			std::cout << "PASS inexact" << std::endl;
		else
			throw SOAPException("Values are not equal: %s != %s",
				(const char *)inputParam.GetString(),
				(const char *)outputParam.GetString());

		std::cout << "PASS: (in=" << inputParam.GetString() << ", out=" << outputParam.GetString() << ")" << std::endl;
		return false;
	}
	catch (SOAPException& sex)
	{
		std::cout << "FAILED: " << sex.What() << std::endl;
	}
	catch (...)
	{
		std::cout << "FAILED (badly)" << std::endl;
	}
	return true;
}

bool
TestEchoFloat(SOAPProxy& proxy,
			const char *uri,
			const char *soapAction,
			bool appendMethod, const char *value)
{
	try
	{
		SOAPString outputValue;

		SOAPMethod method("echoFloat", uri, soapAction, appendMethod);
		method.AddParameter("inputFloat").SetFloat(value);

		std::cout << "Testing " << method.GetName() << ": ";

		const SOAPResponse& response = proxy.Execute(method);
		response.GetReturnValue() >> outputValue;

		std::cout << "PASS: " << outputValue << std::endl;
		return false;
	}
	catch (SOAPException& sex)
	{
		std::cout << "FAILED: " << sex.What() << std::endl;
	}
	catch (...)
	{
		std::cout << "FAILED (badly)" << std::endl;
	}
	return true;
}

bool
TestEchoFloatInvalid(SOAPProxy& proxy,
			const char *uri,
			const char *soapAction,
			bool appendMethod, const char *value)
{
	try
	{
		SOAPString outputValue;

		SOAPMethod method("echoFloat", uri, soapAction, appendMethod);
		method.AddParameter("inputFloat").SetFloat(value);

		std::cout << "Testing " << method.GetName() << ": ";

		const SOAPResponse& response = proxy.Execute(method);
		response.GetReturnValue() >> outputValue;

		std::cout << "FAILED: " << outputValue << std::endl;
		return false;
	}
	catch (SOAPException& sex)
	{
		std::cout << "PASS: " << sex.What() << std::endl;
	}
	catch (...)
	{
		std::cout << "FAILED (badly)" << std::endl;
	}
	return true;
}

bool
TestEchoStruct(SOAPProxy& proxy,
			const char *uri,
			const char *soapAction,
			bool appendMethod)
{
	try
	{
		SOAPInteropStruct inputValue("This is a struct string.", 68, (float)25.2456);

		SOAPMethod method("echoStruct", uri, soapAction, appendMethod);
		method.AddParameter("inputStruct") << inputValue;

		std::cout << "Testing " << method.GetName() << ": ";

		const SOAPResponse& response = proxy.Execute(method);
		SOAPInteropStruct outputValue;
		response.GetReturnValue() >> outputValue;
		if (inputValue != outputValue)
			throw SOAPException("Values are not equal");

		std::cout << "PASS" << std::endl;
		return false;
	}
	catch (SOAPException& sex)
	{
		std::cout << "FAILED: " << sex.What() << std::endl;
	}
	catch (...)
	{
		std::cout << "FAILED (badly)" << std::endl;
	}
	return true;
}


bool
TestEchoIntegerArray(SOAPProxy& proxy,
			const char *uri,
			const char *soapAction,
			bool appendMethod)
{
	try
	{
		SOAPArray<int> inputValue;
		SOAPArray<int> outputValue;

		inputValue.Add(1);
		inputValue.Add(66);
		inputValue.Add(-73);
		inputValue.Add(927353);
		inputValue.Add(16);
		inputValue.Add(0);

		SOAPMethod method("echoIntegerArray", uri, soapAction, appendMethod);
		method.AddParameter("inputIntegerArray") << inputValue;

		std::cout << "Testing " << method.GetName() << ": ";

		const SOAPResponse& response = proxy.Execute(method);
		response.GetReturnValue() >> outputValue;
		if (inputValue != outputValue)
			throw SOAPException("Values are not equal");

		std::cout << "PASS" << std::endl;
		return false;
	}
	catch (SOAPException& sex)
	{
		std::cout << "FAILED: " << sex.What() << std::endl;
	}
	catch (...)
	{
		std::cout << "FAILED (badly)" << std::endl;
	}
	return true;
}


bool
TestEchoFloatArray(SOAPProxy& proxy,
			const char *uri,
			const char *soapAction,
			bool appendMethod)
{
	try
	{
		SOAPArray<float> inputValue;

		inputValue.Add(1.34523452345346754753453);
		inputValue.Add(6.65334598983465754583455);
		inputValue.Add(73.233453452324523453455);
		inputValue.Add(927324985793245335.235423e2);
		inputValue.Add(1.63245234532453452345e10);
		inputValue.Add(1.62345234523452345e-10);
		inputValue.Add(-1.5324523423453246e10);
		inputValue.Add(-1.6234523452345345e-10);
		inputValue.Add(-0.1);
		inputValue.Add(0.1);

		SOAPMethod method("echoFloatArray", uri, soapAction, appendMethod);
		method.AddParameter("inputFloatArray") << inputValue;

		std::cout << "Testing " << method.GetName() << ": ";

		const SOAPResponse& response = proxy.Execute(method);

		SOAPArray<float> outputValue;
		response.GetReturnValue() >> outputValue;

		if (inputValue == outputValue)
			std::cout << "PASS exact" << std::endl;
		else if (almostequal(inputValue, outputValue))
			std::cout << "PASS inexact" << std::endl;
		else
			throw SOAPException("Values are not equal");
		return false;
	}
	catch (SOAPException& sex)
	{
		std::cout << "FAILED: " << sex.What() << std::endl;
	}
	catch (...)
	{
		std::cout << "FAILED (badly)" << std::endl;
	}
	return true;
}


bool
TestEchoStringArray(SOAPProxy& proxy,
			const char *uri,
			const char *soapAction,
			bool appendMethod)
{
	try
	{
		SOAPArray<SOAPString> inputValue;

		inputValue.Add("String 1");
		inputValue.Add("String 2");
		inputValue.Add("Third String");
		inputValue.Add("A Fourth and last string.");

		SOAPMethod method("echoStringArray", uri, soapAction, appendMethod);
		method.AddParameter("inputStringArray") << inputValue;

		std::cout << "Testing " << method.GetName() << ": ";

		const SOAPResponse& response = proxy.Execute(method);
		SOAPArray<SOAPString> outputValue;
		response.GetReturnValue() >> outputValue;
		if (inputValue != outputValue)
			throw SOAPException("Values are not equal");

		std::cout << "PASS" << std::endl;
		return false;
	}
	catch (SOAPException& sex)
	{
		std::cout << "FAILED: " << sex.What() << std::endl;
	}
	catch (...)
	{
		std::cout << "FAILED (badly)" << std::endl;
	}
	return true;
}


bool
TestEchoStructArray(SOAPProxy& proxy,
			const char *uri,
			const char *soapAction,
			bool appendMethod)
{
	try
	{
		SOAPArray<SOAPInteropStruct> inputValue;

		SOAPInteropStruct& val1 = inputValue.Add();
		SOAPInteropStruct& val2 = inputValue.Add();
		SOAPInteropStruct& val3 = inputValue.Add();

		val1.varFloat = (float)46.346;
		val1.varInt = -2352;
		val1.varString = "Array struct string 1.";

		val2.varFloat = (float)2.2;
		val2.varInt = -233752;
		val2.varString = "Array struct string 2.";

		val3.varFloat = (float)26.345e+20;
		val3.varInt = 523552;
		val3.varString = "Array struct string 3.";

		SOAPMethod method("echoStructArray", uri, soapAction, appendMethod);
		method.AddParameter("inputStructArray") << inputValue;

		std::cout << "Testing " << method.GetName() << ": ";

		const SOAPResponse& response = proxy.Execute(method);
		SOAPArray<SOAPInteropStruct> outputValue;
		response.GetReturnValue() >> outputValue;
		if (inputValue != outputValue)
			throw SOAPException("Values are not equal");

		std::cout << "PASS" << std::endl;
		return false;
	}
	catch (SOAPException& sex)
	{
		std::cout << "FAILED: " << sex.What() << std::endl;
	}
	catch (...)
	{
		std::cout << "FAILED (badly)" << std::endl;
	}
	return true;
}


void TestInterop(const char *name,
				 const char *endpoint,
				 const char *soapAction,
				 bool appendMethod,
				 const char *uri)
{
	std::cout << "Testing " << name << " interopability." << std::endl;

	SOAPProxy proxy(endpoint);//, "http://localhost:8080");

#if 1
	SetTraceFile(name, "echoVoid");
	TestEchoVoid(proxy, uri, soapAction, appendMethod);

	SetTraceFile(name, "echoInteger");
	TestEchoInteger(proxy, uri, soapAction, appendMethod, 464);
	SetTraceFile(name, "echoFloat");
	TestEchoFloat(proxy, uri, soapAction, appendMethod, (float)-3.628134987338e-6);
	SetTraceFile(name, "echoString");
	TestEchoString(proxy, uri, soapAction, appendMethod, "This is a test string from EasySOAP++");
	SetTraceFile(name, "echoStruct");
	TestEchoStruct(proxy, uri, soapAction, appendMethod);

	SetTraceFile(name, "echoIntegerArray");
	TestEchoIntegerArray(proxy, uri, soapAction, appendMethod);
	SetTraceFile(name, "echoFloatArray");
	TestEchoFloatArray(proxy, uri, soapAction, appendMethod);
	SetTraceFile(name, "echoStringArray");
	TestEchoStringArray(proxy, uri, soapAction, appendMethod);
	SetTraceFile(name, "echoStructArray");
	TestEchoStructArray(proxy, uri, soapAction, appendMethod);

#else
	// Lets test some boundry cases...
	SetTraceFile(name, "echoInteger_Overflow");
	TestEchoIntegerInvalid(proxy, uri, soapAction, appendMethod, "2147483648");
	SetTraceFile(name, "echoInteger_Underflow");
	TestEchoIntegerInvalid(proxy, uri, soapAction, appendMethod, "-2147483649");
	SetTraceFile(name, "echoInteger_NaN");
	TestEchoFloat(proxy, uri, soapAction, appendMethod, "NaN");
	SetTraceFile(name, "echoInteger_INF");
	TestEchoFloat(proxy, uri, soapAction, appendMethod, HUGE_VAL);
	SetTraceFile(name, "echoInteger_nINF");
	TestEchoFloat(proxy, uri, soapAction, appendMethod, -HUGE_VAL);
	SetTraceFile(name, "echoInteger_pINF");
	TestEchoFloatInvalid(proxy, uri, soapAction, appendMethod, "+INF");
	SetTraceFile(name, "echoInteger_n0");
	TestEchoFloat(proxy, uri, soapAction, appendMethod, "-0.0");
#endif
	SOAPDebugger::Close();

	std::cout << "Done." << std::endl << std::endl;
}

int
main(int argc, char* argv[])
{
	int ret = 0;

	try
	{
		//
		// See: http://www.xmethods.net/ilab/ilab.html
		// This information changes frequently.
		//
		TestInterop("4s4c",
			"http://soap.4s4c.com/ilab/soap.asp",
			"urn:soapinterop", false,
			"http://soapinterop.org/");/**/

		TestInterop("Apache",
			"http://services.xmethods.net:8080/soap/servlet/rpcrouter",
			"urn:soapinterop", false,
			"http://soapinterop.org/");/**/

		TestInterop("Frontier",
			"http://www.soapware.org:80/xmethodsInterop",
			"/xmethodsInterop", false,
			"urn:xmethodsInterop");/**/

		TestInterop("Kafka",
			"http://www.vbxml.com/soapworkshop/services/kafka10/services/endpoint.asp?service=ilab",
			"urn:soapinterop", false,
			"http://soapinterop.org/");/**/

		/*TestInterop("MS SOAP Toolkit 2.0 (untyped)",
			"http://131.107.72.13/stk/InteropService.asp",
			"urn:soapinterop", false,
			"http://soapinterop.org/");/**/

		TestInterop("MSSOAPtk",
			"http://131.107.72.13/stk/InteropTypedService.asp",
			"urn:soapinterop", false,
			"http://soapinterop.org/");/**/

		/*TestInterop("Phalanx",
			"http://www.phalanxsys.com/interop/listener.asp",
			"urn:soapinterop", false,
			"http://soapinterop.org/");/**/

		TestInterop("SOAPRMI",
			"http://rainier.extreme.indiana.edu:1568",
			"urn:soapinterop", false,
			"http://soapinterop.org/");/**/

		TestInterop("SOAPLite",
			"http://services.soaplite.com/interop.cgi",
			"urn:soapinterop", false,
			"http://soapinterop.org/");/**/

		TestInterop("SQLData",
			"http://www.soapclient.com/interop/sqldatainterop.wsdl",
			"/soapinterop", false,
			"http://tempuri.org/message/");/**/

		/****** These have problems... ******/

		TestInterop("WhiteMesa",
			"http://services3.xmethods.net:8080/interop",
			"urn:soapinterop#", true,
			"http://soapinterop.org/");/**/

		/*TestInterop("MS .NET Beta 2 (untyped)",
			"http://131.107.72.13/test/simple.asmx",
			"http://soapinterop.org/", true,
			"http://soapinterop.org/");/**/

		TestInterop("MSdotNET",
			"http://131.107.72.13/test/typed.asmx",
			"http://soapinterop.org/", true,
			"http://soapinterop.org/");/**/
	}
	catch (const SOAPMemoryException&)
	{
		std::cout << "SOAP out of memory." << std::endl;
		ret = -1;
	}
	catch (const SOAPException& ex)
	{
		std::cout << "Caught SOAP exception: " << ex.What() << std::endl;
		ret = 1;
	}

	SOAPDebugger::Close();
	return ret;
}

