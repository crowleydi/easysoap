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
 *
 * $Id$
 */

#include <SOAPServerDispatch.h>
#include <SOAPDispatchHandler.h>



//
// Special exception for mustUnderstand faults
// so we can return the correct faultstring
class SOAPMustUnderstandException : public SOAPException
{
public:
	SOAPMustUnderstandException(const SOAPString& what)
		: SOAPException(what) {}
	SOAPMustUnderstandException(const char *fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		FormattedMessage(fmt, args);
		va_end(args);
	}
	~SOAPMustUnderstandException() {}
};


SOAPServerDispatch::SOAPServerDispatch()
	: m_transport(0)
{
}


SOAPServerDispatch::~SOAPServerDispatch()
{
}


SOAPServerDispatch&
SOAPServerDispatch::DispatchTo(SOAPDispatchHandlerInterface *disp)
{
	m_handlers.Add(disp);
	return *this;
}

SOAPServerDispatch&
SOAPServerDispatch::DispatchTo(SOAPHeaderHandlerInterface *disp)
{
	m_headerHandlers.Add(disp);
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

	m_transport->SetError();
	m_transport->Write(m_response.GetBody().GetMethod(), m_writer.GetBytes(), m_writer.GetLength());
}



int
SOAPServerDispatch::Handle(SOAPTransport& trans)
{
	int retval = 0;
	const char *serverfault = "SOAP-ENV:Server";
	const char *clientfault = "SOAP-ENV:Client";
	const char *faultcode = serverfault;

	m_transport = &trans;

	try
	{
		m_request.Reset();
		m_response.Reset();

		// Parse the SOAP packet
		faultcode = clientfault;
		m_parser.Parse(m_request, trans);
		faultcode = serverfault;

		SOAPMethod& requestMethod = m_request.GetBody().GetMethod();
		requestMethod.SetSoapAction(trans.GetSoapAction());

		SOAPMethod& responseMethod = m_response.GetBody().GetMethod();

		//
		// Set up the "suggested" method return name.  Actual
		// method can change it.  In future we set it up with
		// WSDL.
		m_respname = requestMethod.GetName().GetName();
		m_respname.Append("Response");
		responseMethod.SetName(m_respname, requestMethod.GetName().GetNamespace());

		//
		// Handle any headers we have...
		HandleHeaders(m_request, m_response);

		//
		// Now handle the request
		if (!HandleRequest(m_request, m_response))
		{
			faultcode = clientfault;
			throw SOAPException("Unknown method \"{%s}:%s\"",
				(const char *)requestMethod.GetName().GetNamespace(),
				(const char *)requestMethod.GetName().GetName());
		}

		//
		// Compose our SOAP packet response
		m_response.WriteSOAPPacket(m_writer);
		m_response.GetBody().GetMethod().Reset();

		//
		// Send back the repsonse.
		m_transport->Write(m_response.GetBody().GetMethod(),
			m_writer.GetBytes(),
			m_writer.GetLength());
	}
	catch(SOAPMustUnderstandException& mux)
	{
		//
		// create SOAPFault
		//
		retval = -1;
		WriteFault("SOAP-ENV:MustUnderstand", mux.What());
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

bool
SOAPServerDispatch::HandleRequest(SOAPEnvelope& request, SOAPResponse& response)
{
	//
	// TODO:  This is an O(n) lookup... but n is (hopefully!) small
	bool handled = false;
	for (Handlers::Iterator i = m_handlers.Begin(); i != m_handlers.End(); ++i)
	{
		//
		// We found a handler.  Now dispatch the method
		if ((*i)->ExecuteMethod(request, response.GetBody().GetMethod()))
		{
			handled = true;
			break;
		}
	}

	return handled;
}

void
SOAPServerDispatch::HandleHeaders(SOAPEnvelope& request, SOAPResponse& response)
{
	const SOAPHeader::Headers& headers = request.GetHeader().GetAllHeaders();
	for (SOAPHeader::Headers::ConstIterator h = headers.Begin(); h != headers.End(); ++h)
	{
		const SOAPParameter& header = **h;
		SOAPParameter::Attrs::Iterator actor = header.GetAttributes().Find(SOAPEnv::actor);
		SOAPParameter::Attrs::Iterator mu = header.GetAttributes().Find(SOAPEnv::mustUnderstand);

		if (!mu || (*mu == "0" || *mu == "1"))
		{
			//
			// TODO: Be able to specify/check for a custom QName for this endpoint
			if (!actor || (*actor == SOAP_ACTOR_NEXT))
			{
				bool handled = false;
				//
				// TODO:  This is an O(n) lookup... but n is (hopefully!) small
				for (HeaderHandlers::Iterator i = m_headerHandlers.Begin(); i != m_headerHandlers.End(); ++i)
				{
					//
					// We found a handler.  Now dispatch the method
					if ((*i)->HandleHeader(header, request, response))
					{
						handled = true;
						break;
					}
				}

				if (!handled)
				{
					//
					// check for mustUnderstand == 1
					if (mu && *mu == "1")
						// TODO:  Special MustUnderstand exception so the
						// actor(?) in the SOAPFault can be set correctly.
						throw SOAPMustUnderstandException("Failed to understand header \"{%s}:%s\"",
							(const char *)header.GetName().GetNamespace(),
							(const char *)header.GetName().GetName());
				}
			}
		}
		else
		{
			throw SOAPMustUnderstandException("Invalid value for mustUnderstand attribute on header {%s}:%s: %s",
				(const char *)header.GetName().GetNamespace(),
				(const char *)header.GetName().GetName(),
				(const char *)mu->GetName());
		}
	}
}

void
SOAPTransport::ParseContentType(SOAPString& str, const char *contenttype)
{
	str = "US-ASCII";
	if (contenttype)
	{
		const char *charset = sp_strstr(contenttype, "charset=");
		if (charset)
		{
			charset += 8;
			if (*charset == '\"')
				++charset;
			const char *end = charset;

			while (*end && *end != '\"' && *end != ';' && *end != ' ')
				++end;

			str = "";
			str.Append(charset, end - charset);
		}
	}
}

