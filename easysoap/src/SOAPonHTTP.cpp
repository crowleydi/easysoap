

#include "SOAP.h"
#include "SOAPonHTTP.h"
#include "SOAPEnvelope.h"


#ifndef SOAPUSER_AGENT
#define SOAPUSER_AGENT "EasySoap++/0.1"
#endif // SOAPUSER_AGENT

// returns how big the response payload is
int
SOAPonHTTP::GetPayloadSize()
{
	return m_http.GetContentLength();
}

// read the payload into the buffer.
// can be called multiple times.
// returns 0 if entire payload has been read.

int
SOAPonHTTP::Read(char *buffer, int buffsize)
{
	return m_http.Read(buffer, buffsize);
}

// send the payload.  can only be called ONCE per
// payload. 
//
int
SOAPonHTTP::Write(const SOAPMethod& method, const char *payload, int payloadsize)
{
	char buff[128];
	snprintf(buff, sizeof(buff), "\"%s#%s\"",
		method.GetNamespace(),
		method.GetName());

	m_http.BeginPost(m_path.c_str());
	m_http.WriteHeader("User-Agent", SOAPUSER_AGENT);
	m_http.WriteHeader("Content-Type", "text/xml");
	m_http.WriteHeader("SOAPAction", buff);


	return m_http.PostData(payload, payloadsize);
}

void
SOAPHTTPProtocol::ConnectTo(const SOAPUrl& endpoint, const SOAPUrl& proxy)
{
	Close();
	m_endpoint = endpoint;
	m_proxy = proxy;
}

void
SOAPHTTPProtocol::ConnectTo(const SOAPUrl& endpoint)
{
	Close();
	m_endpoint = endpoint;
	m_proxy = SOAPUrl();
}

void
SOAPHTTPProtocol::WriteHostHeader(const SOAPUrl& url)
{
	if (url.PortIsDefault())
		WriteHeader("Host", url.Hostname());
	else
	{
		char buffer[256];
		snprintf(buffer, sizeof(buffer), "%s:%d", url.Hostname(), url.Port());
		WriteHeader("Host", buffer);
	}
}

int
SOAPHTTPProtocol::Get(const char *path)
{
	StartVerb("GET", path);
	return GetReply();
}

void
SOAPHTTPProtocol::FlushInput()
{
	char buffer[256];
	while (CanRead())
		Read(buffer, sizeof(buffer));
}

void
SOAPHTTPProtocol::StartVerb(const char *verb, const char *path)
{
	Write(verb);
	Write(" ");

	if (m_proxy.Protocol() != SOAPUrl::no_proto)
	{
		// TODO: Get a string back from SOAPUrl
		if (m_endpoint.Protocol() == SOAPUrl::http_proto)
		{
			Write("http://");
			Write(m_endpoint.Hostname());
		}
		else if (m_endpoint.Protocol() == SOAPUrl::https_proto)
		{
			//
			Write("https://");
			Write(m_endpoint.Hostname());
		}
		else
		{
			// not true, some proxies can do FTP GETs
			throw SOAPException("Proxy only handle HTTP destinations");
		}
	}

	if (path && *path)
	{
		Write(path);
	}
	else
	{
		Write("/");
	}

	WriteLine(" HTTP/1.0");
	WriteHostHeader(m_endpoint);
	WriteHeader("Connection", "Keep-Alive");
}

void
SOAPHTTPProtocol::BeginPost(const char *path)
{
	FlushInput();
	if (!Connect())
		throw SOAPSocketException("Unable to make socket connection");
	StartVerb("POST", path);
}

int
SOAPHTTPProtocol::PostData(const char *bytes, int len)
{
	WriteHeader("Content-Length", len);
	WriteLine("");
	Write(bytes, len);
	return GetReply();
}

void
SOAPHTTPProtocol::WriteHeader(const char *header, const char *value)
{
	Write(header);
	Write(": ");
	WriteLine(value);
}

void
SOAPHTTPProtocol::WriteHeader(const char *header, int value)
{
	char buffer[64];
	snprintf(buffer, sizeof(buffer), "%d", value);
	WriteHeader(header, buffer);
}

int
SOAPHTTPProtocol::GetReply()
{
	char buff[2048];
	m_headers.clear();

	int httpreturn = 500;

	// This should read a line that is something like:
	// HTTP/1.1 200 OK
	if (ReadLine(buff, sizeof(buff)) == 0)
	{
		throw SOAPException("Couldn't read response.");
	}

	char *httpretcode = strchr(buff, ' ');
	if (httpretcode)
	{
		httpreturn = atoi(httpretcode);
		if (httpreturn == 0)
			httpreturn = 500;
	}

	while (1)
	{
		if (ReadLine(buff, sizeof(buff)) == 0)
			break;

		if (buff[0] == 0)
			break;
		char *c = strchr(buff, ':');
		if (c)
		{
			*c++ = 0;
			while (*c == ' ')
				*c++ = 0;
			m_headers[buff] = c;
		}
		else
		{
			// I don't think we should get here...
		}
	}

	return httpreturn;
}

const char *
SOAPHTTPProtocol::GetHeader(const char *header)
{
	HeaderMap::iterator i = m_headers.find(header);
	if (i != m_headers.end())
	{
		return i->second.c_str();
	}
	return 0;
}

int
SOAPHTTPProtocol::GetContentLength()
{
	const char *header = GetHeader("Content-Length");
	int len = 0;
	if (header)
		len = atoi(header);
	return len;
}

bool
SOAPHTTPProtocol::Connect()
{
	if (!IsOpen())
	{
		bool proxy = m_proxy.Protocol() != SOAPUrl::no_proto;

		int port					= proxy ? m_proxy.Port()		: m_endpoint.Port();
		const char *host			= proxy ? m_proxy.Hostname()	: m_endpoint.Hostname();
		SOAPUrl::UrlProtocol proto	= proxy ? m_proxy.Protocol()	: m_endpoint.Protocol();

		switch (proto)
		{
		case SOAPUrl::http_proto:
			SOAPProtocolBase::Connect(host, port, false);
			break;
		case SOAPUrl::https_proto:
			SOAPProtocolBase::Connect(host, port, true);
			break;
		default:
			throw SOAPSocketException("Can only handle HTTP protocols");
			break;
		}
		return IsOpen();
	}

	return true;
}
