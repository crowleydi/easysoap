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
#include <SOAPQName.h>
#include <SOAPNamespaces.h>

void
SOAPQName::Set(const char *name, const char *ns)
{
	if (ns)
	{
		m_name = name;
		m_namespace = ns;
	}
	else
	{
		const char *psep = sp_strchr(name, PARSER_NS_SEP[0]);
		if (psep)
		{
			m_name = (psep + 1);
			m_namespace = "";
			m_namespace.Append(name, psep - name);
		}
		else if ((psep = sp_strchr(name, ':')))
		{
			throw SOAPException("You cannot set a QName to a value with an unexpanded namespace: %s", name);
		}
		else
		{
			m_name = name;
			m_namespace.Empty();
		}
	}
}

void
SOAPQName::Clear()
{
	m_name.Empty();
	m_namespace.Empty();
}

bool
SOAPQName::operator==(const char *str) const
{
	const char *sep = sp_strstr(str, PARSER_NS_SEP);
	if (sep)
	{
		return sp_strncmp(m_namespace, str, sep - str) == 0
			&& sp_strcmp(m_name, sep + 1) == 0;
	}
	else
	{
		return m_name == str;
	}
}
