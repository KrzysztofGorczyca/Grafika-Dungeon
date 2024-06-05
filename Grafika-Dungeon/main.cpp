#include "libs.h"
#include <chrono>
#include "Shaders.h"
#include "Camera.h"
#include "Enemy.h"
#include "Model.h"
#include "Input.h"
#include "Player.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

// camera
Camera camera(glm::vec3(-0.000978f, 0.9f, 15.233716f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Player
Player player(glm::vec3(-0.000978f, 0.9f, 15.233716f));

// Testing
glm::vec3 targetPoint(-0.016375f, 0.0f, 10.050759f);

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Grid settings
int gridWidth = 10;
int gridHeight = 10;


int main()
{
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Dungeon", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Mouse capture
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    stbi_set_flip_vertically_on_load(true);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    Shader Shader("vertex.glsl", "fragment.glsl");

    //Enemy
    Enemy enemy1(glm::vec3(-0.016375f, 0.0f, 10.050759f));
    // load models
    // -----------
    Model mapModel("Assets/Map/Map.obj");
    Model chestModel("Assets/Chest/Chest.obj");
    Model enemyModel("Assets/Skeleton/skeleton.obj");
    //Model enemyModel("Assets/Skeleton/skeleton.obj");

    // draw in wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Inicjalizacja zmiennych do mierzenia czasu
    auto lastTime = std::chrono::high_resolution_clock::now();
    float deltaTime = 0.0f;

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.Fonts->AddFontDefault();
    io.Fonts->Build();
    ImGui::StyleColorsDark(); // Choose a default theme

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> elapsedTime = currentTime - lastTime;
        deltaTime = elapsedTime.count();
        lastTime = currentTime;

        // input
        // -----
        processInput(window, camera, deltaTime);
        /*
        if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
            // Start the ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // ImGui Window

            ImGui::Begin("Settings");
            if (ImGui::Button("Set Grid 100x100"))
            {
                gridWidth = 100;
                gridHeight = 100;
            }
            ImGui::Text("Current Grid Size: %d x %d", gridWidth, gridHeight);
            ImGui::End();
            
            // Rendering
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }
        */
        // Player position
        player.SetPosition(camera.Position.x, camera.Position.y, camera.Position.z);
        player.printPlayerPosition();
        player.CheckDistanceAndModifyHealth(targetPoint, 2.0f, 20);

        // render
        // ------
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Shader.use();

        // ustawienia materia³u
        Shader.setInt("material.diffuse", 0);
        Shader.setFloat("material.shininess", 32.0f);

        // ---------Directional light settings-----------------
        Shader.setVec3("light.position", lightPos);
        Shader.setVec3("light.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
        Shader.setVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
        Shader.setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
        Shader.setVec3("viewPos", camera.Position);
        // ---------------------------------------------------

        //-----------Point light settings----------------
        Shader.setFloat("light.constant", 1.0f);
        Shader.setFloat("light.linear", 0.09f);
        Shader.setFloat("light.quadratic", 0.032f);
        // ---------------------------------------------

        // ---------Spot light settings-----------------
        Shader.setVec3("light.position", camera.Position);
        Shader.setVec3("light.direction", camera.Front);
        Shader.setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
        Shader.setFloat("light.outerCutOff", glm::cos(glm::radians(17.5f)));
        // --------------------------------------------

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        Shader.setMat4("projection", projection);
        Shader.setMat4("view", view);
        {
            // render the map model
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
            model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
            Shader.setMat4("model", model);
            mapModel.Draw(Shader);
        }

        {
            // render the chest model
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(1.333f, 0.3f, 7.0f));
            model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
            Shader.setMat4("model", model);
            chestModel.Draw(Shader);
        }

        {
            // render the enemy model
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, enemy1.GetPosition());
            model *= glm::mat4_cast(enemy1.Rotation);
            model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
            Shader.setMat4("model", model);
            enemyModel.Draw(Shader);
        }
        
        enemy1.Update(deltaTime, camera);
        printf("Distance: %f\n", (glm::distance(player.GetPosition(), enemy1.GetPosition())-0.9));
        printf("Player position: (%f, %f, %f)\n", player.GetPosition().x, player.GetPosition().y, player.GetPosition().z);
        printf("Enemy position: (%f, %f, %f)\n", enemy1.GetPosition().x, enemy1.GetPosition().y, enemy1.GetPosition().z);
        //printf("Camera position: (%f, %f, %f)\n", camera.Position.x, camera.Position.y, camera.Position.z);
        //std::cout << glm::distance(player.GetPosition(), enemyEO.GetPosition()) << std::endl;

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    /*
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    */
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
