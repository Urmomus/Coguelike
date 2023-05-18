#include "units.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


ExceptionStatus generate_goblin(Unit *goblin, int level)
{
    srand(time(0));

    goblin->hp = rand() % level + level;
    goblin->dmg = level;
    goblin->defense = rand() % (level / 2);

    return OK;
}

int main()
{
    Unit *attacker = malloc(sizeof(Unit));
    Unit *defender = malloc(sizeof(Unit));

    generate_goblin(attacker, 10);
    generate_goblin(defender, 10);

    printf("%d - attacker damage\n%d - defender hp\n", attacker->dmg, defender->hp);

    attack(attacker, defender);

    printf("%d - attacker damage\n%d - defender hp\n", attacker->dmg, defender->hp);

    return 0;
}
