/*******************************************************************************
**
** abyss.h
**
** This file is part of the ABYSS Web server project.
**
** Copyright (C) 2000 by Moez Mahfoudh <mmoez@bigfoot.com>.
** All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions
** are met:
** 1. Redistributions of source code must retain the above copyright
**    notice, this list of conditions and the following disclaimer.
** 2. Redistributions in binary form must reproduce the above copyright
**    notice, this list of conditions and the following disclaimer in the
**    documentation and/or other materials provided with the distribution.
** 3. The name of the author may not be used to endorse or promote products
**    derived from this software without specific prior written permission.
** 
** THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
** ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
** IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
** ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
** FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
** DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
** OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
** HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
** LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
** OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
** SUCH DAMAGE.
**
*******************************************************************************/

#ifndef _ABYSS_H_
#define _ABYSS_H_

/*********************************************************************
** EDIT THE FOLLOWING LINES TO MEET YOUR CONFIGURATION NEEDS
*********************************************************************/

/*********************************************************************
** Paths and so on...
*********************************************************************/

//#ifdef _WIN32
//#define DEFAULT_ROOT		"c:\\abyss"
//#define DEFAULT_DOCS		DEFAULT_ROOT"\\htdocs"
//#define DEFAULT_CONF_FILE	DEFAULT_ROOT"\\conf\\abyss.conf"
//#define DEFAULT_LOG_FILE	DEFAULT_ROOT"\\log\\abyss.log"
//#else
//#define DEFAULT_ROOT		"/usr/local/abyss"
//#define DEFAULT_DOCS		DEFAULT_ROOT"/htdocs"
//#define DEFAULT_CONF_FILE	DEFAULT_ROOT"/conf/abyss.conf"
//#define DEFAULT_LOG_FILE	DEFAULT_ROOT"/log/abyss.log"
//#endif

/*********************************************************************
** Maximum numer of simultaneous connections
*********************************************************************/

#define MAX_CONN	16

/*********************************************************************
** DON'T CHANGE THE FOLLOWING LINES
*********************************************************************/

/*********************************************************************
** Server Info Definitions
*********************************************************************/

#define SERVER_VERSION		"0.3"
#define SERVER_HVERSION		"ABYSS/0.3"
#define SERVER_HTML_INFO	"<p><HR><b><i><a href=\"http:\057\057abyss.linuxave.net\">ABYSS Web Server</a></i></b> version "SERVER_VERSION"<br>&copy; <a href=\"mailto:mmoez@bigfoot.com\">Moez Mahfoudh</a> - 2000</p>"
#define SERVER_PLAIN_INFO	CRLF"--------------------------------------------------------------------------------"\
							CRLF"ABYSS Web Server version "SERVER_VERSION CRLF"(C) Moez Mahfoudh - 2000"

/*********************************************************************
** General purpose definitions
*********************************************************************/

#ifdef _WIN32
#define strcasecmp(a,b)	stricmp((a),(b))
#else
#define ioctlsocket(a,b,c)	ioctl((a),(b),(c))
#endif	/* _WIN32 */

#ifndef NULL
#define NULL ((void *)0)
#endif	/* NULL */

#ifndef TRUE
#define TRUE	1
#endif	/* TRUE */

#ifndef FALSE
#define FALSE    0
#endif  /* FALSE */

#ifdef _WIN32
#define LBR	"\n"
#else
#define LBR "\n"
#endif	/* _WIN32 */

/*********************************************************************
** Typedefs
*********************************************************************/

typedef unsigned long uint64;
typedef long int64;

typedef unsigned int uint32;
typedef int int32;

typedef unsigned short uint16;
typedef short int16;

typedef unsigned char byte;
typedef unsigned char uint8;
typedef char int8;

/*********************************************************************
** Buffer
*********************************************************************/

typedef struct
{
	void *data;
	uint32 size;
	uint32 staticid;
} TBuffer;

int BufferAlloc(TBuffer *buf,uint32 memsize);
int BufferRealloc(TBuffer *buf,uint32 memsize);
void BufferFree(TBuffer *buf);


/*********************************************************************
** String
*********************************************************************/

