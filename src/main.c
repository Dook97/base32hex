#include <err.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "base32hex.h"

#define USAGE "USAGE: base32hex encode|decode"

#define BASE_BUFSZ 4096
#define READ_QUANTUM 4096

int main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "%s\n", USAGE);
		return 1;
	}

	bool encode = !strcmp(argv[1], "encode");
	bool decode = !strcmp(argv[1], "decode");
	if (!encode && !decode) {
		fprintf(stderr, "%s\n", USAGE);
		return 1;
	}

	ssize_t bufsz = BASE_BUFSZ;
	uint8_t *in = malloc(BASE_BUFSZ);
	if (in == NULL)
		err(1, "malloc: ");

	int readsz;
	int in_len = 0;
	while ((readsz = read(0, in + in_len, READ_QUANTUM)) > 0) {
		in_len += readsz;
		if (in_len + READ_QUANTUM > bufsz) {
			bufsz *= 2;
			in = realloc(in, bufsz);
			if (in == NULL)
				err(1, "realloc: ");
		}
	}
	if (readsz < 0)
		err(1, "read: ");

	uint8_t *buf;
	int32_t osz = encode ? base32hex_encode_alloc(in, in_len, &buf)
			     : base32hex_decode_alloc(in, in_len, &buf);

	if (osz == -1) {
		fprintf(stderr, "error (probably bad input)\n");
		return 1;
	}

	write(1, buf, osz);
	if (encode)
		putc('\n', stdout);

	// why free() when u can let the OS do it 4 u? 8)
}
