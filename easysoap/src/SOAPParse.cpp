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

#ifdef _MSC_VER
#pragma warning (disable: 4786)
#endif // _MSC_VER

#include <expat.h>


#include "SOAP.h"
#include "SOAPParse.h"
#include "SOAPResponseHandler.h"

#include "SOAPNamespaces.h"

#define BUFF_SIZE 1024

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
	if (m_parser)
	{
		XML_ParserFree(m_parser);
		m_parser = 0;
	}

	m_parser = XML_ParserCreateNS((const char *)NULL, (char)PARSER_NS_SEP[0]);
	XML_SetUserData(m_parser, this);

	XML_SetElementHandler(m_parser,
			SOAPParser::_startElement,
			SOAPParser::_endElement);

	XML_SetCharacterDataHandler(m_parser,
			SOAPParser::_characterData);

	XML_SetStartNamespaceDeclHandler(m_parser,
			SOAPParser::_startNamespace);

	XML_SetEndNamespaceDeclHandler(m_parser,
			SOAPParser::_endNamespace);

	SOAPResponseHandler response(resp);
	m_response = &response;

	// make sure our stack is empty
	m_handlerstack.Clear();
	while (1)
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
		int read = trans.Read((char *)buffer, BUFF_SIZE);

		if (!XML_ParseBuffer(m_parser, read, read == 0))
		{
			throw SOAPException(
				"Error while parsing SOAP XML payload: %s",
				XML_ErrorString(XML_GetErrorCode(m_parser)));
		}

		if (m_response->Done())
		{
			XML_ParseBuffer(m_parser, 0, 1);
			break;
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
	SOAPParseEventHandler* handler = 0;
	if (m_handlerstack.IsEmpty())
	{
		if (sp_strcmp(name, SOAPResponseHandler::start_tag) == 0)
		{
			handler = m_response;
		}
		else
		{
			//
			// FIXME:
			// Probably what we should do instead of throw is set
			// a flag that says the response is invalid.  We usually
			// get in here when the HTTP response code is 500 and it
			// gives us back some HTML instead of a SOAP Fault.
			throw SOAPException("Unknown SOAP response tag: %s", name);
		}
	}
	else
	{
		handler = m_handlerstack.Top();
	}

	if (handler)
	{
		const XML_Char **cattrs = attrs;
		while (*cattrs)
		{
			const char *tag = *cattrs++;
			const char *val = *cattrs;
			if (sp_strcmp(tag, FULL_SOAP_XSI PARSER_NS_SEP "type") == 0)
			{
				NamespaceMap::Iterator i = m_nsmap.Find(val);
				if (i)
				{
					// we already mapped it!
					*cattrs = *i;
				}
				else
				{
					char *colon = sp_strchr(val, ':');
					if (colon)
					{
						*colon++ = 0;
						NamespaceMap::Iterator i = m_nsmap.Find(val);
						if (i)
						{
							m_work = *i;
							m_work.Append(PARSER_NS_SEP);
							m_work.Append(colon);
							*--colon = ':';
							*cattrs = m_nsmap[val] = m_work;
						}
						else
						{
							// TODO:
							// this is probably an error...
						}
					}
				}
			}
			++cattrs;
		}
		return m_handlerstack.Push(handler->startElement(name, attrs));
	}

	return m_handlerstack.Push(0);
}

void
SOAPParser::characterData(const XML_Char *str, int len)
{
	SOAPParseEventHandler* handler = m_handlerstack.Top();
	if (handler)
		handler->characterData(str, len);
}

void
SOAPParser::endElement(const XML_Char *name)
{
	SOAPParseEventHandler* handler = m_handlerstack.Top();
	if (handler)
		handler->endElement(name);
	m_handlerstack.Pop();
}

void
SOAPParser::startNamespace(const XML_Char *prefix, const XML_Char *uri)
{
	m_nsmap[prefix] = uri;
}

void
SOAPParser::endNamespace(const XML_Char *prefix)
{
	m_nsmap.Remove(prefix);
}

//
// static methods
//

void
SOAPParser::_startElement(void *userData, const XML_Char *name, const XML_Char **attrs)
{
	SOAPParser *parser = (SOAPParser *)userData;
	parser->startElement(name, attrs);
}

void
SOAPParser::_endElement(void *userData, const XML_Char *name)
{
	SOAPParser *parser = (SOAPParser *)userData;
	parser->endElement(name);
}

void
SOAPParser::_characterData(void *userData, const XML_Char *str, int len)
{
	SOAPParser *parser = (SOAPParser *)userData;
	parser->characterData(str, len);
}

void
SOAPParser::_startNamespace(void *userData, const XML_Char *prefix, const XML_Char *uri)
{
	SOAPParser *parser = (SOAPParser *)userData;
	parser->startNamespace(prefix, uri);
}

void
SOAPParser::_endNamespace(void *userData, const XML_Char *prefix)
{
	SOAPParser *parser = (SOAPParser *)userData;
	parser->endNamespace(prefix);
}


