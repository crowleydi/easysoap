/* HTTP Digest Authentication w/ RC5 from RFC 2617
 * http://www.ietf.org/rfc/rfc2617.txt
 */

#include <string.h>
#include <easysoap/es_conf.h>

#ifdef HAVE_STRCASECMP
#define STRICMP strcasecmp
#else
#ifdef HAVE_STRICMP
#if _MSC_VER >= 1400
#define STRICMP _stricmp
#else
#define STRICMP stricmp
#endif
#else
#error "No stricmp function."
#endif
#endif

#include "global.h"
#include "md5.h"
#include "digcalc.h"

void CvtHex(
    IN HASH Bin,
    OUT HASHHEX Hex
    )
{
    unsigned short i;
    unsigned char j;

    for (i = 0; i < HASHLEN; i++) {
        j = (Bin[i] >> 4) & 0xf;
        if (j <= 9)
            Hex[i*2] = (j + '0');
         else
            Hex[i*2] = (j + 'a' - 10);
        j = Bin[i] & 0xf;
        if (j <= 9)
            Hex[i*2+1] = (j + '0');
         else
            Hex[i*2+1] = (j + 'a' - 10);
    };
    Hex[HASHHEXLEN] = '\0';
}

/* calculate H(A1) as per spec */
void DigestCalcHA1(
    IN const char * pszAlg,
    IN const char * pszUserName,
    IN const char * pszRealm,
    IN const char * pszPassword,
    IN const char * pszNonce,
    IN const char * pszCNonce,
    OUT HASHHEX SessionKey
    )
{
      MD5_CTX Md5Ctx;
      HASH HA1;

      MD5Init(&Md5Ctx);
      MD5Update(&Md5Ctx, pszUserName, strlen(pszUserName));
      MD5Update(&Md5Ctx, ":", 1);
      MD5Update(&Md5Ctx, pszRealm, strlen(pszRealm));
      MD5Update(&Md5Ctx, ":", 1);
      MD5Update(&Md5Ctx, pszPassword, strlen(pszPassword));
      MD5Final(HA1, &Md5Ctx);
      if (STRICMP(pszAlg, "md5-sess") == 0) {
            MD5Init(&Md5Ctx);
            MD5Update(&Md5Ctx, HA1, HASHLEN);
            MD5Update(&Md5Ctx, ":", 1);
            MD5Update(&Md5Ctx, pszNonce, strlen(pszNonce));
            MD5Update(&Md5Ctx, ":", 1);
            MD5Update(&Md5Ctx, pszCNonce, strlen(pszCNonce));
            MD5Final(HA1, &Md5Ctx);
      };
      CvtHex(HA1, SessionKey);
}

/* calculate request-digest/response-digest as per HTTP Digest spec */
void DigestCalcResponse(
    IN const HASHHEX HA1,           /* H(A1) */
    IN const char * pszNonce,       /* nonce from server */
    IN const char * pszNonceCount,  /* 8 hex digits */
    IN const char * pszCNonce,      /* client nonce */
    IN const char * pszQop,         /* qop-value: "", "auth", "auth-int" */
    IN const char * pszMethod,      /* method from the request */
    IN const char * pszDigestUri,   /* requested URL */
    IN const HASHHEX HEntity,       /* H(entity body) if qop="auth-int" */
    OUT HASHHEX Response      /* request-digest or response-digest */
    )
{
      MD5_CTX Md5Ctx;
      HASH HA2;
      HASH RespHash;
      HASHHEX HA2Hex;

      /* calculate H(A2) */
      MD5Init(&Md5Ctx);
      MD5Update(&Md5Ctx, pszMethod, strlen(pszMethod));
      MD5Update(&Md5Ctx, ":", 1);
      MD5Update(&Md5Ctx, pszDigestUri, strlen(pszDigestUri));
      if (STRICMP(pszQop, "auth-int") == 0) {
            MD5Update(&Md5Ctx, ":", 1);
            MD5Update(&Md5Ctx, HEntity, HASHHEXLEN);
      };
      MD5Final(HA2, &Md5Ctx);
       CvtHex(HA2, HA2Hex);

      /* calculate response */
      MD5Init(&Md5Ctx);
      MD5Update(&Md5Ctx, HA1, HASHHEXLEN);
      MD5Update(&Md5Ctx, ":", 1);
      MD5Update(&Md5Ctx, pszNonce, strlen(pszNonce));
      MD5Update(&Md5Ctx, ":", 1);
      if (*pszQop) {
          MD5Update(&Md5Ctx, pszNonceCount, strlen(pszNonceCount));
          MD5Update(&Md5Ctx, ":", 1);
          MD5Update(&Md5Ctx, pszCNonce, strlen(pszCNonce));
          MD5Update(&Md5Ctx, ":", 1);
          MD5Update(&Md5Ctx, pszQop, strlen(pszQop));
          MD5Update(&Md5Ctx, ":", 1);
      };
      MD5Update(&Md5Ctx, HA2Hex, HASHHEXLEN);
      MD5Final(RespHash, &Md5Ctx);
      CvtHex(RespHash, Response);
}

