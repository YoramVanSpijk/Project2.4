#include "Kataru.h"
#include "ObjModel.h"
#include "GameOverGuiComponent.h"
#include "CalibrateGuiComponent.h"
#include "TextWriter.h"

Kataru::Kataru()
{
    if (!glfwInit())
        throw "Could not initialize glwf";

    window = glfwCreateWindow(1400, 800, "Kataru", NULL, NULL);
    gameStateHandler = new GameStateHandler();
    userStatistics = new UserStatistics();
    collisionHandler = new CollisionHandler();

    if (!window)
    {
        glfwTerminate();
        throw "Could not initialize glwf";
    }
    glfwMakeContextCurrent(window);

    colorDetector = new ColorDetector(window);

    tigl::init();
    initImGui();
    init();

    gameStateHandler->SetGamestate(GameStateHandler::GameState::Menu);

    while (!glfwWindowShouldClose(window))
    {
        update();
        draw();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    this->spawner->setOn(false);
    glfwTerminate();
}

Kataru::~Kataru()
{
    delete gameStateHandler;
    delete userStatistics;
    delete collisionHandler;
    delete colorDetector;
}

void Kataru::attachGameObject(GameObject* gameObject, Component* component, glm::vec3 pos)
{
    GameObject* obj = gameObject == nullptr ? new GameObject() : gameObject;
    obj->position = pos;

    if (component != nullptr)
        obj->addComponent(component);

    gameObjects.push_back(obj);
}

void Kataru::attachGuiObject(GuiObject* guiObject, GLFWwindow* window, GuiComponent* guiComponent)
{
    GuiObject* obj = guiObject == nullptr ? new GuiObject(window) : guiObject;

    if (guiComponent != nullptr)
        obj->addGuiComponent(guiComponent);

    menuGuiObjects.push_back(obj);
}

void Kataru::attachCalibrationGuiObject(GuiObject* guiObject, GLFWwindow* window, GuiComponent* guiComponent)
{
    GuiObject* obj = guiObject == nullptr ? new GuiObject(window) : guiObject;

    if (guiComponent != nullptr)
        obj->addGuiComponent(guiComponent);

    calibrationGuiObjects.push_back(obj);
}

void Kataru::attachGameOverGuiObject(GuiObject* guiObject, GLFWwindow* window, GuiComponent* guiComponent)
{
    GuiObject* obj = guiObject == nullptr ? new GuiObject(window) : guiObject;

    if (guiComponent != nullptr)
        obj->addGuiComponent(guiComponent);

    gameOverGuiObjects.push_back(obj);
}

void Kataru::initImGui()
{
    IMGUI_CHECKVERSION();
    ImGuiContext* ctx = ImGui::CreateContext();
    ImGui::SetCurrentContext(ctx);

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
}

void Kataru::setMouseCursorVisibilityMenu()
{
    if (cursorChangeMenu)
    {
        setMouseCursorVisibility(GLFW_CROSSHAIR_CURSOR);
        cursorChangeMenu = false;
    }
}

void Kataru::setMouseCursorVisibilityGame()
{
    if (cursorChangeGame)
    {
        setMouseCursorVisibility(GLFW_CURSOR_DISABLED);
        cursorChangeGame = false;
    }
}

void Kataru::setMouseCursorVisibility(int value)
{
    glfwSetInputMode(window, GLFW_CURSOR, value);
    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
}

void Kataru::init()
{
    glEnable(GL_DEPTH_TEST);

    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        ImGuiIO& io = ImGui::GetIO();

        if (key == GLFW_KEY_ESCAPE)
            glfwSetWindowShouldClose(window, true);

        if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
            io.KeysDown[io.KeyMap[ImGuiKey_Enter]] = true;
        else
            io.KeysDown[io.KeyMap[ImGuiKey_Enter]] = false;

        if (key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS)
            io.KeysDown[io.KeyMap[ImGuiKey_Backspace]] = true;
        else
            io.KeysDown[io.KeyMap[ImGuiKey_Backspace]] = false;

        if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
            io.KeysDown[io.KeyMap[ImGuiKey_LeftArrow]] = true;
        else
            io.KeysDown[io.KeyMap[ImGuiKey_LeftArrow]] = false;

        if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
            io.KeysDown[io.KeyMap[ImGuiKey_RightArrow]] = true;
        else
            io.KeysDown[io.KeyMap[ImGuiKey_RightArrow]] = false;
    });

    cursorChangeMenu = true;
    cursorChangeGame = true;
    lastFrameTime = 0;

    this->tw = TextWriter::getInstance();
    this->spawner = std::unique_ptr<ObjSpawner>(new ObjSpawner());

    this->gameObjects.push_back(new GameObject(new ObjModel("models/steve/Steve.obj", glm::vec4(0xff / 255.0f, 0xe1 / 255.0f, 0x35 / 255.0f, 1)), glm::vec3(0.08f, -0.05f, -0.1f), glm::vec3(0, glm::half_pi<float>()/2, 0), glm::vec3(0.01f, 0.01f, 0.01f)));
    attachGameObject(nullptr, visionCam = new VisionCamera(), glm::vec3(0.0f, 0.0f, 0.0f));
    attachGuiObject(nullptr, window, new MenuGuiComponent(gameStateHandler));
    attachCalibrationGuiObject(nullptr, window, new CalibrateGuiComponent(gameStateHandler, userStatistics));
    attachGameOverGuiObject(nullptr, window, new GameOverGuiComponent(gameStateHandler, userStatistics));
}

