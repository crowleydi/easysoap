// SOAPServerDispatch.cpp: implementation of the SOAPServerDispatch class.
//
//////////////////////////////////////////////////////////////////////

#include "SOAPServerDispatch.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SOAPServerDispatch::SOAPServerDispatch(SOAPTransport& transport, bool deltrans)
	: m_transport(0)
	, m_deltrans(false)
{
	SetTransport(transport, deltrans);
}

SOAPServerDispatch::~SOAPServerDispatch()
{
	if (m_deltrans)
		delete m_transport;
}


SOAPServerDispatch&
SOAPServerDispatch::SetTransport(SOAPTransport& transport, bool deltrans)
{
	if (m_deltrans)
		delete m_transport;
	m_transport = &transport;
	m_deltrans = deltrans;
	return *this;
}



SOAPServerDispatch&
SOAPServerDispatch::Dispatch(const char *ns, SOAPDispatchHandlerInterface *disp)
{
	m_handlers[ns] = disp;
	return *this;
}



//
//
// Quick method for writing a SOAP Fault
//
// TODO:  Fix things so we can write a Fault
// out the correct way....
//
void
SOAPServerDispatch::WriteFault(const char *code, const char *str)
{
	m_writer.Reset();
	m_writer.StartTag("SOAP-ENV:Envelope");
	m_writer.AddXMLNS("SOAP-ENV", SOAP_ENV);
	m_writer.StartTag("SOAP-ENV:Body");
	m_writer.StartTag("SOAP-ENV:Fault");
	m_writer.StartTag("faultcode");
	m_writer.WriteValue(code);
	m_writer.EndTag("faultcode");
	m_writer.StartTag("faultstring");
	m_writer.WriteValue(str);
	m_writer.EndTag("faultstring");
	m_writer.EndTag("SOAP-ENV:Fault");
	m_writer.EndTag("SOAP-ENV:Body");
	m_writer.EndTag("SOAP-ENV:Envelope");

	m_transport->Write(m_response.GetBody().GetMethod(), m_writer.GetBytes(), m_writer.GetLength());
}



int
SOAPServerDispatch::Handle()
{
	int retval = 0;
	const char *serverfault = "SOAP-ENV::Sever";
	const char *clientfault = "SOAP-ENV::Client";
	const char *faultcode = serverfault;
	try
	{
		SOAPParser parser;
		// Parse the SOAP packet
		faultcode = clientfault;
		parser.Parse(m_request, *m_transport);
		faultcode = serverfault;

		//
		// Dispatch the method
		const SOAPMethod& requestMethod = m_request.GetBody().GetMethod();
		SOAPMethod& responseMethod = m_response.GetBody().GetMethod();

		const char *requestNamespace = requestMethod.GetNamespace();
		HandlersMap::Iterator i = m_handlers.Find(requestNamespace);
		if (i)
		{
			// TODO:
			// Make sure SOAPAction is appropriate for found method.
			(*i)->ExecuteMethod(requestMethod, responseMethod);
		}
		else
		{
			faultcode = clientfault;
			throw SOAPException("Could not dispatch method to namespace: %s", requestNamespace);
		}

		//
		// Compose our SOAP packet response
		m_response.WriteSOAPPacket(m_writer);

		//
		// Send back the repsonse.
		m_transport->Write(m_response.GetBody().GetMethod(),
			m_writer.GetBytes(),
			m_writer.GetLength());
	}
	catch(SOAPException& sex)
	{
		//
		// create SOAPFault
		//
		retval = -1;
		WriteFault(faultcode, sex.What());
	}
	catch (...)
	{
		//
		// create SOAPFault
		//
		faultcode = serverfault;
		WriteFault(faultcode, "Serious error occurred.");
		retval = -1;
	}

	return retval;
}

