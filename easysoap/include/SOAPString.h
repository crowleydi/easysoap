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


#ifndef __SOAPSTRING_H__
#define __SOAPSTRING_H__

#include "SOAP.h"
#include "SOAPUtil.h"

class EASYSOAP_EXPORT SOAPString
{
private:
	char	*m_str;
	size_t	m_alloc;

	void Ensure(size_t size)
	{
		if (!m_str || m_alloc < size)
		{
			while (m_alloc < size)
				m_alloc *= 2;
			char *newstr = sp_alloc<char>(m_alloc);
			if (m_str)
			{
				sp_strcpy(newstr, m_str);
				sp_free(m_str);
			}
			m_str = newstr;
		}
	}

	void Assign(const char* str)
	{
		if (str)
		{
			if (m_str)
			{
				// try to copy in place
				size_t rem = m_alloc;
				const char *work = str;
				char *dest = m_str;
				while (rem--)
				{
					if ((*dest++ = *work++) == 0)
						return;
				}
				// we didn't have enough room to copy..
			}

			// we need to alloc some space
			size_t req = sp_strlen(str) + 1;
			if (m_str)
				*m_str = 0;
			Ensure(req);
			sp_strcpy(m_str, str);
		}
		else if (m_str)
		{
			*m_str = 0;
		}
	}

public:
	// Default constructor
	SOAPString(const char *str = 0) : m_str(0), m_alloc(128)
	{
		Assign(str);
	}

	// Copy constructor
	SOAPString(const SOAPString& str) : m_str(0), m_alloc(128)
	{
		Assign(str.m_str);
	}

	// Destructor
	~SOAPString()
	{
		sp_free(m_str);
	}

	// Assignment operator
	SOAPString& operator=(const SOAPString& str)
	{
		Assign(str.m_str);
		return *this;
	}

	SOAPString& operator=(const char *str)
	{
		Assign(str);
		return *this;
	}

	SOAPString& Append(const char *str)
	{
		return Append(str, sp_strlen(str));
	}

	SOAPString& Append(const char *str, size_t len)
	{
		size_t tlen = sp_strlen(m_str);
		size_t need = tlen + len + 1;
		Ensure(need);
		sp_strncpy(m_str + tlen, str, len);
		m_str[need - 1] = 0;
		return *this;
	}

	SOAPString& operator+=(const char *str)
	{
		return Append(str);
	}

	const char *Str() const
	{
		return m_str;
	}

	operator const char *() const
	{
		return Str();
	}

	size_t Length() const
	{
		return m_str ? sp_strlen(m_str) : 0;
	}

	int Compare(const char *str) const
	{
		return sp_strcmp(m_str, str);
	}

	bool operator==(const char *str) const
	{
		return Compare(str) == 0;
	}

	bool operator<(const char *str) const
	{
		return Compare(str) < 0;
	}
};

#ifdef __SOAPHASHMAP_H__

struct SOAPHashCodeFunctor<SOAPString>
{
	size_t operator()(const SOAPString& str) const
	{
		return sp_hashcode(str);
	}
};

struct SOAPEqualsFunctor<SOAPString>
{
	bool operator()(const SOAPString& a, const SOAPString& b) const
	{
		return sp_strcmp(a, b) == 0;
	}
};

struct SOAPHashCodeFunctorNoCase<SOAPString>
{
	size_t operator()(const SOAPString& str) const
	{
		return sp_hashcodecase(str);
	}
};

struct SOAPEqualsFunctorNoCase<SOAPString>
{
	bool operator()(const SOAPString& a, const SOAPString& b) const
	{
		return sp_strcasecmp(a, b) == 0;
	}
};

#endif // __SOAPHASHMAP_H__
#endif // __SOAPSTRING_H__

