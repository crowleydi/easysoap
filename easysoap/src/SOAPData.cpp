// SOAPData.cpp: implementation of the SOAPData class.
//
//////////////////////////////////////////////////////////////////////

#include "SOAPData.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SOAPData::SOAPData()
: m_parent(0)
{
}

SOAPData::SOAPData(const SOAPData& data)
: m_parent(0)
{
	*this = data;
}

SOAPData::~SOAPData()
{
}

SOAPData&
SOAPData::operator =(const SOAPData& data)
{
	m_attrs = data.m_attrs;
	m_value = data.m_value;
	m_type = data.m_type;
	SetName(data.GetName());

	size_t datasize = data.m_data.Size();
	m_data.Resize(datasize);
	for (size_t i = 0; i < datasize; ++i)
	{
		m_data[i].m_parent = this;
		m_data[i] = data.m_data[i];
	}

	return *this;
}

void
SOAPData::SetValue(int val)
{
	char buffer[32];
	sprintf(buffer, "%d", val);
	SetInteger(buffer);
}

void
SOAPData::SetValue(float val)
{
	char buffer[32];
	sprintf(buffer, "%f", (double)val);
	SetInteger(buffer);
}

void
SOAPData::SetValue(double val)
{
	char buffer[32];
	sprintf(buffer, "%f", val);
	SetInteger(buffer);
}

void
SOAPData::SetValue(const char *val)
{
	SetString(val);
}

void
SOAPData::SetInteger(const char *val)
{
	m_value = val;
	m_type = "xsd:int";
}

void
SOAPData::SetFloat(const char *val)
{
	m_value = val;
	m_type = "xsd:float";
}

void
SOAPData::SetDouble(const char *val)
{
	m_value = val;
	m_type = "xsd:double";
}

void
SOAPData::SetString(const char *val)
{
	m_value = val;
	m_type = "xsd:string";
}

SOAPData&
SOAPData::AddData(const char *name)
{
	size_t numitems = m_data.Size();
	m_data.Resize(numitems + 1);
	m_data[numitems].m_parent = this;
	m_data[numitems].SetName(name ? name : "");

	return m_data[numitems];
}

