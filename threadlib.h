struct lock_t {
  uint locked;
  struct lock_t * next;
};

struct array_lock
{
        uint *has_lock;
        uint last;
        uint myplace;
};

struct mcs_lock
{
    int locked;
    struct lock_t * next;
};

