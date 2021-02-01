/*
 * ilmenit's sample benchmark in the "how to optimize for cc65" tutorial.
 *
 * original: https://github.com/ilmenit/CC65-Advanced-Optimizations/blob/master/01-start/game.c
 * (first step, without optimization)
 *
 * Changes from original:
 *  - Benchmark framework replaced by a call to a dummy routine (the emulator does the bench in our case)
 *  - atari.h dependency replaced by a constant to a memory area for the screen
 *  - Removed useless semicolon after functions
 *  - Less unused space on screen (screen entirely fits 2 pages)
 *
 * This is the first unoptimized example on which we added optimizations with modern compilers in mind.
 * This version is fully optimized, sacrificing readability where necessary.
 *
 * Optimization done:
 *  - heavy use of static and const (technically equivalent to "tip 12 - inlining everything by hand", without readability loss)
 *  - favor uint8_t to int when possible (tip 03 - Smallest possible unsigned data types)
 *  - gamestate initialization by return value (aesthetics, should not impact performance)
 *  - variables initialized in for loop (aesthetics, should not impact performance)
 *  - lookup tables (tip 09 - Replace calculations, switches, and screen access by using Lookup Tables)
 *
 */

#include <stdint.h>

#define SCREEN_ADDR ((uint8_t*)0x300)
void dummy_function(void* a, unsigned char* b);

#define SCREEN_SIZE_X 40
#define NO_ENEMIES 30
#define FIRST_DIGIT_CHAR 0x10
#define _countof(array) (sizeof(array) / sizeof(array[0]))

typedef enum e_entity_type {
    ENTITY_DEAD,
    ENTITY_PLAYER,
    ENTITY_ENEMY
} e_entity_type;

typedef struct s_entity {
    uint8_t x;
    uint8_t y;
    uint8_t hp;
    e_entity_type type;
} s_entity;

typedef struct s_player {
    s_entity entity;
    uint8_t attack;
} s_player;


typedef struct s_game_state {
    s_entity enemies[NO_ENEMIES];
    s_player player;
} s_game_state;

/*
static uint8_t d10(uint8_t index) {
    const uint8_t FIRST_DIGIT_CHAR = 0x10;
	return index / 10 + FIRST_DIGIT_CHAR;
}
*/

#define d(index) index / 10 + FIRST_DIGIT_CHAR
static uint8_t const div_10_lookup[] = {
	d(0), d(1), d(2), d(3), d(4), d(5), d(6), d(7), d(8), d(9),
	d(10), d(11), d(12), d(13), d(14), d(15), d(16), d(17), d(18), d(19),
	d(20), d(21), d(22), d(23), d(24), d(25), d(26), d(27), d(28), d(29),
	d(30), d(31), d(32), d(33), d(34), d(35), d(36), d(37), d(38), d(39),
	d(40), d(41), d(42), d(43), d(44), d(45), d(46), d(47), d(48), d(49),
	d(50), d(51), d(52), d(53), d(54), d(55), d(56), d(57), d(58), d(59),
	d(60), d(61), d(62), d(63), d(64), d(65), d(66), d(67), d(68), d(69),
	d(70), d(71), d(72), d(73), d(74), d(75), d(76), d(77), d(78), d(79),
	d(80), d(81), d(82), d(83), d(84), d(85), d(86), d(87), d(88), d(89),
	d(90), d(91), d(92), d(93), d(94), d(95), d(96), d(97), d(98), d(99),
};
#undef d
#define d(index) index % 10 + FIRST_DIGIT_CHAR
static uint8_t const mod_10_lookup[] = {
	d(0), d(1), d(2), d(3), d(4), d(5), d(6), d(7), d(8), d(9),
	d(10), d(11), d(12), d(13), d(14), d(15), d(16), d(17), d(18), d(19),
	d(20), d(21), d(22), d(23), d(24), d(25), d(26), d(27), d(28), d(29),
	d(30), d(31), d(32), d(33), d(34), d(35), d(36), d(37), d(38), d(39),
	d(40), d(41), d(42), d(43), d(44), d(45), d(46), d(47), d(48), d(49),
	d(50), d(51), d(52), d(53), d(54), d(55), d(56), d(57), d(58), d(59),
	d(60), d(61), d(62), d(63), d(64), d(65), d(66), d(67), d(68), d(69),
	d(70), d(71), d(72), d(73), d(74), d(75), d(76), d(77), d(78), d(79),
	d(80), d(81), d(82), d(83), d(84), d(85), d(86), d(87), d(88), d(89),
	d(90), d(91), d(92), d(93), d(94), d(95), d(96), d(97), d(98), d(99),
};
#undef d
#define d(index) SCREEN_ADDR + (index * SCREEN_SIZE_X)
static uint8_t* const screen_line_lookup[] = {
	d(0), d(1), d(2), d(3), d(4), d(5), d(6), d(7),
	d(8), d(9), d(10), d(11), d(12), d(13), d(14), d(15),
	d(16), d(17), d(18), d(19), d(20), d(21), d(22), d(23),
};
#undef d

static void place_enemy(s_entity *e_ptr, uint8_t x, uint8_t y)
{
    e_ptr->x = x;
    e_ptr->y = y;
}

static s_game_state new_game_state()
{
	s_game_state game_state;
    // set enemies
    for (uint8_t index = 0; index < _countof(game_state.enemies); index++)
    {
        s_entity* const e = &game_state.enemies[index];
        place_enemy(e, (index*5) % SCREEN_SIZE_X, index / 5 + 1);
        e->hp = 99;
        e->type = ENTITY_ENEMY;
    };
    // set player
    game_state.player.entity.hp = 99;
    game_state.player.entity.x = SCREEN_SIZE_X/2;
    game_state.player.entity.type = ENTITY_PLAYER;

	return game_state;
}

static char get_entity_tile(e_entity_type type)
{
    switch(type)
    {
        case ENTITY_PLAYER:
            return 'p';
        case ENTITY_ENEMY:
            return 'e';
    }
    return 'x';
}

static void draw_entity(unsigned char *screen_ptr, s_entity *e_ptr)
{
	uint8_t *draw_ptr = screen_line_lookup[e_ptr->y] + e_ptr->x;
    *draw_ptr = get_entity_tile(e_ptr->type);
    *(++draw_ptr) = div_10_lookup[e_ptr->hp];
    *(++draw_ptr) = mod_10_lookup[e_ptr->hp];
}

static void damage_enemy(s_entity *e_ptr)
{
    // damage
    if (e_ptr->hp > 0)
        e_ptr->hp--;
}

static void one_frame(s_game_state *game_state, unsigned char *screen_ptr)
{
    // draw enemies
    for (uint8_t index = 0; index < _countof(game_state->enemies); index++)
    {
        s_entity* const e = &game_state->enemies[index];
        damage_enemy(e);
        draw_entity(screen_ptr, e);
    };
    // draw player
    draw_entity(screen_ptr, &game_state->player.entity);

}

void benched_routine(void)
{
    unsigned char *screen_ptr;

    s_game_state game_state = new_game_state();

    screen_ptr = SCREEN_ADDR;

    for (uint8_t times = 0; times < 100; ++times) {
        one_frame(&game_state, screen_ptr);

		// Call an extern function so the compiler cannot assume intermediary states to be unused
		dummy_function(&game_state, screen_ptr);
	}
}
