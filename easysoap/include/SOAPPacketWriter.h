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


#if !defined(AFX_SOAPPACKETWRITER_H__DB61E902_B4A0_4AA3_A0F7_479D6295BD06__INCLUDED_)
#define AFX_SOAPPACKETWRITER_H__DB61E902_B4A0_4AA3_A0F7_479D6295BD06__INCLUDED_

#include <stdlib.h>
#include <stdio.h>

#include "SOAP.h"

class EASYSOAP_EXPORT SOAPPacketWriter  
{
public:
	SOAPPacketWriter()
	{
		m_instart = false;
		m_buffer = 0;
		m_buffptr = 0;
		m_buffend = 0;
		m_buffsize = 0;
	}

	virtual ~SOAPPacketWriter()
	{
		delete [] m_buffer;
	}

	void Reset()
	{
		m_instart = false;
		m_buffptr = m_buffer;
	}

	const char *	GetBytes()	{return m_buffer;}
	unsigned int	GetLength()	{return m_buffptr - m_buffer;}

	void StartTag(const char *tag)
	{
		EndStart();
		Write("<");
		Write(tag);
		m_instart = true;
	}

	void StartTag(const char *ns, const char *tag)
	{
		EndStart();
		Write("<");
		Write(ns);
		Write(":");
		Write(tag);
		m_instart = true;
	}

	void AddAttr(const char *attr, const char *value)
	{
		Write(" ");
		Write(attr);
		Write("=\"");
		WriteEscaped(value);
		Write("\"");
	}

	void EndTag(const char *tag)
	{
		if (m_instart)
		{
			Write("/>");
			m_instart = false;
		}
		else
		{
			Write("</");
			Write(tag);
			Write(">");
		}
	}

	void EndTag(const char *ns, const char *tag)
	{
		if (m_instart)
		{
			Write("/>");
			m_instart = false;
		}
		else
		{
			Write("</");
			Write(ns);
			Write(":");
			Write(tag);
			Write(">");
		}
	}

	void WriteValue(const char *val)
	{
		EndStart();
		WriteEscaped(val);
	}

	void WriteValue(int val)
	{
		char buffer[64];
		snprintf(buffer, sizeof(buffer), "%d", val);
		WriteValue(buffer);
	}

	void WriteValue(float val)
	{
		char buffer[64];
		snprintf(buffer, sizeof(buffer), "%f", val);
		WriteValue(buffer);
	}

	void WriteValue(double val)
	{
		char buffer[64];
		snprintf(buffer, sizeof(buffer), "%f", val);
		WriteValue(buffer);
	}

private:
	void EndStart()
	{
		if (m_instart)
		{
			Write(">");
			m_instart = false;
		}
	}

	void	Resize()
	{
		m_buffsize *= 2;
		if (m_buffsize == 0)
			m_buffsize = 4096;

		char *newbuff = new char[m_buffsize];
		char *d = newbuff;
		const char *s = m_buffer;
		while (s != m_buffptr)
		{
			*d++ = *s++;
		}

		delete [] m_buffer;
		m_buffer = newbuff;
		m_buffptr = d;
		m_buffend = newbuff + m_buffsize;
	}

	void	Write(const char *str)
	{
		while (*str)
		{
			if (m_buffptr == m_buffend)
				Resize();
			else
				*m_buffptr++ = *str++;
		}
	}

	void	WriteEscaped(const char *str)
	{
		while (*str)
		{
			if (m_buffptr == m_buffend)
				Resize();
			else
			{
				char c = *str++;
				switch (c)
				{
				case '&':	Write("&amp;");		break;
				case '<':	Write("&lt;");		break;
				case '>':	Write("&gt;");		break;
				case '\'':	Write("&apos;");	break;
				case '\"':	Write("&quot;");	break;
				default:
					*m_buffptr++ = c;
					break;
				}
			}
		}
	}

	void	Write(const char *str, int len)
	{
		const char *strend = str + len;
		while (str != strend)
		{
			if (m_buffptr == m_buffend)
				Resize();
			else
				*m_buffptr++ = *str++;
		}
	}

	bool			m_instart;
	char			*m_buffer;
	char			*m_buffptr;
	const char		*m_buffend;
	unsigned int	m_buffsize;
};

#endif // !defined(AFX_SOAPPACKETWRITER_H__DB61E902_B4A0_4AA3_A0F7_479D6295BD06__INCLUDED_)

