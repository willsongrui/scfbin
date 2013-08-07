/* ines.h, by Zhang Haibin 
 */

#ifndef _INES_H
#define _INES_H

#include <stdio.h>

#ifndef WIN32
#include <sys/time.h>
#endif


#ifdef WIN32
#include <winsock2.h>
#endif

// main() will call this function
// parameter name is argv[1] of main()
void inesInitialize(char* name);

#ifndef _TCODE
#define _TCODE
struct TCode
{
   public :
      TCode()
      {
         length=0;
         content=0L;
      }
      int length;
      char* content;
};
#endif

extern int scfID;
extern char versionInfo[];
void initialVersionInfo();

// define TINESMsgHandler as a function pointer
// the function must have two parameter (int and TCode) and return void
typedef void TINESMsgHandler(int id,TCode code);

//#define linkConnected      1
//#define linkDisonnected    0
typedef void TConnectionProcessFunc(int linkid, int state); 

// name is the logic name that you configured in 
// config.server.* or config.client.*
// the protocol must not be text0ines
int setINESMsgHandler(const char* name,TINESMsgHandler function);

// name is the logic name that you configured in 
// config.server.* or config.client.*
int setConnectionHandler(char* name, TConnectionProcessFunc function);


// when SLP use printf { -Function ... 
// system call this function
char* specialFunction(int argc, char* argv[]);

int scfPreSelect(fd_set *readMask,fd_set *writeMask,fd_set *exceptMask,timeval * timer);
void scfPostSelect(fd_set *readMask,fd_set *writeMask,fd_set *exceptMask);

// system main loop call this function
void checkINESEvent();

// system main loop call this function every 100ms.
void checkINESEventOnTick();

// call this function to send event to SLP
// SLP will meet a serviceEvent
// return  0: success
// retuen -1: can not find fsm according to key
int backEvent(char* key,char* content);

void inesToTrace(char* key);

int inesFunctionIsTraced();

void inesTextToMonitor(char* key, char* text);

// call this function to send message to SLP
// SLP use scanf SIB to receive this message
void backCommand(char* content);

// call this funcatio to get a block of data from FSM
void* getINESData(char* key,int length=0);

void* findINESData(int offset,char* key);

#define SCF_REG_INES_DATA(structName) \
{\
   inesDataLength=sizeof(structName);\
}
#define SCF_FIND_INES_DATA(structName,fieldName,key,p) \
{\
   structName x;\
   char* p1=(char*)(&(x.fieldName[0]));\
   char* p2=(char*)(&x);\
   int offset=(int)(p1-p2);\
   *p=(structName*)findINESData(offset,key);\
}

typedef void TINESDataConstructor(void* p);
typedef void TINESDataDestructor(void *p);
typedef void TINESDataEventProc(void *p, long handle, int msgType, int msgID);
typedef void TConsoleCommandHandler(int argc, char* argv[]);

/* for example, you defined a function as Constructor:
      void initialMyINESData(void p)
      {
         TMyINESData* pData=(TMyINESData*)p;
         pData->initial();
      }
   then you can call:
      setINESDataConstructor(initialMyINESData);
   to set the ines data constructor
   when getINESData, if a new pointer, SCF will call this function to initial
*/
void setINESDataConstructor(TINESDataConstructor function);
void setINESDataDestructor(TINESDataDestructor function);
void setINESDataEventProc(TINESDataEventProc function);
void setConsoleCommandHandler(TConsoleCommandHandler function, const char* prompt, const char* name);

// return 0: success
// return -1: invalid link id
// return -2: invalid logic name
// return -3: key not match
// delete a socket link according to link id
// if key is NULL, or key is "", ignore key
int deleteLink(const char* name,int link, const char* key=NULL);

// return 0: success, only for conless client 
// return -1: invalid protocol
// return -2: invalid logic name
// you can set another hostname and port before send message
// the argument 'key' is to correlat the socket link to your instance
int setDestAddress(char* logicName,char* hostName,int port,char* key=(char*)"");

