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

#ifdef _WIN32
#pragma warning (disable: 4786)
#endif // _WIN32

#include <expat.h>


#include "SOAP.h"
#include "SOAPParse.h"
#include "SOAPResponseHandler.h"


#define BUFF_SIZE 1024

template <typename T>
inline T
Minimum(const T& a, const T& b)
{
	return (a > b) ? b : a;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SOAPParser::SOAPParser()
{
	m_parser = 0;
}

SOAPParser::~SOAPParser()
{
}


SOAPResponse&
SOAPParser::Parse(SOAPResponse& resp, SOAPTransport& trans)
{
	int msglen = trans.GetPayloadSize();
	int done = 0;

	if (m_parser)
	{
		XML_ParserFree(m_parser);
		m_parser = 0;
	}

	m_parser = XML_ParserCreateNS(NULL, '|');
	XML_SetUserData(m_parser, this);
	XML_SetElementHandler(m_parser, SOAPParser::startElement, SOAPParser::endElement);
	XML_SetCharacterDataHandler(m_parser, SOAPParser::characterData);

	// make sure our stack is empth
	while (!m_handlerstack.empty())
		m_handlerstack.pop();

	SOAPResponseHandler response(resp);
	m_response = &response;

	// put ourselves on the stack
	m_handlerstack.push(this);

	while (!done)
	{
		//
		// create a buffer to read the HTTP payload into
		//
		void *buffer = XML_GetBuffer(m_parser, BUFF_SIZE);
		if (!buffer)
			throw SOAPMemoryException();

		//
		// read the HTTP payload
		//
		int read = trans.Read((char *)buffer, Minimum(BUFF_SIZE, msglen));

		//
		// figure out if we've finished reading the payload
		//
		if ((msglen -= read) == 0)
			done = 1;
	
		if (!XML_ParseBuffer(m_parser, read, done))
		{
			throw SOAPException(
				"Error while parsing SOAP XML payload: %s",
				XML_ErrorString(XML_GetErrorCode(m_parser)));
		}
	}

	if (m_parser)
	{
		XML_ParserFree(m_parser);
		m_parser = 0;
	}

	return resp;
}

//
// SOAPParseEventHandler methods
//

SOAPParseEventHandler *
SOAPParser::start(const XML_Char *name, const XML_Char **attrs)
{
	//
	// this method should never be called
	//

	return 0;
}

SOAPParseEventHandler *
SOAPParser::startElement(const XML_Char *name, const XML_Char **attrs)
{
	if (strcmp(name, SOAPResponseHandler::start_tag) == 0)
		return m_response->start(name, attrs);

	return 0;
}

void
SOAPParser::characterData(const XML_Char *str, int len)
{
}

void
SOAPParser::endElement(const XML_Char *name)
{
}

//
// static methods
//

void
SOAPParser::startElement(void *userData, const XML_Char *name, const XML_Char **attrs)
{
	SOAPParser *parser = static_cast<SOAPParser *>(userData);
	SOAPParseEventHandler* handler = parser->m_handlerstack.top();
	if (handler)
		parser->m_handlerstack.push(handler->startElement(name, attrs));
	else
		parser->m_handlerstack.push(0);
}

void
SOAPParser::endElement(void *userData, const XML_Char *name)
{
	SOAPParser *parser = static_cast<SOAPParser *>(userData);
	SOAPParseEventHandler* handler = parser->m_handlerstack.top();
	if (handler)
		handler->endElement(name);
	parser->m_handlerstack.pop();
}

void
SOAPParser::characterData(void *userData, const XML_Char *str, int len)
{
	SOAPParser *parser = static_cast<SOAPParser *>(userData);
	SOAPParseEventHandler* handler = parser->m_handlerstack.top();
	if (handler)
		handler->characterData(str, len);
}


