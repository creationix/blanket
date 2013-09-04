#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <uv.h>
#include "buv.h"

static struct bevent_queue queue;

static void buv_queue_push(bevent_t* event) {
  if (queue.tail) {
    assert(queue.tail->next == NULL);
    queue.tail->next = event;
  }
  else {
    queue.head = event;
  }
  queue.tail = event;
}

static bevent_t* buv_queue_shift() {
  bevent_t* event;
  if (!queue.head) {
    assert(queue.tail == NULL);
    return NULL;
  }
  event = queue.head;
  if (event->next) {
    queue.head = event->next;
  }
  else {
    assert(queue.tail == event);
    queue.head = NULL;
    queue.tail = NULL;
  }
  return event;
}

static bevent_t* buv_create(bevent_type type, uv_handle_t* handle) {
  bevent_t* event = malloc(sizeof(event));
  event->type = type;
  event->handle = handle;
  return event;
}


// static void read_cb(uv_stream_t* stream, ssize_t nread, uv_buf_t buf) {
//   buv_queue_push(buv_create(BUV_READ, (uv_handle_t*)handle));
// }
// static void read2_cb(uv_pipe_t* pipe, ssize_t nread, uv_buf_t buf, uv_handle_type pending) {
//   buv_queue_push(buv_create(BUV_READ2, (uv_handle_t*)handle));
// }
// static void write_cb(uv_write_t* req, int status) {
//   buv_queue_push(buv_create(BUV_WRITE, (uv_handle_t*)handle));
// }
// static void connect_cb(uv_connect_t* req, int status) {
//   buv_queue_push(buv_create(BUV_CONNECT, (uv_handle_t*)handle));
// }
// static void shutdown_cb(uv_shutdown_t* req, int status) {
//   buv_queue_push(buv_create(BUV_SHUTDOWN, (uv_handle_t*)handle));
// }
// static void connection_cb(uv_stream_t* server, int status) {
//   buv_queue_push(buv_create(BUV_CONNECTION, (uv_handle_t*)handle));
// }
static void close_cb(uv_handle_t* handle) {
  buv_queue_push(buv_create(BUV_CLOSE, handle));
}
// static void poll_cb(uv_poll_t* handle, int status, int events) {
//   buv_queue_push(buv_create(BUV_POLL, (uv_handle_t*)handle));
// }
static void timer_cb(uv_timer_t* handle, int status) {
  buv_queue_push(buv_create(BUV_TIMER, (uv_handle_t*)handle));
}
// static void async_cb(uv_async_t* handle, int status) {
//   buv_queue_push(buv_create(BUV_ASYNC, (uv_handle_t*)handle));
// }
// static void prepare_cb(uv_prepare_t* handle, int status) {
//   buv_queue_push(buv_create(BUV_PREPARE, (uv_handle_t*)handle));
// }
// static void check_cb(uv_check_t* handle, int status) {
//   buv_queue_push(buv_create(BUV_CHECK, (uv_handle_t*)handle));
// }
// static void idle_cb(uv_idle_t* handle, int status) {
//   buv_queue_push(buv_create(BUV_IDLE, (uv_handle_t*)handle));
// }
// static void exit_cb(uv_process_t*, int exit_status, int term_signal) {
//   buv_queue_push(buv_create(BUV_EXIT, (uv_handle_t*)handle));
// }
// static void walk_cb(uv_handle_t* handle, void* arg) {
//   buv_queue_push(buv_create(BUV_WALK, (uv_handle_t*)handle));
// }
// static void fs_cb(uv_fs_t* req) {
//   buv_queue_push(buv_create(BUV_FS, (uv_handle_t*)handle));
// }
// static void work_cb(uv_work_t* req) {
//   buv_queue_push(buv_create(BUV_WORK, (uv_handle_t*)handle));
// }
// static void after_work_cb(uv_work_t* req, int status) {
//   buv_queue_push(buv_create(BUV_AFTER_WORK, (uv_handle_t*)handle));
// }
// static void getaddrinfo_cb(uv_getaddrinfo_t* req, int status, struct addrinfo* res) {
//   buv_queue_push(buv_create(BUV_GETADDRINFO, (uv_handle_t*)handle));
// }

int buv_timer_start(uv_timer_t* handle, uint64_t timeout, uint64_t repeat) {
  return uv_timer_start(handle, timer_cb, timeout, repeat);
}

void buv_close(uv_handle_t* handle) {
  uv_close(handle, close_cb);
}

bevent_t* buv_next() {
  do {
    if (queue.head) return buv_queue_shift();
  } while (uv_run(uv_default_loop(), UV_RUN_ONCE));
  if (queue.head) return buv_queue_shift();
  return NULL;
}

void buv_dispose(bevent_t* event) {
  if (event) free(event);
}


