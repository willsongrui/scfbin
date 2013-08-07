#include "memdebug.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "ines.h"
//#include "ftp.h"

// If you use printf { -Function ccb 'cmd ...' } SIB in SLP
// this function will be called when that SIB excuting
char* specialFunction(int argc, char* argv[])
{
   // In this function, you can process argc and argv

   if(inesFunctionIsTraced())
   {
      for(int i=0;i<=argc;i++)
         printf("argv[%d]='%s'\n",i,argv[i]);
   }

   // You can call backCommand to return a message to the SLP
   // SLP can use scanf SIB to receive the message
   // backCommand("testok");

   // You can output message on condition inesFunctionIsTraced() success
   // if(inesFunctionIsTraced())
   //    printf(...)
   
   // You can return a string to the CCB that printf SIB specified
   return "ok";
}


// The SCF will call this function in mainloop.
void checkINESEvent()
{
   // In this function you can check Event for other Interface
}

// The SCF will call this function ten times per second
void checkINESEventOnTick()
{
   // In this function you can check Event for other Interface regularly.
}

// If you type 'ines ...' command in inaccess
// SCF will call this function
void cmd_ines(int argc, char* argv[])
{
   // In this function you can analyze the arguments to output 
   // some information to the maintenance interface
   term("this is not a ines program\n");
	//invokeServiceLogic(1, "", "000001");
	//void *p = getINESData("000001", 20);
}

// If a block of data correlated with a FSM
// if you type 'state xxx' command in inaccess
// SCF will call this function to display the information about the data
void printINESData(void* p)
{
   // For example, you define a struct TMyData
   // The TMyData has a member function to display it's content
   // You can can call ((TMyData*)p)->printState() here
   return;
}

// After SCF start up ok, it will call this function
#ifndef WIN32
void initSNMP();
#endif

void inesInitialize(char* name)
{
#ifndef WIN32
   initSNMP();
#endif
   // In this function you can initial something needed
   return;
}

/* for console application, you can impliment a command process function
void myCommandProcHandler(int argc, char* argv[])
{
   // invokeServiceLogicByName can invoke service by name
   // for example, for a 'test' application
   // service in $CINDIR/etc with name test_xxx.bin
   // you can type 'xxx ...' command to invoke the service 
   if(invokeServiceLogicByName(argv[0],argv[argc])>=0)
      return;

   // if no service file here you can define some internal command ...

   // if not match internal command ...
   printf("invalid command '%s'\n",argv[0]);
}
*/

// When SCF starting up, it will call this function
void initialVersionInfo()
{
   // You can construct a version informaiton for your program
   // for example:
   // sprintf(versionInfo,"SCF API DEMO 1.00, Copyright CINtel\n");

   // If you want to develop a consle application, for example: 
   // setConsoleCommandHandler(myCommandProcHandler,"test> ","test");
   // Then scf program can be a console application with prompt test>
}

int scfPreSelect(fd_set *readMask,fd_set *writeMask,fd_set *exceptMask,timeval* timer)
{
   // scf call this function before select
   // if you want to add other fds, write this function
   return 0;
}
void scfPostSelect(fd_set *readMask,fd_set *writeMask,fd_set *exceptMask)
{
   // scf call this function after select
   // if you want to process the fds that selected, write this function
}

#ifdef WIN32
//exit cleanup
void freeINES()
{
}
#endif

