#include <stdio.h>
#include "task_apis.h"
#include "ring_buffer.h"

unsigned int write_ring_buffer_char(struct ring_buffer *rb, unsigned char c)
{
	if (!rb || rb->magic != BL301MSG_MAGIC)
		return 0;

	rb->data[rb->tail++] = c;
	rb->tail %= rb->size;
	rb->len++;

	/* New data will overwrite the old data */
	if (rb->len > rb->size) {
		rb->head = rb->tail;
		rb->len = rb->size;
	}
	return 1;
}
