/*
 * ftplib.h
 *
 *  Created on: Oct 10, 2022
 *      Author: minht
 */

#ifndef FTPLIB_H_
#define FTPLIB_H_

/* FtpAccess() type codes */
#define FTPLIB_DIR 1
#define FTPLIB_DIR_VERBOSE 2
#define FTPLIB_FILE_READ 3
#define FTPLIB_FILE_WRITE 4


/* FtpAccess() mode codes */
#define FTPLIB_ASCII 'A'
#define FTPLIB_IMAGE 'I'
#define FTPLIB_TEXT FTPLIB_ASCII
#define FTPLIB_BINARY FTPLIB_IMAGE

/* connection modes */
#define FTPLIB_PASSIVE 1
#define FTPLIB_PORT 2

/* connection option names */
#define FTPLIB_CONNMODE 1
#define FTPLIB_CALLBACK 2
#define FTPLIB_IDLETIME 3
#define FTPLIB_CALLBACKARG 4
#define FTPLIB_CALLBACKBYTES 5

#define CHECKS_GH		5
#define TIME_PUST		60000			// 60s
#define SIZE_VERBOSE	17

#ifdef __cplusplus
extern "C" {
#endif
typedef struct NetBuf netbuf;


typedef int (*FtpCallback)(netbuf *nControl, int xfered, void *arg);

char *FtpLastResponse(netbuf *nControl);
int FtpConnect(const char *host, netbuf **nControl);
int FtpLogin(const char *user,const char *pass, netbuf *nControl);
int FtpAccess(const char *path, int typ, int mode, netbuf *nControl,
netbuf **nData);
int FtpRead(void *buf, int max, netbuf *nData);
int FtpWrite(void *buf, int len, netbuf *nData);
int FtpClose(netbuf *nData);
int FtpSite(const char *cmd, netbuf *nControl);
int FtpSysType(const char *buf, int max, netbuf *nControl);
int FtpMkdir(const char *path, netbuf *nControl);
int FtpChdir(const char *path, netbuf *nControl);
int FtpCDUp(netbuf *nControl);
int FtpRmdir(const char *path, netbuf *nControl);
int FtpPwd(const char *path, int max, netbuf *nControl);
int FtpNlst(const char *output,const char *path, netbuf *nControl);
int FtpDir(const char *output,const char *path, netbuf *nControl);
int FtpSize(const char *path, int *size, char mode, netbuf *nControl);
int FtpModDate(const char *path,const char *dt, int max, netbuf *nControl);
int FtpGet(const char *output,const char *path, char mode,
netbuf *nControl);
int FtpPut(const char *input,const char *path, char mode,
netbuf *nControl);
int FtpRename(const char *src,const char *dst, netbuf *nControl);
int FtpDelete(const char *fnm, netbuf *nControl);
void FtpQuit(netbuf *nControl);
void App_Ftp(char* data);

#ifdef __cplusplus
};
#endif

#endif /* FTPLIB_H_ */
