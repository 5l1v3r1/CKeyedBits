#include <keyedbits/buff_encoder.h>
#include <stdio.h>
#include <assert.h>

void test_encode_int();

int main() {
  test_encode_int();
  return 0;
}

void test_encode_int() {
  printf("testing encode int...");
  uint8_t buffer[9];
  
  kb_buff_t buff;
  
  // test encode positive 64-bit
  kb_buff_initialize_encode(&buff, buffer, 9);
  bool result = kb_buff_write_int(&buff, 0x181234567L);
  assert(result);
  assert(buff.off == 9);
  assert(buffer[0] == 0xC6);
  assert(buffer[1] == 0x67);
  assert(buffer[2] == 0x45);
  assert(buffer[3] == 0x23);
  assert(buffer[4] == 0x81);
  assert(buffer[5] == 0x1);
  assert(buffer[6] == buffer[7] == buffer[8] == 0);
  
  // test encode negative 64-bit
  kb_buff_initialize_encode(&buff, buffer, 9);
  result = kb_buff_write_int(&buff, -0x123456789L);
  assert(result);
  assert(buff.off == 9);
  assert(buffer[0] == 0xC6);
  assert(*((uint64_t *)(buffer + 1)) == 0xFFFFFFFEDCBA9877L);
  
  // test encode positive 32-bit
  kb_buff_initialize_encode(&buff, buffer, 5);
  result = kb_buff_write_int(&buff, 0x7FFFFFFF);
  assert(result);
  assert(buff.off == 5);
  assert(buffer[0] == 0xA6);
  assert(buffer[1] == 0xFF);
  assert(buffer[2] == 0xFF);
  assert(buffer[3] == 0xFF);
  assert(buffer[4] == 0x7F);
  
  // test encode negative 32-bit
  kb_buff_initialize_encode(&buff, buffer, 5);
  result = kb_buff_write_int(&buff, -0x1234);
  assert(result);
  assert(buff.off == 5);
  assert(buffer[0] == 0xA6);
  assert(*((uint32_t *)(buffer + 1)) == 0xFFFFEDCCL);
  
  printf(" passed!\n");
}
