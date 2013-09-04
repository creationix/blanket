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

void buv_dispose(bevent_t* event) {
  if (event) free(event);
}

bevent_t* buv_next() {
  do {
    if (queue.head) return buv_queue_shift();
  } while (uv_run(uv_default_loop(), UV_RUN_ONCE));
  if (queue.head) return buv_queue_shift();
  return NULL;
}

static void on_timeout(uv_timer_t* handle, int status) {
  /* TODO: report error if any too. */
  buv_queue_push(buv_create(BTIMER_TIMEOUT, (uv_handle_t*)handle));
}

int buv_timer_start(uv_timer_t* handle, uint64_t timeout, uint64_t repeat) {
  return uv_timer_start(handle, on_timeout, timeout, repeat);
}

static void on_close(uv_handle_t* handle) {
  buv_queue_push(buv_create(BHANDLE_CLOSE, handle));
}

void buv_close(uv_handle_t* handle) {
  uv_close(handle, on_close);
}
