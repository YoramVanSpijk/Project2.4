#include "CollisionHandler.h"

CollisionHandler::CollisionHandler()
{
}

CollisionHandler::~CollisionHandler()
{
}

void CollisionHandler::testCollider(GLFWwindow* window, cv::Point2f handPos)
{
    // Move object WASD
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        xTestCollider -= 0.1f;
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        xTestCollider += 0.1f;
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        yTestCollider += 0.1f;
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        yTestCollider -= 0.1f;
    }

    // Sample movable hitbox
    struct Circle handHitBox, objectHitBox;
    handHitBox.r = 15.0f;
    handHitBox.x = handPos.x;
    handHitBox.y = handPos.y;

    objectHitBox.r = 15.0f;
    objectHitBox.x = (xTestCollider + 0.15f) * 25;
    objectHitBox.y = yTestCollider * (200 / 6);

    if (isColliding(handHitBox, objectHitBox)) std::cout << "Collision detected!\n";

    // Draw hitbox
    drawCircle((xTestCollider + 0.15f), yTestCollider, objectHitBox.r / 10);

}

void CollisionHandler::check(cv::Point2f handPos, glm::vec3 objPos)
{
    // struct circles for hitboxes
    struct Circle handHitBox, objectHitBox;
    handHitBox.r = 50.0f;
    handHitBox.x = handPos.x;
    handHitBox.y = handPos.y;

    objectHitBox.r = 15.0f;
    objectHitBox.x = (objPos.x * 2500.0f) * 1.5F;
    objectHitBox.y = ((objPos.y + 0.005f) * 100.0f) * 20.0f;

    if(isColliding(handHitBox, objectHitBox)) std::cout << "Collision detected!\n";

    // Draw circle around object
    drawCircle(objPos.x * 100.0f, (objPos.y + 0.005f) * 100.0f, objectHitBox.r / 10.0f); // Times 100 bc scaling 0.001; times 0.005f > object inside circle
}

void CollisionHandler::drawCircle(float cx, float cy, float r)
{
    auto const segments = 12;
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glLineWidth(2.0f);

    auto hitboxModel = glm::mat4(1.0f);
    hitboxModel = glm::scale(hitboxModel, glm::vec3(0.05f, 0.05f, 1.0f));
    hitboxModel = glm::translate(hitboxModel, glm::vec3(cx, cy, -1.0f));
    tigl::shader->setModelMatrix(hitboxModel);
    tigl::shader->enableTexture(false);
    tigl::shader->enableColorMult(true);

    tigl::begin(GL_LINE_LOOP);
    for (int ii = 0; ii < segments; ii++)
    {
        float theta = 2.0f * 3.1415926f * float(ii) / float(segments); // get the current angle 

        float x = r * cosf(theta); // calculate the x component 
        float y = r * sinf(theta); // calculate the y component 

        tigl::shader->setColorMult(glm::vec4(1.0f, 0.5f, 0.0f, 1.0f)); // Orange
        tigl::addVertex(tigl::Vertex::P(glm::vec3(x + cx, y + cy, 0.0f)));
    }
    tigl::end();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    tigl::shader->enableColorMult(false);
}

bool CollisionHandler::isColliding(Circle hand, Circle obj)
{
    // Print debug data
    //std::cout << "hX: " << hand.x << ", hY: " << hand.y << "\n";
    std::cout << "oX: " << obj.x << ", oY: " << obj.y << "\n";

    // Calculate distance between hitboxes
    auto dx = hand.x - obj.x;
    auto dy = hand.y - obj.y;
    auto distance = sqrt(dx * dx + dy * dy);

    // Check collision
    return distance < hand.r + obj.r;
}
