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


#if !defined(AFX_SOAPSERVERDISPATCH_H__F7015C4A_8459_4090_ABD1_FB3EAB144B39__INCLUDED_)
#define AFX_SOAPSERVERDISPATCH_H__F7015C4A_8459_4090_ABD1_FB3EAB144B39__INCLUDED_


#include "SOAP.h"


class EASYSOAP_EXPORT SOAPDispatchHandlerInterface
{
public:
	virtual void ExecuteMethod(const SOAPMethod& request, SOAPMethod& response) = 0;
};



template <typename T>
class SOAPDispatchHandler : public SOAPDispatchHandlerInterface
{
private:
	typedef void (T::*HandlerFunction)(const SOAPMethod& request, SOAPMethod& response);
	typedef SOAPHashMap<SOAPString, HandlerFunction> DispatchMap;

	SOAPDispatchHandler(const SOAPDispatchHandler&);
	SOAPDispatchHandler& operator=(const SOAPDispatchHandler&);

	void ExecuteMethod(const SOAPMethod& request, SOAPMethod& response)
	{
		DispatchMap::Iterator i = m_dispatchMap.Find(request.GetName().GetName());
		if (i)
		{
			(m_target->*(*i))(request, response);
		}
		else
		{
			throw SOAPException("Unknown method request: [%s]::%s",
				(const char *)request.GetName().GetNamespace(),
				(const char *)request.GetName().GetName());
		}
	}

	DispatchMap	m_dispatchMap;
	T			*m_target;

protected:
	SOAPDispatchHandler(T* target = 0)
		: m_target(target)
	{
	}

	void DispatchTo(T *target)
	{
		m_target = target;
	}

	void DispatchMethod(const char *name, HandlerFunction func)
	{
		m_dispatchMap[name] = func;
	}

};


class EASYSOAP_EXPORT SOAPServerDispatch
{
public:
	SOAPServerDispatch(SOAPTransport& transport, bool deltrans = false);
	~SOAPServerDispatch();

	SOAPServerDispatch& SetTransport(SOAPTransport& transport, bool deltrans = false);
	SOAPServerDispatch& Dispatch(const char *ns, SOAPDispatchHandlerInterface *disp);
	int Handle();

private:

	SOAPServerDispatch(const SOAPServerDispatch&);
	SOAPServerDispatch& operator=(const SOAPServerDispatch&);
	void WriteFault(const char *code, const char *str);

	typedef SOAPHashMap<SOAPString, SOAPDispatchHandlerInterface*> HandlersMap;

	HandlersMap			m_handlers;
	SOAPTransport		*m_transport;
	bool				m_deltrans;
	SOAPResponse		m_response;
	SOAPEnvelope		m_request;
	SOAPPacketWriter	m_writer;
};


#endif // !defined(AFX_SOAPSERVERDISPATCH_H__F7015C4A_8459_4090_ABD1_FB3EAB144B39__INCLUDED_)


