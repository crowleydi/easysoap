//
// tests.cpp
//


//
// some quck and dirty tests and examples for easysoap.
//

#ifdef _WIN32
#pragma warning (disable: 4786)
#endif // _WIN32

#include <iostream>
#include <math.h>
#include "SOAP.h"




inline std::ostream&
operator<<(std::ostream& os, const SOAPString& str)
{
	const char *s = str;
	return os << (s ? s : "(null)");
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
//  Define how we load up the struct
SOAPParameter&
operator<<(SOAPParameter& param, const SOAPInteropStruct& val)
{
	param.SetType("SOAPStruct", SOAPInteropStruct::soap_namespace);
	param.SetBaseType(SOAPTypes::soap_struct);

	param.AddParameter("varString") << val.varString;
	param.AddParameter("varInt") << val.varInt;
	param.AddParameter("varFloat") << val.varFloat;

	return param;
}

//
// Define how we read the struct
const SOAPParameter&
operator>>(const SOAPParameter& param, SOAPInteropStruct& val)
{
	SafeGetParameter(param, "varString") >> val.varString;
	SafeGetParameter(param, "varInt") >> val.varInt;
	SafeGetParameter(param, "varFloat") >> val.varFloat;

	return param;
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
		method.AddParameter("inputInteger").SetInteger(value);

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

		if (inputValue != outputValue)
			throw SOAPException("Values are not equal: %g != %g", inputValue, outputValue);

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

		inputValue.Add(1.3);
		inputValue.Add(6.6535);
		inputValue.Add(73.235);
		inputValue.Add(92735.3e2);
		inputValue.Add(-16e30);
		inputValue.Add(-16e-30);
		inputValue.Add(-16e30);
		inputValue.Add(-163.0);
		inputValue.Add(-0.1);

		SOAPMethod method("echoFloatArray", uri, soapAction, appendMethod);
		method.AddParameter("inputFloatArray") << inputValue;

		std::cout << "Testing " << method.GetName() << ": ";

		const SOAPResponse& response = proxy.Execute(method);

		SOAPArray<float> outputValue;
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


void
TestInterop(SOAPProxy& proxy,
			const char *uri,
			const char *soapAction,
			bool appendMethod)
{
	TestEchoVoid(proxy, uri, soapAction, appendMethod);

	TestEchoInteger(proxy, uri, soapAction, appendMethod, 464);
	TestEchoFloat(proxy, uri, soapAction, appendMethod, -3.6e-6);
	TestEchoString(proxy, uri, soapAction, appendMethod, "This is a test string from EasySOAP++");
	TestEchoStruct(proxy, uri, soapAction, appendMethod);

	TestEchoIntegerArray(proxy, uri, soapAction, appendMethod);
	TestEchoFloatArray(proxy, uri, soapAction, appendMethod);
	TestEchoStringArray(proxy, uri, soapAction, appendMethod);
	TestEchoStructArray(proxy, uri, soapAction, appendMethod);

	// Lets test some boundry cases...
	//TestEchoFloat(proxy, uri, soapAction, appendMethod, -35.6e-22);
	//TestEchoFloat(proxy, uri, soapAction, appendMethod, -35.6e22);
	//TestEchoIntegerInvalid(proxy, uri, soapAction, appendMethod, "2147483648");
	//TestEchoIntegerInvalid(proxy, uri, soapAction, appendMethod, "-2147483649");
	//TestEchoFloat(proxy, uri, soapAction, appendMethod, "NaN");
	//TestEchoFloat(proxy, uri, soapAction, appendMethod, "INF");
	//TestEchoFloat(proxy, uri, soapAction, appendMethod, "-INF");
	//TestEchoFloatInvalid(proxy, uri, soapAction, appendMethod, "+INF");
	//TestEchoFloat(proxy, uri, soapAction, appendMethod, "-0.0");
	//TestEchoFloat(proxy, uri, soapAction, appendMethod, -HUGE_VAL);
	//TestEchoFloat(proxy, uri, soapAction, appendMethod, HUGE_VAL);
}

void TestInterop(const char *name,
				 const char *endpoint,
				 const char *soapaction,
				 bool appendMethod,
				 const char *nspace)
{
	std::cout << "Testing " << name << " interopability." << std::endl;

	SOAPProxy proxy(endpoint);//, "http://localhost:8080");
	TestInterop(proxy, nspace, soapaction, appendMethod);

	std::cout << "Done." << std::endl << std::endl;
}

int
main(int argc, char* argv[])
{
	try
	{
		//
		// See: http://www.xmethods.net/ilab/ilab.html
		// This information changes frequently.
		//
		TestInterop("4s4c 1.3",
			"http://soap.4s4c.com/ilab/soap.asp",
			"urn:soapinterop", false,
			"http://soapinterop.org/");/**/

		TestInterop("Apache 2.1",
			"http://services.xmethods.net:8080/soap/servlet/rpcrouter",
			"urn:soapinterop", false,
			"http://soapinterop.org/");/**/

		TestInterop("Frontier 7.0b26 (Userland)",
			"http://www.soapware.org:80/xmethodsInterop",
			"/xmethodsInterop", false,
			"urn:xmethodsInterop");/**/

		TestInterop("Kafka XSLT",
			"http://www.vbxml.com/soapworkshop/services/kafka10/services/endpoint.asp?service=ilab",
			"urn:soapinterop", false,
			"http://soapinterop.org/");/**/

		/*TestInterop("MS SOAP Toolkit 2.0 (untyped)",
			"http://131.107.72.13/stk/InteropService.asp",
			"urn:soapinterop", false,
			"http://soapinterop.org/");/**/

		TestInterop("MS SOAP Toolkit 2.0 (typed)",
			"http://131.107.72.13/stk/InteropTypedService.asp",
			"urn:soapinterop", false,
			"http://soapinterop.org/");/**/

		TestInterop("Phalanx",
			"http://www.phalanxsys.com/interop/listener.asp",
			"urn:soapinterop", false,
			"http://soapinterop.org/");/**/

		TestInterop("SOAP RMI",
			"http://rainier.extreme.indiana.edu:1568",
			"urn:soapinterop", false,
			"http://soapinterop.org/");/**/

		TestInterop("SOAP::Lite 0.47",
			"http://services.soaplite.com/interop.cgi",
			"urn:soapinterop", false,
			"http://soapinterop.org/");/**/

		TestInterop("SQLData SOAP Server",
			"http://www.soapclient.com/interop/sqldatainterop.wsdl",
			"/soapinterop", false,
			"http://tempuri.org/message/");/**/

		/****** These have problems... ******/

		TestInterop("White Mesa SOAP RPC 2.0",
			"http://services3.xmethods.net:8080/interop",
			"urn:soapinterop#", true,
			"http://soapinterop.org/");/**/

		/*TestInterop("MS .NET Beta 2 (untyped)",
			"http://131.107.72.13/test/simple.asmx",
			"http://soapinterop.org/", true,
			"http://soapinterop.org/");/**/

		TestInterop("MS .NET Beta 2 (typed)",
			"http://131.107.72.13/test/typed.asmx",
			"http://soapinterop.org/", true,
			"http://soapinterop.org/");/**/
	}
	catch (const SOAPMemoryException&)
	{
		std::cout << "SOAP out of memory." << std::endl;
		return 1;
	}
	catch (const SOAPException& ex)
	{
		std::cout << "Caught SOAP exception: " << ex.What() << std::endl;
		return 1;
	}

	return 0;
}

