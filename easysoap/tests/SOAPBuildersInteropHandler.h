// SOAPBuildersInteropHandler.h: interface for the SOAPBuildersInteropHandler class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOAPBUILDERSINTEROPHANDLER_H__C0C1D23F_D31D_4118_BB27_C85AB1FA7645__INCLUDED_)
#define AFX_SOAPBUILDERSINTEROPHANDLER_H__C0C1D23F_D31D_4118_BB27_C85AB1FA7645__INCLUDED_

#include <SOAPDispatchHandler.h>

//
// See http://www.whitemesa.com/interop.htm
//
class SOAPBuildersInteropHandler :
	public SOAPDispatchHandler<SOAPBuildersInteropHandler>
{
public:
	SOAPBuildersInteropHandler();
	virtual ~SOAPBuildersInteropHandler();

	SOAPBuildersInteropHandler* GetTarget(const SOAPEnvelope& request)
	{
		return this;
	}

	//
	// Round 1
	void echoString(const SOAPMethod& req, SOAPMethod& response);
	void echoStringArray(const SOAPMethod& req, SOAPMethod& response);
	void echoInteger(const SOAPMethod& req, SOAPMethod& response);
	void echoIntegerArray(const SOAPMethod& req, SOAPMethod& response);
	void echoFloat(const SOAPMethod& req, SOAPMethod& response);
	void echoFloatArray(const SOAPMethod& req, SOAPMethod& response);
	void echoStruct(const SOAPMethod& req, SOAPMethod& response);
	void echoStructArray(const SOAPMethod& req, SOAPMethod& response);
	void echoVoid(const SOAPMethod& req, SOAPMethod& response);

	//
	// Round 2 Base
	void echoBase64(const SOAPMethod& req, SOAPMethod& response);
	void echoDate(const SOAPMethod& req, SOAPMethod& response);
	void echoDecimal(const SOAPMethod& req, SOAPMethod& response);
	void echoBoolean(const SOAPMethod& req, SOAPMethod& response);

	//
	// Group B test suite
	void echoStructAsSimpleTypes(const SOAPMethod& req, SOAPMethod& response);
	void echoSimpleTypesAsStruct(const SOAPMethod& req, SOAPMethod& response);
	void echo2DStringArray(const SOAPMethod& req, SOAPMethod& response);
	void echoNestedStruct(const SOAPMethod& req, SOAPMethod& response);
	void echoNestedArray(const SOAPMethod& req, SOAPMethod& response);

	//
	// Miscellaneous methods..
	void echoMap(const SOAPMethod& req, SOAPMethod& response);
	void echoDouble(const SOAPMethod& req, SOAPMethod& response);
	void echoDoubleArray(const SOAPMethod& req, SOAPMethod& response);
	void echoHexBinary(const SOAPMethod& req, SOAPMethod& response);
};

class SOAPBuildersHeaderHandler
: public SOAPHeaderHandler<SOAPBuildersHeaderHandler>
{
private:
	static const char *echoHeaderNamespace;

public:
	SOAPBuildersHeaderHandler();
	virtual ~SOAPBuildersHeaderHandler();

	SOAPBuildersHeaderHandler* GetTarget(const SOAPEnvelope& request)
	{
		return this;
	}

	//
	// Group C header handlers
	void echoHdrString(const SOAPParameter& header, SOAPEnvelope& request, SOAPEnvelope& response);
	void echoHdrStruct(const SOAPParameter& header, SOAPEnvelope& request, SOAPEnvelope& response);
};

#endif // !defined(AFX_SOAPBUILDERSINTEROPHANDLER_H__C0C1D23F_D31D_4118_BB27_C85AB1FA7645__INCLUDED_)
