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


#if !defined(AFX_SOAPPARSE_H__751545FF_EF84_42BC_9622_A6CE624F1F14__INCLUDED_)
#define AFX_SOAPPARSE_H__751545FF_EF84_42BC_9622_A6CE624F1F14__INCLUDED_

#include "SOAPTransport.h"
#include "SOAPParseEventHandler.h"
#include "SOAPStack.h"

// avoid include of expat.h
typedef void *XML_Parser;

class SOAPResponse;
class SOAPResponseHandler;

class EASYSOAP_EXPORT SOAPParser : private SOAPParseEventHandler
{
public:

	SOAPParser();
	virtual ~SOAPParser();

	SOAPResponse& Parse(SOAPResponse& resp, SOAPTransport& trans);
private:

	SOAPParseEventHandler* start(const XML_Char *name, const XML_Char **attrs);
	SOAPParseEventHandler* startElement(const XML_Char *name, const XML_Char **attrs);
	void endElement(const XML_Char *name);
	void characterData(const XML_Char *str, int len);

	//
	// the C based XML parser calls through to these methods
	// which pass them on to the C++ interface
	//
	static void startElement(void *userData, const XML_Char *name, const XML_Char **attrs);
	static void endElement(void *userData, const XML_Char *name);
	static void characterData(void *userData, const XML_Char *str, int len);

	XML_Parser							m_parser;

	//
	// the class that handles parsing events
	//
	SOAPResponseHandler					*m_response;

	//
	// our parsing stack
	//
	SOAPStack<SOAPParseEventHandler *>	m_handlerstack;

};

#endif // !defined(AFX_SOAPPARSE_H__751545FF_EF84_42BC_9622_A6CE624F1F14__INCLUDED_)

