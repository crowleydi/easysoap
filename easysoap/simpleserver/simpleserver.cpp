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
//   A Simple EasySoap++ SOAP server 
//
//   Just place this file in your cgi-bin directory (or similar)
//   of your web server and go.
//
//

#include <iostream>

#include <SOAP.h>
#include <SOAPHTTPServer.h>

class DemoCalculatorHandler : public SOAPDispatchHandler<DemoCalculatorHandler>
{
public:
	DemoCalculatorHandler()
	{
		const char * ns = "http://easysoap.sourceforge.net/demos/calculator";
		DispatchMethod("add", ns, &DemoCalculatorHandler::add);
		DispatchMethod("mult", ns, &DemoCalculatorHandler::mult);
	}

	DemoCalculatorHandler* GetTarget(const SOAPEnvelope& request)
	{
		return this;
	}

	void add(const SOAPMethod& request, SOAPMethod& response)
	{
		int a;
		int b;

		request.GetParameter("a") >> a;
		request.GetParameter("b") >> b;

		int n = a + b;

		response.AddParameter("n") << n;
	}

	void mult(const SOAPMethod& request, SOAPMethod& response)
	{
		int a;
		int b;

		request.GetParameter("a") >> a;
		request.GetParameter("b") >> b;

		int n = a * b;

		response.AddParameter("n") << n;
	}
};

//
//
// main for the server
//
int
main(int argc, const char* argv[])
{
	int port = 0;

	if (argc > 1)
		port = atoi(argv[1]);
	if (port == 0)
		port = 6060;

	try
	{
		SOAPHTTPServer server(port);
		DemoCalculatorHandler calchandle;

		std::cout << "Starting SOAP server on port " << port << "." << std::endl
			<< "Hit CTRL-C to stop." << std::endl;

		return server.DispatchTo(&calchandle).Handle();
	}
	catch (SOAPException&)
	{
		return 1;
	}
}

