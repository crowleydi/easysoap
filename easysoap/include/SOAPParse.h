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

class SOAPResponse;
class SOAPResponseHandler;

class EASYSOAP_EXPORT SOAPParser
{
public:

	SOAPParser();
	virtual ~SOAPParser();

	SOAPResponse& Parse(SOAPResponse& resp, SOAPTransport& trans);

	// Used by subscribers/handlers to resolve something like "xsi:integer" into
	// a fully qualified name (or whatever) "http://www.w3.org/1999/XMLSchema-instance#integer"
	void ResolveName(const char *name, SOAPString& result);
	SOAPParameter *GetHRefParam(const SOAPString& name);
	void SetHRefParam(const SOAPString&, SOAPParameter *);

private:

	void startElement(const XML_Char *name, const XML_Char **attrs);
	void endElement(const XML_Char *name);
	void characterData(const XML_Char *str, int len);
	void startNamespace(const XML_Char *prefix, const XML_Char *uri);
	void endNamespace(const XML_Char *prefix);

	//
	// the C based XML parser calls through to these methods
	// which pass them on to the C++ interface
	//
	static void _startElement(void *userData, const XML_Char *name, const XML_Char **attrs);
	static void _endElement(void *userData, const XML_Char *name);
	static void _characterData(void *userData, const XML_Char *str, int len);
	static void _startNamespace(void *userData, const XML_Char *prefix, const XML_Char *uri);
	static void _endNamespace(void *userData, const XML_Char *prefix);

	XML_Parser							m_parser;

	typedef SOAPStack<SOAPParseEventHandler *>	HandlerStack;
	typedef SOAPHashMap<SOAPString, SOAPString> NamespaceMap;
	typedef SOAPHashMap<SOAPString, SOAPParameter*> HRefMap;

	HandlerStack			m_handlerstack;
	SOAPString				m_work;
	SOAPResponseHandler		*m_response;
	NamespaceMap			m_nsmap;
	HRefMap					m_hrefmap;
};

#endif // !defined(AFX_SOAPPARSE_H__751545FF_EF84_42BC_9622_A6CE624F1F14__INCLUDED_)

