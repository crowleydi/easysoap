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
#include <easysoap/SOAPISAPIServer.h>
#include <easysoap/SOAPonHTTP.h>

BEGIN_EASYSOAP_NAMESPACE

class SOAPISAPITransport : public SOAPServerTransport
{
public:
	SOAPISAPITransport(EXTENSION_CONTROL_BLOCK* pECB)
		: m_ecb(pECB), m_error(false), m_leftRead(0), m_ecbData(0)
	{
		
		if (m_ecb)
		{
			if (sp_strcmp(m_ecb->lpszMethod, "POST"))
				throw SOAPException("Invalid HTTP method '%s', only POST is supported.", m_ecb->lpszMethod);

			//
			//  Set thread to impersonate
			HANDLE impersonateToken = 0;
			if (m_ecb->ServerSupportFunction(m_ecb->ConnID,
				HSE_REQ_GET_IMPERSONATION_TOKEN,
				&impersonateToken, 0, 0))
			{
				if (!SetThreadToken(NULL, impersonateToken))
				{
					throw SOAPException("Failed to impersonate user.");
				}
			}


			SOAPHTTPProtocol::ParseContentType(m_contentType, m_charset, m_ecb->lpszContentType);
			m_leftRead = m_ecb->cbTotalBytes;

			char buffer[1024];
			DWORD bsize = sizeof(buffer);
			m_ecb->GetServerVariable(m_ecb->ConnID, "HTTP_SOAPACTION", buffer, &bsize);

			const char *sa = bsize == 0 ? 0 : buffer;
			if (sa && *sa == '\"')
				++sa;
			m_soapaction = sa;
			size_t len = m_soapaction.Length();
			if (len > 0 && m_soapaction[len - 1] == '\"')
				m_soapaction.Str()[len - 1] = 0;
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

		//
		// Clear the thread of any impersonatings it
		// may be doing...
		SetThreadToken(NULL, NULL);
	}

	void SetError()
	{
		m_error = true;
	}

	const char *GetCharset() const
	{
		return m_charset;
	}

	const char *GetContentType() const
	{
		return m_contentType;
	}

	const char *GetSoapAction() const
	{
		return m_soapaction;
	}

	size_t Read(char *buffer, size_t buffsize)
	{
		if (!m_ecb)
			throw SOAPException("Invalid EXTENSION_CONTROL_BLOCK");

		DWORD dwSize = buffsize > m_leftRead ? m_leftRead : buffsize;

		if (m_ecb->cbTotalBytes > m_ecb->cbAvailable)
		{
			if (!m_ecb->ReadClient(m_ecb->ConnID, (void *)buffer, &dwSize))
				throw SOAPException("ReadClient() failed, err=0x%08x",
						GetLastError());
			if (dwSize == 0)
				m_leftRead = 0;
		}
		else
		{
			if (!m_ecbData)
				m_ecbData = m_ecb->lpbData;
			const unsigned char *end = m_ecbData + dwSize;
			while (m_ecbData != end)
				*buffer++ = *m_ecbData++;
		}

		m_leftRead -= dwSize;
		return dwSize;
	}

	size_t Write(const SOAPMethod& method, const char *payload, size_t payloadsize)
	{
		if (!m_ecb)
			throw SOAPException("Invalid EXTENSION_CONTROL_BLOCK");

		HSE_SEND_HEADER_EX_INFO header;
		char httpheaders[256];
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
			"Content-Type: text/xml; charset=\"UTF-8\"\r\n"
			"Content-Length: %d\r\n\r\n",
			EASYSOAP_STRING, EASYSOAP_VERSION_STRING,
			payloadsize);

		header.pszStatus = httpstatus;
		header.pszHeader = httpheaders;
		header.cchStatus = sp_strlen(httpstatus);
		header.cchHeader = sp_strlen(httpheaders);
		header.fKeepConn = TRUE;

		m_ecb->dwHttpStatusCode = httpstatuscode;
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
	const unsigned char *		m_ecbData;
	bool						m_error;
	size_t						m_leftRead;
	SOAPString					m_charset;
	SOAPString					m_contentType;
	SOAPString					m_soapaction;
};

END_EASYSOAP_NAMESPACE

USING_EASYSOAP_NAMESPACE

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

