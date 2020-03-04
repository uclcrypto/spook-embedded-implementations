
int crypto_aead_encrypt(unsigned char* c,  unsigned int* clen,
                        const unsigned char* m,  unsigned int mlen,
                        const unsigned char* ad,  unsigned int adlen,
                        const unsigned char* nsec, const unsigned char* npub,
                        const unsigned char* k);

int crypto_aead_decrypt(unsigned char* m,  unsigned int* mlen,
                        unsigned char* nsec, const unsigned char* c,
                         unsigned int clen, const unsigned char* ad,
                         unsigned int adlen, const unsigned char* npub,
                        const unsigned char* k);
