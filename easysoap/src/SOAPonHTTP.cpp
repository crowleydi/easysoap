

#include "SOAP.h"
#include "SOAPonHTTP.h"
#include "SOAPEnvelope.h"


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

	m_http.Post(m_url);
	m_http.WriteHeader("User-Agent", "EasySoap++/0.1");
	m_http.WriteHeader("Content-Type", "text/xml");
	m_http.WriteHeader("SOAPAction", buff);


	return m_http.PostData(payload, payloadsize);
}

//
// Our HTTP protocol implementation
//
SOAPHTTPProtocol::SOAPHTTPProtocol()
{
	// We try to keep track to see if
	// the connection is open.
}


SOAPHTTPProtocol::~SOAPHTTPProtocol()
{
}

void
SOAPHTTPProtocol::Close()
{
	super::Close();
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
SOAPHTTPProtocol::Get(const SOAPUrl& url)
{
	FlushInput();
	if (!IsOpen())
	{
		// try to reconnect
		Connect(url);
		if (!IsOpen())
			throw SOAPSocketException("Unable to make socket connection");
	}

	const char *path = url.Path();

	if (path && *path)
	{
		Write("GET ");
		Write(path);
		WriteLine(" HTTP/1.0");
	}
	else
	{
		WriteLine("GET / HTTP/1.0");
	}

	WriteHostHeader(url);
	WriteHeader("Connection", "Keep-Alive");
	WriteLine();

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
SOAPHTTPProtocol::Post(const SOAPUrl& url)
{
	FlushInput();
	if (!IsOpen())
	{
		// try to reconnect
		Connect(url);
		if (!IsOpen())
			throw SOAPSocketException("Unable to make socket connection");
	}

	const char *path = url.Path();

	if (path && *path)
	{
		Write("POST ");
		Write(path);
		WriteLine(" HTTP/1.0");
	}
	else
	{
		WriteLine("POST / HTTP/1.0");
	}

	WriteHostHeader(url);
	WriteHeader("Connection", "Keep-Alive");
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

	bool ver11 = false;
	int httpreturn = 500;

	// This should read a line that is something like:
	// HTTP/1.1 200 OK
	if (ReadLine(buff, sizeof(buff)) == 0)
	{
		throw SOAPException("Couldn't read response.");
	}

	if (strncmp(buff, "HTTP/1.1", 8) == 0)
		ver11 = true;

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

	//
	// TODO:  The map and the strcmps need to be
	// case insensitive
	//
	const char *keepalive = GetHeader("Connection");
	if (keepalive)
	{
		if (ver11)
		{
			if (strcmp(keepalive, "close") == 0 ||
				strcmp(keepalive, "Keep-Alive") != 0)
				Close();
		}
		else
		{
			if (strcmp(keepalive, "Keep-Alive") != 0)
				Close();
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
SOAPHTTPProtocol::Connect(const SOAPUrl& url)
{
	Close();
	switch (url.Protocol())
	{
	case SOAPUrl::http_proto:
		SOAPProtocolBase::Connect(url.Hostname(), url.Port(), false);
		break;
	case SOAPUrl::https_proto:
		SOAPProtocolBase::Connect(url.Hostname(), url.Port(), true);
		break;
	default:
		throw SOAPSocketException("Can only handle HTTP protocols");
		break;
	}
	return IsOpen();
}
