#include <stdio.h>
#include <stdlib.h>
#include <uv.h>
#include <buv.h>

static void start_timer(const char* data, uint64_t timeout, uint64_t repeat) {
  uv_timer_t* timer = malloc(sizeof(*timer));
  uv_timer_init(uv_default_loop(), timer);
  timer->data = (void*)data;
  buv_timer_start(timer, timeout, repeat);
}

int main() {
  printf("START\n");

  start_timer("one", 1000, 0);
  start_timer("two", 2000, 0);

  bevent_t* event;
  while ((event = buv_next())) {
    const char* data = (const char*)event->handle->data;
    switch (event->type) {
      case BUV_TIMER:
        printf("%s.ontimeout\n", data);
        uv_timer_stop((uv_timer_t*)event->handle);
        buv_close(event->handle);
        break;
      case BUV_CLOSE:
        printf("%s.onclose\n", data);
        free(event->handle);
        break;
      default: break;
    }
    buv_dispose(event);
  }

  printf("END\n");
  return 0;
}
