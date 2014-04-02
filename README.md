# Abstract

This is going to be an easy-to-use C-based [KeyedBits](https://github.com/unixpickle/KeyedBits) transcoder. It will provide encoding and decoding from event-driven streams. I plan to use this for messaging in my operating system.

# Packet (a.k.a. "buff") encoders

The two headers, `<keyedbits/buff_encoder.h>` and `<keyedbits/buff_decoder.h>` define an interface for reading and writing KeyedBits objects to and from fixed-size data buffers. This is useful when dynamic memory allocation is not available. The obvious drawback is that this API forces you to keep your objects below a certain maximum size in order for the encode/decode to succeed.

## Decoding objects

Let `buffer` be your input buffer of KeyedBits data, and `len` be its length. Setup a decoder as follows:

    kb_buff_t kb;
    kb_buff_initialize_decode(&kb, buffer, len);
