/* 
 * EasySoap++ - A C++ library for SOAP (Simple Object Access Protocol)
 * Copyright (C) 2001 David Crowley; SciTegic, Inc.
 *
 * SSLClient -- A test tool to test EasySoap's SSL capabilitys
 * Copyright (C) 2001 Blaise St-Laurent
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

#ifdef _MSC_VER
#pragma warning (disable: 4786)
#endif // _MSC_VER


#include <easysoap/SOAP.h>
#include <easysoap/SOAPonHTTP.h>
#include <easysoap/SOAPDebugger.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

USING_EASYSOAP_NAMESPACE

// SSL environment variables we are interested in
//

const char* SSLEnvVars[] = {  
"SSL_PROTOCOL",
"SSL_VERSION_LIBRARY",
"SSL_CIPHER_USEKEYSIZE",
"SSL_CIPHER_ALGKEYSIZE",
"SSL_CIPHER",
"SSL_CIPHER_EXPORT",
"SSL_CIPHER_ALGKEYSIZE",
"SSL_SERVER_CERT",
"SSL_SERVER_V_START",
"SSL_SERVER_V_END",
"SSL_SERVER_M_SERIAL",
"SSL_SERVER_A_SIG",
"SSL_SERVER_S_DN",
"SSL_SERVER_S_DN_CN",
"SSL_SERVER_S_DN_Email",
"SSL_SERVER_S_DN_O",
"SSL_SERVER_S_DN_OU",
"SSL_SERVER_S_DN_C",
"SSL_SERVER_S_DN_SP",
"SSL_SERVER_S_DN_L",
"SSL_SERVER_I_DN",
"SSL_SERVER_I_DN_CN",
"SSL_SERVER_I_DN_Email",
"SSL_SERVER_I_DN_O",
"SSL_SERVER_I_DN_OU",
"SSL_SERVER_I_DN_C",
"SSL_SERVER_I_DN_SP",
"SSL_SERVER_I_DN_L",
"SSL_CLIENT_CERT",
"SSL_CLIENT_V_START",
"SSL_CLIENT_V_END",
"SSL_CLIENT_M_SERIAL",
"SSL_CLIENT_A_SIG",
"SSL_CLIENT_S_DN",
"SSL_CLIENT_S_DN_CN",
"SSL_CLIENT_S_DN_Email",
"SSL_CLIENT_S_DN_O",
"SSL_CLIENT_S_DN_OU",
"SSL_CLIENT_S_DN_C",
"SSL_CLIENT_S_DN_SP",
"SSL_CLIENT_S_DN_L",
"SSL_CLIENT_I_DN",
"SSL_CLIENT_I_DN_CN",
"SSL_CLIENT_I_DN_Email",
"SSL_CLIENT_I_DN_O",
"SSL_CLIENT_I_DN_OU",
"SSL_CLIENT_I_DN_C",
"SSL_CLIENT_I_DN_SP",
"SSL_CLIENT_I_DN_L",
"SSL_CIPHER_EXPORT",
"SSL_CIPHER_ALGKEYSIZE",
"SSL_CIPHER_USEKEYSIZE",
"SSL_VERSION_LIBRARY",
0L
} ;
int main(int argc, char* argv[]) 
{
	// set this to be whatever you'd like. Earthshattering is my domain  -- blsl
	const char *ns="http://www.earthshattering.org/test";

	SOAPString endpoint;
	SOAPString keyfile;
	SOAPString password;
	// for use without certificates.
	if (argc == 2) {
			endpoint = argv[1];
	} else if (argc == 4) {
			endpoint = argv[1];
			keyfile = argv[2];
			struct stat buf;
			if (0 != stat(keyfile, &buf)) {
				fprintf(stdout, "unable to open keyfile\n");
				exit(1);
		}
			password = argv[3];
	} else {
			fprintf(stdout, "Incorrect command line params specified. Try this:\n \
							SSLClient <endpoint> [<path-to-cert> <cert-password>]\n\n");
			exit(1);
	}
	try {
		SOAPDebugger::SetFile("./soapdebug.log");
		SOAPDebugger::SetMessageLevel(5);

		SOAPDebugger::Print(3, "Keyfile: %s\n", keyfile.Str());


	
		
		SOAPonHTTP http;
		SOAPProxy proxy(&http);

		if (argc > 2) 
			http.SetCertificateInfo(keyfile.Str(), password.Str());

		http.ConnectTo(SOAPUrl(endpoint));

		SOAPMethod echoSSL("EchoSSL", ns);

		const SOAPResponse &resp = proxy.Execute(echoSSL);

		// write some code to print out the environment variables.
		int i = 0;
		while (SSLEnvVars[i] != 0L) {
				SOAPString tmp;
				resp.GetBody().GetMethod().GetParameter(SSLEnvVars[i]) >> tmp;
				fprintf(stdout, "%s : %s\n", SSLEnvVars[i], tmp.Str());
				++i;
		}
	
			
	} catch(SOAPException &e) {
		fprintf(stdout, "SOAPFault: %s", e.What().Str());
	}
}




