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


#if !defined(AFX_SOAPCGIHANDLER_H__E392FAB3_3022_11D5_B3F3_000000000000__INCLUDED_)
#define AFX_SOAPCGIHANDLER_H__E392FAB3_3022_11D5_B3F3_000000000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//
//
//  A simple Transport class for CGI
//
class SOAPCGIHandler : public SOAPTransport
{
private:
	FILE *m_logfile;
	FILE *m_infile;

public:
	SOAPCGIHandler()
		: m_logfile(0), m_infile(0)
	{
	}

	~SOAPCGIHandler()
	{
		SetLogFile(0);
		SetInFile(0);
	}

	//
	// Log requests to this file.  Used for debugging
	// (copies stdin to this file)
	void SetLogFile(const char *logfile)
	{
		if (m_logfile)
			fclose(m_logfile);
		if (logfile)
			m_logfile = fopen(logfile, "ab");
	}

	//
	// Read input from this file.  Used for debugging.
	// (reads this file instead of stdin)
	void SetInFile(const char *infile)
	{
		if (m_infile)
			fclose(m_infile);
		if (infile)
			m_infile = fopen(infile, "rb");
	}

	const char *GetCharset() const
	{
		return 0;
	}

	size_t Read(char *buffer, size_t buffsize)
	{
		int read = 0;

		if (m_infile)
			read = fread(buffer, 1, buffsize, m_infile);
		else
			read = fread(buffer, 1, buffsize, stdin);

		if (read >0 && m_logfile)
			fwrite(buffer, 1, read, m_logfile);

		return read;
	}

	size_t Write(const SOAPMethod& method, const char *payload, size_t payloadsize)
	{
		printf("SOAPServer: %s/%s\n", EASYSOAP_STRING, EASYSOAP_VERSION_STRING);
		printf("Content-Length: %d\n", payloadsize);
		printf("Content-Type: text/xml; charset=\"UTF-8\"\n\n");

		fwrite(payload, 1, payloadsize, stdout);

		return payloadsize;
	}
};


#endif // !defined(AFX_SOAPCGIHANDLER_H__E392FAB3_3022_11D5_B3F3_000000000000__INCLUDED_)


