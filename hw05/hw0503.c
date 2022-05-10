#include <popt.h>
#include "basic.h"
#include "mmap.h"
#include "color.h"

typedef struct _sBase64 {
	union data {
		uint32_t total: 24;
		struct ascii {
			uint8_t c0;
			uint8_t c1;
			uint8_t c2;
		} __attribute__((scalar_storage_order("big-endian"))) ascii;
		struct base64 {
			uint16_t c0: 6;
			uint16_t c1: 6;
			uint16_t c2: 6;
			uint16_t c3: 6;
		} __attribute__((packed)) __attribute__((scalar_storage_order("big-endian"))) base64;
	} __attribute__((packed)) __attribute__((scalar_storage_order("big-endian"))) data;
} sBase64;

const char encoding_table[64] = {
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
	'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
	'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
	'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
	'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
	'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
	'w', 'x', 'y', 'z', '0', '1', '2', '3',
	'4', '5', '6', '7', '8', '9', '+', '/'
};

int8_t decoding_table[256];

void init_decode_table() {
	memset(decoding_table, -1, sizeof(decoding_table));
	for(size_t i = 0; i < 64; ++i) decoding_table[(uint8_t)encoding_table[i]] = i;
	decoding_table['='] = 0;
}

int main(int argc, char *argv[]) {
	init_decode_table();
	bool isencode = false, isdecode = false;
	char opt, *outname = NULL, *inname = NULL;

	// opt table
	struct poptOption optionsTable[] = {
		{"enc", 'e', POPT_ARG_STRING, &inname, 'e',
			"Encode a file to a text file.", "<input>"},
		{"dec", 'd', POPT_ARG_STRING, &inname, 'd',
			"Decode a text file to a file.", "<input>"},
		{"output", 'o', POPT_ARG_STRING, &outname, 0,
			"Output file name.", "<output>"},
		POPT_AUTOHELP
		POPT_TABLEEND
	};

	// get context
	poptContext optCon = poptGetContext(NULL, argc, (const char **)argv, optionsTable, 0);

	// check if arg is too few
	if(argc < 5) {
		fprintf(stderr, "error: arguments is too few\n");
		poptPrintUsage(optCon, stderr, 0);
		poptFreeContext(optCon);
		return 1;
	}

	// read opt
	while((opt = poptGetNextOpt(optCon)) > 0) {
		switch(opt) {
		case 'e':
			isencode = true;
			break;
		case 'd':
			isdecode = true;
			break;
		}
	}

	// printf(CLR_WHT "inname: %s\noutname: %s\n" CLR_RST, inname, outname);

	// encode and decode at the same time
	if(isencode && isdecode) {
		fprintf(stderr, "error: could not encode and decode at the same time\n");
		poptFreeContext(optCon);
		if(inname) free(inname);
		if(outname) free(outname);
		exit(1);
	}

	// neither encode nor decode
	if(!isencode && !isdecode) {
		fprintf(stderr, "error: you must choose encode or decode\n");
		poptFreeContext(optCon);
		if(inname) free(inname);
		if(outname) free(outname);
		exit(1);
	}

	// input file name is invalid
	if(inname == NULL) {
		fprintf(stderr, "error: invalid input filename\n");
		poptFreeContext(optCon);
		if(inname) free(inname);
		if(outname) free(outname);
		exit(1);
	}
	
	// output file name is invalid
	if(outname == NULL) {
		fprintf(stderr, "error: invalid output filename\n");
		poptFreeContext(optCon);
		if(inname) free(inname);
		if(outname) free(outname);
		exit(1);
	}

	sMmap *inmmp = new_mmap(inname, MAP_SHARED);
	sMmap *outmmp = new_mmap(outname, MAP_SHARED);

	// map failed
	if(inmmp == NULL || outmmp == NULL) {
		fprintf(stderr, "error: failed to map files\n");
		poptFreeContext(optCon);
		if(inname) free(inname);
		if(outname) free(outname);
		exit(1);
	}

	// encode
	if(isencode) {		
		size_t insize = inmmp->sb.st_size, resize = ((insize-1)/3+1)*3, outsize = resize/3*4;
		resize_mmap(outmmp, outsize);
		sBase64 *data = calloc(1, resize);
		memcpy(data, inmmp->addr, insize);
		
		size_t cnt = 0, remaining = insize;
		uint8_t *cur = (uint8_t*)outmmp->addr;
		while(remaining) {
			cur[0] = encoding_table[data[cnt].data.base64.c0];
			cur[1] = encoding_table[data[cnt].data.base64.c1];
			cur[2] = (remaining <= 1 ? '=' : encoding_table[data[cnt].data.base64.c2]);
			cur[3] = (remaining <= 2 ? '=' : encoding_table[data[cnt].data.base64.c3]);
			remaining = (remaining <= 3 ? 0 : remaining-3);
			cur += 4;
			++cnt;
		}
		free(data);
	}

	// decode
	if(isdecode) {		
		size_t insize = inmmp->sb.st_size, resize = ((insize-1)/4+1)*4, outsize = resize/4*3;
		sBase64 *data = calloc(1, resize);

		size_t cnt = 0, remaining = insize;
		uint8_t *cur = (uint8_t*)inmmp->addr;
		while(remaining) {
			for(size_t i = 0; i < min(4, remaining); i++) {
				if(decoding_table[cur[i]] == -1) {
					fprintf(stderr, "error: character '%c' is not base 64 format\n", cur[i]);
					exit(1);
				}
			}
			data[cnt].data.base64.c0 = decoding_table[cur[0]];
			data[cnt].data.base64.c1 = decoding_table[cur[1]];
			data[cnt].data.base64.c2 = (remaining <= 2 ? '\0' : decoding_table[cur[2]]);
			data[cnt].data.base64.c3 = (remaining <= 3 ? '\0' : decoding_table[cur[3]]);

			if(remaining <= 4) {
				remaining = 0;
				if(cur[3] == '=') --outsize;
				if(cur[2] == '=') --outsize;
			}
			else remaining -= 4;

			cur += 4;
			++cnt;
		}
		resize_mmap(outmmp, outsize);
		memcpy(outmmp->addr, data, outsize);

		free(data);
	}

	// sizeof(sBase64);

	free_mmap(inmmp);
	free_mmap(outmmp);
	poptFreeContext(optCon);
	if(inname) free(inname);
	if(outname) free(outname);
	return 0;
}