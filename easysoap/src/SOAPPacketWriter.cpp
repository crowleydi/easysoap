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


#include <SOAP.h>
#include <SOAPPacketWriter.h>

bool SOAPPacketWriter::g_makePretty = false;

SOAPPacketWriter::SOAPPacketWriter()
{
	m_instart = false;
	m_buffer = 0;
	m_buffptr = 0;
	m_buffend = 0;
	m_buffsize = 0;
	m_gensym = 0;
}

SOAPPacketWriter::~SOAPPacketWriter()
{
	delete [] m_buffer;
}

void
SOAPPacketWriter::SetAddWhiteSpace(bool ws)
{
	g_makePretty = ws;
}

const char *
SOAPPacketWriter::GetSymbol(char *buff, const char *prefix)
{
	// A bit dangerious here...
	sprintf(buff, "%s%d", prefix, ++m_gensym);
	return buff;
}

void
SOAPPacketWriter::Reset()
{
	m_instart = false;
	m_buffptr = m_buffer;
	m_nsmap.Clear();
}

const char *
SOAPPacketWriter::GetBytes()
{
	*m_buffptr = 0;
	return m_buffer;
}

unsigned int
SOAPPacketWriter::GetLength()
{
	return m_buffptr - m_buffer;
}

void
SOAPPacketWriter::StartTag(const char *tag)
{
	EndStart();
	Write("<");
	Write(tag);
	m_instart = true;
}

void
SOAPPacketWriter::StartTag(const SOAPQName& tag, const char *prefix)
{
	const char *nstag = 0;
	bool addxmlns = false;
	char buffer[64];

	if (tag.GetNamespace().IsEmpty())
	{
		StartTag(tag.GetName());
		return;
	}

	EndStart();
	Write("<");

	if (!tag.GetNamespace().IsEmpty())
	{
		NamespaceMap::Iterator i = m_nsmap.Find(tag.GetNamespace());
		if (!i)
		{
			addxmlns = true;
			if (prefix)
				nstag = prefix;
			else
				nstag = GetSymbol(buffer, "ns");
		}
		else
		{
			nstag = i->Str();
		}

		Write(nstag);
		Write(":");
	}

	Write(tag.GetName());
	m_instart = true;

	if (addxmlns)
		AddXMLNS(nstag, tag.GetNamespace());
}

void
SOAPPacketWriter::AddAttr(const SOAPQName& tag, const char *value)
{
	const char *nstag = 0;
	bool addxmlns = false;
	char buffer[64];

	if (!m_instart)
		throw SOAPException("XML serialization error.  Adding attribute when not in start tag.");

	if (g_makePretty)
		Write("\r\n\t");
	else
		Write(" ");

	if (!tag.GetNamespace().IsEmpty())
	{
		NamespaceMap::Iterator i = m_nsmap.Find(tag.GetNamespace());
		if (!i)
		{
			addxmlns = true;
			nstag = GetSymbol(buffer, "ns");
		}
		else
		{
			nstag = i->Str();
		}

		Write(nstag);
		Write(":");
	}

	Write(tag.GetName());
	Write("=\"");
	WriteEscaped(value);
	Write("\"");

	if (addxmlns)
		AddXMLNS(nstag, tag.GetNamespace());
}

void
SOAPPacketWriter::AddAttr(const SOAPQName& tag, const SOAPQName& value)
{
	const char *tnstag;
	const char *vnstag = 0;
	bool addtns = false;
	bool addvns = false;
	char tbuff[64];
	char vbuff[64];

	if (!m_instart)
		throw SOAPException("XML serialization error.  Adding attribute when not in start tag.");

	NamespaceMap::Iterator i = m_nsmap.Find(tag.GetNamespace());
	if (!i)
	{
		addtns = true;
		tnstag = GetSymbol(tbuff, "ns");
	}
	else
	{
		tnstag = i->Str();
	}

	if (g_makePretty)
		Write("\r\n\t");
	else
		Write(" ");

	Write(tnstag);
	Write(":");
	Write(tag.GetName());
	Write("=\"");

	if (value.GetNamespace().IsEmpty())
	{
		Write(value.GetName());
	}
	else
	{
		i = m_nsmap.Find(value.GetNamespace());
		if (!i)
		{
			addvns = true;
			vnstag = GetSymbol(vbuff, "ns");
		}
		else
		{
			vnstag = i->Str();
		}

		Write(vnstag);
		Write(":");
		WriteEscaped(value.GetName());

	}

	Write("\"");

	if (addtns)
		AddXMLNS(tnstag, tag.GetNamespace());
	if (addvns)
		AddXMLNS(vnstag, value.GetNamespace());
}

