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
#include <algorithm>

#include <math.h>
#include <time.h>
#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#endif

#include <SOAP.h>
#include <SOAPDebugger.h>
#include <SOAPSocket.h>

#include "interopstruct.h"

const char *httpproxy = 0; // "http://localhost:8080";

const char *default_interop_namespace = "http://soapinterop.org/";
const char *default_interop_soapaction = "urn:soapinterop";
SOAPPacketWriter testresults;

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
	return os << name.GetName();
}

void
SetTraceFile(const char *server, const char *test)
{
	char buffer[256];
	snprintf(buffer, sizeof(buffer), "%s/%s.txt", server, test);
	//SOAPDebugger::SetFile(buffer);
}

double
randdouble()
{
	double f1 = rand();
	double f2 = rand();
	double f3 = (rand() % 40) - 20;
	if (f2 == 0.0)
		return 0.0;
	return f1/f2*pow(10.0, f3);
}

class FPLossException : public SOAPException
{
public:
	FPLossException(float a, float b)
		: SOAPException("Floating point loss: expecting %.9G, got %.9G", a, b)
	{
	}
};

bool
almostequal(float a, float b)
{
	if (fabs(a - b) <= fabs(a) * 0.0000005)
		throw FPLossException(a, b);
	return false;
}

bool
almostequal(const SOAPArray<float>& a, const SOAPArray<float>& b)
{
	if (a.Size() != b.Size())
		return false;
	bool retval = true;
	for (size_t i = 0; i < a.Size(); ++i)
	{
		if (a[i] != b[i] && !almostequal(a[i], b[i]))
		{
			retval = false;
		}
	}
	return retval;
}


bool
almostequal(const SOAPArray<SOAPInteropStruct>& a, const SOAPArray<SOAPInteropStruct>& b)
{
	if (a.Size() != b.Size())
		return false;
	bool retval = true;
	for (size_t i = 0; i < a.Size(); ++i)
	{
		if (a[i] != b[i] && !almostequal(a[i].varFloat, b[i].varFloat))
		{
			retval = false;
		}
	}
	return retval;
}


class UnexpectedSuccessException : public SOAPException
{
public:
	UnexpectedSuccessException(const char *fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		FormattedMessage(fmt, args);
		va_end(args);
	}
};

//
// The struct used to hold endpoint information
//

struct Endpoint
{
	SOAPString name;
	SOAPString wsdl;
	SOAPUrl    endpoint;
	SOAPString soapaction;
	bool	   needsappend;
	SOAPString nspace;

	bool operator<(const Endpoint& p) const
	{
		return name < p.name;
	}
};

const SOAPParameter&
operator>>(const SOAPParameter& param, Endpoint& e)
{
	SOAPString endpoint;
	param.GetParameter("name") >> e.name;
	param.GetParameter("wsdl") >> e.wsdl;
	param.GetParameter("endpoint") >> endpoint; e.endpoint = endpoint;
	param.GetParameter("soapaction") >> e.soapaction;
	e.needsappend = (param.GetParameter("soapactionNeedsMethod").GetInt() != 0);
	param.GetParameter("methodNamespace") >> e.nspace;

	// Try to fix wrong values.
	if (e.nspace == "http://soapinterop.org")
		e.nspace = default_interop_namespace;

	return param;
}

void
GetAllEndpoints(SOAPArray<Endpoint>& ea)
{
	SOAPProxy proxy("http://www.xmethods.net/perl/soaplite.cgi", httpproxy);
	SOAPMethod getAllEndpoints("getAllEndpoints",
		"http://soapinterop.org/ilab",
		"http://soapinterop.org/ilab#", true);

	const SOAPResponse& response = proxy.Execute(getAllEndpoints);
	const SOAPParameter& p = response.GetReturnValue();

	
	for (SOAPParameter::Array::ConstIterator i = p.GetArray().Begin();
			i != p.GetArray().End();
			++i)
	{
		Endpoint& e = ea.Add();
		*(*i) >> e;
	}
}

void
TestBogusMethod(SOAPProxy& proxy, const Endpoint& e)
{
	SOAPMethod method("BogusMethod", e.nspace, e.soapaction, e.needsappend);
	proxy.Execute(method);
}

