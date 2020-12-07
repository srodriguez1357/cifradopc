
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sodium.h>

int Firmas(uint8_t sm[], const uint8_t m[], const int mlen, const uint8_t secret[]) {
	unsigned long long smlen;
	if (crypto_sign(sm, &smlen, m, mlen, secret) == 0)
		return smlen;
}

int Verificar(uint8_t m[], const uint8_t sm[], const int smlen, const uint8_t public[]) {
	unsigned long long mlen;
	if (crypto_sign_open(m, &mlen, sm, smlen, public) == 0) 
		return mlen;
}

int main() {
	uint8_t saltm[128 + crypto_sign_BYTES];
	uint8_t box[128 + crypto_sign_BYTES];
	uint8_t secret[crypto_sign_SECRETKEYBYTES];
	uint8_t public[crypto_sign_PUBLICKEYBYTES];
	

	char line[128];
	FILE* archivo;

	archivo = fopen_s(&archivo, "file.txt", "r");
	memset(box, '\0', 128);
	int mlen = snprintf(box, 128, "%c", archivo);
	int keys = crypto_sign_keypair(public, secret);

	int smlen;
	char option;
	printf("F Firmar V Verificar E Cerrar\n");
	scanf_s("%c", &option);

	while (1) {
		switch (option)
		{
		case 'F':
			smlen = Firmas(saltm, box, mlen, secret);
			if (smlen > 0) {
				printf("Secret Key: ");
				for (size_t i = 0; i < 64; i++)
				{
					printf("%XC", secret[i]);
				}
				printf("\nFirmado\n\n");
				break;
			}
		case 'V':
			mlen = Verificar(box, saltm, smlen, public);
			if (mlen > 0) {
				printf("Public Key:");
				for (size_t i = 0; i < 32; i++)
				{
					printf("%XC", public[i]);
				}
				printf("\nVerificado\n");
				break;
			}
		
		default:
			break;
		}
		scanf_s("%c", &option);
		if (option == 'E')
			break;
	}
	return 0;
}
