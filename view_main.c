#include "units.h"
#include "Map.h"
#include "view.h"

int main()
{
    Unit attacker = {0};
    Unit defender = {0};
    Unit monsters[] = {attacker, defender};
    generate_monsters(monsters, 2, 1);

    print_unit(monsters + 1);
}