void
TestBogusNamespace(SOAPProxy& proxy, const Endpoint& e)
{
	SOAPMethod method("echoVoid", "http://bogusns.com/", e.soapaction, e.needsappend);
	proxy.Execute(method);
	throw UnexpectedSuccessException("Method executed with bogus namespace.");
}

void
TestEchoVoid(SOAPProxy& proxy, const Endpoint& e)
{
	SOAPMethod method("echoVoid", e.nspace, e.soapaction, e.needsappend);

	const SOAPResponse& response = proxy.Execute(method);
	if (response.GetBody().GetMethod().GetNumParameters() != 0)
		throw SOAPException("Received unexpected return values.");
}

void
TestEchoString(SOAPProxy& proxy, const Endpoint& e, const char *value)
{
	SOAPString inputValue = value;

	SOAPMethod method("echoString", e.nspace, e.soapaction, e.needsappend);
	method.AddParameter("inputString") << inputValue;

	const SOAPResponse& response = proxy.Execute(method);
	SOAPString outputValue;
	response.GetReturnValue() >> outputValue;
	if (inputValue != outputValue)
		throw SOAPException("Values are not equal");
}

void
TestEchoInteger(SOAPProxy& proxy, const Endpoint& e, int value)
{
	int inputValue = value;

	SOAPMethod method("echoInteger", e.nspace, e.soapaction, e.needsappend);
	method.AddParameter("inputInteger") << inputValue;

	const SOAPResponse& response = proxy.Execute(method);
	int outputValue = 0;
	response.GetReturnValue() >> outputValue;
	if (inputValue != outputValue)
		throw SOAPException("Values are not equal");
}

const SOAPResponse&
_TestEchoInteger(SOAPProxy& proxy, const Endpoint& e, const char *value)
{
	SOAPMethod method("echoInteger", e.nspace, e.soapaction, e.needsappend);
	method.AddParameter("inputInteger").SetInt(value);

	return proxy.Execute(method);
}

void
TestEchoIntegerForFail(SOAPProxy& proxy, const Endpoint& e, const char *value)
{
	const SOAPResponse& response = _TestEchoInteger(proxy, e, value);
	throw UnexpectedSuccessException("Returned value: %s",
			(const char *)response.GetReturnValue().GetString());
}

void
TestEchoIntegerForPass(SOAPProxy& proxy, const Endpoint& e, const char *value)
{
	const SOAPResponse& response = _TestEchoInteger(proxy, e, value);
	int returnValue;
	response.GetReturnValue() >> returnValue;
}

void
TestEchoFloat(SOAPProxy& proxy, const Endpoint& e, float value)
{
	float inputValue = value;

	SOAPMethod method("echoFloat", e.nspace, e.soapaction, e.needsappend);
	SOAPParameter& inputParam = method.AddParameter("inputFloat");
	inputParam << inputValue;

	const SOAPResponse& response = proxy.Execute(method);
	const SOAPParameter& outputParam = response.GetReturnValue();
	float outputValue = 0;
	outputParam >> outputValue;

	if (inputValue != outputValue && !almostequal(inputValue, outputValue))
		throw SOAPException("Values are not equal: %s != %s",
			(const char *)inputParam.GetString(),
			(const char *)outputParam.GetString());
}

void
TestEchoFloatStringValue(SOAPProxy& proxy, const Endpoint& e, const char *value)
{
	SOAPMethod method("echoFloat", e.nspace, e.soapaction, e.needsappend);
	SOAPParameter& inputParam = method.AddParameter("inputFloat");
	inputParam.SetFloat(value);

	const SOAPResponse& response = proxy.Execute(method);
	if (response.GetReturnValue().GetString() != value)
		throw SOAPException("Wrong return value: %s",
			(const char *)response.GetReturnValue().GetString());
}

const SOAPResponse&
_TestEchoFloat(SOAPProxy& proxy, const Endpoint& e, const char *value)
{
	SOAPMethod method("echoFloat", e.nspace, e.soapaction, e.needsappend);
	SOAPParameter& inputParam = method.AddParameter("inputFloat");
	inputParam.SetFloat(value);

	return proxy.Execute(method);
}

