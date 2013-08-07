#ifndef __FUNCAPI_H
#define __FUNCAPI_H

#include <stdio.h>
#ifdef _LINUX
#include <time.h>
#include <sys/time.h>
#else
#include <time.h>
#endif

void ascToBCD2(char* dest,const char* src,unsigned char append);
void ascToBCD0(char* dest,const char* src);
void bcdToASC2(char* dest,const char* src,int length);

void ascToBin(char* dest,char* src,int length);
void binToASC(char* dest,char* src,int length);

// get the string before '|'
char* getStringI(char** p,char* result,int resultLen=256);
// get the string before space or tab
char* getStringST(char** p,char* result,int resultLen=256);
// get the string before ',' 
char* getStringComma(char** p,char* result,int resultLen=256);
// skip space or tab
char* skipST(char** p);
// reverse skip space or tab
char* reverseSkipST(char** p);

// like fgets, skip null line and #line
char* fGetLine(char *buf, int buf_size, FILE *fp, int* lineNo=NULL);

//convert string to UPPER
void toupperStr(char *str);

void substituteStr(const char* src, const char* s1, char* s2, char *dest);
int strReplace(char* s, int len, const char* pattern, int patternLen, const char* replace, int replaceLen, int maxLen);
void alignStr(char* str,char* dest,char c,int len);
void trimStr(char*);
void trimStrPunctuation(char* s);
void trimStrL(char*);
void trimStrR(char*);
void trimAllSP(char*);  //rm all SP in str
void pickWord(const char* str, const char* sep, int i, char* result, int len);

void strCopy(char* dest,const char* src,int len);
void strCat(char*& destStr,char* catStr,int len);
void deleteLastEnter(char* s);

int unicodeToUTF8Str(int unicode, char* pUTF8);
int UTF8toUCS2(char *pUTF8, char *pUCS, int UTFlen, int bigEndian=0);
int UCS2toUTF8(char* pUCS, char* pUTF8, int UCSLen, int bigEndian=0);

int  GBKToUCS2(char* src, char* buf, int bufSize, int& destLen);
int  UCS2ToGBK(char* src, int srcLen, char* buf, int bufSize, int& destLen);

int UTF8ToGBK(char* src, char* buf, int bufSize);
int GBKToUTF8(char* src, char* buf, int bufSize);

extern "C"
{
void md5(char*src, char* dest, int srcLen);
}

char* getLocalIpAddress();
char* getLocalHostName();

// level 1, min match; level 2, max match
char* keytreeInterpret(char* number,int level);

#define  MIMELINELIMIT 76
//return char* need delete by ines developer, for mime, lineLimit should be MIMELINELIMIT
char* base64Encode(const unsigned char* pSrc,int nSrcLen, int lineLimit=-1);
unsigned char* base64Decode(const char* pSrc,int& nDstLen);

//TXMLHandle*, return void* need delete by ines developer
int cpimEncode(void* handle, char* buf, int bufSize, char* con, int conLength);
void* cpimDecode(char** src,int len, char** con, int* conLength);

#endif
