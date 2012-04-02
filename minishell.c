#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

#define MAX_SIZE 3000
#define MAX_PROCESSES_WORKERS 10
#define MAX_PARAM 20
#define MAX_OUTPUT 30

int ana_out(char*);
int exec_cmd(int, int, char*);
char* analyse(char*);

int main(int argc, const char *argv[])
{
  char ligne[MAX_SIZE];
  /*char reponse[MAX_OUTPUT][MAX_SIZE];*/
  char reponse[MAX_SIZE];
  int pid, status, premier, dernier, i, c, nbytes;
  int fd[2], df[2], fd_in, fd_out;
  char* cmd;
  status = premier = 1;
  i = dernier = 0;
  gets( ligne );
  printf( "%s\n", ligne );
  cmd = strtok( ligne, "|" );
  do {
    if ( pipe( fd ) == -1 ) {
      printf( "Erreur: %s\n", strerror( errno ) );
      exit( 1 );
    }
    if ( pipe( df ) == -1 ) {
      printf( "Erreur: %s\n", strerror( errno ) );
      exit( 1 );
    }
    fd_in = fd_out = -1;
    printf( "%s\n", cmd );
    switch( pid = fork() ){
      case -1:
        printf("Impossible de forker\n");
        printf( "%s\n", strerror( errno ) );
        exit( 1 );
        break;
      case 0:
        close( fd[0] );
        close( df[1] );
        fd_out = fd[1];
        if ( !premier ) {
          fd_in = df[0];
        }
        else {
          close( df[0] );
        }
        exec_cmd( fd_in, fd_out, cmd );
        break;
      default:
        close( fd[1] );
        close( df[0] );
        if ( !premier ) {
          /*
           *close( 1 );
           *dup( df[1] );
           *close( df[1] );
           */
          write( df[1], reponse, strlen( reponse ) + 1 );
        }
        close( df[1] ),
        close( 0 );
        dup( fd[0] );
        close( fd[0] );
        wait( &status );
        if ( !status ) {
          nbytes = read( 0, reponse, sizeof( reponse ) );
          reponse[nbytes] = 0;
        } 
        printf( "%s\n", reponse );
        premier = 0;
        break;
    }
  } while ( ( cmd = strtok( NULL, "|" ) ) != NULL );
  return 0;
}

int ana_out( char* cmd)
{
  char* fic_out,* buf;
  int fd_out;
  char* token = ">>";
  fic_out = strtok( cmd, token );
  if ( !strcmp( fic_out, cmd ) ) {
    token = ">";
    printf( "egal\n" );
  }
  while( buf = strtok( NULL, token ) ){
    if ( buf == NULL ) {
      break;
    }
    fic_out = buf;
  }
  fic_out = strtok( fic_out, "<" );
  printf( "%s\n",fic_out );
  return fd_out = open( fic_out, O_CREAT|O_WRONLY, 0666 );
}
/*
 *char* analyse( char* cmd)
 *{
 *  char* buf,* chr;
 *  FILE* fd;
 *  char* const specChar = "&><";
 *  buf = strtok( cmd, " " );
 *  if ( chr = strchr( specChar, buf[0] ) ) {
 *    switch( chr - specChar + 1 ){
 *      case 1:
 *        [>code<]
 *        break;
 *        case 2:
 *        if ( buf[1] ) {
 *          close( 1 );
 *          if ( ! ( fd = fopen( buf + 1, "w" ) ) ) {
 *            printf( "%s\n", strerror( errno ) );
 *            exit( 1 );
 *          }
 *          dup( fd[0] );
 *        }
 *          break;
 *      default:
 *        [>nothing to do<]
 *        break;
 *    }
 *  }
 *}
 */
int exec_cmd( int fd_in, int fd_out, char* cmd )
{
  char* optList[MAX_PARAM];
  int j = 0;
  if ( fd_in > -1 ) {
    close( 0 );
    dup( fd_in );
    close( fd_in );
  }
  if ( fd_out > -1 ) {
    close( 1 );
    dup( fd_out );
    close( fd_out );
  }
  *optList = strtok( cmd, " " );
  while( ++j < MAX_PARAM && ( optList[j] = strtok( NULL, " ") ) );
  optList[j] = ( char* ) 0;
  j = 0;
  execvp( *optList, optList );
  printf( "%s\n", strerror( errno ) );
  return -1;
}