void
TestEchoFloatForFail(SOAPProxy& proxy, const Endpoint& e, const char *value)
{
	const SOAPResponse& response = _TestEchoFloat(proxy, e, value);
	throw UnexpectedSuccessException("Returned value: %s",
		(const char *)response.GetReturnValue().GetString());
}

void
TestEchoFloatForPass(SOAPProxy& proxy, const Endpoint& e, const char *value)
{
	const SOAPResponse& response = _TestEchoFloat(proxy, e, value);
	float result;
	response.GetReturnValue() >> result;
}

void
TestEchoStruct(SOAPProxy& proxy, const Endpoint& e)
{
	SOAPInteropStruct inputValue("This is a struct string.", 68, (float)25.24345356);

	SOAPMethod method("echoStruct", e.nspace, e.soapaction, e.needsappend);
	method.AddParameter("inputStruct") << inputValue;

	const SOAPResponse& response = proxy.Execute(method);
	SOAPInteropStruct outputValue;
	response.GetReturnValue() >> outputValue;
	if (inputValue != outputValue && !almostequal(inputValue.varFloat, outputValue.varFloat))
		throw SOAPException("Values are not equal");
}


void
TestEchoIntegerArray(SOAPProxy& proxy, const Endpoint& e, int numvals)
{
	SOAPArray<int> inputValue;
	SOAPArray<int> outputValue;

	for (int i = 0; i < numvals; ++i)
		inputValue.Add(rand());

	SOAPMethod method("echoIntegerArray", e.nspace, e.soapaction, e.needsappend);
	// Here I call SetArrayType() to make sure that for zero-length
	// arrays the array type is correct.  We have to set it manually
	// for zero length arrays because we can't determine the type from
	// elements in the array!
	SOAPParameter& param = method.AddParameter("inputIntegerArray");
	param << inputValue;
	param.SetArrayType("int");

	const SOAPResponse& response = proxy.Execute(method);
	response.GetReturnValue() >> outputValue;
	if (inputValue != outputValue)
		throw SOAPException("Values are not equal");
}


void
TestEchoFloatArray(SOAPProxy& proxy, const Endpoint& e, int numvals)
{
	SOAPArray<float> inputValue;
	for (int i = 0; i < numvals; ++i)
		inputValue.Add(randdouble());

	SOAPMethod method("echoFloatArray", e.nspace, e.soapaction, e.needsappend);
	// Here I call SetArrayType() to make sure that for zero-length
	// arrays the array type is correct.  We have to set it manually
	// for zero length arrays because we can't determine the type from
	// elements in the array!
	SOAPParameter& param = method.AddParameter("inputFloatArray");
	param << inputValue;
	param.SetArrayType("float");

	const SOAPResponse& response = proxy.Execute(method);

	SOAPArray<float> outputValue;
	response.GetReturnValue() >> outputValue;

	if (inputValue != outputValue && !almostequal(inputValue, outputValue))
		throw SOAPException("Values are not equal");
}


void
TestEchoStringArray(SOAPProxy& proxy, const Endpoint& e, int numvals)
{
	SOAPArray<SOAPString> inputValue;
	for (int i = 0; i < numvals; ++i)
	{
		char buffer[256];
		sprintf(buffer, "This is test string #%d, rn=%d", i, rand());
		inputValue.Add(buffer);
	}

	SOAPMethod method("echoStringArray", e.nspace, e.soapaction, e.needsappend);
	// Here I call SetArrayType() to make sure that for zero-length
	// arrays the array type is correct.  We have to set it manually
	// for zero length arrays because we can't determine the type from
	// elements in the array!
	SOAPParameter& param = method.AddParameter("inputStringArray");
	param << inputValue;
	param.SetArrayType("string");

	const SOAPResponse& response = proxy.Execute(method);
	SOAPArray<SOAPString> outputValue;
	response.GetReturnValue() >> outputValue;
	if (inputValue != outputValue)
		throw SOAPException("Values are not equal");
}


