//
// tests.cpp
//


//
// some quck and dirty tests and examples for easysoap.
//

#ifdef _WIN32
#pragma warning (disable: 4786)
#endif // _WIN32

#include "SOAP.h"

#include <iostream>

inline std::ostream&
operator<<(std::ostream& os, const SOAPString& str)
{
	return os << (const char *)str;
}

void
WriteParameter(const SOAPParameter& param)
{
	switch (param.GetType())
	{
	case SOAPTypes::xsd_null:
		std::cout << "null: " << param.GetName();
		break;
	case SOAPTypes::xsd_int:
		std::cout << "int: " << param.GetName() << "=" << param.GetInt();
		break;
	case SOAPTypes::xsd_float:
		std::cout << "float: " << param.GetName() << "=" << param.GetFloat();
		break;
	case SOAPTypes::xsd_double:
		std::cout << "double: " << param.GetName() << "=" << param.GetDouble();
		break;
	case SOAPTypes::xsd_string:
		std::cout << "string: " << param.GetName() << "=" << param.GetString();
		break;

	case SOAPTypes::soap_array:
		{
			std::cout << "Array {" << std::endl;

			const SOAPParameter::Array& val = param.GetArray();
			for (SOAPParameter::Array::ConstIterator i = val.Begin(); i != val.End(); ++i)
				WriteParameter(*i);

			std::cout << "}";
		}
		break;
	case SOAPTypes::soap_struct:
		{
			std::cout << "Struct {" << std::endl;

			const SOAPParameter::Struct& val = param.GetStruct();
			for (SOAPParameter::Struct::Iterator i = val.Begin(); i != val.End(); ++i)
				WriteParameter(*i);

			std::cout << "}";
		}
		break;
	default:
		std::cout << "unknown type";
		break;
	}

	std::cout << std::endl;
}

const SOAPResponse&
WriteResponse(const SOAPResponse& response)
{
	if (response.GetBody().IsFault())
	{
		const SOAPFault& fault = response.GetBody().GetFault();
		std::cout << response.GetBody().GetMethod().GetName() << ":" << std::endl
			<< "  code: " << fault.GetFaultCode().GetString() << std::endl
			<< "string: " << fault.GetFaultString().GetString() << std::endl
			<< "detail: " << fault.GetDetail().GetString() << std::endl
			<< " actor: " << fault.GetFaultActor().GetString() << std::endl;
	}
	else
	{
		std::cout << response.GetBody().GetMethod().GetName() << std::endl;

		const SOAPMethod& method = response.GetBody().GetMethod();
		for (size_t i = 0; i < method.GetNumParameters(); ++i)
			WriteParameter(method.GetParameter(i));

		std::cout << std::endl;
	}
	return response;
}

SOAPString
getStateName(SOAPProxy& soap, int statenum)
{
	SOAPMethod	getStateName("getStateName", "/My/Examples");
	SOAPParameter& param = getStateName.AddParameter();
	param.SetValue(statenum);

	return soap.Execute(getStateName).GetReturnValue().GetString();
}

void
getStateList(SOAPProxy& soap)
{
	SOAPMethod	getStateList("getStateList", "/My/Examples");
	SOAPParameter& listParam = getStateList.AddParameter();
	listParam.AddParameter().SetValue(13);
	listParam.AddParameter().SetValue(79);
	listParam.AddParameter().SetValue(15);

	WriteResponse(soap.Execute(getStateList));
}

void
getStateStruct(SOAPProxy& soap)
{
	SOAPMethod	getStateStruct("getStateStruct", "/My/Examples");

	SOAPParameter& structParam = getStateStruct.AddParameter();
	structParam.AddParameter("a").SetValue(1);
	structParam.AddParameter("b").SetValue(5);

	WriteResponse(soap.Execute(getStateStruct));
}

void
bogusCall(SOAPProxy& soap)
{
	SOAPMethod	getStateStruct("bogus", "/My/Examples");
	getStateStruct.AddParameter("value").SetValue(10);

	WriteResponse(soap.Execute(getStateStruct));
}

