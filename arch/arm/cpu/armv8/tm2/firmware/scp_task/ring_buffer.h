#ifndef __MESON_RING_BUFFER_H__
#define __MESON_RING_BUFFER_H__

struct ring_buffer {
	unsigned int magic;      // magic number
	unsigned int lock;       // exclusive lock (implement future)
	unsigned int size;       // total size of ring buffer data
	unsigned int head;       // head offset, kernel move it
	unsigned int tail;       // tail offset, M3 move it
	unsigned int len;        // available log data in ring buffer
	char data[4];            // log buffer payload start
};

#define BL301MSG_MAGIC 0xABC123
#define BL301MSG_BUF_BASE 0xFFFDD000

unsigned int write_ring_buffer_char(struct ring_buffer *rb, unsigned char c);
#endif
