#include "basic.h"
#include "pm2.h"
#include "zhprint.h"
#include "mymatch.h"
#include <dirent.h>
#include "color.h"

void attr0_modifier(sSaveGame *sg) {
	char buf[BUFSIZ];
	uint16_t *arr = &sg->attr->stamina;
	static const char *term[10] = {
		"體力","腕力","智力","氣質","魅力","道德","信仰","罪孽","感受","疲勞"
	};


	size_t selectidx = -1;
	uint16_t val = -1;
	while(1) {
		for(size_t i = 0; i < 10; i++) printf("%2ld: %s(%hu)\n", i+1, term[i], arr[i]);
		printf(" 0: 離開\n");
		printf("請選擇欲修改之項目 (1-10), 輸入 0 離開: ");
		while(!fgets_n(buf, sizeof(buf), stdin));
		sscanf(buf, "%lu", &selectidx);
		if(selectidx == 0) return;
		if(selectidx > 4) break;

		while(1) {
			printf("請輸入欲修改之數值 (0-999): ");
			while(!fgets_n(buf, sizeof(buf), stdin));
			sscanf(buf, "%hu", &val);
			if(val > 999)  {
				system("clear");
				printf(CLR_RED "輸入錯誤，請重新輸入\n" CLR_RST);
				continue;
			}
			arr[selectidx-1] = val;
			printf("%s 已被修改為 %hu\n", term[selectidx-1], val);
			break;
		}
	}
}

void attr1_modifier(sSaveGame *sg) {
	char buf[BUFSIZ];
	uint16_t *arr = &sg->attr->fgt_rep;
	static const char *term[4] = {
		"戰士評價","魔法評價","社交評價","家事評價"
	};


	size_t selectidx = -1;
	uint16_t val = -1;
	while(1) {
		for(size_t i = 0; i < 4; i++) printf("%2ld: %s(%hu)\n", i+1, term[i], arr[i]);
		printf("0: 離開\n");
		printf("請選擇欲修改之項目 (1-4), 輸入 0 離開: ");
		while(!fgets_n(buf, sizeof(buf), stdin));
		sscanf(buf, "%lu", &selectidx);
		if(selectidx == 0) return;
		if(selectidx > 4) break;

		while(1) {
			printf("請輸入欲修改之數值 (0-999): ");
			while(!fgets_n(buf, sizeof(buf), stdin));
			sscanf(buf, "%hu", &val);
			if(val > 999)  {
				system("clear");
				printf(CLR_RED "輸入錯誤，請重新輸入\n" CLR_RST);
				continue;
			}
			arr[selectidx-1] = val;
			printf("%s 已被修改為 %hu\n", term[selectidx-1], val);
			break;
		}
	}
}

void attr2_modifier(sSaveGame *sg) {
	char buf[BUFSIZ];
	uint16_t *arr = &sg->attr->fgt_skl;
	static const char *term[6] = {
		"戰鬥技術","攻擊能力","防禦能力","魔法技術","魔法能力","抗魔能力"
	};


	size_t selectidx = -1;
	uint16_t val = -1;
	while(1) {
		for(size_t i = 0; i < 6; i++) printf("%ld: %s(%hu)\n", i+1, term[i], arr[i]);
		printf("0: 離開\n");
		printf("請選擇欲修改之項目 (1-6), 輸入 0 離開: ");
		while(!fgets_n(buf, sizeof(buf), stdin));
		sscanf(buf, "%lu", &selectidx);
		if(selectidx == 0) return;
		if(selectidx > 4) break;

		while(1) {
			printf("請輸入欲修改之數值 (0-999): ");
			while(!fgets_n(buf, sizeof(buf), stdin));
			sscanf(buf, "%hu", &val);
			if(val > 999)  {
				system("clear");
				printf(CLR_RED "輸入錯誤，請重新輸入\n" CLR_RST);
				continue;
			}
			arr[selectidx-1] = val;
			printf("%s 已被修改為 %hu\n", term[selectidx-1], val);
			break;
		}
	}
}
void attr3_modifier(sSaveGame *sg) {
	char buf[BUFSIZ];
	uint16_t *arr = &sg->attr->decorum;
	static const char *term[6] = {
		"禮儀表現","藝術修養","談話技巧","烹飪技術","掃除洗衣","待人接物"
	};


	size_t selectidx = -1;
	uint16_t val = -1;
	while(1) {
		for(size_t i = 0; i < 6; i++) printf("%ld: %s(%hu)\n", i+1, term[i], arr[i]);
		printf("0: 離開\n");
		printf("請選擇欲修改之項目 (1-6), 輸入 0 離開: ");
		while(!fgets_n(buf, sizeof(buf), stdin));
		sscanf(buf, "%lu", &selectidx);
		if(selectidx == 0) return;
		if(selectidx > 4) break;

		while(1) {
			printf("請輸入欲修改之數值 (0-999): ");
			while(!fgets_n(buf, sizeof(buf), stdin));
			sscanf(buf, "%hu", &val);
			if(val > 999)  {
				system("clear");
				printf(CLR_RED "輸入錯誤，請重新輸入\n" CLR_RST);
				continue;
			}
			arr[selectidx-1] = val;
			printf("%s 已被修改為 %hu\n", term[selectidx-1], val);
			break;
		}
	}
}

