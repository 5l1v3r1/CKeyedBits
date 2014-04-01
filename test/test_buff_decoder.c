#include <keyedbits/buff_decoder.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

void test_decode_int();
void test_decode_string();
void test_decode_double();
void test_decode_data();

static void _test_double(const char * str, double value);
static void _test_inval_double(const char * str);

int main() {
  test_decode_int();
  test_decode_string();
  test_decode_double();
  return 0;
}

void test_decode_int() {
  printf("testing integer decode...");
  const char * number32 = "\xA6\x37\x13\x37\x13";
  const char * number64 = "\xC6\x37\x13\x37\x90\x78\x56\x34\x12";
  
  kb_buff_t buff;
  kb_header_t header;
  
  // Try 32-bit number
  
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
  
  result = kb_buff_read_header(&buff, &header);
  assert(!result);
  
  // Try 64-bit number
  
  kb_buff_initialize_decode(&buff, (void *)number64, 9);
  result = kb_buff_read_header(&buff, &header);
  assert(result);
  assert(header.typeField == 6);
  assert(header.reserved == 0);
  assert(header.lenLen == 2);
  
  result = kb_buff_read_int(&buff, header.lenLen, &number);
  assert(result);
  assert(number == 0x1234567890371337);
  
  result = kb_buff_read_header(&buff, &header);
  assert(!result);
  
  printf(" passed\n");
}

void test_decode_string() {
  printf("testing string decode...");
  char * buffer = "\x81" "alex rocks!";
  kb_buff_t buff;
  kb_header_t header;
  
  kb_buff_initialize_decode(&buff, (void *)buffer, strlen(buffer) + 1);
  bool result = kb_buff_read_header(&buff, &header);
  assert(result);
  assert(header.typeField == 1);
  assert(header.nullTerm);
  
  const char * out = NULL;
  uint64_t len;
  result = kb_buff_read_string(&buff, &out, &len);
  assert(result);
  assert(len == 11);
  assert(!strcmp(out, "alex rocks!"));
  
  result = kb_buff_read_header(&buff, &header);
  assert(!result);
  
  printf(" passed!\n");
}

void test_decode_double() {
  _test_double("3", 3);
  _test_double("3.14159265", 3.14159265);
  _test_double("0.31415e1", 3.1415);
  _test_double("0.31415e+1", 3.1415);
  _test_double("31.415e-1", 3.1415);
  _test_double("-2.1e+10", -21000000000.0);
  _test_double("-3e-2", -0.03);
  _test_inval_double("-");
  _test_inval_double("-3.");
  _test_inval_double("3.");
  _test_inval_double("3.22e");
  _test_inval_double("3.2e-+1");
  _test_inval_double("3.2e1-1");
  _test_inval_double("e1");
}

void test_decode_data() {
  const char * buff1 = "\x05\x03" "hey";
  const char * buff2 = "\x25\x03\x00" "hey";
  const char * buff3 = "\x45\x03\x00\x00" "hey";
  const char * buff4 = "\x65\x03\x00\x00\x00" "hey";

  // TODO: here, try to decode all of those buffers as "hey"
}

static void _test_double(const char * str, double value) {
  printf("testing double \"%s\"...", str);
  char ptr[64];
  sprintf(ptr, "\x87%s", str);
  
  kb_buff_t buff;
  kb_header_t header;
  
  kb_buff_initialize_decode(&buff, (void *)ptr, strlen(str) + 2);
  bool result = kb_buff_read_header(&buff, &header);
  assert(result);
  assert(header.typeField == 7);
  assert(header.nullTerm);
  
  double d;
  result = kb_buff_read_double(&buff, &d);
  assert(result);
  assert(fabs(d - value) < fabs(value / 100000000.0));
  
  printf(" passed!\n");
}

static void _test_inval_double(const char * str) {
  printf("testing (invalid) double %s...", str);
  char ptr[64];
  sprintf(ptr, "\x87%s", str);
  kb_buff_t buff;
  kb_header_t header;
  kb_buff_initialize_decode(&buff, (void *)ptr, strlen(str) + 2);
  bool result = kb_buff_read_header(&buff, &header);
  assert(result);
  assert(header.typeField == 7);
  assert(header.nullTerm);
  
  double d;
  result = kb_buff_read_double(&buff, &d);
  assert(!result);
  printf(" passed!\n");
}

