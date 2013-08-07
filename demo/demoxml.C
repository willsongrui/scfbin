#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "xmlapi.h"
main(int argc, char* argv[])
{
   if(argc!=2)
   {
      printf("Usage: %s <filename>\n",argv[0]);
      return 1;
   }
   FILE* f=fopen(argv[1],"rb");
   if(f==NULL)
   {
      printf("open file %s error\n",argv[1]);
      return 1;
   }
   printf("xmp api version is: %s\n",xmlGetAPIVersion());
   char buffer[65536];
   int nread=fread(buffer,1,sizeof(buffer),f);
   fclose(f);
   buffer[nread]=0;
   TXMLHandle h=xmlGetTree(buffer);
   xmlPrintTree(h);
   xmlFreeTree(h);
   return 0;
}

void term(const char* s,...)
{
   write(1,s,strlen(s));
}

void SCFERROR(const char* s,...)
{
   printf("%s",s);
}
