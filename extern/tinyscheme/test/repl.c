//
// TinyScheme REPL
// 

#include "scheme.h"
#include "scheme-private.h"
#include "../source/scheme.c"

#ifndef InitFile
# define InitFile "libs/start-tiny.scm"
# define UpperInitFile "../libs/start-tiny.scm"
#endif

#define JOIN2(a,b) a##b

#define banner "TinyScheme 2.3.0"

#if WIN32
#include <io.h>
#endif


#if defined(__APPLE__) && !defined (OSX)
int main()
{
     extern MacTS_main(int argc, char **argv);
     char**    argv;
     int argc = ccommand(&argv);
     MacTS_main(argc,argv);
     return 0;
}
int MacTS_main(int argc, char **argv) {
#else
int main(int argc, char **argv) {
#endif
	scheme sc = { 0 };
  FILE *fin;
  char *file_name=InitFile;
  int retcode;
  int isfile=1;

  if(argc==1) {
    printf(banner);
  }
  if(argc==2 && strcmp(argv[1],"-?")==0) {
    printf("Usage: tinyscheme -?\n");
    printf("or:    tinyscheme [<file1> <file2> ...]\n");
    printf("followed by\n");
    printf("          -1 <file> [<arg1> <arg2> ...]\n");
    printf("          -c <Scheme commands> [<arg1> <arg2> ...]\n");
    printf("assuming that the executable is named tinyscheme.\n");
    printf("Use - as filename for stdin.\n");
    return 1;
  }
  if(!scheme_init(&sc)) {
    fprintf(stderr,"Could not initialize!\n");
    return 2;
  }
  scheme_set_input_port_file(&sc, stdin);
  scheme_set_output_port_file(&sc, stdout);
#if USE_DL
  scheme_define(&sc,sc.global_env,mk_symbol(&sc,"load-extension"),mk_foreign_func(&sc, scm_load_ext));
#endif
  argv++;
#ifdef WIN32
  if(_access(file_name,0)!=0) {
#else
  if(access(file_name,0)!=0) {
#endif
    char *p=getenv("TINYSCHEMEINIT");
    if(p!=0) {
      file_name=p;
    }
  }
  do {
    if(strcmp(file_name,"-")==0) {
      fin=stdin;
    } else if(strcmp(file_name,"-1")==0 || strcmp(file_name,"-c")==0) {
      pointer args=sc.NIL;
      isfile=file_name[1]=='1';
      file_name=*argv++;
      if(strcmp(file_name,"-")==0) {
        fin=stdin;
      } else if(isfile) {
        fin=fopen(file_name,"r");
      }
      for(;*argv;argv++) {
        pointer value=mk_string(&sc,*argv);
        args=cons(&sc,value,args);
      }
      args=reverse_in_place(&sc,sc.NIL,args);
      scheme_define(&sc,sc.global_env,mk_symbol(&sc,"*args*"),args);

    } else {
      start_load:
      fin=fopen(file_name,"r");
    }
    if(isfile && fin==0) {
      if (file_name == InitFile) {
        file_name = UpperInitFile;
        goto start_load;
      }
      fprintf(stderr,"Could not open file %s\n",file_name);
    } else {
      if(isfile) {
        scheme_load_named_file(&sc,fin,file_name);
      } else {
        scheme_load_string(&sc,file_name);
      }
      if(!isfile || fin!=stdin) {
        if(sc.retcode!=0) {
          fprintf(stderr,"Errors encountered reading %s\n",file_name);
        }
        if(isfile) {
          fclose(fin);
        }
      }
    }
    file_name=*argv++;
  } while(file_name!=0);
  if(argc==1) {
    scheme_load_named_file(&sc,stdin,0);
  }
  retcode=sc.retcode;
  scheme_deinit(&sc);

  return retcode;
}