#include <stdint.h>
#include <stdbool.h>

typedef struct {
  unsigned typeField : 3;
  unsigned reserved : 2;
  unsigned lenLen : 2;
  unsigned nullTerm : 1;
} __attribute__((packed)) kb_header_t;

/**
 * For encoding/decoding in a fixed-size buffer.
 */
typedef struct {
  const void * buff;
  uint64_t len;
  uint64_t off;
} kb_buff_t;