void Kataru::update()
{
    double currentFrameTime = glfwGetTime();
    double deltaTime = currentFrameTime - lastFrameTime;
    lastFrameTime = currentFrameTime;

    for (size_t i = 0; i < gameObjects.size(); i++)
        gameObjects[i]->update(deltaTime);

    gameStateHandler->GetGamestate(&currentGameState);

    switch (currentGameState)
    {
        case GameStateHandler::GameState::Menu:
            setMouseCursorVisibilityMenu();

            for (size_t i = 0; i < menuGuiObjects.size(); i++)
                menuGuiObjects[i]->update(deltaTime);

            break;
        case GameStateHandler::GameState::Calibration:
            setMouseCursorVisibilityMenu();
            for (size_t i = 0; i < gameObjects.size(); i++)
                gameObjects[i]->update(deltaTime);

            for (size_t i = 0; i < calibrationGuiObjects.size(); i++)
                calibrationGuiObjects[i]->update(deltaTime);

            break;
        case GameStateHandler::GameState::Game:
            setMouseCursorVisibilityGame();
            this->spawner->update(deltaTime);

            //tw->writeText({0, 0, 0}, "Score: " + std::to_string(userStatistics->GetUserScore()));

            for (size_t i = 0; i < this->gameObjects.size(); i++)
                this->gameObjects[i]->update(deltaTime);

            break;
        case GameStateHandler::GameState::GameOver:
            for (size_t i = 0; i < gameOverGuiObjects.size(); i++)
                gameOverGuiObjects[i]->update(deltaTime);

            break;
        case GameStateHandler::GameState::Quit:
            glfwSetWindowShouldClose(window, true);
            break;
    }
}

void Kataru::draw()
{
    glClearColor(0.2f, 0.2f, 0.6f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int width, height;
    glfwGetWindowSize(window, &width, &height);
    glViewport(0, 0, width, height);

    glm::mat4 projection = glm::perspective(glm::radians(55.0f), width / (float)height, 0.1f, 100.0f);
    tigl::shader->setProjectionMatrix(projection);

    colorDetector->loop(visionCam->getFrame(), showCalibrationROI);

    switch (currentGameState)
    {
        case GameStateHandler::GameState::Menu:
            this->spawner->setOn(false);
            for (size_t i = 0; i < menuGuiObjects.size(); i++)
                menuGuiObjects[i]->draw();

            showCalibrationROI = false;
            break;
        case GameStateHandler::GameState::Calibration:
            this->spawner->setOn(false);

            showCalibrationROI = true;

            for (size_t i = 0; i < calibrationGuiObjects.size(); i++)
                calibrationGuiObjects[i]->draw();

            break;
        case GameStateHandler::GameState::Game:
            this->spawner->setOn(true);
            this->spawner->draw();

            for (size_t i = 0; i < this->spawner->getObjects().size(); i++)
                collisionHandler->check(colorDetector->getCurrentPoint(), this->spawner->getObjects()[i]->getPosition());

            for (size_t i = 0; i < this->gameObjects.size(); i++)
                this->gameObjects[i]->draw();

            showCalibrationROI = false;
            break;
        case GameStateHandler::GameState::GameOver:
            for (size_t i = 0; i < gameOverGuiObjects.size(); i++)
                gameOverGuiObjects[i]->draw();

            showCalibrationROI = false;
            break;
    }
}