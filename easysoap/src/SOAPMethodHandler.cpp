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

#include "SOAPMethodHandler.h"
#include "SOAPMethod.h"
#include "SOAPNamespaces.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SOAPMethodHandler::SOAPMethodHandler(SOAPMethod& method)
: m_method(&method)
{

}

SOAPMethodHandler::~SOAPMethodHandler()
{

}

SOAPParseEventHandler *
SOAPMethodHandler::start(SOAPParser& parser, const XML_Char *name, const XML_Char **attrs)
{
	m_method->Reset();
	m_method->SetName(name);
	return this;
}

SOAPParseEventHandler *
SOAPMethodHandler::startElement(SOAPParser& parser, const XML_Char *name, const XML_Char **attrs)
{
	m_paramHandler.SetParameter(m_method->AddParameter());
	return m_paramHandler.start(parser, name, attrs);
}