typedef struct
{
	TBuffer buffer;
	uint32 size;
} TString;

int StringAlloc(TString *s);
int StringConcat(TString *s,char *s2);
int StringBlockConcat(TString *s,char *s2,char **ref);
void StringFree(TString *s);
char *StringData(TString *s);


/*********************************************************************
** List
*********************************************************************/

typedef struct
{
	void **item;
	uint16 size,maxsize;
	int autofree;
} TList;

void ListInit(TList *sl);
void ListInitAutoFree(TList *sl);
void ListFree(TList *sl);
int ListAdd(TList *sl,void *str);
int ListAddFromString(TList *list,char *c);
int ListFindString(TList *sl,char *str,uint16 *index);


/*********************************************************************
** Table
*********************************************************************/

typedef struct 
{
	char *name,*value;
	uint16 hash;
} TTableItem;

typedef struct
{
	TTableItem *item;
	uint16 size,maxsize;
} TTable;

void TableInit(TTable *t);
void TableFree(TTable *t);
int TableAdd(TTable *t,char *name,char *value);
int TableAddReplace(TTable *t,char *name,char *value);
int TableFindIndex(TTable *t,char *name,uint16 *index);
char *TableFind(TTable *t,char *name);


/*********************************************************************
** Thread
*********************************************************************/

#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif	/* _WIN32 */

typedef uint32 (*TThreadProc)(void *);
#ifdef _WIN32
typedef HANDLE TThread;
#else
typedef pthread_t TThread;
typedef void* (*PTHREAD_START_ROUTINE)(void *);
#endif	/* _WIN32 */

int ThreadCreate(TThread *t,TThreadProc func,void *arg);
int ThreadRun(TThread *t);
int ThreadStop(TThread *t);
int ThreadKill(TThread *t);
void ThreadWait(uint32 ms);


/*********************************************************************
** Mutex
*********************************************************************/

#ifdef _WIN32
typedef HANDLE TMutex;
#else
typedef pthread_mutex_t TMutex;
#endif	/* _WIN32 */

int MutexCreate(TMutex *m);
int MutexLock(TMutex *m);
int MutexUnlock(TMutex *m);
int MutexTryLock(TMutex *m);
void MutexFree(TMutex *m);


/*********************************************************************
** Pool
*********************************************************************/

typedef struct _TPoolZone
{
	char *pos,*maxpos;
	struct _TPoolZone *next,*prev;
	char data[1];
} TPoolZone;

typedef struct
{
	TPoolZone *firstzone,*currentzone;
	uint32 zonesize;
	TMutex mutex;
} TPool;

int PoolCreate(TPool *p,uint32 zonesize);
void PoolFree(TPool *p);

void *PoolAlloc(TPool *p,uint32 size);
char *PoolStrdup(TPool *p,char *s);


/*********************************************************************
** Socket
*********************************************************************/

#ifdef _WIN32
#include <winsock.h>
#else
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#endif	/* _WIN32 */

#define TIME_INFINITE	0xffffffff

#ifdef _WIN32
typedef SOCKET TSocket;
#else
typedef uint32 TSocket;
#endif	/* _WIN32 */

typedef struct in_addr TIPAddr;

int SocketInit();

int SocketCreate(TSocket *s);
int SocketClose(TSocket *s);

uint32 SocketWrite(TSocket *s, char *buffer, uint32 len);
uint32 SocketRead(TSocket *s, char *buffer, uint32 len);
uint32 SocketPeek(TSocket *s, char *buffer, uint32 len);

int SocketConnect(TSocket *s, TIPAddr *addr, uint16 port);
int SocketBind(TSocket *s, TIPAddr *addr, uint16 port);

int SocketListen(TSocket *s, uint32 backlog);
int SocketAccept(TSocket *s, TSocket *ns,TIPAddr *ip);

uint32 SocketError();

uint32 SocketWait(TSocket *s,int rd,int wr,uint32 timems);

int SocketBlocking(TSocket *s, int b);
uint32 SocketAvailableReadBytes(TSocket *s);

