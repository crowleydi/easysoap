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

		std::cout << "SUCCESS" << std::endl;
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
			bool appendMethod)
{
	try
	{
		SOAPString inputValue = "This is a test input string.";
		SOAPString outputValue;

		SOAPMethod method("echoString", uri, soapAction, appendMethod);
		method.AddParameter("inputString") << inputValue;

		std::cout << "Testing " << method.GetName() << ": ";

		const SOAPResponse& response = proxy.Execute(method);
		response.GetReturnValue() >> outputValue;
		if (inputValue != outputValue)
			throw SOAPException("Values are not equal");

		std::cout << "SUCCESS" << std::endl;
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
			bool appendMethod)
{
	try
	{
		int inputValue = 252456;
		int outputValue = 0;

		SOAPMethod method("echoInteger", uri, soapAction, appendMethod);
		method.AddParameter("inputInteger") << inputValue;

		std::cout << "Testing " << method.GetName() << ": ";

		const SOAPResponse& response = proxy.Execute(method);
		response.GetReturnValue() >> outputValue;
		if (inputValue != outputValue)
			throw SOAPException("Values are not equal");

		std::cout << "SUCCESS" << std::endl;
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
			bool appendMethod)
{
	try
	{
		float inputValue = (float)25.2456;
		float outputValue = 0;

		SOAPMethod method("echoFloat", uri, soapAction, appendMethod);
		method.AddParameter("inputFloat") << inputValue;

		std::cout << "Testing " << method.GetName() << ": ";

		const SOAPResponse& response = proxy.Execute(method);
		response.GetReturnValue() >> outputValue;
		if (inputValue != outputValue)
			throw SOAPException("Values are not equal");

		std::cout << "SUCCESS" << std::endl;
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
TestEchoStruct(SOAPProxy& proxy,
			const char *uri,
			const char *soapAction,
			bool appendMethod)
{
	try
	{
		SOAPInteropStruct inputValue("This is a struct string.", 68, (float)25.2456);
		SOAPInteropStruct outputValue;

		SOAPMethod method("echoStruct", uri, soapAction, appendMethod);
		method.AddParameter("inputStruct") << inputValue;

		std::cout << "Testing " << method.GetName() << ": ";

		const SOAPResponse& response = proxy.Execute(method);
		response.GetReturnValue() >> outputValue;
		if (inputValue != outputValue)
			throw SOAPException("Values are not equal");

		std::cout << "SUCCESS" << std::endl;
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

		std::cout << "SUCCESS" << std::endl;
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
		SOAPArray<float> outputValue;

		inputValue.Add(1.3);
		inputValue.Add(6.6535);
		inputValue.Add(73.235);
		inputValue.Add(92735.3e2);
		inputValue.Add(-16e30);

		SOAPMethod method("echoFloatArray", uri, soapAction, appendMethod);
		method.AddParameter("inputFloatArray") << inputValue;

		std::cout << "Testing " << method.GetName() << ": ";

		const SOAPResponse& response = proxy.Execute(method);
		response.GetReturnValue() >> outputValue;
		if (inputValue != outputValue)
			throw SOAPException("Values are not equal");

		std::cout << "SUCCESS" << std::endl;
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
		SOAPArray<SOAPString> outputValue;

		inputValue.Add("String 1");
		inputValue.Add("String 2");
		inputValue.Add("Third String");
		inputValue.Add("A Fourth and last string.");

		SOAPMethod method("echoStringArray", uri, soapAction, appendMethod);
		method.AddParameter("inputStringArray") << inputValue;

		std::cout << "Testing " << method.GetName() << ": ";

		const SOAPResponse& response = proxy.Execute(method);
		response.GetReturnValue() >> outputValue;
		if (inputValue != outputValue)
			throw SOAPException("Values are not equal");

		std::cout << "SUCCESS" << std::endl;
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
		SOAPArray<SOAPInteropStruct> outputValue;

		SOAPInteropStruct val;
		val.varFloat = (float)46.346;
		val.varInt = -2352;
		val.varString = "Array struct string.";

		inputValue.Add(val);
		inputValue.Add(val);
		inputValue.Add(val);

		SOAPMethod method("echoStructArray", uri, soapAction, appendMethod);
		method.AddParameter("inputStructArray") << inputValue;

		std::cout << "Testing " << method.GetName() << ": ";

		const SOAPResponse& response = proxy.Execute(method);
		response.GetReturnValue() >> outputValue;
		if (inputValue != outputValue)
			throw SOAPException("Values are not equal");

		std::cout << "SUCCESS" << std::endl;
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

	TestEchoInteger(proxy, uri, soapAction, appendMethod);
	TestEchoFloat(proxy, uri, soapAction, appendMethod);
	TestEchoString(proxy, uri, soapAction, appendMethod);
	TestEchoStruct(proxy, uri, soapAction, appendMethod);

	TestEchoIntegerArray(proxy, uri, soapAction, appendMethod);
	TestEchoFloatArray(proxy, uri, soapAction, appendMethod);
	TestEchoStringArray(proxy, uri, soapAction, appendMethod);
	TestEchoStructArray(proxy, uri, soapAction, appendMethod);
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

		TestInterop(
			"Apache 2.1",
			"http://services.xmethods.net:8080/soap/servlet/rpcrouter",
			"urn:soapinterop", false,
			"http://soapinterop.org/");/**/

		TestInterop("Frontier 7.0b26 (Userland)",
			"http://www.soapware.org:80/xmethodsInterop",
			//"http://localhost:8080/xmethodsInterop",
			"/xmethodsInterop", false,
			"urn:xmethodsInterop");/**/

		TestInterop("Kafka XSLT",
			"http://www.vbxml.com/soapworkshop/services/kafka10/services/endpoint.asp?service=ilab",
			"urn:soapinterop", false,
			"http://soapinterop.org/");/**/

		TestInterop("MS SOAP Toolkit 2.0",
			"http://131.107.72.13/stk/InteropTypedService.asp",
			"urn:soapinterop", false,
			"http://soapinterop.org/");/**/

		TestInterop("MS .NET Beta 2",
			"http://131.107.72.13/test/typed.asmx",
			"http://soapinterop.org/", true,
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

		TestInterop("White Mesa SOAP RPC 1.4",
			"http://services2.xmethods.net:8080/interop",
			"urn:soapinterop#", true,
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

