#include "ringbuff.h"

int16_t ringbuffer_push(ringbuffer_t *c, uint8_t data)
{
    uint16_t next;

    next = c->head + 1;  // next is where head will point to after this write.
    if (next >= c->maxlen)
        next = 0;

    if (next == c->tail)  // if the head + 1 == tail, circular buffer is full
        return -1;

    c->buffer[c->head] = data;  // Load data and then move
    c->head = next;             // head to next data offset.
    return 0;  // return success to indicate successful push.
}
int16_t ringbuffer_pop(ringbuffer_t *c, uint8_t *data)
{
    uint16_t next;

    if (c->head == c->tail)  // if the head == tail, we don't have any data
        return -1;

    next = c->tail + 1;  // next is where tail will point to after this read.
    if(next >= c->maxlen)
        next = 0;

    *data = c->buffer[c->tail];  // Read data and then move
    c->tail = next;              // tail to next offset.
    return 0;  // return success to indicate successful push.
}
uint8_t Is_ringbuf_available(ringbuffer_t *ring)
{
	uint8_t head =  ring->head;
	uint8_t tail =  ring->tail;
	uint8_t maxlen =  ring->maxlen;	
	if(tail - head>0)
	{
		return maxlen - (tail + head);
	}
	return head - tail;
}
void ringbuffer_init(ringbuffer_t *ring, uint8_t *buf, int len)
{
	ring->buffer = buf;
	ring->head = 0;
	ring->tail = 0;
	ring->maxlen = len;
}

