#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <ctype.h>
#include <stddef.h>

#define MAX_SIZE 300
#define MAX_PROCESSES_WORKERS 10
#define MAX_PARAM 20
#define MAX_OUTPUT 30
#define MAX_FILE_SIZE 300

char** analyse(char*);
int exec_cmd(int, int, char*);

int main(int argc, const char *argv[])
{
  char ligne[MAX_SIZE], reponse[MAX_SIZE];
  int pid, status, premier, dernier, i, c, nbytes;
  int fd[2], df[2], fd_in, fd_out, mode;
  char* ligne_cpy,* cmd,* cmd_cmplx;
  char** arr;



  puts( "******************Bienvenu dans le dauphine shell************************" );
  //while ( 1 ) {
    printf( "%>" );
    gets( ligne );
    printf( "%s\n",ligne );
    ligne_cpy = strdup( ligne );
    while ( cmd_cmplx = strsep( &ligne_cpy, ";" ) ){
      status = premier = 1;
      arr = ( char** ) malloc( 4 * sizeof( char * ) );
      arr[2] = ( char* ) malloc( MAX_SIZE * sizeof( char ) );
      arr[1] = ( char* ) malloc( MAX_SIZE * sizeof( char ) );
      arr[0] = ( char* ) malloc( MAX_SIZE * sizeof( char ) );
      arr[3] = ( char * ) malloc( 5 * sizeof( char ) );
      //reponse = ( char* ) malloc( MAX_SIZE * sizeof( char ) );
      while ( cmd = strsep( &cmd_cmplx, "|" ) ){
        dernier = ( cmd_cmplx == NULL );
        if ( pipe( fd ) == -1 ) {
          printf( "Erreur: %s\n", strerror( errno ) );
          exit( 1 );
        }
        if ( pipe( df ) == -1 ) {
          printf( "Erreur: %s\n", strerror( errno ) );
          exit( 1 );
        }
        fd_in = fd_out = -1;
        switch( pid = fork() ){
          case -1:
            printf("Impossible de forker\n");
            printf( "%s\n", strerror( errno ) );
            exit( 1 );
            break;
          case 0:
            close( fd[0] );
            close( df[1] );

            arr = analyse( cmd );
            /*
             *for (i = 0; i < 3; i++) {
             *  printf( "%s\n", arr[i] );
             *}
             */
            mode = O_WRONLY | O_CREAT;
            mode |= arr[3] ? O_APPEND : O_TRUNC;
            fd_out = open( arr[2], mode, 0666 );
            printf( "%d\n", fd_out );
            mode = O_RDONLY;
            fd_in = open( arr[1], mode, 0666 );
            if ( fd_out == -1 ) {
              fd_out = fd[1];
              if ( dernier ) {
                fd_out = dup( 1 );
              }
            }
            if ( !premier ) {
              if ( fd_in == -1 ) {
                fd_in = df[0];
              }
            }
            else {
              close( df[0] );
            }
            exec_cmd( fd_in, fd_out, arr[0] );
            break;
          default:
            close( fd[1] );
            close( df[0] );
            /*
             *if ( !premier ) {
             *  close( 1 );
             *  dup( df[1] );
             *  close( df[1] );
             *  printf( "all\n" );
             *  write( 1, reponse, strlen( reponse ) + 1 );
             *  printf( "laa\n" );
             *}
             */
            close( df[1] );
            close( 0 );
            dup( fd[0] );
            close( fd[0] );
            wait( &status );
            if ( !status ) {
              nbytes = read( 0, reponse, MAX_SIZE );
              reponse[nbytes] = 0;
              /*
               *if ( dernier ) {
               *  printf( "%s\n", reponse );
               *}
               */
            }
            premier = 0;
            break;
        }
      }
      for (i = 0; i < 3; i++) {
        free( arr[i] );
      }
      free( arr );
    }
    free( ligne_cpy );
  //}

  return 0;
}

char** analyse( char* cmd )
{
  char* i,* j,* k;
  char** arr;
  int append, copyHasStarted, mode;

  arr = ( char** ) malloc( 4 * sizeof( char * ) );
  i = arr[2] = ( char* ) malloc( MAX_SIZE * sizeof( char ) );
  j = arr[1] = ( char* ) malloc( MAX_SIZE * sizeof( char ) );
  k = arr[0] = ( char* ) malloc( MAX_SIZE * sizeof( char ) );
  arr[3] = ( char * ) malloc( 5 * sizeof( char ) );
  append = copyHasStarted = 0;

  while ( *cmd ){
    if ( *cmd == '>' ) {
      if( cmd[1] == '>' ){
        append = 1;
      }
      while ( append[++cmd] && ( cmd[append] != '<') ) {
        if ( !isspace( cmd[append] ) ) {
          *(i++) = cmd[append];
          copyHasStarted = 1;
        }
        else if ( copyHasStarted ) {
          copyHasStarted = 0;
          break;
        }
      }
    }
    else if ( *cmd == '<' ) {
      while ( *(++cmd) && *cmd != '>' ) {
        if ( !isspace( *cmd ) ) {
          *(j++) = cmd[append];
          copyHasStarted = 1;
        }
        else if ( copyHasStarted ) {
          copyHasStarted = 0;
          break;
        }
      }
    }
    else if ( *cmd == '&' ) {
      /* code */
    }
    else{
      *(k++) = *cmd;
    }
    cmd++;
  }

  if ( append ) {
    arr[3] = "TRUE";
  }

  return arr;
}
int ana_out( char* cmd)
{
  char* fic_out,* buf;
  int fd_out;
  char* token = ">>";
  fic_out = strtok( cmd, token );
  if ( !strcmp( fic_out, cmd ) ) {
    token = ">";
  }
  while( buf = strtok( NULL, token ) ){
    if ( buf == NULL ) {
      break;
    }
    fic_out = buf;
  }
  fic_out = strtok( fic_out, "<" );
  return fd_out = open( fic_out, O_CREAT|O_WRONLY, 0666 );
}

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

