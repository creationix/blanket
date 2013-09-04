typedef enum {
  BUV_READ = 1,
  BUV_READ2,
  BUV_WRITE,
  BUV_CONNECT,
  BUV_SHUTDOWN,
  BUV_CONNECTION,
  BUV_CLOSE,
  BUV_POLL,
  BUV_TIMER,
  BUV_ASYNC,
  BUV_PREPARE,
  BUV_CHECK,
  BUV_IDLE,
  BUV_EXIT,
  BUV_WALK,
  BUV_FS,
  BUV_WORK,
  BUV_AFTER_WORK,
  BUV_GETADDRINFO
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

