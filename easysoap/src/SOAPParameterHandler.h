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


#if !defined(AFX_SOAPPARAMETERHANDLER_H__124ED3B5_721D_4AB5_8B65_32D5C4F86420__INCLUDED_)
#define AFX_SOAPPARAMETERHANDLER_H__124ED3B5_721D_4AB5_8B65_32D5C4F86420__INCLUDED_

#include "SOAP.h"
#include "SOAPParseEventHandler.h"

class SOAPArrayHandler;
class SOAPStructHandler;
class SOAPParameter;

class SOAPParameterHandler : public SOAPParseEventHandler  
{
public:

	SOAPParameterHandler();
	virtual ~SOAPParameterHandler();

	void SetParameter(SOAPParameter& param)	{m_param = &param;}

	virtual SOAPParseEventHandler* start(const XML_Char *name, const XML_Char **attrs);
	virtual SOAPParseEventHandler* startElement(const XML_Char *name, const XML_Char **attrs);
	virtual void endElement(const XML_Char *name);
	virtual void characterData(const XML_Char *str, int len);

private:
	SOAPParameter			*m_param;
	SOAPTypes::xsd_type		m_type;
	SOAPString				m_str;
	SOAPStructHandler		*m_structHandler;
	SOAPArrayHandler		*m_arrayHandler;
	bool					m_setvalue;
};

#endif // !defined(AFX_SOAPPARAMETERHANDLER_H__124ED3B5_721D_4AB5_8B65_32D5C4F86420__INCLUDED_)