// return >=0: linkID
// return <0 : failure
// the argument 'key' is to correlat the socket link to your instance
int tcpConnectToServer(char* logicName,char* hostName,int port,char* key);

char* tcpProtocolType(const char* logicName);

// get peer ip address according to logicName and link id
char* tcpGetPeerIp(char* logicName,int linkID);

// get local ip address according to logicName and link id
char* tcpGetLocalIp(char* logicName,int linkID);

//get peer port according to logicName and link id
int tcpGetPeerPort(char* logicName,int linkID);

//get local port according to logicName and link id
int tcpGetLocalPort(char* logicName,int linkID);

// if a long connections and have alive link id, return the link id
// else return -1;
int tcpGetAliveLink(const char* name);

// get key according to logicName and link id
// return 0: success, only for conless client 
// return -1: invalid logicName or linkID
int getLinkKey(const char* logicName, int linkID, char* key);

// set key to the current link that receved message right now
// return 0: success
// return -1: invalid current link
int setCurrentLinkKey(char* key);

// call this function to send message to socket
// return  0: success
// return -1: invalid name
// return -2: socket client send, invalid linkID
// return -3: socket server send, invalid linkID
// return -4: socket client send, link is NULL
// return -5: socket server send, link is NULL
// return -6: socket link send error
int sendOut(char* name,int& linkID,char* s,int len,int segment=0,char* key=NULL);

// call this function to send message to sms fifo 
// return  0: success
// return  -10: fifo is NULL 
int send2fifo(const char* content,int length);

// call this function to invoke a service logic
// correlationKey will assign to the CCB correlationKey
// return 0: success
// return -1: create fsm failure
int invokeServiceLogic(char* arg,char* correlationKey=NULL);
int invokeServiceLogic(int serviceKey, char* arg, char* correlationKey=NULL);
int invokeServiceLogic(int serviceKey, char* logicName, int linkID, char* arg, int argLength, char* correlationKey=NULL, void* inesData=NULL);

int invokeServiceLogicByName(const char* name,char* argv);

// cmd_state will call this function to display ines data
void printINESData(void* p);

// return >=0: connections number
// return -1:  no such name 
// returm -2:  connections disabled
int inesConnectionState(const char* name);

// return 0:  open
// return -1: invalid logic name
// return -2: socket disabled
// return -3: invalid link
// return -4: link state is not open
int inesLinkState(char* name,char* link);

void* scfNewSharedMemory(int size, int& firstCreate);
void scfUnlockSharedMemory();
void scfLockSharedMemory();

#ifdef WIN32
#define SCF_NEW_SHARED_MEM(type,p,firstCreate) { p=new type; firstCreate=1; }
#else
#define SCF_NEW_SHARED_MEM(type,p,firstCreate)\
{\
   p=(type*)scfNewSharedMemory(sizeof(type),firstCreate);\
}
#endif

/* For HTTP */
int httpGet( const char* logicName, const char* strurl, const char* key,const char* header=NULL,int persistentConnection=0);

int httpPost(const char* logicName, const char* strurl, const char* data, \
             const char* key, int length=-1,const char* header=NULL,      \
             const char* contentType=NULL,const char* soapAction=NULL,\
             char* sAuthorization=NULL,int persistentConnection=0);
int httpReq(const char* logicName, char* reqMethod, const char* strurl, const char* data, \
             const char* key, int length=-1,const char* header=NULL,      \
             const char* contentType=NULL,const char* soapAction=NULL,\
             char* sAuthorization=NULL,int persistentConnection=0);
            
int httpResponse(const char* logicName, int linkID, \
                 int statusCode, const char* data = NULL, \
                 int nLength=-1,const char* protocol=NULL,\
                 int closeLink=1,char*  version=NULL,const char* header=NULL);

