// SOAPQName.cpp: implementation of the SOAPQName class.
//
//////////////////////////////////////////////////////////////////////

#include "SOAP.h"
#include "SOAPQName.h"
#include "SOAPNamespaces.h"

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
		char *psep = sp_strstr(name, PARSER_NS_SEP);
		if (psep)
		{
			char c = *psep;
			*psep = 0;
			m_namespace = name;
			m_name = (psep + 1);
			*psep = c;
		}
		else
		{
			m_name = name;
			m_namespace = "";
		}
	}
}

void
SOAPQName::Clear()
{
	m_name = "";
	m_namespace = "";
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