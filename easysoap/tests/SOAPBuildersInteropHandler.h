// SOAPBuildersInteropHandler.h: interface for the SOAPBuildersInteropHandler class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOAPBUILDERSINTEROPHANDLER_H__C0C1D23F_D31D_4118_BB27_C85AB1FA7645__INCLUDED_)
#define AFX_SOAPBUILDERSINTEROPHANDLER_H__C0C1D23F_D31D_4118_BB27_C85AB1FA7645__INCLUDED_

#include <SOAPDispatchHandler.h>

class SOAPBuildersInteropHandler :
	public SOAPDispatchHandler<SOAPBuildersInteropHandler>
{
public:
	SOAPBuildersInteropHandler();
	virtual ~SOAPBuildersInteropHandler();

	void echoVoid(const SOAPMethod& req, SOAPMethod& response);
	void echoInteger(const SOAPMethod& req, SOAPMethod& response);
	void echoFloat(const SOAPMethod& req, SOAPMethod& response);
	void echoString(const SOAPMethod& req, SOAPMethod& response);
	void echoStruct(const SOAPMethod& req, SOAPMethod& response);
	void echoIntegerArray(const SOAPMethod& req, SOAPMethod& response);
	void echoFloatArray(const SOAPMethod& req, SOAPMethod& response);
	void echoStringArray(const SOAPMethod& req, SOAPMethod& response);
	void echoStructArray(const SOAPMethod& req, SOAPMethod& response);
};

#endif // !defined(AFX_SOAPBUILDERSINTEROPHANDLER_H__C0C1D23F_D31D_4118_BB27_C85AB1FA7645__INCLUDED_)
