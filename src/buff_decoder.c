#include <keyedbits/buff_decoder.h>

static bool _parse_int(const char * buff, int * number);

void kb_buff_initialize_decode(kb_buff_t * kb, uint8_t * buff, uint64_t len) {
  kb->buff = buff;
  kb->len = len;
  kb->off = 0;
}

bool kb_buff_read_header(kb_buff_t * kb, kb_header_t * header) {
  if (kb->off == kb->len) return false;
  (*header) = *((const kb_header_t *)(kb->buff + kb->off));
  kb->off++;
  return true;
}

bool kb_buff_read_string(kb_buff_t * kb, char * out, uint64_t max) {
  uint64_t i;
  for (i = 0; i < max && kb->off < kb->len; i++) {
    const char ch = *((const char *)(kb->buff + kb->off));
    kb->off++;
    out[i] = ch;
    if (!ch) return true;
  }
  return false;
}

bool kb_buff_read_double(kb_buff_t * kb, double * out) {
  char string[64];
  if (!kb_buff_read_string(kb, string, 64)) return false;
  
  double value = 0;
  bool negative = false;
  bool decimal = false;
  double decScale = 1;
  uint64_t i;
  
  // parse the string before the possible exponent
  for (i = 0; i < 64; i++) {
    char ch = string[i];
    if (ch == 'e' || !ch) break;
    
    if (ch == '.' && !decimal) {
      decimal = true;
    } else if (ch >= '0' && ch <= '9') {
      double number = (double)(ch - '0');
      if (decimal) {
        value += decScale * number;
        decScale /= 10.0;
      } else {
        value *= 10.0;
        value += number;
      }
    } else if (ch == '-' && i == 0) {
      negative = true;
    } else {
      return false;
    }
  }
  
  // check if the string ended properly
  if (string[i] == '.' || string[i] == '-') return false;
  if (string[i] != 'e') return true;
  
  // read the exponent
  int exponent = 0;
  if (!_parse_int(&string[i + 1], &exponent)) return false;
  
  double multiple = 1.0;
  double product = 10.0;
  bool negExp = exponent < 0;
  if (negExp) exponent *= -1;
  
  while (exponent) {
    if (exponent & 1) multiple *= product;
    product *= 10.0;
    exponent >>= 1;
  }
  
  if (negExp) multiple = 1.0 / multiple;
  
  return value * (negative ? -1.0 : 1.0) * multiple;
}

bool kb_buff_read_int(kb_buff_t * kb, uint8_t lenLen, uint64_t * out) {
  
}

bool kb_buff_read_data(kb_buff_t * kb,
                       uint8_t lenLen,
                       uint8_t * out,
                       uint64_t max) {
                         
}

bool kb_buff_read_key(kb_buff_t * kb, char * out, uint64_t max) {
  
}

static bool _parse_int(const char * buff, int * number) {
  int value = 0;
  bool negative = false;
  uint64_t i;
  for (i = 0; buff[i]; i++) {
    if (buff[i] == '-' && i == 0) {
      negative = true;
    } else if (buff[i] == '+' && i == 0) {
      continue;
    } else if (buff[i] >= '0' && buff[i] <= '9') {
      int num = (int)(buff[i] - '0');
      value *= 10;
      value += num;
    } else {
      return false;
    }
  }
  
  if (buff[i] == '-' || buff[i] == '+') return false;
  (*number) = value;
  return true;
}
