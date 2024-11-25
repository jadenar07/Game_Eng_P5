#include "Scene.h"
#include "ShaderProgram.h"

class win : public Scene {
public:
    int ENEMY_COUNT = 1;
    
    ~win();
    
    void initialise() override;
    void update(float delta_time) override;
    void render(ShaderProgram *program) override;
};