void
TestEchoStructArray(SOAPProxy& proxy, const Endpoint& e, int numvals)
{
	SOAPArray<SOAPInteropStruct> inputValue;
	for (int i = 0; i < numvals; ++i)
	{
		SOAPInteropStruct& val = inputValue.Add();
		char buffer[256];
		sprintf(buffer, "This is struct string #%d, rn=%d", i, rand());
		val.varString = buffer;
		val.varFloat = randdouble();
		val.varInt = rand();
	}

	SOAPMethod method("echoStructArray", e.nspace, e.soapaction, e.needsappend);
	// Here I call SetArrayType() to make sure that for zero-length
	// arrays the array type is correct.  We have to set it manually
	// for zero length arrays because we can't determine the type from
	// elements in the array!
	SOAPParameter& param = method.AddParameter("inputStructArray");
	param << inputValue;
	param.SetArrayType(SOAPInteropStruct::soap_name, SOAPInteropStruct::soap_namespace);

	const SOAPResponse& response = proxy.Execute(method);
	SOAPArray<SOAPInteropStruct> outputValue;
	response.GetReturnValue() >> outputValue;
	if (inputValue != outputValue && !almostequal(inputValue, outputValue))
		throw SOAPException("Values are not equal");
}

void
TestEchoBase64(SOAPProxy& proxy, const Endpoint& e)
{
	SOAPArray<char> inputBinary, outputBinary;

	int size = rand() % 501 + 500;
	inputBinary.Resize(size);
	for (int i = 0; i < size; ++i)
		inputBinary[i] = rand();

	SOAPMethod method("echoBase64", e.nspace, e.soapaction, e.needsappend);
	method.AddParameter("inputBase64") << inputBinary;
	const SOAPResponse& response = proxy.Execute(method);
	response.GetReturnValue() >> outputBinary;
	if (inputBinary != outputBinary)
		throw SOAPException("Values are not equal");
}

void
TestEchoInteger(SOAPProxy& proxy, const Endpoint& e)
{
	TestEchoInteger(proxy, e, rand());
}

void
TestEchoInteger_MostPositive(SOAPProxy& proxy, const Endpoint& e)
{
	TestEchoInteger(proxy, e, 2147483647);
}

void
TestEchoInteger_MostNegative(SOAPProxy& proxy, const Endpoint& e)
{
	TestEchoInteger(proxy, e, -2147483648);
}

void
TestEchoInteger_Overflow(SOAPProxy& proxy, const Endpoint& e)
{
	TestEchoIntegerForFail(proxy, e, "2147483648");
}

void
TestEchoInteger_Underflow(SOAPProxy& proxy, const Endpoint& e)
{
	TestEchoIntegerForFail(proxy, e, "-2147483649");
}

void
TestEchoInteger_Junk1(SOAPProxy& proxy, const Endpoint& e)
{
	TestEchoIntegerForFail(proxy, e, "1234junk");
}

void
TestEchoInteger_Junk2(SOAPProxy& proxy, const Endpoint& e)
{
	TestEchoIntegerForPass(proxy, e, "\r\n\t 1234 \r\n\t");
}

void
TestEchoFloat(SOAPProxy& proxy, const Endpoint& e)
{
	TestEchoFloat(proxy, e, 31391236.0);
}

void
TestEchoFloat_NaN(SOAPProxy& proxy, const Endpoint& e)
{
	TestEchoFloatStringValue(proxy, e, "NaN");
}

void
TestEchoFloat_INF(SOAPProxy& proxy, const Endpoint& e)
{
	TestEchoFloatStringValue(proxy, e, "INF");
}

void
TestEchoFloat_negINF(SOAPProxy& proxy, const Endpoint& e)
{
	TestEchoFloatStringValue(proxy, e, "-INF");
}

void
TestEchoFloat_DoubleOverflow(SOAPProxy& proxy, const Endpoint& e)
{
	TestEchoFloatForFail(proxy, e, "1.8e308");
}

void
TestEchoFloat_DoubleUnderflow(SOAPProxy& proxy, const Endpoint& e)
{
	TestEchoFloatForFail(proxy, e, "2.4e-324");
}

void
TestEchoFloat_SingleOverflow(SOAPProxy& proxy, const Endpoint& e)
{
	TestEchoFloatForFail(proxy, e, "3.5e38");
}

void
TestEchoFloat_SingleUnderflow(SOAPProxy& proxy, const Endpoint& e)
{
	TestEchoFloatForFail(proxy, e, "6.9e-46");
}

