typedef enum {
  BHANDLE_CLOSE = 1,
  BTIMER_TIMEOUT
} bevent_type;

struct bevent_s {
  bevent_type type;
  uv_handle_t* handle;
  struct bevent_s* next;
};

typedef struct bevent_s bevent_t;

struct bevent_queue {
  bevent_t* head;
  bevent_t* tail;
};

// Call this as your main loop.  Blocks till there is an event. Returns the
// event or NULL if the loop is done.
bevent_t* buv_next();

// Callback-less wrapper around uv_timer_start
int buv_timer_start(uv_timer_t* handle, uint64_t timeout, uint64_t repeat);

// Callback-less wrapper around uv_close
void buv_close(uv_handle_t* handle);

void buv_dispose(bevent_t* event);