//
// ITFinity Currency Conversion
//
void
CurrencyConversion()
{
	SOAPProxy soap("http://www.itfinity.net/soap/exrates/default.asp");
	SOAPMethod GetRate("GetRate", "http://www.itfinity.net/soap/exrates/exrates.xsd");

	GetRate.AddParameter("fromCurr").SetValue("USD");
	GetRate.AddParameter("ToCurr").SetValue("JPY");

	WriteResponse(soap.Execute(GetRate));
}

void
GetInternetTime()
{
	SOAPProxy soap("http://www.lemurlabs.com/rpcrouter");
	SOAPMethod getInternetTime("getInternetTime", "urn:lemurlabs-ITimeService");
	WriteResponse(soap.Execute(getInternetTime));
}

void
PingService()
{
	SOAPProxy soap("http://www.lemurlabs.com/rpcrouter");
	SOAPMethod getInternetTime("getInternetTime", "urn:lemurlabs-ITimeService");
	WriteResponse(soap.Execute(getInternetTime));
}

void
Whois(const char *who)
{
	SOAPProxy soap("http://www.razorsoft.net/ssss4c/whois.asp");
	SOAPMethod whois("whois", "http://www.pocketsoap.com/whois");

	whois.AddParameter("name").SetValue(who);

	WriteResponse(soap.Execute(whois));
}

void FortuneDictionaryList()
{
	SOAPProxy soap("http://www.lemurlabs.com:80/rpcrouter");
	SOAPMethod fortune("getDictionaryNameList", "urn:lemurlabs-Fortune");

	WriteResponse(soap.Execute(fortune));
}

void Fortune(const char *dictionary = 0)
{
	SOAPProxy soap("http://www.lemurlabs.com:80/rpcrouter");
	SOAPMethod fortune;

	fortune.SetNamespace("urn:lemurlabs-Fortune");
	if (dictionary)
	{
		fortune.SetName("getFortuneByDictionary");
		fortune.AddParameter("dictionaryName").SetValue(dictionary);
	}
	else
	{
		fortune.SetName("getAnyFortune");
	}

	WriteResponse(soap.Execute(fortune));
}

float
StockQuote(const char *symbol)
{
	SOAPProxy soap("http://services.xmethods.net:80/soap");
	SOAPMethod stockquote("getQuote", "urn:xmethods-delayed-quotes");

	stockquote.AddParameter("symbol").SetValue(symbol);

	return soap.Execute(stockquote).GetReturnValue();
}

float
GetTemperature(const char *zipcode)
{
	SOAPProxy soap("http://services.xmethods.net:80/soap/servlet/rpcrouter");
	SOAPMethod gettemp("getTemp", "urn:xmethods-Temperature");

	gettemp.AddParameter("zipcode").SetValue(zipcode);

	return soap.Execute(gettemp).GetReturnValue();
}

int
TestSOAPUrlParsing()
{
	try
	{
		std::cerr << "Testing url parsing...";
		SOAPUrl url;

		url = "http://www.yahoo.com";
		url = "ftp://dcrowley@scitegic.com/";
		url = "http://www.yahoo.com/search?soap";
		url = "https://:password@scitegic.com";
	}
	catch (SOAPException& ex)
	{
		std::cerr << "failed: " << ex.What() << std::endl;
		return 1;
	}

	std::cerr << "okay." << std::endl;
	return 0;
}

