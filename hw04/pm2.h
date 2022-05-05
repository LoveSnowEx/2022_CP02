#pragma once

#include "basic.h"
#include "mmap.h"

#define ITEMCAP (50U);

size_t offset_date_year = 0x0032; // ~0033
size_t offset_date_month = 0x0034; // ~0035
size_t offset_date_dayofweek = 0x0036; // ~0037
size_t offset_date_day = 0x0038; // ~0039

// 003a ~ 003d ??

size_t offset_atttr0 = 0x003e;  // ~ 0051
size_t offset_atttr1 = 0x0054;  // ~ 005B
size_t offset_atttr2 = 0x005c;  // ~ 0067
size_t offset_atttr3 = 0x0068;  // ~ 0073

size_t offset_money = 0x00b0; // ~ 00b3
size_t offset_age = 0x00ba;  // ~ 00bb
size_t offset_birth_day = 0x00be;  // ~ 00bf
size_t offset_birth_month = 0x00c2;  // ~ 00c5
size_t offset_birth_year = 0x00c6;  // ~ 00c7
size_t offset_body = 0x00d2;  // ~ 00db
size_t offset_healthy = 0x00ea;  //  ~00eb
size_t offset_bad = 0x00ec;  //  ~00ed
size_t offset_personal = 0x00ee;  //  ~00ef
// personal detail: f0 ff
size_t offset_le = 0x0118; // 
size_t offset_relation = 0x025c; //
size_t offset_equipment = 0x043a;  // 
size_t offset_storage = 0x0580;  // +5504

typedef struct _sAttribute {
	// block 0
	uint16_t stamina;
	uint16_t strnth;
	uint16_t intell;
	uint16_t elegance;
	uint16_t glamour;
	uint16_t morality;
	uint16_t faith;
	uint16_t karma;
	uint16_t senstv;
	uint16_t stress;
	uint16_t reserve;  // not sure
	// block 1
	uint16_t fgt_rep;
	uint16_t nag_rep;
	uint16_t scl_rep;
	uint16_t hkp_rep;
	// block 2
	uint16_t fgt_skl;
	uint16_t attack;
	uint16_t defense;
	uint16_t mag_skl;
	uint16_t mag_atk;
	uint16_t mag_def;
	// block 3
	uint16_t decorum;
	uint16_t art_skl;
	uint16_t speech;
	uint16_t cooking;
	uint16_t cln_ldr;
	uint16_t personal;
} sAttribute;

typedef struct _sItem {
	uint16_t id;
	uint16_t cost;
	// armor 1000(E8 03), weapon 2000(D0 07)
	// cloth 3000(B8 0B), accessory 4000(A0 0F)
	uint16_t type;  // if euqipped -> + 1
	uint16_t reserve;
	char name[16];
	uint16_t loc_point; // 0x0040 "@ " 
	char effect_en[66];
	char effect_zh[18];
} sItem;  // size 110 Byte

typedef struct _sItemStorage {
	uint16_t item_cnt;
	uint16_t item_cap;
	sItem *items;
} sItemStorage;


typedef struct _sSaveGame {
	sMmap *mmp;
	sAttribute *attr;
	uint16_t *healthy;
	uint32_t *money;
	sItemStorage *storage;
} sSaveGame;

sSaveGame *new_savegame(const char *filename) {
	sMmap *mmp = new_mmap(filename, MAP_SHARED);
	if(!mmp) {
		fprintf(stderr, "error: failed to open save game");
		return NULL;
	}
	sSaveGame *sav = malloc(sizeof(sSaveGame));
	sav->mmp = mmp;
	sav->attr = (sAttribute*)(mmp->addr+offset_atttr0);
	sav->healthy = (uint16_t*)(mmp->addr+offset_healthy);
	sav->money = (uint32_t*)(mmp->addr+offset_money);
	sav->storage = (sItemStorage*)(mmp->addr+offset_storage);
	return sav;
}

int free_savegame(sSaveGame *sav) {
	if(free_mmap(sav->mmp) == -1) {
		return -1;
	}
	free(sav);
	return 0;
}

sMmap *mmp_itemlist = NULL;
sItem *itemlist = NULL;
i32 itemlist_cnt = 0;

void init_itemlist() {
	mmp_itemlist = new_mmap("ITEM.LBX", MAP_SHARED);
	itemlist = (sItem*)mmp_itemlist->addr;
	itemlist_cnt = mmp_itemlist->sb.st_size/sizeof(sItem);
}

void free_itemlist() {
	free_mmap(mmp_itemlist);
}