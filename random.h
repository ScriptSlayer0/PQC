#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "BinOp.h"

#ifdef _WIN32
    #include <windows.h>
    #include <wincrypt.h>

    unsigned int get_random_bytes() {
        HCRYPTPROV hCryptProv;
        BYTE buf;

        if (!CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT)) {
            fprintf(stderr, "CryptAcquireContext failed\n");
            exit(EXIT_FAILURE);
        }

        if (!CryptGenRandom(hCryptProv, sizeof(buf), &buf)) {
            fprintf(stderr, "CryptGenRandom failed\n");
            CryptReleaseContext(hCryptProv, 0);
            exit(EXIT_FAILURE);
        }

        CryptReleaseContext(hCryptProv, 0);
        return buf;
    }

#else
    #include <fcntl.h>
    #include <unistd.h>

    unsigned int get_random_bytes() {
        int fd = open("/dev/urandom", O_RDONLY);
        unsigned int buf;

        if (fd == -1) {
            perror("open");
            exit(EXIT_FAILURE);
        }

        if (read(fd, &buf, sizeof(buf)) != sizeof(buf)) {
            perror("read");
            close(fd);
            exit(EXIT_FAILURE);
        }

        close(fd);
        return buf;
    }
#endif

int randombelow(int max) {
    return get_random_bytes() % max;
}

int* randarray(int size, int max) {
    int* array = (int*)malloc(size * sizeof(int));
    if (array == NULL) {
        perror("Array Allocation Failed");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < size; i++) {
        array[i] = randombelow(max);
    }

    return array;
}

// Define Q
#define Q 8380417

// NIST FIPS 203: Algorithm 8
int* SamplePolyCBD(uint8_t* B, int eta, int n) {
    int* f = (int*)malloc(256 * sizeof(int));
    int* b = BytesToBits(B, n); // Asegúrate de pasar el argumento adicional necesario

    for (int i = 0; i < 256; i++) {
        int x = 0, y = 0;
        for (int j = 0; j < n; j++) {
            x += b[2 * i * eta + j];
            y += b[2 * i * eta + eta + j];
        }
        f[i] = ((x - y) % Q + Q) % Q;
    }

    return f;
}
