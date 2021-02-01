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
 * This version is limited to optimizations that do not compromise on redability.
 *
 * Optimization done:
 *  - heavy use of static and const (technically equivalent to "tip 12 - inlining everything by hand", without readability loss)
 *  - favor uint8_t to int when possible (tip 03 - Smallest possible unsigned data types)
 *  - gamestate initialization by return value (aesthetics, should not impact performance)
 *  - variables initialized in for loop (aesthetics, should not impact performance)
 *
 */

#include <stdint.h>

#define SCREEN_ADDR ((unsigned char*)0x300)
void dummy_function(void* a, unsigned char* b);

#define SCREEN_SIZE_X 40
#define NO_ENEMIES 30
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
    const uint8_t FIRST_DIGIT_CHAR = 0x10;
    unsigned char *draw_ptr = &screen_ptr[e_ptr->y * SCREEN_SIZE_X + e_ptr->x];
    *draw_ptr = get_entity_tile(e_ptr->type);
    *(++draw_ptr) = e_ptr->hp / 10 + FIRST_DIGIT_CHAR;
    *(++draw_ptr) = e_ptr->hp % 10 + FIRST_DIGIT_CHAR;
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
