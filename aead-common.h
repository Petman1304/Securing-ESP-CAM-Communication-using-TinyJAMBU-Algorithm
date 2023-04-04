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

#ifndef LWCRYPTO_AEAD_COMMON_H
#define LWCRYPTO_AEAD_COMMON_H
 
 #include <stddef.h>
 
 #ifdef __cplusplus
 extern "C" {
 #endif
 
 typedef int (*aead_cipher_encrypt_t)
      (unsigned char *c, unsigned long long *clen,
       const unsigned char *m, unsigned long long mlen,
       const unsigned char *ad, unsigned long long adlen,
       const unsigned char *nsec,
       const unsigned char *npub,
       const unsigned char *k);
  
  typedef int (*aead_cipher_decrypt_t)
      (unsigned char *m, unsigned long long *mlen,
      unsigned char *nsec,
      const unsigned char *c, unsigned long long clen,
      const unsigned char *ad, unsigned long long adlen,
      const unsigned char *npub,
      const unsigned char *k);
 
 typedef int (*aead_hash_t)
     (unsigned char *out, const unsigned char *in, unsigned long long inlen);
 
 typedef void (*aead_hash_init_t)(void *state);
 
 typedef void (*aead_hash_update_t)
     (void *state, const unsigned char *in, unsigned long long inlen);
 
 typedef void (*aead_hash_finalize_t)(void *state, unsigned char *out);
 
 typedef void (*aead_xof_absorb_t)
     (void *state, const unsigned char *in, unsigned long long inlen);
 
 typedef void (*aead_xof_squeeze_t)
     (void *state, unsigned char *out, unsigned long long outlen);
 
 #define AEAD_FLAG_NONE              0x0000
 
 #define AEAD_FLAG_LITTLE_ENDIAN     0x0001
 
 #define AEAD_FLAG_SC_PROTECT_KEY    0x0002
 
 #define AEAD_FLAG_SC_PROTECT_ALL    0x0004
 
 typedef struct
 {
     const char *name;               
     unsigned key_len;               
     unsigned nonce_len;             
     unsigned tag_len;               
     unsigned flags;                 
     aead_cipher_encrypt_t encrypt;  
     aead_cipher_decrypt_t decrypt;  
 } aead_cipher_t;
 
 typedef struct
 {
     const char *name;           
     size_t state_size;          
     unsigned hash_len;          
     unsigned flags;             
     aead_hash_t hash;           
     aead_hash_init_t init;      
     aead_hash_update_t update;  
     aead_hash_finalize_t finalize; 
     aead_xof_absorb_t absorb;   
     aead_xof_squeeze_t squeeze; 
 } aead_hash_algorithm_t;
 
 int aead_check_tag
     (unsigned char *plaintext, unsigned long long plaintext_len,
      const unsigned char *tag1, const unsigned char *tag2,
      unsigned tag_len);
 
 int aead_check_tag_precheck
     (unsigned char *plaintext, unsigned long long plaintext_len,
      const unsigned char *tag1, const unsigned char *tag2,
      unsigned tag_len, int precheck);
 
 #ifdef __cplusplus
 }
 #endif
 
 #endif
