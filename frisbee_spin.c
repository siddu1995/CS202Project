#include "types.h"
#include "stat.h"
#include "user.h"
#include "threadlib.h"

uint thread_count;
uint pass_count;

struct lock_t *lock;

struct
{
      int *token;
      int *pass;
      int *new_token;
} spinlock;

void *pass_frisbee(void *args){

        for(;;)
        {
                lock_acquire(lock);
                if(*spinlock.pass<pass_count)
                {
                        (*spinlock.token)++;
                        (*spinlock.pass)++;
                         printf(1, "Pass no:%d Thread %d pass to Thread %d\n",*spinlock.pass,*spinlock.new_token,*spinlock.token);
                }
                lock_release(lock);
                if(*spinlock.new_token==thread_count-1)
                        *spinlock.new_token=*spinlock.token;
                else (*spinlock.new_token)++;
if(*spinlock.token==thread_count-1)
                        *spinlock.token=-1;
                if(*spinlock.pass>=pass_count)
                        break;
        }
        return 0;
}

int
main(int argc, char *argv[])
{
      int x=uptime();
      int i;

      if(argc != 3)
      {
        printf(1, "Wrong number of arguments. Exiting...\n");
        exit();
      }

      thread_count = atoi(argv[1]);
      pass_count = atoi(argv[2]);
      lock_init(lock);

      int token=0;
      int pass=0;
      int new_token=0;
      spinlock.pass=&pass;
      spinlock.token=&token;
      spinlock.new_token=&new_token;
      void *args=(void *)&spinlock;

      for (i = 0; i < thread_count; ++i)
      {
          thread_create(pass_frisbee, args);
      }


     for (i = 0; i < thread_count; ++i)
     {
        thread_join();
     }

     printf(1, "Done with everything :)\n");
     int y=uptime();
     printf(1, "time taken: %d\n",y-x);
     exit();
 }

