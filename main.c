#include <stdio.h>
#include <string.h>
#include "crypto_aead.h"
#include <stdlib.h>

#define KEY_SIZE 16
#define NONCE_SIZE 16
#define TAG_SIZE 16
#define BLOCK_SIZE 64


#define KEYBYTES 32
#define NONCEBYTES 12
#define TAGBYTES 8

int main() {
    unsigned char key[KEYBYTES] = {0}; // use a random key instead of all zeros
    unsigned char nonce[NONCEBYTES] = {0}; // use a random nonce instead of all zeros
    unsigned char ad[] = {0}; // no associated data
    unsigned long long adlen = 0;
    unsigned char* plaintext = malloc(sizeof(unsigned char));

    printf("insert plain text :");
    scanf("%s", plaintext);

    unsigned long long plaintext_len = strlen(plaintext);
    unsigned char ciphertext[plaintext_len];
    unsigned long long ciphertext_len = 0;
    unsigned char tag[TAGBYTES] = {0};

    int ret = crypto_aead_encrypt(ciphertext, &ciphertext_len,
                                  plaintext, plaintext_len,
                                  ad, adlen,
                                  NULL , // no additional authenticated data
                                  nonce,
                                  key);

    if (ret != 0) {
        printf("Encryption failed\n");
        return 1;
    }

    printf("Ciphertext: ");
    for (int i = 0; i < plaintext_len; i++) {
        printf("%c", ciphertext[i]);
    }
    printf("\n");

	int inv = crypto_aead_decrypt(plaintext, &plaintext_len, NULL, ciphertext, ciphertext_len, ad, adlen, nonce, key);

	if(inv != 0){
		printf("Decrypt failed");
		return 1;
	}
	printf("Plaintext: ");
	for(int i= 0; i < plaintext_len; i++){
		printf("%c", plaintext[i]);
	}

    return 0;
}


