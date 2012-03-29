#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define DEFAULT_REDIRECTION "tmp"
#define STDIN 0
#define STDOUT 1
#define STDERR 2

char* trim(char* str){
  char* end;

  while(isspace(*str))str++;

  if(*str == 0)
    return str;

  end = str + strlen(str) - 1;
  while(end > str && isspace(*end))end--;

  *(end+1) = 0;

  return str;
}

int amIBackgrounded(char* cmd)
{
  char* tmp = trim(cmd);
  if(*( tmp + strlen(tmp) - 1)== '&'){
    *( tmp + strlen(tmp) - 1 ) = 0;
    return 1;
  }
  return 0;
}

char* stripSpaces(char* str)
{
  char* out = malloc(strlen(str)*sizeof(char));
  do {
    if (! isspace(*str)) {
      printf("%c\n", *str);
      *( out++ ) =  *str;
    }
  } while (*(str++));
  return out;
}

/*char* goodChain(char* cmd)*/
/*{*/
  /*char* end;*/
  /*end = cmd + strlen(cmd) - 1;*/
  /*if(amIBackgrounded(cmd))*/
    /**end = 0;*/
  /*return trim(cmd);*/
/*}*/

char** haveAGoodRedirection(char* str)
{
  int i = 0, descr;
  char* res = NULL;
  char** redirect = malloc( 3 * sizeof(char*) );
  while (*str) {
    /* code */
  }
  /*
   *res = strtok(str, "<>");
   *do {
   *  *( redirect + i ) = (char*) malloc(strlen(res) * sizeof(char));
   *  strcpy(redirect[i], trim( res ));
   *  i++;
   *} while (res = strtok(NULL, "<>"));
   *return redirect;
   */
}

int main(int argc, const char *argv[])
{
  char test[] = " ls > ps< pwd & ";
  char* res = NULL;
  int runInBackgroud = amIBackgrounded(test), i = 0;
  res = stripSpaces(test);
  printf("%s\n", res);
  res = strtok(trim( test ), "|");
  do {
    haveAGoodRedirection(res);
    /*char** redirection = haveAGoodRedirection(res);*/
    /*while (*( redirection + i ) != NULL) {*/
      /*printf("%d\n", i);*/
      /*printf("%s\n", *( redirection + i++ ));*/
    /*}*/
    printf("%s\n", res);
  } while (res=strtok(NULL, "|"));
  if (runInBackgroud) {
    printf("%s\n", "Ich war in background");
  }
  return 0;
}