/*********************************************************************
** File
*********************************************************************/

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifndef NAME_MAX
#define NAME_MAX	1024
#endif

#if defined(_WIN32) && !defined(__BORLANDC__)
#define O_APPEND	_O_APPEND
#define O_CREAT 	_O_CREAT 
#define O_EXCL		_O_EXCL
#define O_RDONLY	_O_RDONLY
#define O_RDWR		_O_RDWR 
#define O_TRUNC		_O_TRUNC
#define O_WRONLY	_O_WRONLY
#define O_TEXT		_O_TEXT
#define O_BINARY	_O_BINARY

#define A_HIDDEN	_A_HIDDEN
#define A_NORMAL	_A_NORMAL
#define A_RDONLY	_A_RDONLY
#define	A_SUBDIR	_A_SUBDIR
#else
#define	A_SUBDIR	1
#define O_BINARY	0
#define O_TEXT		0
#endif	/* _WIN32 */

#ifdef _WIN32
#ifndef __BORLANDC__
typedef struct _stati64 TFileStat;
#else
typedef struct stati64 TFileStat;
#endif // __BORLANDC__
typedef struct _finddata_t TFileInfo;
typedef long TFileFind;
#else

#include <unistd.h>
#include <dirent.h>

typedef struct stat TFileStat;

typedef struct finddata_t
{
	char name[NAME_MAX+1];
	int attrib;
	uint64 size;
	time_t time_write;
} TFileInfo;

typedef struct 
{
	char path[NAME_MAX+1];
	DIR *handle;
} TFileFind;

#endif

typedef int TFile;

int FileOpen(TFile *f, char *name, uint32 attrib);
int FileOpenCreate(TFile *f, char *name, uint32 attrib);
int FileClose(TFile *f);

int FileWrite(TFile *f, void *buffer, uint32 len);
int32 FileRead(TFile *f, void *buffer, uint32 len);

int FileSeek(TFile *f, uint64 pos, uint32 attrib);
uint64 FileSize(TFile *f);

int FileStat(char *filename,TFileStat *filestat);

int FileFindFirst(TFileFind *filefind,char *path,TFileInfo *fileinfo);
int FileFindNext(TFileFind *filefind,TFileInfo *fileinfo);
void FileFindClose(TFileFind *filefind);

/*********************************************************************
** Server (1/2)
*********************************************************************/

typedef struct _TServer
{
	TSocket listensock;
	TFile logfile;
	TMutex logmutex;
	char *name;
	char *filespath;
	uint16 port;
	uint32 keepalivetimeout,keepalivemaxconn,timeout;
	TList handlers;
	TList defaultfilenames;
	void *defaulthandler;
	int advertise;
	void *userdata;
	int stopped;
#ifdef _UNIX
	uid_t uid;
	gid_t gid;
	TFile pidfile;
#endif	
} TServer;


/*********************************************************************
** Conn
*********************************************************************/

#define BUFFER_SIZE	4096 

typedef struct _TConn
{
	TServer *server;
	uint32 buffersize,bufferpos;
	uint32 inbytes,outbytes;	
	TSocket socket;
	TIPAddr peerip;
	TThread thread;
	int connected;
	void (*job)(struct _TConn *);
	char buffer[BUFFER_SIZE];
} TConn;

TConn *ConnAlloc();
void ConnFree(TConn *c);

int ConnCreate(TConn *c, TSocket *s, void (*func)(TConn *));
int ConnProcess(TConn *c);
int ConnKill(TConn *c);

int ConnWrite(TConn *c,void *buffer,uint32 size);
int ConnRead(TConn *c, uint32 timems);
void ConnReadInit(TConn *c);
int ConnReadLine(TConn *c,char **z,uint32 timems);
int ConnReadRaw(TConn *c, char *buffer, uint32 len, uint32 timeout, uint32 *read);
int ConnWriteFromFile(TConn *c,TFile *file,uint64 start,uint64 end,
			void *buffer,uint32 buffersize,uint32 rate);


/*********************************************************************
** Range
*********************************************************************/

int RangeDecode(char *str,uint64 filesize,uint64 *start,uint64 *end);

/*********************************************************************
** Date
*********************************************************************/

