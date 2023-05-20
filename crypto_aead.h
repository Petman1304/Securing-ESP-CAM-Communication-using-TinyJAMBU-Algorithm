#ifndef CRYPTO_AEAD
#define CRYPTO_AEAD

// generating a ciphertext c[0],c[1],...,c[*clen-1]
// from a plaintext m[0],m[1],...,m[mlen-1]
// and associated data ad[0],ad[1],...,ad[adlen-1]
// and secret message number nsec[0],nsec[1],...
// and public message number npub[0],npub[1],...
// and secret key k[0],k[1],...
int crypto_aead_encrypt(
  unsigned char *c, unsigned long *clen,
  const unsigned char *m, unsigned long mlen,
  const unsigned char *ad, unsigned long adlen,
  const unsigned char *nsec,
  const unsigned char *npub,
  const unsigned char *k);

char* imageToString(char* imagePath);

unsigned char* base64Decode(const char* input, size_t inputLen, size_t* outputLen);

void writeImageFile(char* imagePath, unsigned char* imageData, size_t imageSize);

void stringToImage(char* imagePath, char* imageString) ;
// generating a plaintext m[0],m[1],...,m[*mlen-1]
// and secret message number nsec[0],nsec[1],...
// from a ciphertext c[0],c[1],...,c[clen-1]
// and associated data ad[0],ad[1],...,ad[adlen-1]
// and public message number npub[0],npub[1],...
// and secret key k[0],k[1],...
int crypto_aead_decrypt(
  unsigned char *m, unsigned long *mlen,
  unsigned char *nsec,
  const unsigned char *c, unsigned long clen,
  const unsigned char *ad, unsigned long adlen,
  const unsigned char *npub,
  const unsigned char *k);

#endif
