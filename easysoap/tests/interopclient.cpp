
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
// with EasySoap++.  For more information on interop
// testing, see:
//
//    http://www.xmethods.net/ilab/ilab.html
//

#ifdef _WIN32
#pragma warning (disable: 4786)
#endif // _WIN32

#include <iostream>
#include <math.h>
#include <SOAP.h>
#include <SOAPDebugger.h>

#include "interopstruct.h"

const char *httpproxy = 0; // "http://localhost:8080";
const char *default_interop_namespace = "http://soapinterop.org/";
const char *default_interop_soapaction = "urn:soapinterop";

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

inline std::ostream&
operator<<(std::ostream& os, const SOAPQName& name)
{
	return os << name.GetName() << "[" << name.GetNamespace() << "]";
}

void
SetTraceFile(const char *server, const char *test)
{
	char buffer[256];
	snprintf(buffer, sizeof(buffer), "%s/%s.txt", server, test);
	SOAPDebugger::SetFile(buffer);
}

//
// The struct used to hold endpoint information
//

struct Endpoint
{
	SOAPString name;
	SOAPString wsdl;
	SOAPString endpoint;
	SOAPString soapaction;
	bool	   needsappend;
	SOAPString nspace;
};

const SOAPParameter&
operator>>(const SOAPParameter& param, Endpoint& e)
{
	param.GetParameter("name") >> e.name;
	param.GetParameter("wsdl") >> e.wsdl;
	param.GetParameter("endpoint") >> e.endpoint;
	param.GetParameter("soapaction") >> e.soapaction;
	e.needsappend = (param.GetParameter("soapactionNeedsMethod").GetInt() != 0);
	param.GetParameter("methodNamespace") >> e.nspace;

	return param;
}

void
GetAllEndpoints(SOAPArray<Endpoint>& e)
{
	SOAPProxy proxy("http://www.xmethods.net/perl/soaplite.cgi", httpproxy);
	SOAPMethod getAllEndpoints("getAllEndpoints",
		"http://soapinterop.org/ilab",
		"http://soapinterop.org/ilab#", true);

	const SOAPResponse& response = proxy.Execute(getAllEndpoints);
	const SOAPParameter& p = response.GetReturnValue();

	p >> e;
}

bool
almostequal(float a, float b)
{
	return (fabs(a - b) <= fabs(a) * 0.0000005);
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
			if (!almostequal(a[i], b[i]))
			{
				retval = false;
			}
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
TestEchoInteger(SOAPProxy& proxy,
			const char *uri,
			const char *soapAction,
			bool appendMethod, const char *value)
{
	try
	{
		SOAPMethod method("echoInteger", uri, soapAction, appendMethod);
		SOAPParameter& inputParam = method.AddParameter("inputInteger");
		inputParam.SetFloat(value);

		std::cout << "Testing " << method.GetName() << ": ";

		const SOAPResponse& response = proxy.Execute(method);
		const SOAPParameter& outputParam = response.GetReturnValue();

		std::cout << "PASS: input=" << inputParam.GetString()
			<<" output=" << outputParam.GetString() << std::endl;

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

		const char *exact = 0;
		if (inputValue == outputValue)
			exact = "exact";
		else if (almostequal(inputValue, outputValue))
			exact = "inexact";
		else
			throw SOAPException("Values are not equal: %s != %s",
				(const char *)inputParam.GetString(),
				(const char *)outputParam.GetString());

		std::cout << "PASS " << exact << ": (in=" << inputParam.GetString()
			<< ", out=" << outputParam.GetString() << ")" << std::endl;
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
		SOAPMethod method("echoFloat", uri, soapAction, appendMethod);
		SOAPParameter& inputParam = method.AddParameter("inputFloat");
		inputParam.SetFloat(value);

		std::cout << "Testing " << method.GetName() << ": ";

		const SOAPResponse& response = proxy.Execute(method);
		const SOAPParameter& outputParam = response.GetReturnValue();

		std::cout << "PASS: input=" << inputParam.GetString()
			<< " output=" << outputParam.GetString() << std::endl;
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


void TestInterop(const Endpoint& e)
{
	const char *name = e.name;
	const char *endpoint = e.endpoint;
	const char *soapAction = e.soapaction;
	bool appendMethod = e.needsappend;
	const char *uri = e.nspace;

	std::cout << "Testing " << name << " interopability." << std::endl;

	SOAPProxy proxy(endpoint, httpproxy);

#if 1
	SetTraceFile(name, "echoVoid");
	TestEchoVoid(proxy, uri, soapAction, appendMethod);

	SetTraceFile(name, "echoInteger");
	TestEchoInteger(proxy, uri, soapAction, appendMethod, 464);
	SetTraceFile(name, "echoFloat");
	TestEchoFloat(proxy, uri, soapAction, appendMethod, (float)1.34523452345346754753453);
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
	SetTraceFile(name, "echoString_Null");
	TestEchoString(proxy, uri, soapAction, appendMethod, 0);
	SetTraceFile(name, "echoInteger_Null");
	TestEchoInteger(proxy, uri, soapAction, appendMethod, (const char *)0);
	SetTraceFile(name, "echoFloat_Null");
	TestEchoFloat(proxy, uri, soapAction, appendMethod, (const char *)0);

	SetTraceFile(name, "echoInteger_Overflow");
	TestEchoIntegerInvalid(proxy, uri, soapAction, appendMethod, "2147483648");
	SetTraceFile(name, "echoInteger_Underflow");
	TestEchoIntegerInvalid(proxy, uri, soapAction, appendMethod, "-2147483649");

	SetTraceFile(name, "echoFloat_NaN");
	TestEchoFloat(proxy, uri, soapAction, appendMethod, "NaN");
	SetTraceFile(name, "echoFloat_INF");
	TestEchoFloat(proxy, uri, soapAction, appendMethod, HUGE_VAL);
	SetTraceFile(name, "echoFloat_nINF");
	TestEchoFloat(proxy, uri, soapAction, appendMethod, -HUGE_VAL);
	SetTraceFile(name, "echoFloat_pINF");
	TestEchoFloat(proxy, uri, soapAction, appendMethod, "+INF");

	SetTraceFile(name, "echoFloat_n0");
	TestEchoFloat(proxy, uri, soapAction, appendMethod, "-0.0");
#endif
	SOAPDebugger::Close();

	std::cout << "Done." << std::endl << std::endl;
}

int
main(int argc, char* argv[])
{
	int ret = 0;
	bool testlocal = true;

	try
	{
		for (int i = 1; i < argc; ++i)
		{
			if (sp_strcmp(argv[i], "-a"))
			{
				SOAPArray<Endpoint> endpoints;
				GetAllEndpoints(endpoints);

				for (size_t i = 0; i < endpoints.Size(); ++i)
					TestInterop(endpoints[i]);
			}
			else if (argv[i][0] == '-')
			{
				throw SOAPException("Unknown commandline argument: %s", argv[i]);
			}
			else
			{
				testlocal = false;
				Endpoint e;
				e.name = argv[i];
				e.endpoint = argv[i];
				e.nspace = default_interop_namespace;
				e.soapaction = default_interop_soapaction;
				e.needsappend = false;
				TestInterop(e);
			}
		}

		if (testlocal)
		{
			// Just test against localhost
			Endpoint e;
			e.name = "localhost";
			e.endpoint = "http://localhost:80/cgi-bin/interopserver";
			e.nspace = default_interop_namespace;
			e.soapaction = default_interop_soapaction;
			e.needsappend = false;
			TestInterop(e);
		}
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

