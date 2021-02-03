/*
 * ilmenit's sample benchmark in the "how to optimize for cc65" tutorial.
 *
 * original: https://github.com/ilmenit/CC65-Advanced-Optimizations/blob/master/12-inline%20functions/game.c
 * (fully optimized version)
 *
 * Changes from original:
 *  - Benchmark framework replaced by a call to a dummy routine (the emulator does the bench in our case)
 *  - atari.h dependency replaced by a constant to a memory area for the screen
 *  - Removed useless semicolon after functions
 *  - Less unused space on screen (screen entirely fits 2 pages)
 *  - get_entity_tile is const (puts it in segment RODATA instead of DATA)
 *
 */

//Note: 4xx instead of 3xx in others, to let some space for big global lookup tables
#define SCREEN_ADDR ((unsigned char*)0x400)
void dummy_function(void* a, unsigned char* b);

#define SCREEN_SIZE_X 40
#define SCREEN_SIZE_Y 24
#define NO_ENEMIES 30
#define NO_ENTITIES (NO_ENEMIES+1)
#define PLAYER_INDEX (NO_ENEMIES)
#define MAX_LOOKUP_VALUE 100
#define FIRST_DIGIT_CHAR 0x10

#define ENTITY_DEAD 0
#define ENTITY_PLAYER 1
#define ENTITY_ENEMY 2
typedef unsigned char e_entity_type;

// +1 for player
typedef struct s_entity {
    unsigned char x[NO_ENTITIES];
    unsigned char y[NO_ENTITIES];
    unsigned char hp[NO_ENTITIES];
    e_entity_type type[NO_ENTITIES];
} s_entity;

typedef struct s_player {
    unsigned char attack;
} s_player;


typedef struct s_game_state {
    s_entity entities;
    s_player player;
} s_game_state;

// ZP data

#pragma bss-name (push,"ZEROPAGE")
#pragma data-name (push,"ZEROPAGE")
unsigned char index1;
unsigned char calc1;
unsigned char calc2;
unsigned char calc3;
unsigned char calc4;
#pragma bss-name (pop)
#pragma data-name (pop)
#pragma zpsym ("index1");
#pragma zpsym ("calc1");
#pragma zpsym ("calc2");
#pragma zpsym ("calc3");
#pragma zpsym ("calc4");

/// Data
s_game_state game_state;


// Lookup tables
char const get_entity_tile[] = {
    'x', 'p', 'e'
};

unsigned char div_10_lookup[MAX_LOOKUP_VALUE];
unsigned char mod_10_lookup[MAX_LOOKUP_VALUE];
unsigned char *screen_line_lookup[SCREEN_SIZE_Y];

void place_enemy(unsigned char x, unsigned char y)
{
    game_state.entities.x[index1] = x;
    game_state.entities.y[index1] = y;
}

void set_entities()
{
    // set entities
    for (index1 = 0; index1 < NO_ENEMIES; index1++)
    {
        place_enemy((index1*5) % SCREEN_SIZE_X, index1 / 5 + 1);
        game_state.entities.hp[index1] = 99;
        game_state.entities.type[index1] = ENTITY_ENEMY;
    }
    // set player
    game_state.entities.hp[PLAYER_INDEX] = 99;
    game_state.entities.x[PLAYER_INDEX] = SCREEN_SIZE_X/2;
    game_state.entities.type[PLAYER_INDEX] = ENTITY_PLAYER;
}

void one_frame()
{
    register unsigned char *draw_ptr;
    for (index1 = 0; index1 < NO_ENEMIES; ++index1)
    {
        // inlined damage_enemy
        calc1 = game_state.entities.hp[index1];
        if (calc1 > 0)
        {
            --calc1;
            game_state.entities.hp[index1] = calc1;
        }
    }

    for (index1 = 0; index1 < NO_ENTITIES; ++index1)
    {
        // inlined drawing of entities
        calc1 = game_state.entities.y[index1];
        draw_ptr = screen_line_lookup[calc1];
        draw_ptr += game_state.entities.x[index1];
        calc1 = game_state.entities.type[index1];
        calc2 = get_entity_tile[calc1];
        calc1 = game_state.entities.hp[index1];
        calc3 = div_10_lookup[calc1];
        calc4 = mod_10_lookup[calc1];
        draw_ptr[0] = calc2;
        draw_ptr[1] = calc3;
        draw_ptr[2] = calc4;
    }
}

void init_lookup_tables()
{
    unsigned char *screen_ptr = SCREEN_ADDR;
    // init screen lookup
    for (index1 = 0; index1 < SCREEN_SIZE_Y; ++index1)
        screen_line_lookup[index1] = &screen_ptr[index1 * SCREEN_SIZE_X];

    for (index1 = 0; index1 < MAX_LOOKUP_VALUE; ++index1)
    {
        div_10_lookup[index1] = index1/10 + FIRST_DIGIT_CHAR;
        mod_10_lookup[index1] = index1%10 + FIRST_DIGIT_CHAR;
    }
}


void benched_routine(void)
{
    unsigned char times;

    init_lookup_tables();
    set_entities();

    for (times = 0; times < 100; ++times) {
        one_frame();
		dummy_function(&game_state, SCREEN_ADDR);
	}
}