void
TestEchoFloat_Junk1(SOAPProxy& proxy, const Endpoint& e)
{
	TestEchoFloatForFail(proxy, e, "1234junk");
}

void
TestEchoFloat_Junk2(SOAPProxy& proxy, const Endpoint& e)
{
	TestEchoFloatForPass(proxy, e, "\r\n\t 1234 \r\n\t");
}

void
TestEchoString(SOAPProxy& proxy, const Endpoint& e)
{
	TestEchoString(proxy, e, "This is a test string from EasySoap++");
}

void
TestEchoIntegerArray(SOAPProxy& proxy, const Endpoint& e)
{
	TestEchoIntegerArray(proxy, e, rand() % 10 + 5);
}

void
TestEchoFloatArray(SOAPProxy& proxy, const Endpoint& e)
{
	TestEchoFloatArray(proxy, e, rand() % 10 + 5);
}

void
TestEchoStringArray(SOAPProxy& proxy, const Endpoint& e)
{
	TestEchoStringArray(proxy, e, rand() % 10 + 5);
}

void
TestEchoStructArray(SOAPProxy& proxy, const Endpoint& e)
{
	TestEchoStructArray(proxy, e, rand() % 10 + 5);
}

void
TestEchoIntegerArrayZeroLen(SOAPProxy& proxy, const Endpoint& e)
{
	TestEchoIntegerArray(proxy, e, 0);
}

void
TestEchoFloatArrayZeroLen(SOAPProxy& proxy, const Endpoint& e)
{
	TestEchoFloatArray(proxy, e, 0);
}

void
TestEchoStringArrayZeroLen(SOAPProxy& proxy, const Endpoint& e)
{
	TestEchoStringArray(proxy, e, 0);
}

void
TestEchoStructArrayZeroLen(SOAPProxy& proxy, const Endpoint& e)
{
	TestEchoStructArray(proxy, e, 0);
}

typedef void (*TestFunction)(SOAPProxy&, const Endpoint&);

void
TestForPass(SOAPProxy& proxy, const Endpoint& e, const char *testname, TestFunction func)
{
	const char *type = 0;
	SOAPString msg;

	try
	{
		SetTraceFile(e.name , testname);
		std::cout << "Testing " << testname << ": ";
		func(proxy, e);
		type = "PASS";
		msg = "PASS";
	}
	catch (FPLossException& sex)
	{
		type = "FP LOSS";
		msg = sex.What();
	}
	catch (SOAPSocketException& sex)
	{
		type = "NETWORK ERROR";
		msg = sex.What();
	}
	catch (SOAPFaultException& sex)
	{
		type = "FAULT";
		msg = sex.What();
	}
	catch (SOAPException& sex)
	{
		type = "FAIL";
		msg = sex.What();
	}
	catch (...)
	{
		type = "UNKNOWN";
		msg = "Unknown error, problem with EasySoap++";
	}

	std::cout << type;
	if (msg)
		std::cout << ": " << msg;
	std::cout << std::endl;

	testresults.StartTag("Test");
	testresults.AddAttr("name", testname);

	testresults.StartTag("Result");
	testresults.WriteValue(type);
	testresults.EndTag("Result");

	testresults.StartTag("Message");
	testresults.WriteValue(msg);
	testresults.EndTag("Message");

	testresults.EndTag("Test");
}

void
TestForFault(SOAPProxy& proxy, const Endpoint& e, const char *testname, TestFunction func)
{
	const char *type;
	SOAPString msg;
	try
	{
		SetTraceFile(e.name , testname);
		std::cout << "Testing " << testname << ": ";
		func(proxy, e);
		type = "FAIL";
		msg = "FAIL";
	}
	catch (SOAPFaultException& sex)
	{
		type = "PASS";
		msg = sex.What();
	}
	catch (UnexpectedSuccessException& sex)
	{
		type = "FAIL";
		msg = sex.What();
	}
	catch (SOAPSocketException& sex)
	{
		type = "NETWORK ERROR";
		msg = sex.What();
	}
	catch (SOAPException& sex)
	{
		type = "FAIL";
		msg = sex.What();
	}
	catch (...)
	{
		type = "UNKNOWN";
		msg = "Unknown error, problem with EasySoap";
	}

	std::cout << type;
	if (msg)
		std::cout << ": " << msg;
	std::cout << std::endl;

	testresults.StartTag("Test");
	testresults.AddAttr("name", testname);

	testresults.StartTag("Result");
	testresults.WriteValue(type);
	testresults.EndTag("Result");

	testresults.StartTag("Message");
	testresults.WriteValue(msg);
	testresults.EndTag("Message");

	testresults.EndTag("Test");
}

