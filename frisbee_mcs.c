#include "types.h"
#include "stat.h"
#include "user.h"
#include "threadlib.h"

typedef struct frisbee
{
      int *token;
      int *location;
      int *pass_count;
} frisbee;

void *pass_frisbee(void *args){
      frisbee *f = (frisbee*) args;
      struct lock_t lock;
      lock.locked=0;

      int i;
      for (i = 0; i < *f->pass_count; ++i)
      {
         lock_acquire(&lock);
         (*f->token)++;
         *f->location = getpid();
          printf(1, "Token is %d and frisbee is currently with thread %d\n",*f->token,*f->location);
          lock_release(&lock);
    }

    return 0;
}

int
main(int argc, char *argv[])
{
 int thread_count = 0;
      int pass_count = 0;
      int i;

      if(argc != 3)
      {
        printf(1, "Wrong number of arguments. Exiting...\n");
        exit();
      }

      thread_count = atoi(argv[1]);
      pass_count = atoi(argv[2]);


      int count = 0;
      int location = 0;

      frisbee f;
      f.token = &count;
      f.location = &location;
      f.pass_count = &pass_count;

      void *args = (void *) &f;

      for (i = 0; i < thread_count; ++i)
      {
          thread_create(pass_frisbee, args);
      }
     printf(1, "Done with everything :)\n");
     exit(); 
}