void attr_modifier(sSaveGame *sg) {
	char buf[BUFSIZ];
	static const char *term[4] = {
		"一般","評價","戰鬥","家務表現"
	};
	void (*f[4])(sSaveGame *) = {
		attr0_modifier,
		attr1_modifier,
		attr2_modifier,
		attr3_modifier,
	};

	size_t selectidx = -1;
		while(1) {
		for(size_t i = 0; i < 4; i++) printf("%ld: %s\n", i+1, term[i]);
		printf("0: 離開\n");
		printf("請選擇欲修改之項目 (1-4), 輸入 0 離開: ");
		while(!fgets_n(buf, sizeof(buf), stdin));
		sscanf(buf, "%lu", &selectidx);
		if(selectidx == 0) return;
		if(selectidx > 4)  {
			system("clear");
			printf(CLR_RED "輸入錯誤，請重新輸入\n" CLR_RST);
			continue;
		}
		system("clear");
		f[selectidx-1](sg);
		system("clear");
	}
}

void healthy_modifier(sSaveGame *sg) {
	char buf[BUFSIZ];

	int16_t val =-1;
	while(1) {
		printf("當前疾病指數: %u\n", *sg->healthy);
		printf("請輸入欲修改之數值 (0-100)，輸入 -1 離開: ");
		while(!fgets_n(buf, sizeof(buf), stdin));
		sscanf(buf, "%hd", &val);
		if(val== -1) return;
		if(val > 100)  {
			system("clear");
			printf(CLR_RED "輸入錯誤，請重新輸入\n" CLR_RST);
			continue;
		}
		*sg->healthy = val;
		printf("疾病指數已被修改為 %hu\n", val);
	}
}

void money_modifier(sSaveGame *sg) {
	char buf[BUFSIZ];

	int32_t val =-1;
	while(1) {
		printf("當前金額: %u\n", *sg->money);
		printf("請輸入欲修改之金額 (0-%lld)，輸入 -1 離開: ", (unsigned long long)UINT32_MAX);
		while(!fgets_n(buf, sizeof(buf), stdin));
		sscanf(buf, "%u", &val);
		if(val > (unsigned long long)UINT32_MAX)  {
			system("clear");
			printf(CLR_RED "輸入錯誤，請重新輸入\n" CLR_RST);
			continue;
		}
		*sg->money = val;
		printf("金錢已被修改為 %hu\n", val);
	}
}

void item_modifier(sSaveGame *sg) {
	printf("全物品清單:\n");
	char s[BUFSIZ], buf[BUFSIZ], *cur = s;
	memset(s, 0, sizeof(s));

	sItem *items = itemlist;
	for(size_t i = 0; i < itemlist_cnt; ++i) {
		sprintf(cur, "%-4hu %4hu ", items[i].id, items[i].type);
		cur = strchr(cur, '\0');
		
		szhprint(buf, items[i].name, NULL);
		sprintf(cur, "%s ", buf);
		cur = strchr(cur, '\0');

		szhprint(buf, items[i].effect_en, NULL);
		sprintf(cur, "%s ", buf);
		cur = strchr(cur, '\0');
		
		szhprint(buf, items[i].effect_zh, NULL);
		sprintf(cur, "%s\n", buf);
		cur = strchr(cur, '\0');
	}
	printf("%s", s);
	printf("輸入 enter 以繼續...");
	fgets_n(buf, sizeof(buf), stdin);

	// int32_t val =-1;
	// while(1) {

	// 	printf("當前金額: %u\n", *sg->money);
	// 	printf("請輸入欲修改之金額 (0-%lld)，輸入 -1 離開: ", (unsigned long long)UINT32_MAX);
	// 	while(!fgets_n(buf, sizeof(buf), stdin));
	// 	sscanf(buf, "%u", &val);
	// 	if(val > (unsigned long long)UINT32_MAX)  {
	// 		system("clear");
	// 		printf(CLR_RED "輸入錯誤，請重新輸入\n" CLR_RST);
	// 		continue;
	// 	}
	// 	*sg->money = val;
	// 	printf("金錢已被修改為 %hu\n", val);
	// }
}

