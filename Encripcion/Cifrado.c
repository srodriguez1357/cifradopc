#include <stdlib.h>
#include <stdio.h>
#include <sodium.h>

char Mensaje[100];
#define MESSAGE ((unsigned char *) Mensaje)
#define MESSAGE_LEN 100
#define CIPHERTEXT_LEN (crypto_secretbox_MACBYTES + MESSAGE_LEN)

void dump_hex_buff(unsigned char buf[], unsigned int len)
{
    int i;
    for (i = 0; i < len; i++) printf("%02X ", buf[i]);
    printf("\n");
}

int main(int argc, char* argv[])
{
    FILE* archivo;
    archivo = fopen("texto.txt", "r");
    fgets(Mensaje,100, archivo);
    unsigned char cifrado[CIPHERTEXT_LEN];
    unsigned char descifrado[MESSAGE_LEN];
    unsigned char llave[crypto_secretbox_KEYBYTES];
    unsigned char nonce[crypto_secretbox_NONCEBYTES];

    crypto_secretbox_keygen(llave);
    printf("secret key generated:\n");
    dump_hex_buff(llave, crypto_secretbox_KEYBYTES);

    randombytes_buf(nonce, sizeof nonce);
    printf("nonce:\n");
    dump_hex_buff(nonce, sizeof nonce);

    char option;
    printf("C Cifrar D Descifrar E) Cerrar\n");
    scanf("%c", &option);

    while (1)
    {
        switch (option)
        {
        case 'C':
            crypto_secretbox_easy(cifrado, MESSAGE, MESSAGE_LEN, nonce, llave);
            printf("ciphertext:\n");
            dump_hex_buff(cifrado, CIPHERTEXT_LEN);
            break;
        case 'D':
            if (crypto_secretbox_open_easy(descifrado, cifrado, CIPHERTEXT_LEN, nonce, llave) == 0) {
                printf("decrypted data (hex):\n");
                dump_hex_buff(descifrado, MESSAGE_LEN);
                printf("decrpyted data (ascii):%s\n", descifrado);
            }
            break;
        default:
            break;
        }
        scanf("%c", &option);

        if (option == 'E')
            break;
        
    }
    return 0;
}