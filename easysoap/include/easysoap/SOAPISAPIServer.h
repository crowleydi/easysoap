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


#if !defined(AFX_SOAPISAPIHANDLER_H__E392FAB3_3022_11D5_B3F3_000000000000__INCLUDED_)
#define AFX_SOAPISAPIHANDLER_H__E392FAB3_3022_11D5_B3F3_000000000000__INCLUDED_

#include <httpext.h>
#include <easysoap/SOAPServer.h>

BEGIN_EASYSOAP_NAMESPACE

class EASYSOAP_EXPORT SOAPISAPIServer : public SOAPServer<SOAPISAPIServer>
{
public:
	SOAPISAPIServer() : m_ioport(NULL) {}
	SOAPISAPIServer(HANDLE ioport) : m_ioport(ioport) {}

	int Handle();
	int Handle(EXTENSION_CONTROL_BLOCK *pECB);

private:

	SOAPISAPIServer(const SOAPISAPIServer&);
	SOAPISAPIServer& operator=(const SOAPISAPIServer&);

	HANDLE				m_ioport;
};

class EASYSOAP_EXPORT SOAPISAPIImpersonateUser
{
public:
	SOAPISAPIImpersonateUser(EXTENSION_CONTROL_BLOCK *);
	~SOAPISAPIImpersonateUser();

private:
	SOAPISAPIImpersonateUser(const SOAPISAPIImpersonateUser&);
	SOAPISAPIImpersonateUser& operator=(const SOAPISAPIImpersonateUser&);

	EXTENSION_CONTROL_BLOCK	*m_ecb;
};

END_EASYSOAP_NAMESPACE

#endif // !defined(AFX_SOAPISAPIHANDLER_H__E392FAB3_3022_11D5_B3F3_000000000000__INCLUDED_)

