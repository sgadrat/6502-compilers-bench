#include <stdint.h>

//
// Your typical RPG foo
//

typedef struct {
	uint8_t pv;
	uint8_t attack;
} Monster;

static void monster_init(Monster* monster) {
	monster->pv = 10;
	monster->attack = 2;
}

static void monster_take_hit(Monster* monster, uint8_t attack) {
	if (monster->pv > attack) {
		monster->pv -= attack;
	}else {
		monster->pv = 0;
	}
}

//
// A hero wielding his weapon
//

typedef struct {
	uint8_t attack;
	uint8_t durability;
} Weapon;

typedef struct {
	uint8_t pv;
	uint8_t mana;
	Weapon weapon;
} Hero;

static void hero_init(Hero* hero);
static void hero_change_weapon(Hero* hero, Weapon new_weapon);
static void hero_hit_monster(Hero* hero, Monster* monster);

static const Weapon fist = {.attack = 1, .durability = 255};
static const Weapon sword = {.attack = 3, .durability = 10};

static void hero_init(Hero* hero) {
	hero->pv = 10;
	hero->mana = 10;
	hero_change_weapon(hero, fist);
}

static void hero_change_weapon(Hero* hero, Weapon new_weapon) {
	hero->weapon = new_weapon;
}

static void hero_hit_monster(Hero* hero, Monster* monster) {
	monster_take_hit(monster, hero->weapon.attack);
	--hero->weapon.durability;
	if (hero->weapon.durability == 0) {
		hero_change_weapon(hero, fist);
	}
}

//
// Benched routine: initialize things and begin a fight!
//

void benched_routine() {
	// Game state:
	//  hero is in zero page,
	//  There is 3 monsters at the begining of page 4xx
	Hero* hero = (Hero*)0x0080;
	Monster* monsters = (Monster*)0x0400;
	const uint8_t NB_MONSTERS = 3;

	// Initialize gamestate
	hero_init(hero);
	hero_change_weapon(hero, sword);

	for (uint8_t monster_num = 0; monster_num < NB_MONSTERS; ++monster_num) {
		monster_init(&monsters[monster_num]);
	}

	// Fight!
	hero_hit_monster(hero, monsters + 1);
}
