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
//
//
//   CGI Program for the SOAPBuilders Interoperability Lab
//   http://www.xmethods.net/ilab/
//
//
//

#include <SOAP.h>
#include <SOAPCGIHandler.h>

#include "interopstruct.h"

const char *interop_namespace = "http://soapinterop.org/";
//
//
// Quick method for writing a SOAP Fault
//
// TODO:  Fix things so we can write a Fault
// out the correct way....
//
void
WriteFault(const char *code, const char *str)
{
	printf("Content-Type: text/xml; charset=\"UTF-8\"\n\n");

	printf("<SOAP-ENV:Envelope xmlns:SOAP-ENV=\"http://schemas.xmlsoap.org/soap/envelope/\">");
	printf("<SOAP-ENV:Body>");
	printf("<SOAP-ENV:Fault>");
	printf("<faultcode>%s</faultcode>", code);
	printf("<faultstring>%s</faultstring>", str);
	printf("</SOAP-ENV:Fault>");
	printf("</SOAP-ENV:Body>");
	printf("</SOAP-ENV:Envelope>");
}

void
echoVoid(const SOAPMethod& req, SOAPMethod& resp)
{
	// We could do things like
	// check the number of passed in parameters
	// But that is really a job for WSDL.
	resp.SetName("echoVoidResponse", interop_namespace);
}

void
echoInteger(const SOAPMethod& req, SOAPMethod& resp)
{
	int val;

	req.GetParameter("inputInteger") >> val;

	resp.SetName("echoIntegerResponse", interop_namespace);
	resp.AddParameter("return") << val;
}

void
echoFloat(const SOAPMethod& req, SOAPMethod& resp)
{
	float val;

	req.GetParameter("inputFloat") >> val;

	resp.SetName("echoFloatResponse", interop_namespace);
	resp.AddParameter("return") << val;
}

void
echoString(const SOAPMethod& req, SOAPMethod& resp)
{
	SOAPString val;

	req.GetParameter("inputString") >> val;

	resp.SetName("echoStringResponse", interop_namespace);
	resp.AddParameter("return") << val;
}

void
echoStruct(const SOAPMethod& req, SOAPMethod& resp)
{
	SOAPInteropStruct val;

	req.GetParameter("inputStruct") >> val;

	resp.SetName("echoStructResponse", interop_namespace);
	resp.AddParameter("return") << val;
}

void
echoIntegerArray(const SOAPMethod& req, SOAPMethod& resp)
{
	SOAPArray<int> val;

	req.GetParameter("inputIntegerArray") >> val;

	resp.SetName("echoIntegerArrayResponse", interop_namespace);
	resp.AddParameter("return") << val;
}

void
echoFloatArray(const SOAPMethod& req, SOAPMethod& resp)
{
	SOAPArray<float> val;

	req.GetParameter("inputFloatArray") >> val;

	resp.SetName("echoFloatArrayResponse", interop_namespace);
	resp.AddParameter("return") << val;
}

void
echoStringArray(const SOAPMethod& req, SOAPMethod& resp)
{
	SOAPArray<SOAPString> val;

	req.GetParameter("inputStringArray") >> val;

	resp.SetName("echoStringArrayResponse", interop_namespace);
	resp.AddParameter("return") << val;
}

void
echoStructArray(const SOAPMethod& req, SOAPMethod& resp)
{
	SOAPArray<SOAPInteropStruct> val;

	req.GetParameter("inputStructArray") >> val;

	resp.SetName("echoStructArrayResponse", interop_namespace);
	resp.AddParameter("return") << val;
}

void
MethodHandler(const SOAPEnvelope& req, SOAPResponse& resp)
{
	SOAPMethod& respm = resp.GetBody().GetMethod();
	const SOAPMethod& meth = req.GetBody().GetMethod();

	const SOAPString& methname = meth.GetName().GetName();
	const SOAPString& methns = meth.GetName().GetNamespace();


	if (methns == "http://soapinterop.org/")
	{
		if (methname == "echoVoid")
		{
			echoVoid(meth, respm);
		}
		else if (methname == "echoInteger")
		{
			echoInteger(meth, respm);
		}
		else if (methname == "echoFloat")
		{
			echoFloat(meth, respm);
		}
		else if (methname == "echoString")
		{
			echoString(meth, respm);
		}
		else if (methname == "echoStruct")
		{
			echoStruct(meth, respm);
		}
		else if (methname == "echoIntegerArray")
		{
			echoIntegerArray(meth, respm);
		}
		else if (methname == "echoFloatArray")
		{
			echoFloatArray(meth, respm);
		}
		else if (methname == "echoStringArray")
		{
			echoStringArray(meth, respm);
		}
		else if (methname == "echoStructArray")
		{
			echoStructArray(meth, respm);
		}
		else
			throw SOAPException("Unknown method: %s[%s]",
				(const char *)methname, (const char *)methns);
	}
	else
	{
		throw SOAPException("Don't know how to handle methods in namespace '%s'",
			(const char *)methns);
	}
}

int
main(int argc, char* argv[], char *env[])
{
	int retval = 0;
	const char *faultcode = "SOAP::Sever";
	try
	{
		SOAPParser p;
		SOAPResponse resp;
		SOAPEnvelope env;
		SOAPPacketWriter w;
		SOAPCGIHandler cgi;
		//cgi.SetLogFile("C:/validate.log");
		//cgi.SetInFile("C:/validate.log");

		w.SetAddWhiteSpace();

		faultcode = "SOAP::Client";
		p.Parse(env, cgi);
		faultcode = "SOAP::Server";

		//
		// TODO: Test SOAPAction, method namespace
		//
		MethodHandler(env, resp);

		resp.WriteSOAPPacket(w);
		cgi.Write(resp.GetBody().GetMethod(), w.GetBytes(), w.GetLength());
	}
	catch(SOAPException& sex)
	{
		//
		// create SOAPFault
		//
		retval = 1;
		WriteFault(faultcode, sex.What());
	}
	catch (...)
	{
		//
		// create SOAPFault
		//
		faultcode = "SOAP::Server";
		WriteFault(faultcode, "Serious error occurred.");
		retval = 1;
	}


	return retval;
}

