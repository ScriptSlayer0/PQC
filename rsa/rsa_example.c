//gcc -o examples/rsa_example rsa/rsa_example.c -lssl -lcrypto
#include "rsa.h"
#include <stdio.h>
#include <string.h>

#define __need_fsize

int main() {
    // Generate RSA key pair
    KEY keypair = gen_key();

    // Save keys to PEM files
    save_pubkey(keypair, "./");
    save_privkey(keypair, "./");
    EVP_PKEY_free(keypair);

    // Load keys from PEM files
    KEY pub_key = load_pubkey("./");
    KEY priv_key = load_privkey("./");

    RSA_fencrypt(pub_key, "./test.txt", NULL);
    unsigned char *decrypted = RSA_fdecrypt(priv_key, "./test.txt");

    printf("%02x\n", decrypted);

    EVP_PKEY_free(pub_key);
    EVP_PKEY_free(priv_key);

    return 0;
}
