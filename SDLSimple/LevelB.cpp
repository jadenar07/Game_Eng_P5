#include "LevelB.h"
#include "Utility.h"

#define LEVEL_WIDTH 14
#define LEVEL_HEIGHT 8
#define FIXED_TIMESTEP 0.0166666f

constexpr char SPRITESHEET_FILEPATH[] = "/Users/jadenritchie/Desktop/SDLSimple/SDLSimple/assets/215080.png",
           ENEMY_FILEPATH[]       = "/Users/jadenritchie/Desktop/SDLSimple/SDLSimple/assets/kira.png";

bool game_loss_b = false;


unsigned int LEVELB_DATA[] =
{
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 2,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 2, 2,
    3, 1, 1, 1, 1, 1, 1, 0, 1, 2, 2, 2, 2, 2,
    3, 2, 2, 2, 2, 2, 2, 0, 2, 2, 2, 2, 2, 2
};

LevelB::~LevelB()
{
    delete [] m_game_state.enemies;
    delete    m_game_state.player;
    delete    m_game_state.map;
    Mix_FreeChunk(m_game_state.jump_sfx);
    Mix_FreeMusic(m_game_state.bgm);
}

void LevelB::initialise()
{
    m_game_state.next_scene_id = -1;
    
    GLuint map_texture_id = Utility::load_texture("/Users/jadenritchie/Desktop/SDLSimple/SDLSimple/brick_medium_4.png");
    m_game_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVELB_DATA, map_texture_id, 1.0f, 4, 1);
    
    // Code from main.cpp's initialise()
    /**
     George's Stuff
     */
    // Existing
    int player_walking_animation[4][4] =
    {
        { 8, 9, 10, 11 },  // left
                { 5, 4, 7, 6 },    // right
                { 13, 12, 13, 14 },// up
                { 1, 2, 3, 0 }
    };

    glm::vec3 acceleration = glm::vec3(0.0f, -4.81f, 0.0f);
    
    GLuint player_texture_id = Utility::load_texture(SPRITESHEET_FILEPATH);
    
    m_game_state.player = new Entity(
        player_texture_id,         // texture id
        5.0f,                      // speed
        acceleration,              // acceleration
        5.0f,                      // jumping power
        player_walking_animation,  // animation index sets
        0.0f,                      // animation time
        4,                         // animation frame amount
        0,                         // current animation index
        4,                         // animation column amount
        4,                         // animation row amount
        1.0f,                      // width
        1.0f,                       // height
        PLAYER
    );
        
    m_game_state.player->set_position(glm::vec3(5.0f, 0.0f, 0.0f));
    initial_player_position = glm::vec3(3.0f, 0.0f, 0.0f);


    // Jumping
    m_game_state.player->set_jumping_power(5.0f);
    
    /**
    Enemies' stuff */
    GLuint enemy_texture_id = Utility::load_texture(ENEMY_FILEPATH);

    m_game_state.enemies = new Entity[ENEMY_COUNT];

    for (int i = 0; i < ENEMY_COUNT; i++)
    {
    m_game_state.enemies[i] =  Entity(enemy_texture_id, 1.0f, 1.0f, 1.0f, ENEMY, GUARD, IDLE);
    }


    m_game_state.enemies[0].set_position(glm::vec3(6.0f, 0.0f, 0.0f));
    m_game_state.enemies[0].set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));
    m_game_state.enemies[0].set_jumping_power(5.0f);
    

    
    m_game_state.jump_sfx = Mix_LoadWAV("/Users/jadenritchie/Desktop/SDLSimple/SDLSimple/assets/madara.png");
}

void LevelB::update(float delta_time)
{
    m_game_state.player->update(delta_time, m_game_state.player, m_game_state.enemies, ENEMY_COUNT, m_game_state.map);
    static float jump_timer = 0.0f;
    jump_timer += FIXED_TIMESTEP;
    const float jump_interval = 2.0f;
    
    
    for (int i = 0; i < ENEMY_COUNT; i++) {
        if (m_game_state.player->check_collision(&m_game_state.enemies[i])) {
            m_game_state.player->check_collision_x(&m_game_state.enemies[i], 1);
            m_game_state.player->check_collision_y(&m_game_state.enemies[i], 1);

            if (lives > 0) {
                lives--;
                std::cout << "inside get lives" << std::endl;
                m_game_state.player->set_position(initial_player_position); 
            } else {
                game_loss_b = true;
            }

            return;
        }

        if (m_game_state.enemies[i].get_is_active()) {
            m_game_state.enemies[i].update(FIXED_TIMESTEP, m_game_state.player, NULL, 0, m_game_state.map);
        }
    }
    
    if (m_game_state.player->get_position().y < -10.0f) {
        m_game_state.next_scene_id = 1;
    }

    if (lives == 0) {
        game_loss_b = true;
        return;
    }

    m_game_state.player->collided_with_enemy_x = false;
    m_game_state.player->collided_with_enemy_y = false;
    
    
    if (jump_timer >= jump_interval) {
//                if (g_game_state.enemies[1].get_is_active() && g_game_state.enemies[1].get_collided_bottom()) {
        m_game_state.enemies[0].jump();
//                }
        jump_timer = 0.0f;
    }
}

void LevelB::render(ShaderProgram *program)
{
    m_game_state.map->render(program);
    m_game_state.player->render(program);
    m_game_state.enemies[0].render(program);
    glm::vec3 player_position = m_game_state.player->get_position();
    player_position.y += 1.5f;
    player_position.x -= 1.0f;
    Utility::draw_text(program, Utility::get_font_id(), std::to_string(lives), 0.3f, 0.025f, player_position);
    
    if (game_loss_b) {
        glm::vec3 message_position = glm::vec3(player_position.x - 1.5f, player_position.y - 2.0f, 0.0f);
        Utility::draw_text(program, Utility::get_font_id(), "YOU LOST", 0.5f, 0.05f, message_position);
    }
}
