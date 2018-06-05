#include "types.h"
#include "stat.h"
#include "user.h"
#include "threadlib.h"
uint thread_count;
uint pass_count;
struct array_lock *lock;
struct lock_t *lk;
struct
{
      int *token;
      int *pass;
      int *new_token;
} spinlock;

void *pass_frisbee(void *args){

        for(;;)
        {
                int k = array_lock_acquire(lock,thread_count);
                if(*spinlock.pass<pass_count)
                {
                        (*spinlock.token)++;
                        (*spinlock.pass)++;
                        printf(1, "Pass number no:%d, Thread %d passes token to Thread %d\n",*spinlock.pass,k%thread_count,(k+1)%thread_count);
                }
                array_lock_release(lock,thread_count,k);
                if(*spinlock.pass>=pass_count){
                        break;
                }
        }
        return 0;
}

int
main(int argc, char *argv[])
{
        int i;
        int x = uptime();
        if(argc != 3)
        {
                printf(1, "Wrong number of arguments. Exiting...\n");
                exit();
        }
        thread_count = atoi(argv[1]);
        pass_count = atoi(argv[2]);
        array_lock_init(lock,thread_count);

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
        int y = uptime();
        printf(1,"Total time is %d\n",y-x);
        printf(1, "Done with everything\n");
        exit();
}