char* httpGetBody(const char* httpPacket);
char* httpGetURLParameter(const char* url, const char* varName, char* value, int length);

int httpUpLoadFile(const char* logicName, const char* key, const char* url, \
                   const char* fileName, int persistentConnection=0);

/*
 * Function  : Get status code from HTTP-Server's reponse packet.
 * Parameter :
 *    httpPacket is HTTP-Server's reponse past on call-back function.
 * Return    :
 *    > 0, status code of the response.
 *    < 0, failure.
 *
 * Mark: Please reference to RFC2616 or RFC1945 to learn valid status code.
 */
int getHttpResponseStatus(const char *httpPacket);

/*
 * Function  : Get version from HTTP-Server's reponse packet.
 * Parameter :
 *    httpPacket is HTTP-Server's reponse past on call-back function.
 *    version is used to save value of HTTP's version.
 *    size is the size of parameter version.
 *
 * Return   :
 *    = 0, success.
 *    < 0, failure.
 *
 * Mark : HTTP/1.0 and HTTP/1.1 is valid vales of HTTP's version currently.
 *        HTTP/0.9 nearly unused now.
 */
int getHttpResponseVersion(const char *httpPacket, char *version, int size);

/*
 * Function  : Get request method from HTTP-Client's request packet.
 * Parameter :
 *    httpPacket is HTTP-Client's request past on call-back function.
 *    method is used to save value of HTTP's request method.
 *    size is the size of parameter method.
 *
 * Return   :
 *    = 0, success.
 *    < 0, failure.
 *
 * Mark : Please reference to RFC2616 or RFC1945 to learn valid method.
 */
int getHttpRequestMethod(const char *httpPacket, char *method, int size);

/*
 * Function  : Get version from HTTP-Client's request packet.
 * Parameter :
 *    httpPacket is HTTP-Client's request past on call-back function.
 *    version is used to save value of HTTP's version.
 *    size is the size of parameter version.
 *
 * Return   :
 *    = 0, success.
 *    < 0, failure.
 *
 * Mark : HTTP/1.0 and HTTP/1.1 is valid vales of HTTP's version currently.
 *        HTTP/0.9 nearly unused now.
 */
int getHttpRequestVersion(const char *httpPacket, char *version, int size);

/*
 * Function  : Get file from HTTP-Client's request packet.
 * Parameter :
 *    httpPacket is HTTP-Client's request past on call-back function.
 *    file is used to save value of HTTP's version.
 *    size is the size of parameter file.
 *
 * Return   :
 *    = 0, success.
 *    < 0, failure.
 */
int getHttpRequestFile(const char *httpPacket, char *file, int size);

/*
 * Function  : Get value of header from HTTP-Client's request packet
 *    or HTTP-Server's response packet.
 * Parameter :
 *    packet is HTTP-Client's request packet or HTTP-Server's response packet.
 *    tag is key word of HTTP header.
 *    value is used to save value of HTTP header.
 *    size is the size of parameter value.
 *
 * Return   :
 *   = 0, success.
 *   < 0, failure.
 */
int getHttpHeader(const char *packet, const char *tag, char *value, int size);

void SCFERROR( const char* fmt, ...);
void SCFERROR( int errorID, const char* fmt, ...);
void SCFERROR(int errID,int level, const char* fmt, ... );
void SCFINFO( const char* fmt, ...);
void term( const char* fmt, ...);

void printBin(const char* s,int length);
void printBinInfo(const char* s, int length, int fsm, const char* info);
void printBinInfo(const char* s, int length, const char* fsmKey, const char* info);
void printBinInfo(const char* s, int length, const char* info);
void printTextInfo(const char* fmt, ...);

void vxmlTextEncode(char* result,char* text);
void vxmlTextDecode(char* s);

/* you can use these two function only in msgProcessFunc and connectionProcessFunc */
char* getCurrentLogicName();
char* getCurrentClientName();

