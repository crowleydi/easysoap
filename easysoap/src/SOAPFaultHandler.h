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


#if !defined(AFX_SOAPFAULTHANDLER_H__84DF153C_6C26_4F4C_A0EC_FCC63C1704E5__INCLUDED_)
#define AFX_SOAPFAULTHANDLER_H__84DF153C_6C26_4F4C_A0EC_FCC63C1704E5__INCLUDED_

#include "SOAPParameterHandler.h"

class SOAPFault;

class SOAPFaultHandler : public SOAPParseEventHandler  
{
public:
	SOAPFaultHandler(SOAPFault& fault);
	virtual ~SOAPFaultHandler();

	virtual SOAPParseEventHandler* start(SOAPParser& parser, const XML_Char *name, const XML_Char **attrs);
	virtual SOAPParseEventHandler* startElement(SOAPParser& parser, const XML_Char *name, const XML_Char **attrs);

	static const char *start_tag;
private:
	SOAPFault				*m_fault;
	SOAPParameterHandler	m_paramHandler;
};

#endif // !defined(AFX_SOAPFAULTHANDLER_H__84DF153C_6C26_4F4C_A0EC_FCC63C1704E5__INCLUDED_)