void
SOAPPacketWriter::AddAttr(const char *attr, const char *value)
{
	if (!m_instart)
		throw SOAPException("XML serialization error.  Adding attribute when not in start tag.");

	if (g_makePretty)
		Write("\r\n\t");
	else
		Write(" ");

	Write(attr);
	Write("=\"");
	WriteEscaped(value);
	Write("\"");
}

void
SOAPPacketWriter::AddXMLNS(const char *prefix, const char *ns)
{
	NamespaceMap::Iterator i = m_nsmap.Find(ns);
	if (!i)
	{
		m_nsmap[ns] = prefix;

		if (g_makePretty)
			Write("\r\n\t");
		else
			Write(" ");

		Write("xmlns");
		if (prefix)
		{
			Write(":");
			Write(prefix);
		}
		Write("=\"");
		WriteEscaped(ns);
		Write("\"");
	}
}

void
SOAPPacketWriter::EndTag(const char *tag)
{
	if (m_instart)
	{
		Write("/>");
		if (g_makePretty)
			Write("\r\n");
		m_instart = false;
	}
	else
	{
		Write("</");
		Write(tag);
		Write(">");
		if (g_makePretty)
			Write("\r\n");
	}
}

void
SOAPPacketWriter::EndTag(const SOAPQName& tag)
{
	if (tag.GetNamespace().IsEmpty())
	{
		EndTag(tag.GetName());
		return;
	}

	if (m_instart)
	{
		Write("/>");
		if (g_makePretty)
			Write("\r\n");
		m_instart = false;
	}
	else
	{
		Write("</");

		if (!tag.GetNamespace().IsEmpty())
		{
			NamespaceMap::Iterator i = m_nsmap.Find(tag.GetNamespace());
			if (!i)
				throw SOAPException("EndTag: Could not find tag for namespace: %s",
					(const char *)tag.GetNamespace());

			Write(i->Str());
			Write(":");
		}

		Write(tag.GetName());
		Write(">");
		if (g_makePretty)
			Write("\r\n");
	}
}

void
SOAPPacketWriter::WriteValue(const char *val)
{
	if (m_instart)
	{
		Write(">");
		m_instart = false;
	}
	WriteEscaped(val);
}

void
SOAPPacketWriter::EndStart()
{
	if (m_instart)
	{
		Write(">");
		if (g_makePretty)
			Write("\r\n");
		m_instart = false;
	}
}

void
SOAPPacketWriter::Resize()
{
	m_buffsize *= 2;
	if (m_buffsize == 0)
		m_buffsize = 4096;

	char *newbuff = new char[m_buffsize];
	char *d = newbuff;
	const char *s = m_buffer;
	while (s != m_buffptr)
		*d++ = *s++;

	delete [] m_buffer;
	m_buffer = newbuff;
	m_buffptr = d;
	m_buffend = newbuff + m_buffsize;
}

void
SOAPPacketWriter::Write(const char *str)
{
	if (str)
	{
		while (*str)
		{
			if (m_buffptr == m_buffend)
				Resize();
			else
				*m_buffptr++ = *str++;
		}
	}
}

void
SOAPPacketWriter::WriteEscaped(const char *str)
{
	if (str)
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
				case '\r':	Write("&#xd;");		break;
				default:
					*m_buffptr++ = c;
					break;
				}
			}
		}
	}
}

void
SOAPPacketWriter::Write(const char *str, int len)
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

void
SOAPPacketWriter::SetNamespace(const char *ns, const char *tag)
{
	if (!ns || !*ns)
		throw SOAPException("Cannot add empty namespace");

	m_nsmap[ns] = tag;
}

