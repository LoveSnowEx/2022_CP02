#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "madoka.h"

#define SJMAX 20
#define MHSJMAX 4
#define MJMAX 3

#define container_of(ptr, type, member)					\
	({								\
		const typeof(((type *)0)->member ) *__mptr = (ptr);	\
		(type *)((char *)__mptr - offsetof(type,member));	\
	})

#define new(TYPE, args...) TYPE ## _ctor(malloc(sizeof(TYPE)), ## args)
#define delete(TYPE, ptr)	do				\
				{				\
					TYPE ## _dtor(ptr);	\
					free(ptr);		\
				}				\
				while(0)

int energy = 0;

int main(void)
{
	
    int r = 0;
    int sj_cnt = SJMAX - MHSJMAX - MJMAX;
    Shoujo *sj_list[SJMAX] = {0};
    Mahoushoujo *mhsj_list[MHSJMAX] = {0};
    Majo *mj_list[MJMAX] = {0};

    Mahoushoujo *homura = mhsj_list[1] = new(Mahoushoujo, "Homura", "Madoka..", Homura_skill);
    Mahoushoujo *sayako = mhsj_list[2] = new(Mahoushoujo, "Sayaka", "QQ", Sayaka_skill);
    Mahoushoujo *kyoko = mhsj_list[3] = new(Mahoushoujo, "Kyoko", "QQ", Kyoko_skill);

    mj_list[0] = new(Majo, "No name", "Majo is hopeless.");

    sj_list[0] = new(Shoujo, "Madoka", "OAO");

    for (int i = 1; i < MHSJMAX; ++i)
        sj_list[i] = mhsj_list[i];

    sj_list[MHSJMAX] = mj_list[0];
    for (int i = MHSJMAX + MJMAX; i < SJMAX; ++i)
        sj_list[i] = new(Shoujo, "shoujo", "shoujo is blessing.");

    while (true)
    {
        printf("--------------------------\n");
        printf("energy: %d\n", energy);
        printf("--------------\n");
        for (int i = 0; i < SJMAX; ++i)
        {
            if (sj_list[i])
            {
                printf("%s: ", sj_list[i]->name);
                sj_list[i]->do_wish(sj_list[i]);
            }
        }

        for (int i = 0; i < MHSJMAX; ++i)
        {
            if (mhsj_list[i])
            {
                for (int o = 0; o < MJMAX; ++o)
                {
                    if (mj_list[o])
                    {
                        mhsj_list[i]->skill(mhsj_list[i], mj_list[o]);
                        mhsj_list[i]->attack(mhsj_list[i], mj_list[o]);
                    }
                }
            }
        }

        for (int i = 0; i < MJMAX; ++i)
        {
            if (!mj_list[i])
                continue;

            do {
                r = rand() % (MHSJMAX - 1) + 1;
            } while (!mhsj_list[r]);
            mj_list[0]->attack(mj_list[i], mhsj_list[r]);

            if (sj_cnt == 0)
                continue;
            do {
                r = rand() % (SJMAX - MJMAX - MHSJMAX) + MJMAX + MHSJMAX;
            } while (!sj_list[r]);
            mj_list[0]->kekkai(mj_list[i], sj_list[r]);
        }

        for (int i = 0; i < SJMAX; ++i)
        {
            if (!sj_list[i])
                continue;
            if (sj_list[i]->is_despair(sj_list[i]))
            {
                sj_list[i]->despair(sj_list[i]);
                if (i == 2)
                {
                    mhsj_list[2] = NULL;
                    mj_list[1] = sj_list[2] = (void *)mhsj_to_mj((Mahoushoujo *)sj_list[2]);
	                energy += 10000;
                }
            }
            if (sj_list[i]->is_dead(sj_list[i]))
            {
                delete(Shoujo, sj_list[i]);
                sj_list[i] = NULL;
                if (i < MHSJMAX)
                {
                    mhsj_list[i] = NULL;
                    if (i == 2)
                        sj_list[2 + MHSJMAX] = mj_list[2] = new(Majo, "Nightmare", "Hahaha.");
                }
                else if (i < MHSJMAX + MJMAX)
                {
                    mj_list[i - MHSJMAX] = NULL;
                }
                else
                    sj_cnt--;
            }
        }
        if (((Shoujo *)homura)->kimoji <= -80)
        {
            Mahoushoujo *homura = mhsj_list[0] = new(Mahoushoujo, "Madoka", "Mo daijobu daiyo", Madoka_skill);
        }
    }
	
    return 0;
}