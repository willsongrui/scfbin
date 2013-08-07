#ifndef _XMLAPI_H
#define _XMLAPI_H 
// --------------------------------------------------------------
// about XML process
// --------------------------------------------------------------
typedef void* TXMLHandle;

char* xmlGetAPIVersion();

char* xmlTextEncode(char* xmlText);

// return 0 if xml string format error
TXMLHandle xmlGetTree(char* xmlString);

// print the tree to inaccess
void xmlPrintTree(TXMLHandle);

void xmlFreeTree(TXMLHandle);

// get the value of the xml element that the path indicated
// path format: tag/tag/tag
//              tag/tag[n]/tag
// return 0 if success
int xmlGetValue(TXMLHandle,const char* path,char* value);
int xmlGetValue(TXMLHandle,const char* path,char* value,int length);

//return direct child count or return -1 if failed.
int xmlGetDirectChildren(TXMLHandle,char* path);
int xmlGetDirectChildrenByTag(TXMLHandle,char* path);
int xmlGetElemName(TXMLHandle,char* path,int index,char* name);

// get the nth child's tag name of the xml element that the path indicated
// if path[0]==0, tagName will be the root's name
// path format: tag/tag/tag
//              tag/tag[n]/tag
// return 0 if success
int xmlGetChild(TXMLHandle,char* path,int childNo,char* tagName);
int xmlGetChild(TXMLHandle,char* path,int childNo,char* tagName,int length);

// get the nth attribute name & value of the xml element that the path indicated
// path format: tag/tag/tag
//              tag/tag[n]/tag
// return 0 if success
int xmlGetAttribute(TXMLHandle,char* path,int childNo,char* attrName, char* attrValue);
int xmlGetAttribute(TXMLHandle,char* path,int childNo,char* attrName, char* attrValue, int length);

//getAttribut directly by path
//add by huangyu 20050622
int xmlGetAttribute(TXMLHandle, char* path0, char* value, int length);

//simple XML API
//use this interface can transfer a XML Tree easier.
//Add by LYH 2003.2.25
char *sXmlGetTag(TXMLHandle handle);
char *sXmlGetValue(TXMLHandle handle);
int sXmlGetChildNum(TXMLHandle handle);
TXMLHandle sXmlGetChild(TXMLHandle handle, int i);
TXMLHandle sXmlGetChild(TXMLHandle handle, char *subTag);
int sXmlGetAttrNum(TXMLHandle handle);
char* sXmlGetAttrName(TXMLHandle handle, int i);
char* sXmlGetAttrValue(TXMLHandle handle, int i);
char* sXmlGetAttrValue(TXMLHandle handle, const char* attrName);
char* sXmlGetValue(TXMLHandle handle, const char* path);

//xml parser options
#define XML_FORMAT_CONTENT 0
#define XML_SHOW_PARSEPROGRESS 1
//other options may be added later

void sXmlSetOptions(int oName, int oValue);
int sXmlGetOptions(int oName);
//flag,0: query parent's brothers, default value
//     1: do not query parent's brothers
int xmlQuery(TXMLHandle handle,char* path0,char* value,int length,TXMLHandle* pXML,int flag=0);

#endif
