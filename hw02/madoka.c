#include "basic.h"
#include "madoka.h"

#define ENTITY_HP 100
#define SHOUJO_HP (ENTITY_HP)
#define MAHOUSHOUJO_HP (SHOUJO_HP*3)
#define MAJO_HP (SHOUJO_HP*50)
#define MAHOUSHOUJO_ATK 100
#define MAJO_ATK 10


Entity *Entity_ctor(Entity *this) {
	this->hp = ENTITY_HP;
	this->is_dead = Entity_is_dead;
	return this;
}

void Entity_dtor(__attribute__((unused)) Entity *this) {
	return;
}

int Entity_is_dead(void *this) {
	return ((Entity*)this)->hp == 0;
}


Shoujo *Shoujo_ctor(Shoujo *this, const char *name, const char *wish) {
	Entity_ctor((Entity*)this);
	((Entity*)this)->hp = SHOUJO_HP;
	this->name = strdup(name);
	this->wish = strdup(wish);
	this->kimoji = 100;
	this->is_dead = Entity_is_dead;
	this->is_despair = Shoujo_is_despair;
	this->do_wish = Shoujo_do_wish;
	this->despair = Shoujo_despair;
	return this;
}

void Shoujo_dtor(Shoujo *this) {
	Entity_dtor((Entity*)this);
	free(this->name);
	free(this->wish);
}

int Shoujo_is_despair(void *this) {
	return ((Shoujo*)this)->kimoji <= -100;
}

void Shoujo_do_wish(void *this) {
	printf("%s\n", ((Shoujo*)this)->wish);
}

void Shoujo_despair(void *this) {
	((Entity*)this)->hp = 0;
}


Mahoushoujo *Mahoushoujo_ctor(Mahoushoujo *this, const char *name, const char *wish, Skill skill) {
	Shoujo_ctor((Shoujo*)this, name, wish);
	((Entity*)this)->hp = MAHOUSHOUJO_HP;
	((Shoujo*)this)->do_wish = Mahoushoujo_do_wish;
	((Shoujo*)this)->despair = Mahoushoujo_despair;
	this->atk = MAHOUSHOUJO_ATK;
	this->is_dead = Entity_is_dead;
	this->do_wish = Mahoushoujo_do_wish;
	this->attack = Mahoushoujo_attack;
	this->skill = skill;
	return this;
}

void Mahoushoujo_dtor(Mahoushoujo *this) {
	Shoujo_dtor((Shoujo*)this);
}

void Mahoushoujo_do_wish(void *this) {
	printf("%s\nBut nothing is good.\n", ((Shoujo*)this)->wish);
	((Shoujo*)this)->kimoji -= 10;
}

void Mahoushoujo_attack(Mahoushoujo *this, Entity *enemy) {
	((Entity*)enemy)->hp = fmax(0, ((Entity*)enemy)->hp - this->atk);
}

void Mahoushoujo_despair(__attribute__((unused)) void *this) {
	printf("Watashii de, hondo baga.\n");
}


Majo *Majo_ctor(Majo *this, const char *name, const char *wish) {
	Shoujo_ctor((Shoujo*)this, name, wish);
	((Entity*)this)->hp = MAJO_HP;
	((Shoujo*)this)->despair = Majo_despair;
	this->atk = MAJO_ATK;
	this->is_dead = Entity_is_dead;
	this->attack = Majo_attack;
	this->kekkai = Majo_kekkai;
	return this;
}

void Majo_dtor(Majo *this) {
	Shoujo_dtor((Shoujo*)this);
}

void Majo_attack(Majo *this, Entity *enemy) {
	((Entity*)enemy)->hp = fmax(0, ((Entity*)enemy)->hp - this->atk);
}

void Majo_kekkai(__attribute__((unused)) Majo *this, Shoujo *sj) {
	sj->kimoji -= 100;
}

void Majo_despair(__attribute__((unused)) void *this) {
	return;
}


Majo *mhsj_to_mj(Mahoushoujo *mhsj) {
	Majo *mj = malloc(sizeof(Majo));
	Majo_ctor(mj, ((Shoujo*)mhsj)->name, ((Shoujo*)mhsj)->wish);
	Mahoushoujo_dtor(mhsj);
	free(mhsj);
	return mj;
}


void Madoka_skill(__attribute__((unused)) void *this, __attribute__((unused)) void *target) {
	printf("Madoka become god, end.\n");
	exit(0);
}

void Homura_skill(void *this, __attribute__((unused)) void *target) {
	if(((Entity*)this)->hp >= 50) return;

	printf("This round is hopeless, Homura go to next round.\n");
	exit(0);
}

void Sayaka_skill(void *this, __attribute__((unused)) void *target) {
	((Entity*)this)->hp += 30;
	((Shoujo*)this)->kimoji -= 30;
}

void Kyoko_skill(void *this, void *target) {
	if(strcmp(((Shoujo*)target)->name, "Sayaka") != 0) return;
	
	((Entity*)this)->hp = ((Entity*)target)->hp = 0;
}