void modifier_menu(sSaveGame *sg) {
	char buf[BUFSIZ];
	static const char *term[4] = {
		"屬性","健康狀態","金錢","道具"
	};
	void (*f[4])(sSaveGame *) = {
		attr_modifier,
		healthy_modifier,
		money_modifier,
		item_modifier
	};

	size_t selectidx = -1;
	while(1) {
		for(size_t i = 0; i < 4; ++i) printf("%ld: %s\n", i+1, term[i]);
		printf("0: 離開\n");
		printf("請選擇欲修改之項目 (1-4), 輸入 0 離開: ");
		while(!fgets_n(buf, sizeof(buf), stdin));
		sscanf(buf, "%lu", &selectidx);
		if(selectidx == 0) return;
		if(selectidx > 4)  {
			system("clear");
			printf(CLR_RED "輸入錯誤，請重新輸入\n" CLR_RST);
			continue;
		}
		system("clear");
		f[selectidx-1](sg);
		system("clear");
	}
}

void start_menu() {
	char dirname[BUFSIZ], buf[BUFSIZ];
	printf("請輸入美少女夢工廠 2 所在的目錄: ");
	fgets_n(dirname, sizeof(dirname), stdin);

	DIR *d = opendir(dirname);
	if(!d) {
		printf("錯誤: 無法開啟該目錄\n");
		exit(1);
	}

	struct dirent *dir;
	buf[0] = '\0';
	while((dir = readdir(d)) != NULL) {
		for(char *end = dir->d_name; *end; end++) {
			if(isprint(*end)) continue;
			*end = '\0';
			break;
		}
		strncatf(buf, "%s ", sizeof(buf), dir->d_name);
		strcat(buf, " ");
	}
	closedir(d);

	char **ppFilelist;
	size_t listsize = mymatch(&ppFilelist, buf, "*.GNX");

	size_t selectidx = -1;
	while(1) {
		printf("位於 %s 目錄下的存檔:\n", dirname);
		for(size_t i = 0; i < listsize; ++i) {
			// printf("path: |%s/%s|\n", dirname, ppFilelist[i]);
			printf("%ld: %s\n", i+1, ppFilelist[i]);
		}
		printf("0: 離開\n");
		printf("請輸入欲修改之檔案 (1-%ld)，輸入 0 離開: ", listsize);
		while(!fgets_n(buf, sizeof(buf), stdin));
		sscanf(buf, "%ld", &selectidx);
		if(selectidx == 0) {
			for(size_t i = 0; i < listsize; ++i) {
				if(ppFilelist[i]) free(ppFilelist[i]);
			}
			free(ppFilelist);
			return;
		}
		if(selectidx > listsize) {
			system("clear");
			printf(CLR_RED "輸入錯誤，請重新輸入\n" CLR_RST);
			continue;
		}
		sprintf(buf, "%s/%s", dirname, ppFilelist[selectidx-1]);
		printf("open: %s\n", buf);
		sSaveGame *sg = new_savegame(buf);
		// printf("val: %hu\n", sg->attr->stamina);
		system("clear");
		modifier_menu(sg);
		system("clear");
		free_savegame(sg);
	}

		// if(res == 0) continue;
		// sprintf(filename, ppList[0], sizeof(filename));
		// sprintf(s, "%s/%s", dirname, ppList[0]);
		// printf("file: |%s|\n", s);
		
		// sMmap *mmp = new_mmap(s, MAP_SHARED);
		// if(!mmp) continue;
		
		// free_mmap(mmp);
}

int main() {
	// char s[BUFSIZ], buf[BUFSIZ], *cur = s;
	// memset(s, 0, sizeof(s));
	// freopen("in.txt", "r", stdin);

	// sItem *items = itemlist;
	// for(size_t i = 0; i < itemlist_cnt; ++i) {
	// 	sprintf(cur, "%-4hu %4hu ", items[i].id, items[i].type);
	// 	cur = strchr(cur, '\0');
		
	// 	szhprint(buf, items[i].name, NULL);
	// 	sprintf(cur, "%s ", buf);
	// 	cur = strchr(cur, '\0');

	// 	szhprint(buf, items[i].effect_en, NULL);
	// 	sprintf(cur, "%s ", buf);
	// 	cur = strchr(cur, '\0');
		
	// 	szhprint(buf, items[i].effect_zh, NULL);
	// 	sprintf(cur, "%s\n", buf);
	// 	cur = strchr(cur, '\0');
	// }
	// printf("%s", s);
	init_itemlist();
	start_menu();
	free_itemlist();
	return 0;
}