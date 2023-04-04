#include <stdio.h>
#include <string.h>
#include "tinyjambu.h"

#define KEY_SIZE 16
#define NONCE_SIZE 16
#define TAG_SIZE 16
#define BLOCK_SIZE 64

void encrypt(unsigned char* plaintext, int plaintext_len,
             unsigned char* key, unsigned char* nonce,
             unsigned char* ciphertext, unsigned char* tag) {
    unsigned char block[BLOCK_SIZE];
    int i, j;
    
    /* Initialize the TinyJAMBU state with the key and nonce */
    tinyjambu_state state;
    tinyjambu_init(&state, KEY_SIZE, key, NONCE_SIZE, nonce);
    
    /* Generate the keystream blocks and XOR with the plaintext to produce the ciphertext */
    for (i = 0; i < plaintext_len; i += BLOCK_SIZE) {
        tinyjambu_generate(&state, block);
        for (j = 0; j < BLOCK_SIZE && i+j < plaintext_len; j++) {
            ciphertext[i+j] = plaintext[i+j] ^ block[j];
        }
    }
    
    /* Generate the MAC using the HMAC-SHA256 algorithm */
    hmac_sha256(key, KEY_SIZE, ciphertext, plaintext_len, tag);
}

int decrypt(unsigned char* ciphertext, int ciphertext_len,
            unsigned char* key, unsigned char* nonce,
            unsigned char* tag, unsigned char* plaintext) {
    unsigned char block[BLOCK_SIZE];
    int i, j;
    
    /* Initialize the TinyJAMBU state with the key and nonce */
    tinyjambu_state state;
    tinyjambu_init(&state, KEY_SIZE, key, NONCE_SIZE, nonce);
    
    /* Generate the keystream blocks and XOR with the ciphertext to produce the plaintext */
    for (i = 0; i < ciphertext_len; i += BLOCK_SIZE) {
        tinyjambu_generate(&state, block);
        for (j = 0; j < BLOCK_SIZE && i+j < ciphertext_len; j++) {
            plaintext[i+j] = ciphertext[i+j] ^ block[j];
        }
    }
    
    /* Verify the MAC using the HMAC-SHA256 algorithm */
    return hmac_sha256_verify(key, KEY_SIZE, ciphertext, ciphertext_len, tag);
}

int main() {
    /* Generate a random key and nonce */
    unsigned char key[KEY_SIZE];
    unsigned char nonce[NONCE_SIZE];
    get_random_bytes(key, KEY_SIZE);
    get_random_bytes(nonce, NONCE_SIZE);
    
    /* Encrypt and authenticate a message */
    unsigned char plaintext[] = "Hello, world!";
    int plaintext_len = strlen(plaintext);
    unsigned char ciphertext[plaintext_len];
    unsigned char tag[TAG_SIZE];
    encrypt(plaintext, plaintext_len, key, nonce, ciphertext, tag);
    
    /* Decrypt and verify the message */
    unsigned char decrypted[plaintext_len];
    int verified = decrypt(ciphertext, plaintext_len, key, nonce, tag, decrypted);
    
    if (verified) {
        printf("Decrypted message: %s\n", decrypted);
    } else {
        printf("Message authentication failed!\n");
    }
    
    return 0;
}
