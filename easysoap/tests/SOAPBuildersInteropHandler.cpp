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

	//
	// Group A/base methods
	DispatchMethod("echoString",	interop_namespace,
		&SOAPBuildersInteropHandler::echoString);

	DispatchMethod("echoStringArray",	interop_namespace,
		&SOAPBuildersInteropHandler::echoStringArray);

	DispatchMethod("echoInteger",	interop_namespace,
		&SOAPBuildersInteropHandler::echoInteger);

	DispatchMethod("echoIntegerArray",	interop_namespace,
		&SOAPBuildersInteropHandler::echoIntegerArray);

	DispatchMethod("echoFloat",		interop_namespace,
		&SOAPBuildersInteropHandler::echoFloat);

	DispatchMethod("echoFloatArray",	interop_namespace,
		&SOAPBuildersInteropHandler::echoFloatArray);

	DispatchMethod("echoStruct",	interop_namespace,
		&SOAPBuildersInteropHandler::echoStruct);

	DispatchMethod("echoStructArray",	interop_namespace,
		&SOAPBuildersInteropHandler::echoStructArray);

	DispatchMethod("echoVoid",	interop_namespace,
		&SOAPBuildersInteropHandler::echoVoid);

	DispatchMethod("echoBase64",	interop_namespace,
		&SOAPBuildersInteropHandler::echoBase64);

	DispatchMethod("echoHexBinary",	interop_namespace,
		&SOAPBuildersInteropHandler::echoHexBinary);

	DispatchMethod("echoDate",	interop_namespace,
		&SOAPBuildersInteropHandler::echoDate);

	DispatchMethod("echoDecimal",	interop_namespace,
		&SOAPBuildersInteropHandler::echoDecimal);

	DispatchMethod("echoBoolean",	interop_namespace,
		&SOAPBuildersInteropHandler::echoBoolean);

	//
	// Group B methods
	DispatchMethod("echoSimpleTypesAsStruct",	interop_namespace,
		&SOAPBuildersInteropHandler::echoSimpleTypesAsStruct);

	DispatchMethod("echoStructAsSimpleTypes",	interop_namespace,
		&SOAPBuildersInteropHandler::echoStructAsSimpleTypes);

	DispatchMethod("echo2DStringArray",	interop_namespace,
		&SOAPBuildersInteropHandler::echo2DStringArray);

	DispatchMethod("echoNestedStruct",	interop_namespace,
		&SOAPBuildersInteropHandler::echoNestedStruct);

	DispatchMethod("echoNestedArray",	interop_namespace,
		&SOAPBuildersInteropHandler::echoNestedArray);

	//
	// Miscellaneous methods
	DispatchMethod("echoMap",	interop_namespace,
		&SOAPBuildersInteropHandler::echoMap);
}

SOAPBuildersInteropHandler::~SOAPBuildersInteropHandler()
{

}


void
SOAPBuildersInteropHandler::echoVoid(const SOAPMethod& req, SOAPMethod& resp)
{
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
	SOAPStruct val;

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
	SOAPArray<SOAPStruct> val;

	req.GetParameter("inputStructArray") >> val;

	resp.AddParameter("return") << val;
}


void
SOAPBuildersInteropHandler::echoBase64(const SOAPMethod& req, SOAPMethod& resp)
{
	SOAPArray<char> val;

	req.GetParameter("inputBase64") >> SOAPBase64(val);
	resp.AddParameter("return") << SOAPBase64(val);
}


void
SOAPBuildersInteropHandler::echoHexBinary(const SOAPMethod& req, SOAPMethod& resp)
{
	SOAPArray<char> val;

	req.GetParameter("inputHexBinary") >> SOAPHex(val);
	resp.AddParameter("return") << SOAPHex(val);
}


void
SOAPBuildersInteropHandler::echoDate(const SOAPMethod& req, SOAPMethod& resp)
{
	SOAPString val;

	req.GetParameter("inputDate") >> val;
	// we don't support the type natively so we have to set it.
	(resp.AddParameter("return") << val).SetType("dateTime");
}


void
SOAPBuildersInteropHandler::echoBoolean(const SOAPMethod& req, SOAPMethod& resp)
{
	bool val;

	req.GetParameter("inputBoolean") >> val;
	resp.AddParameter("return") << val;
}


void
SOAPBuildersInteropHandler::echoDecimal(const SOAPMethod& req, SOAPMethod& resp)
{
	SOAPString val;

	req.GetParameter("inputDecimal") >> val;
	// we don't support the type natively so we have to set it.
	(resp.AddParameter("return") << val).SetType("decimal");
}


void
SOAPBuildersInteropHandler::echoStructAsSimpleTypes(const SOAPMethod& req, SOAPMethod& resp)
{
	SOAPStruct val;
	req.GetParameter("inputStruct") >> val;

	resp.AddParameter("outputString") << val.varString;
	resp.AddParameter("outputInteger") << val.varInt;
	resp.AddParameter("outputFloat") << val.varFloat;
}


void
SOAPBuildersInteropHandler::echoSimpleTypesAsStruct(const SOAPMethod& req, SOAPMethod& resp)
{
	SOAPStruct val;
	req.GetParameter("inputString") >> val.varString;
	req.GetParameter("inputInteger") >> val.varInt;
	req.GetParameter("inputFloat") >> val.varFloat;

	resp.AddParameter("return") << val;
}


void
SOAPBuildersInteropHandler::echo2DStringArray(const SOAPMethod& req, SOAPMethod& resp)
{
	SOAP2DArray< SOAPString > val;
	req.GetParameter("input2DStringArray") >> val;
	resp.AddParameter("return") << val;
}


void
SOAPBuildersInteropHandler::echoNestedStruct(const SOAPMethod& req, SOAPMethod& resp)
{
	SOAPStructStruct val;
	req.GetParameter("inputStruct") >> val;
	resp.AddParameter("return") << val;
}


void
SOAPBuildersInteropHandler::echoNestedArray(const SOAPMethod& req, SOAPMethod& resp)
{
	SOAPArrayStruct val;
	req.GetParameter("inputStruct") >> val;
	resp.AddParameter("return") << val;
}


class SOAPTypeTraits< SOAPHashMap<SOAPString, int> > : public SOAPMapTypeTraits
{
};


void
SOAPBuildersInteropHandler::echoMap(const SOAPMethod& req, SOAPMethod& resp)
{
	SOAPHashMap<SOAPString, int> val;
	req.GetParameter((int)0) >> val;
	resp.AddParameter("return") << val;
}


