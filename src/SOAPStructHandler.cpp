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


#ifdef _MSC_VER
#pragma warning (disable: 4786)
#endif // _MSC_VER

#include "SOAPStructHandler.h"
#include "SOAPParameterHandler.h"

#include <easysoap/SOAPParameter.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

USING_EASYSOAP_NAMESPACE

SOAPStructHandler::SOAPStructHandler()
{
	m_paramHandler = new SOAPParameterHandler();
}

SOAPStructHandler::~SOAPStructHandler()
{
	delete m_paramHandler;
}

SOAPParseEventHandler *
SOAPStructHandler::start(SOAPParser&, const char *, const char **)
{
	m_param->SetIsStruct();
	return this;
}

SOAPParseEventHandler *
SOAPStructHandler::startElement(SOAPParser& parser, const char *name, const char **attrs)
{
	const char *id = 0;
	const char *href = 0;

	const char **cattrs = attrs;
	while (*cattrs)
	{
		const char *tag = *cattrs++;
		const char *val = *cattrs++;

		if (sp_strcmp(tag, "id") == 0)
		{
			id = val;
			break;
		}
		else if (sp_strcmp(tag, "href") == 0)
		{
			href = val;
			break;
		}
	}

	SOAPParameter *param = &m_param->AddParameter(name);

	if (href)
		parser.SetHRefParam(param);
	if (id)
		parser.SetIdParam(id, param);

	m_paramHandler->SetParameter(param);
	return m_paramHandler->start(parser, name, attrs);
}


