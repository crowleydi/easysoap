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


#include <httpext.h>
#include <SOAPISAPIServer.h>


class SOAPISAPITransport : public SOAPTransport
{
public:
	SOAPISAPITransport(EXTENSION_CONTROL_BLOCK* pECB)
		: m_ecb(pECB), m_error(false), m_leftRead(0)
	{
		if (m_ecb)
		{
			if (sp_strcmp(m_ecb->lpszMethod, "POST"))
				throw SOAPException("Invalid HTTP method '%s', only POST is supported.", m_ecb->lpszMethod);

			ParseContentType(m_charset, m_ecb->lpszContentType);
			m_leftRead = m_ecb->cbTotalBytes;
		}
	}

	~SOAPISAPITransport()
	{
		//
		// We do this in the destructor to ensure that this step is
		// taken care of...
		if (m_ecb)
		{
			DWORD dwState = m_error ? HSE_STATUS_ERROR : HSE_STATUS_SUCCESS;
			m_ecb->ServerSupportFunction(m_ecb->ConnID, HSE_REQ_DONE_WITH_SESSION, &dwState, NULL, 0);
			m_ecb = 0;
		}
	}

	void SetError()
	{
		m_error = true;
	}

	const char *GetCharset() const
	{
		return m_charset;
	}

	size_t Read(char *buffer, size_t buffsize)
	{
		if (!m_ecb)
			throw SOAPException("Invalid EXTENSION_CONTROL_BLOCK");

		DWORD dwSize = buffsize > m_leftRead ? m_leftRead : buffsize;

		if (m_ecb->cbTotalBytes > m_ecb->cbAvailable)
		{
			m_ecb->ReadClient(m_ecb->ConnID, (void *)buffer, &dwSize);
		}
		else
		{
			memcpy(buffer, m_ecb->lpbData, dwSize);
		}

		m_leftRead -= dwSize;
		return dwSize;
	}

	size_t Write(const SOAPMethod& method, const char *payload, size_t payloadsize)
	{
		if (!m_ecb)
			throw SOAPException("Invalid EXTENSION_CONTROL_BLOCK");

		HSE_SEND_HEADER_EX_INFO header;
		char httpheaders[128];
		DWORD dwSize = payloadsize;

		const char *httpstatus = 0;
		int httpstatuscode = 0;

		if (m_error)
		{
			httpstatus = "500 Internal Server Error";
			httpstatuscode = 500;
		}
		else
		{
			httpstatus = "200 OK";
			httpstatuscode = 200;
		}

		snprintf(httpheaders, sizeof(httpheaders),
			"SOAPServer: %s/%s\r\n"
			"Content-Length: %d\r\n\r\n",
			EASYSOAP_STRING, EASYSOAP_VERSION_STRING,
			payloadsize);

		header.pszStatus = httpstatus;
		header.pszHeader = httpheaders;
		header.cchStatus = sp_strlen(httpstatus);
		header.cchHeader = sp_strlen(httpheaders);
		header.fKeepConn = TRUE;

		m_ecb->dwHttpStatusCode = httpstatuscode;
		m_ecb->lpszContentType = "text/xml; charset=\"UTF-8\"";
		m_ecb->ServerSupportFunction(m_ecb->ConnID, HSE_REQ_SEND_RESPONSE_HEADER_EX, &header, NULL, 0);
		m_ecb->WriteClient(m_ecb->ConnID,
			(void *)payload,
			&dwSize,
// Disable Nagling for IIS >= 5.0
#if HSE_VERSION_MAJOR < 5
			HSE_IO_SYNC | 0x00001000
#else
			HSE_IO_SYNC | HSE_IO_NODELAY
#endif
			);

		return dwSize;
	}

private:
	SOAPISAPITransport();
	SOAPISAPITransport(const SOAPISAPITransport&);
	SOAPISAPITransport& operator=(const SOAPISAPITransport&);

	EXTENSION_CONTROL_BLOCK*	m_ecb;
	bool						m_error;
	size_t						m_leftRead;
	SOAPString					m_charset;
};

int
SOAPISAPIServer::Handle()
{
	unsigned long	pN1, pN2; 
	OVERLAPPED*		pOverLapped;

	while(GetQueuedCompletionStatus(m_ioport, &pN1, &pN2, 
			&pOverLapped, INFINITE))
	{
		if (pOverLapped == (OVERLAPPED*)0xFFFFFFFF)
			break;
		try
		{
			if (pN2 == 0)
			{
				SOAPISAPITransport isapi((EXTENSION_CONTROL_BLOCK*)pN1);
				m_dispatch.Handle(isapi);
			}
			else
			{
				// Huh?!?!?
			}
		}
		catch (SOAPException&)
		{
		}
		catch (...)
		{
		}
	}

	return 0;
}

