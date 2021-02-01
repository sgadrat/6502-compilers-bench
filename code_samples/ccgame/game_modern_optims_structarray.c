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
 * This is the first onoptimized example on which we added big optimizations with modern compilers in mind
 *
 * Optimization done:
 *  - heavy use of static and const (technically equivalent to "tip 12 - inlining everything by hand", without readability loss)
 *  - favor uint8_t to int when possible (tip 03 - Smallest possible unsigned data types)
 *  - gamestate initialization by return value (aesthetics, should not impact performance)
 *  - variables initialized in for loop (aesthetics, should not impact performance)
 *
 *  - structs of array (tip 05 - Replace "array of structs" to "struct of arrays")
 *    - Wast cycles with gcc
 *  - (not done) lookup tables (tip 09 - Replace calculations, switches, and screen access by using Lookup Tables)
 *
 */

#include <stdint.h>

#define SCREEN_ADDR ((unsigned char*)0x300)
void dummy_function(void* a, unsigned char* b);

#define SCREEN_SIZE_X 40
#define NO_ENEMIES 30
#define NO_ENTITIES (NO_ENEMIES+1)
#define PLAYER_INDEX (NO_ENEMIES)
#define _countof(array) (sizeof(array) / sizeof(array[0]))

typedef enum e_entity_type {
    ENTITY_DEAD,
    ENTITY_PLAYER,
    ENTITY_ENEMY
} e_entity_type;

typedef struct s_entity {
    uint8_t x[NO_ENTITIES];
    uint8_t y[NO_ENTITIES];
    uint8_t hp[NO_ENTITIES];
    e_entity_type type[NO_ENTITIES];
} s_entity;

typedef struct s_player {
    uint8_t attack;
} s_player;


typedef struct s_game_state {
    s_entity entities;
    s_player player;
} s_game_state;

static void place_enemy(s_game_state* game_state, uint8_t entity, uint8_t x, uint8_t y)
{
	game_state->entities.x[entity] = x;
	game_state->entities.y[entity] = y;
}

static s_game_state new_game_state()
{
	s_game_state game_state;
    // set enemies
    for (uint8_t index = 0; index < NO_ENEMIES; index++)
    {
        place_enemy(&game_state, index, (index*5) % SCREEN_SIZE_X, index / 5 + 1);
        game_state.entities.hp[index] = 99;
        game_state.entities.type[index] = ENTITY_ENEMY;
    };
    // set player
    game_state.entities.hp[PLAYER_INDEX] = 99;
    game_state.entities.x[PLAYER_INDEX] = SCREEN_SIZE_X/2;
    game_state.entities.type[PLAYER_INDEX] = ENTITY_PLAYER;

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

static void draw_entity(s_game_state const* game_state, unsigned char *screen_ptr, uint8_t entity)
{
    const uint8_t FIRST_DIGIT_CHAR = 0x10;
    unsigned char *draw_ptr = &screen_ptr[game_state->entities.y[entity] * SCREEN_SIZE_X + game_state->entities.x[entity]];
    *draw_ptr = get_entity_tile(game_state->entities.type[entity]);
    *(++draw_ptr) = game_state->entities.hp[entity] / 10 + FIRST_DIGIT_CHAR;
    *(++draw_ptr) = game_state->entities.hp[entity] % 10 + FIRST_DIGIT_CHAR;
}

static void damage_enemy(s_game_state* game_state, uint8_t entity)
{
    // damage
    if (game_state->entities.hp[entity] > 0)
        game_state->entities.hp[entity]--;
}

static void one_frame(s_game_state *game_state, unsigned char *screen_ptr)
{
    // draw enemies
    for (uint8_t index = 0; index < NO_ENEMIES; index++)
    {
        damage_enemy(game_state, index);
        draw_entity(game_state, screen_ptr, index);
    };
    // draw player
    draw_entity(game_state, screen_ptr, PLAYER_INDEX);

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
