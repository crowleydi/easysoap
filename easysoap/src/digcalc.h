/* HTTP Digest Authentication w/ RC5 from RFC 2617
 * http://www.ietf.org/rfc/rfc2617.txt
 */

#ifndef _digcalc_h_
#define _digcalc_h_

#define HASHLEN 16
typedef char HASH[HASHLEN];
#define HASHHEXLEN 32
typedef char HASHHEX[HASHHEXLEN+1];
#define IN
#define OUT

/* calculate H(A1) as per HTTP Digest spec */
void DigestCalcHA1(
    IN const char * pszAlg,
    IN const char * pszUserName,
    IN const char * pszRealm,
    IN const char * pszPassword,
    IN const char * pszNonce,
    IN const char * pszCNonce,
    OUT HASHHEX SessionKey
    );

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
    );

#endif // _digcalc_h_

