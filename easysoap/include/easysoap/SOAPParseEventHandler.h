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


#if !defined(AFX_SOAPPARSEEVENTHANDLER_H__144ECE87_83A1_49B0_B48B_5112E1FFDA99__INCLUDED_)
#define AFX_SOAPPARSEEVENTHANDLER_H__144ECE87_83A1_49B0_B48B_5112E1FFDA99__INCLUDED_

#ifdef _MSC_VER
#pragma warning(disable: 4275)
#endif // _MSC_VER

#include "es_conf.h"


BEGIN_EASYSOAP_NAMESPACE

class SOAPParser;

/**
*
*/
class SOAPParseEventHandler
{
public:
	SOAPParseEventHandler()
	{
	}

	virtual ~SOAPParseEventHandler() {}

	virtual SOAPParseEventHandler* start(
			SOAPParser& parser,
			const char *name,
			const char **attrs) = 0;

	virtual SOAPParseEventHandler* startElement(
			SOAPParser& parser,
			const char *name,
			const char **attrs) = 0;

	virtual void endElement(
			SOAPParser& parser,
			const char *name)
	{
		endElement(name);
	};

	virtual void endElement(
			const char * /*name*/)
	{
	};

	virtual void characterData(
			SOAPParser& parser,
			const char * str,
			int len)
	{
		characterData(str, len);
	};

	virtual void characterData(
			const char * /*str*/,
			int /*len*/)
	{
	};
};

END_EASYSOAP_NAMESPACE

#endif // !defined(AFX_SOAPPARSEEVENTHANDLER_H__144ECE87_83A1_49B0_B48B_5112E1FFDA99__INCLUDED_)

