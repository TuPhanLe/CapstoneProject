#ifndef RINGBUFF_H
#define RINGBUFF_H
#include <stdint.h>

typedef struct {
    uint8_t *  buffer;
    int head;
    int tail;
    int maxlen;
} ringbuffer_t;
int16_t ringbuffer_push(ringbuffer_t *c, uint8_t data);
int16_t ringbuffer_pop(ringbuffer_t *c, uint8_t *data);
void ringbuffer_init(ringbuffer_t *ring, uint8_t *buf, int len);
uint8_t Is_ringbuf_available(ringbuffer_t *ring);

#endif

