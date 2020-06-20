#include "Kataru.h"
#include "ObjModel.h"

Kataru::Kataru()
{
    if (!glfwInit())
        throw "Could not initialize glwf";

    window = glfwCreateWindow(1400, 800, "Kataru", NULL, NULL);
    gameStateHandler = new GameStateHandler();

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
    delete this->spawner;
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

    guiObjects.push_back(obj);
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

    //this->spawner = new ObjSpawner;

    attachGameObject(nullptr, new VisionCamera(window), glm::vec3(0.0f, 0.0f, 0.0f));
    //attachGameObject(nullptr, new ObjModel("models/car/honda_jazz.obj"), glm::vec3(0.0f, 0.0f, 0.0f));
    attachGuiObject(nullptr, window, new MenuGuiComponent(gameStateHandler));
}

void Kataru::update()
{
    double currentFrameTime = glfwGetTime();
    double deltaTime = currentFrameTime - lastFrameTime;
    lastFrameTime = currentFrameTime;

    gameStateHandler->GetGamestate(&currentGameState);

    //this->spawner->update(deltaTime);

    switch (currentGameState)
    {
    case GameStateHandler::GameState::Menu:
        setMouseCursorVisibilityMenu();

        for (size_t i = 0; i < guiObjects.size(); i++)
            guiObjects[i]->update(deltaTime);

        break;
    case GameStateHandler::GameState::Game:
        setMouseCursorVisibilityGame();

        for (size_t i = 0; i < gameObjects.size(); i++)
            gameObjects[i]->update(deltaTime);

        break;
    case GameStateHandler::GameState::Quit:
        glfwSetWindowShouldClose(window, true);
        break;
    }
}

void Kataru::draw()
{
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    glViewport(0, 0, width, height);

    glm::mat4 projection = glm::perspective(glm::radians(55.0f), width / (float)height, 0.1f, 100.0f);
    tigl::shader->setProjectionMatrix(projection);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //this->spawner->draw();

    switch (currentGameState)
    {
        case GameStateHandler::GameState::Menu:
            for (size_t i = 0; i < guiObjects.size(); i++)
                guiObjects[i]->draw();

            break;
        case GameStateHandler::GameState::Game:
            for (size_t i = 0; i < gameObjects.size(); i++)
                gameObjects[i]->draw();

            break;
    }
}
