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
//   A Simple CGI Program for EasySoap++
//
//
//

#include <SOAP.h>

#include <stdio.h>
#include <SOAPCGIHandler.h>

class DemoCalculatorHandler : public SOAPDispatchHandler<DemoCalculatorHandler>
{
public:
	DemoCalculatorHandler()
	{
		DispatchTo(this);
		DispatchMethod("add", &DemoCalculatorHandler::add);
		DispatchMethod("mult", &DemoCalculatorHandler::mult);
	}

	~DemoCalculatorHandler()
	{
	}

	void add(const SOAPMethod& request, SOAPMethod& response)
	{
		int a;
		int b;

		request.GetParameter("a") >> a;
		request.GetParameter("b") >> b;

		int n = a + b;

		response.AddParameter("n") << n;
		response.SetName("addResponse", request.GetNamespace());
	}

	void mult(const SOAPMethod& request, SOAPMethod& response)
	{
		int a;
		int b;

		request.GetParameter("a") >> a;
		request.GetParameter("b") >> b;

		int n = a * b;

		response.AddParameter("n") << n;
		response.SetName("addResponse", request.GetNamespace());
	}
};

//
//
// main for the CGI handler;
//
int
main(int argc, char* argv[], char *env[])
{
	SOAPCGIDispatch cgi;
	DemoCalculatorHandler calchandle;

	return cgi.Dispatch("http://easysoap.sourceforge.net/demos/calculator", &calchandle).Handle();
}

