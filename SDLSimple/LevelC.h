#include "Scene.h"
#include "ShaderProgram.h"

class LevelC : public Scene {
public:
    int ENEMY_COUNT = 3;
    
    LevelC(int& lives) : lives(lives) {}
    ~LevelC();
    
    void initialise() override;
    void update(float delta_time) override;
    void render(ShaderProgram *program) override;
private:
    glm::vec3 initial_player_position;
    int lives;
};

