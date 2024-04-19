// Following is the simple (annotated) example discussed in class 1/13/2014

/*
  To compile type:  (Note '$' is the screen prompt -- don't type this in)

  $ gcc -Wall hello.c -o hello

  To run type:

  $ ./hello bob carol tom

  You should see:

  Hello World
  myargs[0] = (0, ./a.out)
  myargs[1] = (1, bob)
  myargs[2] = (2, carol)
*/

#include <stdio.h>  // include definitions and function prototypes for I/O

/*
   We dont' have objects in 'C', but we do have structs to create
   aggregate data types.  Below I'm creating essentially a record
   consisting of two fields.  This record (struct in 'C' terminology)
   will consist of the position (an int) of a command-line argument
   and a pointer to the first character of an array (contiguous chunk)
   of characters that constitutes the string argument at position,
   pos.  In 'C' strings are arrays (contiguous chunks of member) of
   characters terminated by a null character (denoted '\0').  We don't
   have string objects in 'C'.  The length of the string is not stored
   anywhere.  If you want to know the length of the string, you can
   call the strlen() function.  But, in order to do this, you must
   include <string.h>
*/

struct mystruct {
  int   pos;   // position of the command line argument
  char *str;   // command line string at position, pos
};
  
int main(int argc, char** argv)  // argv is an array of strings
{
  int i;  // declarations go at the beginning of the function
  struct mystruct myargs[10];  // array of 10 mystruct structures
  
  printf("Hello World\n");

  for (i = 0; i < argc; i++) {
    //printf("argv[%d] = %s\n", i, argv[i]);
    myargs[i].pos = i;
    myargs[i].str = argv[i];
  }

  for (i = 0; i < argc; i++) {
    printf("myargs[%d] = (%d, %s)\n", i, myargs[i].pos, myargs[i].str);    
  }
  
  return 0;   // functions returns an int; for main, 0 return means 'OK'
}
