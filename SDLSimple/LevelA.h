#include "Scene.h"
#include "ShaderProgram.h"


class LevelA : public Scene {
public:
    int ENEMY_COUNT = 2;

    LevelA(int& lives) : lives(lives) {}
    ~LevelA();
    
    void initialise() override;
    void update(float delta_time) override;
    void render(ShaderProgram *program) override;
private:
    glm::vec3 initial_player_position;
    int lives;
};

