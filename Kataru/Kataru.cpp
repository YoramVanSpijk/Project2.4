#include "Kataru.h"
#include "ObjModel.h"
#include "GameOverGuiComponent.h"

Kataru::Kataru()
{
    if (!glfwInit())
        throw "Could not initialize glwf";

    window = glfwCreateWindow(1400, 800, "Kataru", NULL, NULL);
    gameStateHandler = new GameStateHandler();
    userStatistics = new UserStatistics();

    if (!window)
    {
        glfwTerminate();
        throw "Could not initialize glwf";
    }
    glfwMakeContextCurrent(window);

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

    glfwTerminate();
}

Kataru::~Kataru()
{

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
            if (key == GLFW_KEY_ESCAPE)
                glfwSetWindowShouldClose(window, true);
        });

    cursorChangeMenu = true;
    cursorChangeGame = true;
    lastFrameTime = 0;

    this->spawner = std::unique_ptr<ObjSpawner>(new ObjSpawner());

    attachGameObject(nullptr, new VisionCamera(window), glm::vec3(0.0f, 0.0f, 0.0f));
    attachGuiObject(nullptr, window, new MenuGuiComponent(gameStateHandler));
    attachGameOverGuiObject(nullptr, window, new GameOverGuiComponent(gameStateHandler, userStatistics));
}

void Kataru::update()
{
    double currentFrameTime = glfwGetTime();
    double deltaTime = currentFrameTime - lastFrameTime;
    lastFrameTime = currentFrameTime;

    gameStateHandler->GetGamestate(&currentGameState);

    switch (currentGameState)
    {
    case GameStateHandler::GameState::Menu:
        setMouseCursorVisibilityMenu();

        for (size_t i = 0; i < menuGuiObjects.size(); i++)
            menuGuiObjects[i]->update(deltaTime);

        break;
    case GameStateHandler::GameState::Game:
        setMouseCursorVisibilityGame();
        this->spawner->update(deltaTime);
        for (size_t i = 0; i < gameObjects.size(); i++)
            gameObjects[i]->update(deltaTime);

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

    switch (currentGameState)
    {
        case GameStateHandler::GameState::Menu:
            this->spawner->setOn(false);
            for (size_t i = 0; i < menuGuiObjects.size(); i++)
                menuGuiObjects[i]->draw();

            break;
        case GameStateHandler::GameState::Calibration:
            // TODO: Calibration phase

            break;
        case GameStateHandler::GameState::Game:
            this->spawner->setOn(true);
            this->spawner->draw();
            for (size_t i = 0; i < gameObjects.size(); i++)
                gameObjects[i]->draw();

            break;
        case GameStateHandler::GameState::GameOver:
            for (size_t i = 0; i < gameOverGuiObjects.size(); i++)
                gameOverGuiObjects[i]->draw();

            break;
    }
}