void
TestInterop(const Endpoint& e)
{
	SOAPProxy proxy(e.endpoint, httpproxy);

	testresults.StartTag("Server");
	testresults.AddAttr("name", e.name);

	testresults.StartTag("Endpoint");
	testresults.WriteValue(e.endpoint.GetString());
	testresults.EndTag("Endpoint");

	testresults.StartTag("SoapAction");
	testresults.WriteValue(e.soapaction);
	testresults.EndTag("SoapAction");

	testresults.StartTag("NameSpace");
	testresults.WriteValue(e.nspace);
	testresults.EndTag("NameSpace");

	TestForFault(proxy, e, "BogusMethod",				TestBogusMethod);
	TestForFault(proxy, e, "BogusNamespace",			TestBogusNamespace);
	TestForPass(proxy, e, "echoVoid",					TestEchoVoid);
	TestForPass(proxy, e, "echoInteger",				TestEchoInteger);
	TestForPass(proxy, e, "echoInteger_MostPositive",	TestEchoInteger_MostPositive);
	TestForPass(proxy, e, "echoInteger_MostNegative",	TestEchoInteger_MostNegative);
	TestForFault(proxy, e, "echoInteger_Overflow",		TestEchoInteger_Overflow);
	TestForFault(proxy, e, "echoInteger_Underflow",		TestEchoInteger_Underflow);
	TestForFault(proxy, e, "echoInteger_Junk1"	,		TestEchoInteger_Junk1);
	TestForPass(proxy, e, "echoInteger_Junk2"	,		TestEchoInteger_Junk2);
	TestForPass(proxy, e, "echoFloat",					TestEchoFloat);
	TestForPass(proxy, e, "echoFloat_NaN",				TestEchoFloat_NaN);
	TestForPass(proxy, e, "echoFloat_INF",				TestEchoFloat_INF);
	TestForPass(proxy, e, "echoFloat_negINF",			TestEchoFloat_negINF);
	TestForFault(proxy, e, "echoFloat_SingleOverflow",	TestEchoFloat_SingleOverflow);
	TestForFault(proxy, e, "echoFloat_SingleUnderflow",	TestEchoFloat_SingleUnderflow);
	TestForFault(proxy, e, "echoFloat_DoubleOverflow",	TestEchoFloat_DoubleOverflow);
	TestForFault(proxy, e, "echoFloat_DoubleUnderflow",	TestEchoFloat_DoubleUnderflow);
	TestForFault(proxy, e, "echoFloat_Junk1",			TestEchoFloat_Junk1);
	TestForPass(proxy, e, "echoFloat_Junk2",			TestEchoFloat_Junk2);
	TestForPass(proxy, e, "echoString",					TestEchoString);
	TestForPass(proxy, e, "echoStruct",					TestEchoStruct);
	TestForPass(proxy, e, "echoIntegerArray",			TestEchoIntegerArray);
	TestForPass(proxy, e, "echoFloatArray",				TestEchoFloatArray);
	TestForPass(proxy, e, "echoStringArray",			TestEchoStringArray);
	TestForPass(proxy, e, "echoStructArray",			TestEchoStructArray);
	TestForPass(proxy, e, "echoIntegerArray_ZeroLen",	TestEchoIntegerArrayZeroLen);
	TestForPass(proxy, e, "echoFloatArray_ZeroLen",		TestEchoFloatArrayZeroLen);
	TestForPass(proxy, e, "echoStringArray_ZeroLen",	TestEchoStringArrayZeroLen);
	TestForPass(proxy, e, "echoStructArray_ZeroLen",	TestEchoStructArrayZeroLen);
	TestForPass(proxy, e, "echoBase64",					TestEchoBase64);

	testresults.EndTag("Server");
}

