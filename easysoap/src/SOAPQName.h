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


#if !defined(AFX_SOAPQNAME_H__E392FAB3_3022_11D5_B3F3_000000000000__INCLUDED_)
#define AFX_SOAPQNAME_H__E392FAB3_3022_11D5_B3F3_000000000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class EASYSOAP_EXPORT SOAPQName  
{
public:
	SOAPQName();
	SOAPQName(const SOAPQName&);
	~SOAPQName();

	SOAPQName& operator=(const SOAPQName&);
	SOAPQName& operator=(const char *);

	void Set(const char *name, const char *ns = 0);

	bool operator==(const SOAPQName&) const;
	bool operator==(const char *) const;

	const SOAPString& GetName() const;
	const SOAPString& GetNamespace() const;

	void Clear();

private:
	SOAPString	m_name;
	SOAPString	m_namespace;
};

#endif // !defined(AFX_SOAPQNAME_H__E392FAB3_3022_11D5_B3F3_000000000000__INCLUDED_)