#include <time.h>

typedef struct tm TDate;

int DateToString(TDate *tm,char *s);
int DateToLogString(TDate *tm,char *s);

int DateDecode(char *s,TDate *tm);

int32 DateCompare(TDate *d1,TDate *d2);

int DateFromGMT(TDate *d,time_t t);
int DateFromLocal(TDate *d,time_t t);

int DateInit();

/*********************************************************************
** Base64
*********************************************************************/

void Base64Encode(char *s,char *d);

/*********************************************************************
** Session
*********************************************************************/

typedef enum
{
	m_unknown,m_get,m_put,m_head,m_post,m_delete,m_trace,m_options
} TMethod;

typedef struct _TSession
{
	TMethod method;
	uint32 nbfileds;
	char *uri;
	char *query;
	char *host;
	char *from;
	char *useragent;
	char *referer;
	char *requestline;
	char *user;
	uint16 port;
	TList cookies;
	TList ranges;

	uint16 status;
	TString header;

	int keepalive,cankeepalive;
	int done;

	TServer *server;
	TConn *conn;

	uint8 versionminor,versionmajor;

	TTable request_headers,response_headers;

	TDate date;

	int chunkedwrite,chunkedwritemode;
} TSession;

/*********************************************************************
** Request
*********************************************************************/

#define CR		'\r'
#define LF		'\n'
#define CRLF	"\r\n"

int RequestValidURI(TSession *r);
int RequestValidURIPath(TSession *r);
int RequestUnescapeURI(TSession *r);

char *RequestHeaderValue(TSession *r,char *name);

int RequestRead(TSession *r);
void RequestInit(TSession *r,TConn *c);
void RequestFree(TSession *r);

int RequestAuth(TSession *r,char *credential,char *user,char *pass);

/*********************************************************************
** Response
*********************************************************************/

int ResponseAddField(TSession *r,char *name,char *value);
void ResponseWrite(TSession *r);

int ResponseChunked(TSession *s);

void ResponseStatus(TSession *r,uint16 code);
void ResponseStatusErrno(TSession *r);

int ResponseContentType(TSession *r,char *type);
int ResponseContentLength(TSession *r,uint64 len);

void ResponseError(TSession *r);


/*********************************************************************
** HTTP
*********************************************************************/

char *HTTPReasonByStatus(uint16 status);

int32 HTTPRead(TSession *s,char *buffer,uint32 len);

int HTTPWrite(TSession *s,char *buffer,uint32 len);
int HTTPWriteEnd(TSession *s);

/*********************************************************************
** Server (2/2)
*********************************************************************/

typedef int (*URIHandler) (TSession *);

int ServerCreate(TServer *srv,char *name,uint16 port,char *filespath,
				  char *logfilename);
void ServerFree(TServer *srv);

void ServerInit(TServer *srv);
void ServerRun(TServer *srv);
void ServerSetUserData(TServer *srv, void *userdata);
void *ServerGetUserData(TServer *srv);

int ServerAddHandler(TServer *srv,URIHandler handler);
void ServerDefaultHandler(TServer *srv,URIHandler handler);

int LogOpen(TServer *srv, char *filename);
void LogWrite(TServer *srv,char *c);
void LogClose(TServer *srv);

void ResetServerStopFlag(TServer *srv);
void StopServer(TServer *srv);
int IsServerStopped(TServer *srv);


/*********************************************************************
** MIMEType
*********************************************************************/

void MIMETypeInit();
int MIMETypeAdd(char *type,char *ext);
char *MIMETypeFromExt(char *ext);
char *MIMETypeFromFileName(char *filename);
char *MIMETypeGuessFromFile(char *filename);


/*********************************************************************
** Conf
*********************************************************************/

int ConfReadMIMETypes(char *filename);
int ConfReadServerFile(char *filename,TServer *srv);


/*********************************************************************
** Trace
*********************************************************************/

void TraceMsg(char *fmt,...);
void TraceExit(char *fmt,...);


/*********************************************************************
** Session
*********************************************************************/

int SessionLog(TSession *s);

#endif	/* _ABYSS_H_ */
