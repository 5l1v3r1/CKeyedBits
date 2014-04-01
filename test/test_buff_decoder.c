#include <keyedbits/buff_decoder.h>
#include <assert.h>
#include <stdio.h>

void test_decode_int();

int main() {
  test_decode_int();
  return 0;
}

void test_decode_int() {
  printf("testing integer decode...");
  const char * number32 = "\xA6\x37\x13\x37\x13";
  const char * number64 = "\xC6\x37\x13\x37\x90\x78\x56\x34\x12";
  
  kb_buff_t buff;
  kb_header_t header;
  
  // try 32-bit number
  kb_buff_initialize_decode(&buff, (void *)number32, 5);
  bool result = kb_buff_read_header(&buff, &header);
  assert(result);
  assert(header.typeField == 6);
  assert(header.reserved == 0);
  assert(header.lenLen == 1);
  
  int64_t number;
  result = kb_buff_read_int(&buff, header.lenLen, &number);
  assert(result);
  assert(number == 0x13371337);
  
  // try 64-bit number
  kb_buff_initialize_decode(&buff, (void *)number64, 9);
  result = kb_buff_read_header(&buff, &header);
  assert(result);
  assert(header.typeField == 6);
  assert(header.reserved == 0);
  assert(header.lenLen == 2);
  
  result = kb_buff_read_int(&buff, header.lenLen, &number);
  assert(result);
  assert(number == 0x1234567890371337);
  printf(" passed\n");
}
