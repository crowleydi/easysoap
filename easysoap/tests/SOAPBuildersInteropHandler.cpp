// SOAPBuildersInteropHandler.cpp: implementation of the SOAPBuildersInteropHandler class.
//
//////////////////////////////////////////////////////////////////////

#include "SOAPBuildersInteropHandler.h"
#include "interopstruct.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SOAPBuildersInteropHandler::SOAPBuildersInteropHandler()
{
	const char *interop_namespace = "http://soapinterop.org/";

	DispatchTo(this);

	DispatchMethod("echoVoid",	interop_namespace,
		&SOAPBuildersInteropHandler::echoVoid);

	DispatchMethod("echoInteger",	interop_namespace,
		&SOAPBuildersInteropHandler::echoInteger);

	DispatchMethod("echoFloat",		interop_namespace,
		&SOAPBuildersInteropHandler::echoFloat);

	DispatchMethod("echoString",	interop_namespace,
		&SOAPBuildersInteropHandler::echoString);

	DispatchMethod("echoStruct",	interop_namespace,
		&SOAPBuildersInteropHandler::echoStruct);

	DispatchMethod("echoIntegerArray",	interop_namespace,
		&SOAPBuildersInteropHandler::echoIntegerArray);

	DispatchMethod("echoFloatArray",	interop_namespace,
		&SOAPBuildersInteropHandler::echoFloatArray);

	DispatchMethod("echoStringArray",	interop_namespace,
		&SOAPBuildersInteropHandler::echoStringArray);

	DispatchMethod("echoStructArray",	interop_namespace,
		&SOAPBuildersInteropHandler::echoStructArray);

	DispatchMethod("echoDate",	interop_namespace,
		&SOAPBuildersInteropHandler::echoDate);

	DispatchMethod("echoBase64",	interop_namespace,
		&SOAPBuildersInteropHandler::echoBase64);
}

SOAPBuildersInteropHandler::~SOAPBuildersInteropHandler()
{

}


void
SOAPBuildersInteropHandler::echoVoid(const SOAPMethod& req, SOAPMethod& resp)
{
	// We could do things like
	// check the number of passed in parameters
	// But that is really a job for WSDL.
}

void
SOAPBuildersInteropHandler::echoInteger(const SOAPMethod& req, SOAPMethod& resp)
{
	int val;

	req.GetParameter("inputInteger") >> val;

	resp.AddParameter("return") << val;
}

void
SOAPBuildersInteropHandler::echoFloat(const SOAPMethod& req, SOAPMethod& resp)
{
	float val;

	req.GetParameter("inputFloat") >> val;

	resp.AddParameter("return") << val;
}

void
SOAPBuildersInteropHandler::echoString(const SOAPMethod& req, SOAPMethod& resp)
{
	SOAPString val;

	req.GetParameter("inputString") >> val;

	resp.AddParameter("return") << val;
}

void
SOAPBuildersInteropHandler::echoStruct(const SOAPMethod& req, SOAPMethod& resp)
{
	SOAPInteropStruct val;

	req.GetParameter("inputStruct") >> val;

	resp.AddParameter("return") << val;
}

void
SOAPBuildersInteropHandler::echoIntegerArray(const SOAPMethod& req, SOAPMethod& resp)
{
	SOAPArray<int> val;

	req.GetParameter("inputIntegerArray") >> val;

	resp.AddParameter("return") << val;
}

void
SOAPBuildersInteropHandler::echoFloatArray(const SOAPMethod& req, SOAPMethod& resp)
{
	SOAPArray<float> val;

	req.GetParameter("inputFloatArray") >> val;

	resp.AddParameter("return") << val;
}

void
SOAPBuildersInteropHandler::echoStringArray(const SOAPMethod& req, SOAPMethod& resp)
{
	SOAPArray<SOAPString> val;

	req.GetParameter("inputStringArray") >> val;

	resp.AddParameter("return") << val;
}

void
SOAPBuildersInteropHandler::echoStructArray(const SOAPMethod& req, SOAPMethod& resp)
{
	SOAPArray<SOAPInteropStruct> val;

	req.GetParameter("inputStructArray") >> val;

	resp.AddParameter("return") << val;
}


void
SOAPBuildersInteropHandler::echoBase64(const SOAPMethod& req, SOAPMethod& resp)
{
	SOAPArray<char> val;

	req.GetParameter("inputBase64") >> val;

	resp.AddParameter("return") << val;
}

void
SOAPBuildersInteropHandler::echoDate(const SOAPMethod& req, SOAPMethod& resp)
{
	SOAPString val;

	const SOAPParameter& inputDate = req.GetParameter("inputDate");
	inputDate >> val;

	(resp.AddParameter("return") << val).SetType("dateTime");
}


