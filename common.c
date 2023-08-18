#include "common.h"

char* get_file_content(char* path, size_t* flen)
{
   FILE *fp;
   //size_t size = 0;
   char* file_content;

   /* Read File to get size */
   fp = fopen(path, "rb");
   if(fp == NULL) {
       return "";
   }

   fseek(fp, 0L, SEEK_END);
   *flen = ftell(fp)+1;
   fclose(fp);

   /* Read File for Content */
   fp = fopen(path, "r");
   file_content = memset(malloc(*flen), '\0', *flen);
   int state = fread(file_content, 1, *flen-1, fp);

   if(!state)
   {
       printf("get_file_content: unable to read from provided file\n");
       return NULL;
   }

   fclose(fp);

   return file_content;
}