/* return NULL, if logicName or linkID don't existed*/
char* getClientName(char* logicName,int linkID);

int getSockInfoByName(char* logicName, int* isClient=NULL, int* port=NULL, char* protocolName=NULL, int* connections=NULL, char* hostName=NULL);

extern int inesDataLength;

int  scsmGetLinkedID(long handle);
int  scsmGetXmlEventID(long handle);
int  scsmGetInvokeID(long handle);
int  scsmGetTCPrimitiveID(long handle);
void scsmSetTimer(long handle, int timer);
void scsmEndSelf(long handle);
long scsmGetAsnTree(long handle);
long scsmCreateNew();
void scsmTcapSetDestAddr(long handle,const char* addr);
void scsmTcapApplyDialogueID(long handle);

long        scfAsnGetTree(long treeHandle, int id);
const char* scfAsnGetStrValue(long treeHandle, int* length);
int         scfAsnGetIntValue(long treeHandle);

typedef void TTimeOutFunc (int, void*);
//these two funcs can not use in initialVersionInfo()
int apiSetTimer(void* data, int nTime, TTimeOutFunc* func);
void apiResetTimer(int timerID);

/* this part for set route rule,usually, the rule 
is used by SLPL to set destionation node name*/

/*
 route policy ,needed by addRoute,don't change it.
 P21 : STR,EQUAL,M-B and Polling
 P22 : STR,MATCH,M-B and No Polling
 P23 : STR,MATCH,M-B and Polling
 P24 : STR,MATCH,M-B Switch and Polling
*/
#define P21  21
#define P22  22
#define P23  23
#define P24  24

/* return value define,don't change it.  */
#define RULE_NOT_EXISTED -1
#define ROUTEID_INVALID  -2
#define ROUTEID_EXISTED  -3
#define ROUTE_NOT_REACHED   -4
#define ROUTETAB_FULL  -5
#define ROUTEKEY_INVALID -6
#define ROUTEID_NOT_EXISTED -7

typedef int TRouteStateFunc(const char* name); 

/*scfAddRoute:
 * add a route table to system.
 * routeID:   defined by user,it need more than 100000.
 * routePolicy: system predefined route policy.
 * routeStateFunc: detect destnation route device.set by user,when route device is not INES .
 */
int scfAddRoute(int routeID,int routePolicy,TRouteStateFunc* routeStateFunc=NULL);

/*scfSetRouteStateFunc:
 * reset route state function.
 * routeID:   defined by user or serviceconfig.
 * return: 0 ,success; or ROUTEID_NOT_EXISTED;
 */
int scfSetRouteStateFunc(int routeID,TRouteStateFunc* routeStateFunc=NULL);

/*API addRule:
 * add rule to route table.
 * routeID: defined by user,it need more than 100000,ensure the route table have add by user.
 * key   :  defined by user.
 * primary: primary device.
 * secondary: secondary device,optional.
 * return :  0: Success; 
            -1: Invalid ruleID; 
            -2:No romm.
 */
int scfAddRule(int routeID,const char* key,const char* primary,const char* secondary=NULL);

/*remove route table*/ 
int scfRemoveRoute(int routeID);

/*get routename,route policy set by addRoute
  name : output buffer,set as primary or secondary.
  key  : "?" ,getRouteAny
         "%" ,getRouteAny and M-B switch.
         other, set by routePolicy.

  return 0:Success
        -2:Invalid routeID
        -3:Invalid Key
        -4:Not Reached
*/
int scfGetRoute(int routeID,const char* value,char* name);


/* output monitor msg for INES, msgType must between 30 and 60 */
void outputOtherMsgRcv(int msgType, const char* srcIP, int srcPort, const char* destIP, int destPort, const char* content, int len);
void outputOtherMsgSend(int msgType, const char* srcIP, int srcPort, const char* destIP, int destPort, const char* content, int len);

#endif