int
main(int argc, char* argv[])
{
	int ret = 0;
	srand(time(0));
	const char *xmlname = 0;
	try
	{
		const char *servicename = 0;
		bool testlocal = true;
		bool doall = false;
		bool execute = true;
		bool doappend = false;
		bool makedirs = false;

		SOAPArray<Endpoint> endpoints;
		SOAPPacketWriter::SetAddWhiteSpace(true);
		SOAPHashMapNoCase<SOAPString, bool> skips;

		const char *soapaction = default_interop_soapaction;
		const char *nspace = default_interop_namespace;

		for (int i = 1; i < argc;)
		{
			SOAPString val = argv[i++];
			if (val == "-a")
			{
				doall = true;
				testlocal = false;
			}
			else if (val == "-xml")
			{
				xmlname = argv[i++];
			}
			else if (val == "-l")
			{
				execute = false;
			}
			else if (val == "-a+")
			{
				doappend = true;
			}
			else if (val == "-a-")
			{
				doappend = false;
			}
			else if (val == "-p")
			{
				httpproxy = argv[i++];
			}
			else if (val == "-n")
			{
				servicename = argv[i++];
			}
			else if (val == "-ns")
			{
				nspace = argv[++i];
			}
			else if (val == "-sa")
			{
				soapaction = argv[i++];
			}
			else if (val == "-skip")
			{
				skips[argv[i++]] = true;
			}
			else if (val == "-mkdir")
			{
				makedirs = true;
			}
			else if (val[0] == '-')
			{
				throw SOAPException("Unknown commandline argument: %s", (const char *)val);
			}
			else
			{
				testlocal = false;
				Endpoint& e = endpoints.Add();
				e.endpoint = val;
				e.name = servicename ? servicename : (const char *)e.endpoint.Hostname();
				e.nspace = nspace;
				e.soapaction = soapaction;
				e.needsappend = doappend;
				servicename = 0;
			}
		}

		if (doall)
		{
			GetAllEndpoints(endpoints);
		}

		if (testlocal)
		{
			// Just test against localhost
			Endpoint& e = endpoints.Add();
			e.name = "localhost";

//  FIX ME:
//  Make this configurable from the makefile
#ifdef _WIN32
			e.endpoint = "http://localhost:80/cgi-bin/interopserver.exe";
#else
			e.endpoint = "http://localhost:80/cgi-bin/interopserver";
#endif // _WIN32

			e.nspace = default_interop_namespace;
			e.soapaction = default_interop_soapaction;
			e.needsappend = false;
		}

		char buffer[256];
		time_t ltime = time(0);
		struct tm *ltm = localtime(&ltime);
		strftime(buffer, 256, "%d-%b-%Y %H:%M", ltm);

		testresults.StartTag("InteropTests");
		testresults.StartTag("Date");
		testresults.WriteValue(buffer);
		testresults.EndTag("Date");


		std::sort(endpoints.Begin(), endpoints.End());

		for (size_t j = 0; j < endpoints.Size(); ++j)
		{
			Endpoint& e = endpoints[j];

			std::cout	<< "      Name: " << e.name << std::endl
						<< "  Endpoint: " << e.endpoint.GetString() << std::endl
						<< " Namespace: " << e.nspace << std::endl
						<< "SOAPAction: " << e.soapaction
						<< (e.needsappend ? "(method)" : "") <<	std::endl
						<< std::endl;

			if (makedirs)
#ifdef _WIN32
				_mkdir(e.name);
#else
				mkdir(e.name, 0755);
#endif

			if (execute && !skips.Find(e.name))
				TestInterop(e);

			std::cout
				<< std::endl
				<< "----------------------------------------------------------------"
				<< std::endl
				<< std::endl;
		}

		testresults.EndTag("InteropTests");
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

	if (xmlname)
	{
		FILE *xmlfile = fopen(xmlname, "wb");
		if (xmlfile)
		{
			fwrite(testresults.GetBytes(), 1, testresults.GetLength(), xmlfile);
			fclose(xmlfile);
		}
	}

	SOAPDebugger::Close();
	return ret;
}