void
TestInterop(SOAPProxy& proxy)
{
	try
	{
		SOAPMethod echoVoid				("echoVoid",			"urn:xmethodsInterop");
		SOAPMethod echoString			("echoString",			"urn:xmethodsInterop");
		SOAPMethod echoStringArray		("echoStringArray",		"urn:xmethodsInterop");
		SOAPMethod echoInteger			("echoInteger",			"urn:xmethodsInterop");
		SOAPMethod echoIntegerArray		("echoIntegerArray",	"urn:xmethodsInterop");
		SOAPMethod echoFloat			("echoFloat",			"urn:xmethodsInterop");
		SOAPMethod echoFloatArray		("echoFloatArray",		"urn:xmethodsInterop");
		SOAPMethod echoStructure		("echoStructure",		"urn:xmethodsInterop");
		SOAPMethod echoStructureArray	("echoStructureArray",	"urn:xmethodsInterop");

		WriteResponse(proxy.Execute(echoVoid));

		echoString.AddParameter("inputString").SetValue("This is a test string.");
		WriteResponse(proxy.Execute(echoString));

		SOAPParameter& strarray = echoStringArray.AddParameter("inputStringArray");
		strarray.AddParameter().SetValue("String 1");
		strarray.AddParameter().SetValue("String 2");
		strarray.AddParameter().SetValue("String 3");
		WriteResponse(proxy.Execute(echoStringArray));

		echoInteger.AddParameter("inputInteger").SetValue(1691);
		WriteResponse(proxy.Execute(echoInteger));

		SOAPParameter& intarray = echoIntegerArray.AddParameter("inputIntegerArray");
		intarray.AddParameter().SetValue(34);
		intarray.AddParameter().SetValue(44);
		intarray.AddParameter().SetValue(4);
		intarray.AddParameter().SetValue(1245);
		intarray.AddParameter().SetValue(315);
		WriteResponse(proxy.Execute(echoIntegerArray));

		echoFloat.AddParameter("inputFloat").SetValue((float)8.946);
		WriteResponse(proxy.Execute(echoFloat));

		SOAPParameter& floatarray = echoFloatArray.AddParameter("inputFloatArray");
		floatarray.AddParameter().SetValue((float)34.26);
		floatarray.AddParameter().SetValue((float)3.44);
		floatarray.AddParameter().SetValue((float)124.5);
		floatarray.AddParameter().SetValue((float)31.4635);
		WriteResponse(proxy.Execute(echoFloatArray));

		/*
		*  This doesn't seem to be implemented on the server as of yet...
		*
		SOAPParameter& structure = echoStructure.AddParameter();
		structure.AddParameter("integer").SetValue(45);
		structure.AddParameter("string").SetValue("This is a string in a structure");
		structure.AddParameter("floatarray") = floatarray;
		WriteResponse(proxy.Execute(echoStructure));
		*/

		std::cout << "Success!" << std::endl << std::endl;
	}
	catch (SOAPException& ex)
	{
		std::cout << "Caught SOAP exception: " << ex.What() << std::endl;
	}
}

void TestInterop(const char *name, const char *url)
{
	std::cout << "Testing " << name << " interopability." << std::endl;

	SOAPProxy proxy(url);
	TestInterop(proxy);
}

int
main(int argc, char* argv[])
{
	try
	{
		// See: http://www.xmethods.net/ilab/ilab.html
		TestInterop("Apache 2.1 RC",			"http://services.xmethods.net/soap/servlet/rpcrouter");
		TestInterop("4s4c 1.3",					"http://services2.xmethods.net/ssss4c/ilab/soap.asp");
		//TestInterop("MS SOAP 2.0 B1",			"http://services.xmethods.net/XMethodsInterop/XMethodsInterop.asp");
		//TestInterop("MS .NET Beta 1",			"http://services2.xmethods.net/DotNet/XMInterop.asmx");
		TestInterop("SOAP::Lite 0.46",			"http://services.xmethods.net/perl/soaplite.cgi");
		//TestInterop("White Mesa SOAP RPC 1.4",	"http://delta/interop");

		// Lets go over the internet and make some calls
		// some methods from www.xmethods.com
		/* Is lemurlabs.com gone?
		GetInternetTime();
		Fortune();
		FortuneDictionaryList();
		Fortune("linuxcookie");
		*/


		//Whois("scitegic.com");

		std::cout << "Temperature in La Jolla, CA: ";
		std::cout.flush();
		std::cout << GetTemperature("92122") << std::endl;

		std::cout << "Delayed quotes: " << std::endl;
		std::cout.flush();
		std::cout << "MSFT " << StockQuote("MSFT") << std::endl;
		std::cout << "IBM  " << StockQuote("IBM") << std::endl;
		std::cout << "DELL " << StockQuote("DELL") << std::endl;
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

