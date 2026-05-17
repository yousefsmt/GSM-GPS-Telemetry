#ifndef COMMON_RING_BUFFER_H_
#define COMMON_RING_BUFFER_H_

#include "mcu.h"

// Simple ring buffer.
typedef struct {
	int len;
	volatile char* buf;
	volatile int  pos;
	volatile int  ext;
} RingBuffer_t;

#define ringbuf_write( rb, x ) \
  rb.buf[ rb.ext ] = x; \
  if ( ( rb.ext + 1 ) >= rb.len ) { rb.ext = 0; } \
  else { rb.ext = rb.ext + 1; }


static inline char ringbuf_read( RingBuffer_t* buf )
{
	if ( buf->pos == buf->ext )
	{
		return '\0';
	}

	char read = buf->buf[ buf->pos ];

	buf->pos = ( buf->pos < ( buf->len - 1 ) ) ? ( buf->pos + 1 ) : 0;

	return read;
}

#endif /* COMMON_RING_BUFFER_H_ */