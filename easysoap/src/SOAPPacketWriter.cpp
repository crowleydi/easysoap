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


#include <easysoap/SOAP.h>
#include <easysoap/SOAPPacketWriter.h>

USING_EASYSOAP_NAMESPACE

bool SOAPPacketWriter::g_makePretty = false;

SOAPPacketWriter::SOAPPacketWriter()
: m_instart(false)
, m_buffer(0)
, m_buffptr(0)
, m_buffend(0)
, m_buffsize(0)
, m_gensym(0)
, m_level(0)
{
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
	m_gensym = 0;
	m_level = 0;
	m_instart = false;
	m_buffptr = m_buffer;
	m_nsmap.Clear();
	m_nsarray.Resize(0);
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
	PushLevel();

	EndStart();
	Write("<");
	Write(tag);
	m_instart = true;
}

void
SOAPPacketWriter::StartTag(const SOAPQName& tag, const char *prefix)
{
	const char *nsprefix = 0;
	bool addxmlns = false;
	char buffer[64];

	if (tag.GetNamespace().IsEmpty())
	{
		StartTag(tag.GetName());
		return;
	}

	PushLevel();

	EndStart();
	Write("<");

	if (!tag.GetNamespace().IsEmpty())
	{
		NamespaceMap::Iterator i = m_nsmap.Find(tag.GetNamespace());
		if (!i)
		{
			addxmlns = true;
			if (prefix)
				nsprefix = prefix;
			else
				nsprefix = GetSymbol(buffer, "ns");
		}
		else
		{
			nsprefix = i->prefix;
		}

		Write(nsprefix);
		Write(":");
	}

	Write(tag.GetName());
	m_instart = true;

	if (addxmlns)
		AddXMLNS(nsprefix, tag.GetNamespace());
}

void
SOAPPacketWriter::AddAttr(const SOAPQName& tag, const char *value)
{
	const char *nsprefix = 0;
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
			nsprefix = GetSymbol(buffer, "ns");
		}
		else
		{
			nsprefix = i->prefix;
		}

		Write(nsprefix);
		Write(":");
	}

	Write(tag.GetName());
	Write("=\"");
	WriteEscaped(value);
	Write("\"");

	if (addxmlns)
		AddXMLNS(nsprefix, tag.GetNamespace());
}

void
SOAPPacketWriter::AddAttr(const SOAPQName& tag, const SOAPQName& value)
{
	const char *tnsprefix;
	const char *vnsprefix = 0;
	bool addtns = false;
	bool addvns = false;
	char tbuff[64];
	char vbuff[64];

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
			addtns = true;
			tnsprefix = GetSymbol(tbuff, "ns");
		}
		else
		{
			tnsprefix = i->prefix;
		}

		Write(tnsprefix);
		Write(":");
	}

	Write(tag.GetName());
	Write("=\"");

	if (value.GetNamespace().IsEmpty())
	{
		Write(value.GetName());
	}
	else
	{
		NamespaceMap::Iterator i = m_nsmap.Find(value.GetNamespace());
		if (!i)
		{
			addvns = true;
			vnsprefix = GetSymbol(vbuff, "ns");
		}
		else
		{
			vnsprefix = i->prefix;
		}

		Write(vnsprefix);
		Write(":");
		WriteEscaped(value.GetName());

	}

	Write("\"");

	if (addtns)
		AddXMLNS(tnsprefix, tag.GetNamespace());
	if (addvns)
		AddXMLNS(vnsprefix, value.GetNamespace());
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
		NamespaceInfo& ni = m_nsmap[ns];
		ni.prefix = prefix;
		ni.level = m_level;
		ni.value = ns;

		m_nsarray.Add(ni);

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

	PopLevel();
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

			Write(i->prefix);
			Write(":");
		}

		Write(tag.GetName());
		Write(">");
		if (g_makePretty)
			Write("\r\n");
	}

	PopLevel();
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
SOAPPacketWriter::Write(const char *str, unsigned int len)
{
	const char *strend = str + len;
	while (str != strend)
	{
		if (m_buffptr == m_buffend)
			Resize();
		*m_buffptr++ = *str++;
	}
}

void
SOAPPacketWriter::PopLevel()
{
	for (NamespaceArray::Iterator i = m_nsarray.End(); i != m_nsarray.Begin();)
	{
		--i;
		if (i->level == m_level)
			m_nsmap.Remove(i->value);
		else
			break;
	}
	--m_level;
}

void
SOAPPacketWriter::PushLevel()
{
	++m_level;
}

