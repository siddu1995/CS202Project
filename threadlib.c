#include "types.h"
#include "stat.h"
#include "fcntl.h"
#include "user.h"
#include "x86.h"
#include "threadlib.h"

struct lock_t lock;
struct lock_t *l=0;

void thread_create(void *(*start_routine)(void*), void *arg)
{
  void *nSp = malloc(4096);
  int rc;
  rc = clone(nSp, 4096);

  if(rc == 0)
  {
    (*start_routine)(arg);
    exit();
  }
}

void thread_join()
{
  wait();
}

void lock_init(struct lock_t *lk)
{
    lk->locked = 0;
}

void lock_acquire(struct lock_t *lk)
{
  while(xchg(&lk->locked, 1) != 0)
      ;
}

void lock_release(struct lock_t *lk)
{
  xchg(&lk->locked, 0);
}


//arraylock
uint fetch_and_inc(struct array_lock *lk)
{

        return xchg(&lk->last,lk->last+1);

}

void array_lock_init(struct array_lock *lk,int size)
{
        int i;
        lk->has_lock = (uint*)malloc(size*sizeof(uint));
        for(i=0;i<size;i++)
        {
                lk->has_lock[i]=0;
        }
        lk->last = 0;
        lk->has_lock[0]=1;
        lk->myplace=0;
}

int array_lock_acquire(struct array_lock *lk, int size)
{
        int myplace = fetch_and_inc(lk);

        while(lk->has_lock[myplace%size]==0){printf(1,"");};
        return myplace;
}

void array_lock_release(struct array_lock *lk,int size,int myplace)
{
        lk->has_lock[myplace%size]=0;
        lk->has_lock[(myplace+1)%size]=1;
}

//MCS

struct lock_t* fetch_and_store(struct lock_t **i) {
    struct lock_t *temp = l;
    l = *i;
    *i = temp;
    return temp;
}

int compare_and_swap(struct lock_t **i) {
    if(l!= (*i)) return 0;
    l = 0;
    return 1;
}

void mcs_lock_acquire(struct lock_t *i)
{
    i->next = 0;
    struct lock_t *pred = fetch_and_store(&i);
    if(pred != 0) {
        i->locked = 1;
        pred->next = i;
        while(i->locked == 1);
    }
}

void mcs_lock_release(struct lock_t *i)
{
    if(i->next == 0) {
        if(compare_and_swap(&i) == 1) return;
        while(i->next == 0);
    }
    i->next->locked = 0;
}

