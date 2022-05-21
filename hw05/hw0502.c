#include <popt.h>
#include "basic.h"
#include "mmap.h"
#include "genres.h"

typedef struct _sID3v1Tags {
	char header[3];  // "TAG"
	char title[30];
	char artist[30];
	char album[30];
	char year[4];
	char comment[30];  // if comment length < 29, last two bytes is zero byte and track
	uint8_t genre;
} sID3v1Tags;

int main(int argc, char *argv[]) {
	bool isdelete = false, islist = false, settag = false;
	char opt, buf[BUFSIZ];
	i32 tracktag = -1;
	char *titletag = NULL,
			*artisttag = NULL,
			*albumtag = NULL,
			*yeartag = NULL,
			*commenttag = NULL,
			*genretag = NULL;

	// opt table
	struct poptOption optionsTable[] = {
		{NULL, 't', POPT_ARG_STRING, &titletag, 't',
			"Modifies a Title tag", "<tag>"},
		{NULL, 'T', POPT_ARG_INT, &tracktag, 'T',
			"Modifies a Track tag", "<tag>"},
		{NULL, 'a', POPT_ARG_STRING, &artisttag, 'a',
			"Modifies an Artist tag", "<tag>"},
		{NULL, 'A', POPT_ARG_STRING, &albumtag, 'A',
			"Modifies an Album tag", "<tag>"},
		{NULL, 'y', POPT_ARG_STRING, &yeartag, 'y',
			"Modifies a Year tag", "<tag>"},
		{NULL, 'c', POPT_ARG_STRING, &commenttag, 'c',
			"Modifies a Comment tag", "<tag>"},
		{NULL, 'g', POPT_ARG_STRING, &genretag, 'g',
			"Modifies a Genre tag", "<tag>"},
		{NULL, 'l', POPT_ARG_NONE, 0, 'l',
			"Lists an ID3 tag", NULL},
		{NULL, 'd', POPT_ARG_NONE, 0, 'd',
			"Deletes an ID3 tag", NULL},
		{NULL, 'h', POPT_ARG_NONE, 0, 'h',
			"Displays this help info", NULL},
		POPT_AUTOHELP
		POPT_TABLEEND
	};

	// get context
	poptContext optCon = poptGetContext(NULL, argc, (const char **)argv, optionsTable, 0);

	// read opt
	while((opt = poptGetNextOpt(optCon)) > 0) {
		switch(opt) {
		case 'l':
			islist = true;
			break;
		case 'd':
			isdelete = true;
			break;
		}
	}

	// an error occurred during option processing
	if (opt < -1) {
		fprintf(stderr, "error: %s: %s\n",
				poptBadOption(optCon, POPT_BADOPTION_NOALIAS),
				poptStrerror(opt));
		poptFreeContext(optCon);
		exit(1);
	}

	// check any tag to be set
	settag = (titletag || artisttag || albumtag || yeartag || commenttag || genretag || tracktag != -1);

	const char *filename;
	while((filename = poptGetArg(optCon)) != NULL) {
		sMmap *mmp = new_mmap(filename, MAP_SHARED);

		// map failed
		if(mmp == NULL) {
			fprintf(stderr, "error: failed to open file \"%s\"\n", filename);
			continue;
		}

		if(islist || isdelete) printf("%s:\n", filename);

		sID3v1Tags *tags = (sID3v1Tags*)(mmp->addr+mmp->sb.st_size-128);
		bool hastag = strncmp(tags->header, "TAG", 3) == 0;
		
		if(!hastag) {
			if(islist) fprintf(stderr, "warning: this file has no tag, nothing to be print\n");
			if(isdelete) fprintf(stderr, "warning: this file has no tag, nothing to be delete\n");
			if(settag) {
				resize_mmap(mmp, mmp->sb.st_size+128);
				tags = (sID3v1Tags*)(mmp->addr+mmp->sb.st_size-128);
				memset(tags, 0, sizeof(sID3v1Tags));
				strcpy(tags->header, "TAG");
				tags->comment[28] = 1;
				tags->genre = 255;
			}
		}
		else {
			if(islist) {
				memcpy(buf, tags->title, 30);
				buf[30] = '\0';
				printf("  Title   : %s\n", buf);

				memcpy(buf, tags->artist, 30);
				buf[30] = '\0';
				printf("  Artist  : %s\n", buf);

				memcpy(buf, tags->album, 30);
				buf[30] = '\0';
				printf("  Album   : %s\n", buf);

				memcpy(buf, tags->year, 4);
				buf[4] = '\0';
				printf("  Year    : %s\n", buf);

				if(tags->genre != 255) {
					printf("  Genre   : %s (%hhu)\n", genres[tags->genre], tags->genre);
				}
				else {
					printf("  Genre   : None\n");
				}

				memcpy(buf, tags->comment, 30);
				buf[30] = '\0';
				printf("  Comment : %s\n", buf);

				if(strnlen(tags->comment, 30) <29 && tags->comment[28] == 0)
					printf("  Track   : %hhu\n", tags->comment[29]);
				else 
					printf("  Track   :\n");
			}
			if(isdelete) {
				if(!settag) resize_mmap(mmp, mmp->sb.st_size-128);
				else {
					memset(tags, 0, sizeof(sID3v1Tags));
					strcpy(tags->header, "TAG");
					tags->comment[28] = 1;
					tags->genre = 255;
				}
			}
		}
		if(settag) {
			if(titletag != NULL) {
				memcpy(tags->title, titletag, 30);
			}
			if(artisttag != NULL) {
				memcpy(tags->artist, artisttag, 30);
			}
			if(albumtag != NULL) {
				memcpy(tags->album, albumtag, 30);
			}
			if(yeartag != NULL) {
				memcpy(tags->year, yeartag, 4);
			}
			if(commenttag != NULL) {
				memcpy(tags->comment, commenttag, 30);
			}
			// to set track 
			if(tracktag != -1) {
				tags->comment[28] = 0;
				tags->comment[29] = tracktag;
			}
			if(genretag != NULL) {
				i32 genre_id = -1;
				for(size_t i = 0; i < GENRES_NUM; ++i) {
					if(strcmp(genretag, genres[i]) == 0) {
						genre_id = i;
						break;
					}
				}
				if(genre_id == -1) {
					fprintf(stderr, "warning: genre \"%s\" not found\n", genretag);
				}
				else {
					tags->genre = genre_id;
				}
			}
		}
		
		if(islist || isdelete) printf("\n");

		free_mmap(mmp);
	}
	
	if(titletag) free(titletag);
	if(artisttag) free(artisttag);
	if(albumtag) free(albumtag);
	if(yeartag) free(yeartag);
	if(commenttag) free(commenttag);
	if(genretag) free(genretag);
	poptFreeContext(optCon);
	return 0;
}