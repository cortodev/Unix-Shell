#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_SIZE 3000
#define MAW_PROCESSES_WORKERS 10

int isBackgrounded(char*);
char* findCommande(char*, char*);
void findRedirection(char*);

int main(int argc, const char *argv[])
{
  char ligne[MAX_SIZE];
  char* cmd;
  char* i = ligne;
  int pid, status;
  do {
    puts("%>");
    fgets( ligne, MAX_SIZE, stdin );
    if( ! ( pid = fork() ) ){
      int pid_cmd, cmd_en_status;
      printf("Je suis le processus %d et j'execute la ligne : %s\n", getpid(), ligne);
      cmd = strtok(ligne, "|");
      do {
        if ( ! ( pid_cmd = fork() ) ) {
          printf("Je suis le processus %d et j'execute la cmd : %s\n", getpid(), cmd);
          _exit(0);
        }
        waitpid( pid_cmd, &cmd_en_status, WNOHANG );
      } while (cmd = strtok(NULL, "|"));
      _exit(0);
    }
    waitpid( pid, &status, WNOHANG );
    printf("Je suis le pere tout est ok\n");
  } while (1);
return 0;
}

int isBackgrounded(char* ligne){
  char* i = ligne;
  while (i = strchr(i, '&')) {
    if( ! isdigit(*( i + 1 ))){
      return 1;
    }
  }
  return 0;
}

char* findCommande(char* ligne, char* out)
{
  strcpy(out, "");
  while ( *ligne && *ligne != '|' ) {
    if (! isspace(*ligne) ) {
      printf("%c", *ligne);
      strncat(out, ligne, 1);
    }
    ligne++;
  }
  return out;
}
