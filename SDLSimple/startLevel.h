#include "Scene.h"
#include "ShaderProgram.h"

class startLevel : public Scene {
public:
    int ENEMY_COUNT = 1;
    
    std::string message_one;
    std::string message_two;

    startLevel(const std::string& message_one, const std::string& message_two = "")
        : message_one(message_one), message_two(message_two) {}

    ~startLevel();
    
    void initialise() override;
    void update(float delta_time) override;
    void render(ShaderProgram *program) override;
};

