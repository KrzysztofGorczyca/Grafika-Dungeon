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

bool menu = true;

// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;
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

vector<Enemy> enemies; // Wektor przechowujący przeciwników

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

	{
		Enemy enemy0(glm::vec3(-0.016375f, 0.0f, 10.050759f));
    	enemies.push_back(enemy0);

    	Enemy enemy1(glm::vec3(0.036355f, 0.000000f, -2.577032f));
    	enemies.push_back(enemy1);

    	Enemy enemy3(glm::vec3(-5.59401f, 0.0f, -4.34549f));
    	enemies.push_back(enemy3);

    	Enemy enemy4(glm::vec3(4.30192f, 0.0f, -4.18849f));
    	enemies.push_back(enemy4);

    	Enemy enemy5(glm::vec3(16.9623f, 0.0f, -3.83829f));
    	enemies.push_back(enemy5);

    	Enemy enemy6(glm::vec3(17.0234f, 0.0f, -12.5754f));
    	enemies.push_back(enemy6);

    	Enemy enemy7(glm::vec3(5.97025f, 0.0f, -23.3051f));
    	enemies.push_back(enemy7);

    	Enemy enemy8(glm::vec3(-11.093f, 0.0f, -23.3374f));
    	enemies.push_back(enemy8);

    	Enemy enemy9(glm::vec3(-11.1941f, 0.0f, -19.7457f));
    	enemies.push_back(enemy9);

    	Enemy enemy10(glm::vec3(-15.5271f, 0.0f, -23.477f));
    	enemies.push_back(enemy10);

    	Enemy enemy11(glm::vec3(-28.4578f, 0.0f, -23.371f));
    	enemies.push_back(enemy11);

    	Enemy enemy12(glm::vec3(-28.1101f, 0.0f, -29.3522f));
    	enemies.push_back(enemy12);

    	Enemy enemy13(glm::vec3(-28.199f, 0.0f, -17.4238f));
    	enemies.push_back(enemy13);

        Enemy enemy14(glm::vec3(5.99937f, 0.0f, 2.57415f));
        enemies.push_back(enemy14);

        Enemy enemy15(glm::vec3(5.41255f, 0.0f, 8.69031f));
        enemies.push_back(enemy15);

        Enemy enemy16(glm::vec3(6.00506f, 0.0f, 21.0736f));
        enemies.push_back(enemy16);

        Enemy enemy17(glm::vec3(-6.37469f, 0.0f, 21.0618f));
        enemies.push_back(enemy17);

        Enemy enemy18(glm::vec3(-12.6804f, 0.0f, 21.4268f));
        enemies.push_back(enemy18);

        Enemy enemy19(glm::vec3(-28.1926f, 0.0f, 20.8219f));
        enemies.push_back(enemy19);

        Enemy enemy20(glm::vec3(-28.1475f, 0.0f, 11.6525f));
        enemies.push_back(enemy20);

        Enemy enemy21(glm::vec3(-28.0663f, 0.0f, 5.56968f));
        enemies.push_back(enemy21);

        Enemy enemy22(glm::vec3(-28.1336f, 0.0f, -7.38187f));
        enemies.push_back(enemy22);

        Enemy enemy23(glm::vec3(-32.0134f, 0.0f, -9.16037f));
        enemies.push_back(enemy23);

        Enemy enemy24(glm::vec3(-23.7428f, 0.0f, -9.16687f));
        enemies.push_back(enemy24);

        Enemy enemy25(glm::vec3(-12.4485f, 0.0f, -9.25784f));
        enemies.push_back(enemy25);

        Enemy enemy26(glm::vec3(-1.59904f, 0.0f, -12.1365f));
        enemies.push_back(enemy26);

        Enemy enemy27(glm::vec3(0.136875f, 0.0f, -12.4627f));
        enemies.push_back(enemy27);

        Enemy enemy28(glm::vec3(-14.0106f, 0.0f, -2.22209f));
        enemies.push_back(enemy28);

        Enemy enemy29(glm::vec3(-14.0527f, 0.0f, 9.95003f));
        enemies.push_back(enemy29);
	}
    // load models
    // -----------
    Model mapModel("Assets/Map/Map.obj");
    Model chestModel("Assets/Chest/Chest.obj");
    Model enemyModel("Assets/Skeleton/skeleton.obj");
    Model swordModel("Assets/Blade/Blade.obj");
    Model menuHoveredModel("Assets/Menu/menuHovered.obj");

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
        processInput(window, camera, deltaTime, player);
        player.UpdateSwordAnimation(deltaTime, enemies);
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
        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        Shader.setMat4("projection", projection);
        Shader.setMat4("view", view);

        Shader.setInt("material.diffuse", 0);
        Shader.setFloat("material.shininess", 32.0f);

        if (menu) {

            camera.Position = glm::vec3(0.0f, 0.0f, 2.0f);

            // ---------Directional light settings-----------------
            {
                Shader.setVec3("light.position", lightPos);
                Shader.setVec3("light.ambient", glm::vec3(1.0f, 1.0f, 1.0f));
                Shader.setVec3("light.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
                Shader.setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
                Shader.setVec3("viewPos", camera.Position);
            }
            // ---------------------------------------------------

            //-----------Point light settings----------------
            {
                Shader.setFloat("light.constant", 1.0f);
                Shader.setFloat("light.linear", 0.09f);
                Shader.setFloat("light.quadratic", 0.032f);
            }
            // ---------------------------------------------

            // ---------Spot light settings-----------------
            {
                Shader.setVec3("light.position", camera.Position);
                Shader.setVec3("light.direction", camera.Front);
                Shader.setFloat("light.cutOff", glm::cos(glm::radians(35.0f)));
                Shader.setFloat("light.outerCutOff", glm::cos(glm::radians(25.0f)));
            }
            // --------------------------------------------

            // render the menu model
            {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
                model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
                Shader.setMat4("model", model);
                menuHoveredModel.Draw(Shader);
            }

            if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
            {
                camera.Position = glm::vec3(-0.000978f, 0.9f, 15.233716f);
                menu = false;
            }

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
        else
        {
            {
                // ustawienia materiu
                {
                    Shader.setInt("material.diffuse", 0);
                    Shader.setFloat("material.shininess", 32.0f);
                }

                // ---------Directional light settings-----------------
                {
                    Shader.setVec3("light.position", lightPos);
                    Shader.setVec3("light.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
                    Shader.setVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
                    Shader.setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
                    Shader.setVec3("viewPos", camera.Position);
                }
                // ---------------------------------------------------

                //-----------Point light settings----------------
                {
                    Shader.setFloat("light.constant", 1.0f);
                    Shader.setFloat("light.linear", 0.09f);
                    Shader.setFloat("light.quadratic", 0.032f);
                }
                // ---------------------------------------------

                // ---------Spot light settings-----------------
                {
                    Shader.setVec3("light.position", camera.Position);
                    Shader.setVec3("light.direction", camera.Front);
                    Shader.setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
                    Shader.setFloat("light.outerCutOff", glm::cos(glm::radians(17.5f)));
                }
                // --------------------------------------------

                // render the map model
                {
                    glm::mat4 model = glm::mat4(1.0f);
                    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
                    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
                    Shader.setMat4("model", model);
                    mapModel.Draw(Shader);
                }

                // render the chest model
                {
                    glm::mat4 model = glm::mat4(1.0f);
                    model = glm::translate(model, glm::vec3(1.333f, 0.3f, 7.0f));
                    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
                    Shader.setMat4("model", model);
                    chestModel.Draw(Shader);
                }

                
                player.UpdateSwordAnimation(deltaTime, enemies);

                for (Enemy& enemy : enemies)
                {
                    enemy.Update(deltaTime, camera);
                    enemy.Position = enemy.GetPosition();
                }

                enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](const Enemy& enemy) {
                    return enemy.GetHealth() <= 0.0f;
                    }), enemies.end());

                // render the enemy model
                for (const Enemy& enemy : enemies)
                {
                    glm::mat4 model = glm::mat4(1.0f);
                    model = glm::translate(model, enemy.GetPosition());
                    model *= glm::mat4_cast(enemy.Rotation);
                    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
                    Shader.setMat4("model", model);
                    enemyModel.Draw(Shader);
                }

                // render the sword model
                {
                    glm::vec3 swordPosition = camera.Position
                        + camera.Right * player.swordOffset.x
                        + camera.Up * player.swordOffset.y
                        + camera.Front * player.swordOffset.z;

                    glm::mat4 model = glm::mat4(1.0f);
                    model = glm::translate(model, swordPosition);

                    // Uzyskaj rotacj kamery jako macierz 4x4
                    glm::vec3 cameraFront = camera.Front;
                    glm::vec3 cameraRight = camera.Right;
                    glm::vec3 cameraUp = camera.Up;

                    glm::mat4 cameraRotation = glm::mat4(1.0f);
                    cameraRotation[0] = glm::vec4(cameraRight, 0.0f);
                    cameraRotation[1] = glm::vec4(cameraUp, 0.0f);
                    cameraRotation[2] = glm::vec4(-cameraFront, 0.0f); // Negacja, bo kamera patrzy w przeciwn stron
                    cameraRotation[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

                    // Zastosuj rotacj kamery do modelu miecza
                    model *= cameraRotation;


                    model *= glm::mat4_cast(player.swordRotation);
                    // Lokalna rotacja miecza, jeli jest potrzebna
                    model = glm::rotate(model, glm::radians(70.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                    model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 1.0f));
                    model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(1.0f, 0.0f, 0.0f));

                    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
                    Shader.setMat4("model", model);
                    swordModel.Draw(Shader);
                }

                /*
                std::string enemytemp = "enemy";
                for (int i = 1; i < 3; i++)
                {
                    enemytemp += std::to_string(i);
                    std::cout << enemytemp << std::endl;
                    enemytemp = "enemy";
                }
				*/

                //printf("Distance: %f\n", (glm::distance(player.GetPosition(), enemy1.GetPosition())-0.9));
                std::cout<<"(" << player.Position.x << "f," << "0.0f," << player.Position.z << "f" <<")" << std::endl;
                //printf("Enemy position: (%f, %f, %f)\n", enemy1.GetPosition().x, enemy1.GetPosition().y, enemy1.GetPosition().z);
                //printf("Camera position: (%f, %f, %f)\n", camera.Position.x, camera.Position.y, camera.Position.z);
                //std::cout << glm::distance(player.GetPosition(), enemyEO.GetPosition()) << std::endl;

                glfwSwapBuffers(window);
                glfwPollEvents();
            }
        }
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
    if(!menu)
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
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
