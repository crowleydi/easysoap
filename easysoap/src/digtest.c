/* HTTP Digest Authentication w/ RC5 from RFC 2617
 * http://www.ietf.org/rfc/rfc2617.txt
 */

#include <stdio.h>
#include "digcalc.h"

void main(int argc, char ** argv) {

      char * pszNonce = "dcd98b7102dd2f0e8b11d0f600bfb0c093";
      char * pszCNonce = "0a4f113b";
      char * pszUser = "Mufasa";
      char * pszRealm = "testrealm@host.com";
      char * pszPass = "Circle Of Life";
      char * pszAlg = "md5";
      char szNonceCount[9] = "00000001";
      char * pszMethod = "GET";
      char * pszQop = "auth";
      char * pszURI = "/dir/index.html";
      HASHHEX HA1;
      HASHHEX HA2 = "";
      HASHHEX Response;

      DigestCalcHA1(pszAlg, pszUser, pszRealm, pszPass, pszNonce,
pszCNonce, HA1);
      DigestCalcResponse(HA1, pszNonce, szNonceCount, pszCNonce, pszQop,
       pszMethod, pszURI, HA2, Response);
      printf("Response = %s\n", Response);
};

