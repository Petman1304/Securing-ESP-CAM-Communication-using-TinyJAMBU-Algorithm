/*
    2  * Copyright (C) 2020 Southern Storm Software, Pty Ltd.
    3  *
    4  * Permission is hereby granted, free of charge, to any person obtaining a
    5  * copy of this software and associated documentation files (the "Software"),
    6  * to deal in the Software without restriction, including without limitation
    7  * the rights to use, copy, modify, merge, publish, distribute, sublicense,
    8  * and/or sell copies of the Software, and to permit persons to whom the
    9  * Software is furnished to do so, subject to the following conditions:
   10  *
   11  * The above copyright notice and this permission notice shall be included
   12  * in all copies or substantial portions of the Software.
   13  *
   14  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
   15  * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   16  * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   17  * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   18  * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   19  * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
   20  * DEALINGS IN THE SOFTWARE.
   21  */ 
#ifndef LWCRYPTO_TINYJAMBU_H
#define LWCRYPTO_TINYJAMBU_H 
#include "aead-common.h"
 
#ifdef __cplusplus
 extern "C" {
#endif
 
 #define TINY_JAMBU_128_KEY_SIZE 16
 
 #define TINY_JAMBU_192_KEY_SIZE 24
 
 #define TINY_JAMBU_256_KEY_SIZE 32
 
 #define TINY_JAMBU_TAG_SIZE 8
 
 #define TINY_JAMBU_NONCE_SIZE 12
 
 extern aead_cipher_t const tiny_jambu_128_cipher;
 
 extern aead_cipher_t const tiny_jambu_192_cipher;
 
 extern aead_cipher_t const tiny_jambu_256_cipher;
 
 int tiny_jambu_128_aead_encrypt
     (unsigned char *c, unsigned long long *clen,
      const unsigned char *m, unsigned long long mlen,
      const unsigned char *ad, unsigned long long adlen,
      const unsigned char *nsec,
      const unsigned char *npub,
      const unsigned char *k);
 
 int tiny_jambu_128_aead_decrypt
     (unsigned char *m, unsigned long long *mlen,
      unsigned char *nsec,
      const unsigned char *c, unsigned long long clen,
      const unsigned char *ad, unsigned long long adlen,
      const unsigned char *npub,
      const unsigned char *k);
 
 int tiny_jambu_192_aead_encrypt
     (unsigned char *c, unsigned long long *clen,
      const unsigned char *m, unsigned long long mlen,
      const unsigned char *ad, unsigned long long adlen,
      const unsigned char *nsec,
      const unsigned char *npub,
      const unsigned char *k);
 
 int tiny_jambu_192_aead_decrypt
     (unsigned char *m, unsigned long long *mlen,
      unsigned char *nsec,
      const unsigned char *c, unsigned long long clen,
      const unsigned char *ad, unsigned long long adlen,
      const unsigned char *npub,
      const unsigned char *k);
 
 int tiny_jambu_256_aead_encrypt
     (unsigned char *c, unsigned long long *clen,
      const unsigned char *m, unsigned long long mlen,
      const unsigned char *ad, unsigned long long adlen,
      const unsigned char *nsec,
      const unsigned char *npub,
      const unsigned char *k);
 
 int tiny_jambu_256_aead_decrypt
     (unsigned char *m, unsigned long long *mlen,
      unsigned char *nsec,
      const unsigned char *c, unsigned long long clen,
      const unsigned char *ad, unsigned long long adlen,
      const unsigned char *npub,
      const unsigned char *k);
 
 #ifdef __cplusplus
 }
 #endif
 
 #endif