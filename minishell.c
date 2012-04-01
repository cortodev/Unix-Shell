#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>

#define MAX_SIZE 3000
#define MAX_PROCESSES_WORKERS 10
#define MAX_PARAM 20
#define MAX_OUTPUT 3000

void exec_cmd(char*);

int main(int argc, const char *argv[])
{
  char ligne[MAX_SIZE];
  char ligne_reponse[MAX_SIZE];
  int pid, status;
  int fd[2];
  char* cmd;
  gets( ligne );
  cmd = strtok( ligne, "|" );
  do {
    pipe( fd );
    switch( pid = fork() ){
      case -1:
        printf("Impossible de forker\n");
        printf( "%s\n", strerror( errno ) );
        exit( 1 );
        break;
      case 0:
        close( 1 );
        dup( fd[1] );
        close( fd[0] );
        exec_cmd( cmd );
        break;
      default:
        close( 0 );
        dup( fd[0] );
        close( fd[1] );
        while ( gets( ligne_reponse ) ) {
          printf( "Le fils repond: %s\n", ligne_reponse );
        }
        wait( &status );
        printf("Et je termine\n");
        break;
    }
  } while ( cmd = strtok( NULL, "|" ) );
  return 0;
}

void exec_cmd( char* cmd )
{
  char* optList[MAX_PARAM], buf[80];
  int j = 0;
  strcpy( buf, "/bin/" );
  strcat( buf, strtok( cmd, " " ) );
  *optList = buf;
  printf("J'execute\n");
  while( ++j < MAX_PARAM && ( optList[j] = strtok( NULL, " ") ) );
  optList[j] = ( char* ) 0;
  j = 0;
  execv( *optList, optList );
  printf( "%s\n", strerror( errno ) );
  exit( 1 );
}
