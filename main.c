#include <stdio.h>
#include <string.h>

#include <stdlib.h>

#include "crypto_aead.h"
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
    unsigned long adlen = 0;
    unsigned char* plaintext = imageToString("example2.png");

    // printf("insert plain text :");
    // scanf("%s", plaintext);

    unsigned long plaintext_len = strlen(plaintext);
    unsigned char ciphertext[plaintext_len];
    unsigned long ciphertext_len = 0;
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

    FILE* filePointer = fopen("output.txt", "w"); // Open file in write mode
    if (filePointer != NULL) { // Check if file was successfully opened
        
        fprintf(filePointer, "Plaintext : \n");
        fprintf(filePointer, "%s", plaintext);
        fprintf(filePointer, "\n \n");
        fprintf(filePointer, "Ciphertext:");
        fprintf(filePointer, "%s", ciphertext); // Write string to file
        fprintf(filePointer, "\n \n ");
        }
    printf("\n");

    unsigned char decrypt[ciphertext_len];
	int inv = crypto_aead_decrypt(decrypt, &plaintext_len, NULL, ciphertext, ciphertext_len, ad, adlen, nonce, key);

    stringToImage("output1.txt", decrypt);

    fprintf(filePointer, "Decrypt : \n");
    fprintf(filePointer, "%d", decrypt);
    fclose(filePointer);

	if(inv != 0){
		printf("Decrypt failed");
		return 1;
	}

    return 0;
}


