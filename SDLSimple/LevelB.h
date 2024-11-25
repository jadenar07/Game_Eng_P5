#include "Scene.h"

class LevelB : public Scene {
public:
    int ENEMY_COUNT = 1;
    
    
    LevelB(int& lives) : lives(lives) {}
    ~LevelB();
    
    void initialise() override;
    void update(float delta_time) override;
    void render(ShaderProgram *program) override;
private:
    glm::vec3 initial_player_position;
    int lives;
};
