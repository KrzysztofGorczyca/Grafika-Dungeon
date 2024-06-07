#include "libs.h"
#include <chrono>
#include "Shaders.h"
#include "Camera.h"
#include "Enemy.h"
#include "Model.h"
#include "Input.h"
#include "Player.h"
#include "Chest.h"
#include <random>

// Struktura reprezentuj¹ca AABB
struct AABB {
    glm::vec3 min;
    glm::vec3 max;

    // Metoda sprawdzaj¹ca czy dany punkt znajduje siê wewn¹trz AABB
    bool contains(const glm::vec3& point) const {
        return point.x >= min.x && point.x <= max.x &&
            point.y >= min.y && point.y <= max.y &&
            point.z >= min.z && point.z <= max.z;
    }

    // Metoda sprawdzaj¹ca czy dany AABB koliduje z innym AABB
    bool intersects(const AABB& other) const {
        return (min.x <= other.max.x && max.x >= other.min.x) &&
            (min.y <= other.max.y && max.y >= other.min.y) &&
            (min.z <= other.max.z && max.z >= other.min.z);
    }
};

// Klasa reprezentuj¹ca wêze³ drzewa AABB
class AABBNode {
public:
    AABB aabb;
    std::vector<AABBNode*> children;

    AABBNode(const AABB& aabb) : aabb(aabb) {}

    ~AABBNode() {
        for (auto child : children) {
            delete child;
        }
    }
};

// Funkcja do sprawdzania kolizji miêdzy dwoma AABB
bool checkCollision(const glm::vec3& playerPosition, const AABBNode* root) {
    if (!root) {
        return false;
    }

    // Jeœli gracz jest poza obszarem drzewa, to nie ma kolizji
    if (!root->aabb.contains(playerPosition)) {
        return false;
    }

    // Jeœli to liœæ, to sprawdzamy czy gracz koliduje z AABB tego liœcia
    if (root->children.empty()) {
        return root->aabb.contains(playerPosition);
    }

    // Rekurencyjnie sprawdzamy kolizjê z dzieæmi wêz³a
    for (auto child : root->children) {
        if (checkCollision(playerPosition, child)) {
            return true;
        }
    }

    return false;
}

// Funkcja do tworzenia drzewa AABB na podstawie wektorów opisuj¹cych obiekty na mapie
AABBNode* buildAABBTree(const std::vector<AABB>& objectAABBs) {
    if (objectAABBs.empty()) {
        return nullptr;
    }

    // Tworzymy korzeñ drzewa
    AABB combinedAABB = objectAABBs[0];
    for (size_t i = 1; i < objectAABBs.size(); ++i) {
        const AABB& aabb = objectAABBs[i];
        combinedAABB.min = glm::min(combinedAABB.min, aabb.min);
        combinedAABB.max = glm::max(combinedAABB.max, aabb.max);
    }

    AABBNode* root = new AABBNode(combinedAABB);

    // Tworzymy dzieci korzenia i rekurencyjnie dodajemy obiekty
    for (const auto& aabb : objectAABBs) {
        if (root->aabb.intersects(aabb)) {
            root->children.push_back(new AABBNode(aabb));
        }
    }

    return root;
}

// Funkcja wczytuj¹ca plik OBJ i zapisuj¹ca wierzcho³ki, normalne i indeksy
void loadOBJ(const std::string& filename, std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals, std::vector<unsigned int>& indices) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open OBJ file: " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string token;
        iss >> token;

        if (token == "v") {
            glm::vec3 vertex;
            iss >> vertex.x >> vertex.y >> vertex.z;
            vertices.push_back(vertex);
        }
        else if (token == "vn") {
            glm::vec3 normal;
            iss >> normal.x >> normal.y >> normal.z;
            normals.push_back(normal);
        }
        else if (token == "f") {
            unsigned int vIndex, nIndex;
            std::string vIndices[3], nIndices[3];
            iss >> vIndices[0] >> vIndices[1] >> vIndices[2];

            for (int i = 0; i < 3; ++i) {
                std::istringstream viss(vIndices[i]);
                viss >> vIndex;
                indices.push_back(vIndex - 1); // OBJ indeksuje od 1, dlatego odejmujemy 1
                if (viss.peek() == '/') {
                    viss.ignore();
                    if (viss.peek() != '/') {
                        viss >> nIndex;
                    }
                }
            }
        }
    }

    // Wyœwietlanie wczytanych danych
    /*
	std::cout << "Vertices:" << std::endl;
    for (const auto& vertex : vertices) {
        std::cout << vertex.x << " " << vertex.y << " " << vertex.z << std::endl;
    }

    std::cout << "Normals:" << std::endl;
    for (const auto& normal : normals) {
        std::cout << normal.x << " " << normal.y << " " << normal.z << std::endl;
    }

    std::cout << "Indices:" << std::endl;
    for (const auto& index : indices) {
        std::cout << index << std::endl;
    }
    */
    file.close();
}

AABBNode* buildMapAABBTree() {
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> indices;

    loadOBJ("Assets/Map/Map.obj", vertices, normals, indices);

    std::vector<AABB> objectAABBs;

    // Przekszta³æ wierzcho³ki na obiekty AABB
    for (size_t i = 0; i < indices.size(); i += 3) {
        glm::vec3 v0 = vertices[indices[i]];
        glm::vec3 v1 = vertices[indices[i + 1]];
        glm::vec3 v2 = vertices[indices[i + 2]];

        glm::vec3 min = glm::vec3(
            std::min(v0.x, std::min(v1.x, v2.x)),
            std::min(v0.y, std::min(v1.y, v2.y)),
            std::min(v0.z, std::min(v1.z, v2.z))
        );

        glm::vec3 max = glm::vec3(
            std::max(v0.x, std::max(v1.x, v2.x)),
            std::max(v0.y, std::max(v1.y, v2.y)),
            std::max(v0.z, std::max(v1.z, v2.z))
        );

        AABB objectAABB;
        objectAABB.min = min;
        objectAABB.max = max;

        objectAABBs.push_back(objectAABB);
    }

    return buildAABBTree(objectAABBs);
}

// Funkcja do sprawdzania kolizji miêdzy graczem a map¹
bool checkPlayerMapCollisionAndMoveBack(glm::vec3& playerPosition, const AABBNode* mapRoot, glm::vec3& previousPlayerPosition) {
    bool collision = checkCollision(playerPosition, mapRoot);
    if (collision) {
        //std::cout << "Kolizja z map¹! Pozycja gracza: (" << playerPosition.x << ", " << playerPosition.y << ", " << playerPosition.z << ")" << std::endl;
        //std::cout << "Poprzednia pozycja gracza: (" << previousPlayerPosition.x << ", " << previousPlayerPosition.y << ", " << previousPlayerPosition.z << ")" << std::endl;
    }
	return collision;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

bool menu = true;
bool game = false;
bool died = false;
AABBNode* mapAABBRoot = nullptr;

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

glm::vec3 skeletonHandOffset(0.088918f, 0.77669f, 0.014702f);

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Grid settings
int gridWidth = 10;
int gridHeight = 10;

vector<Enemy> enemies; // Wektor przechowuj¹cy przeciwników
vector<Chest> chests; // Wektor przechowuj¹cy skrzynie

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> dis(0.0, 1.0);
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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Dungeon", glfwGetPrimaryMonitor(), NULL);
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

    //Chest
    {
        if (dis(gen) < 0.25) {
            Chest chest1(glm::vec3(-3.33969f, 0.3f, -5.33003f), 90.0f);
            chests.push_back(chest1);
        }

        if (dis(gen) < 1.0) {
            Chest chest2(glm::vec3(2.9929f, 0.3f, -5.2583f), 90.0f);
            chests.push_back(chest2);
        }

        if (dis(gen) < 0.25) {
            Chest chest3(glm::vec3(9.01465f, 0.3f, 5.79264f), 90.0f);
            chests.push_back(chest3);
        }

        if (dis(gen) < 0.25) {
            Chest chest4(glm::vec3(10.1609f, 0.3f, 6.9917f), 0.0f);
            chests.push_back(chest4);
        }

        if (dis(gen) < 0.25) {
            Chest chest5(glm::vec3(8.97347f, 0.3f, 8.18384f), -90.0f);
            chests.push_back(chest5);
        }

        if (dis(gen) < 0.25) {
            Chest chest6(glm::vec3(7.17944f, 0.3f, 21.43f), 0.0f);
            chests.push_back(chest6);
        }

        if (dis(gen) < 0.25) {
            Chest chest7(glm::vec3(5.97503f, 0.3f, 22.2366f), -90.0f);
            chests.push_back(chest7);
        }

        if (dis(gen) < 0.25) {
            Chest chest8(glm::vec3(-5.09801f, 0.3f, 19.8613f), 90.0f);
            chests.push_back(chest8);
        }

        if (dis(gen) < 0.25) {
            Chest chest9(glm::vec3(-17.0788f, 0.3f, 19.8751f), 90.0f);
            chests.push_back(chest9);
        }

        if (dis(gen) < 0.25) {
            Chest chest10(glm::vec3(-9.87756f, 0.3f, 7.02046f), 0.0f);
            chests.push_back(chest10);
        }

        if (dis(gen) < 0.25) {
            Chest chest11(glm::vec3(-11.0927f, 0.3f, 5.82239f), 90.0f);
            chests.push_back(chest11);
        }

        if (dis(gen) < 0.25) {
            Chest chest12(glm::vec3(-14.1043f, 0.3f, 11.1898f), -90.0f);
            chests.push_back(chest12);
        }

        if (dis(gen) < 0.25) {
            Chest chest13(glm::vec3(-15.2466f, 0.3f, 9.99498f), 180.0f);
            chests.push_back(chest13);
        }

        if (dis(gen) < 0.25) {
            Chest chest14(glm::vec3(-14.0877f, 0.3f, -5.30439f), 90.0f);
            chests.push_back(chest14);
        }

        if (dis(gen) < 0.25) {
            Chest chest15(glm::vec3(-11.0805f, 0.3f, -7.98703f), -90.0f);
            chests.push_back(chest15);
        }

        if (dis(gen) < 0.25) {
            Chest chest16(glm::vec3(-9.89421f, 0.3f, -9.17691f), 0.0f);
            chests.push_back(chest16);
        }

        if (dis(gen) < 0.25) {
            Chest chest17(glm::vec3(-15.2498f, 0.3f, -12.1821f), 180.0f);
            chests.push_back(chest17);
        }

        if (dis(gen) < 0.25) {
            Chest chest18(glm::vec3(-14.0587f, 0.3f, -13.3533f), 90.0f);
            chests.push_back(chest18);
        }

        if (dis(gen) < 0.25) {
            Chest chest19(glm::vec3(-3.0095f, 0.3f, -10.9699f), -90.0f);
            chests.push_back(chest19);
        }

        if (dis(gen) < 0.25) {
            Chest chest20(glm::vec3(4.81888f, 0.3f, -12.1666f), 180.0f);
            chests.push_back(chest20);
        }

        if (dis(gen) < 0.25) {
            Chest chest21(glm::vec3(14.055f, 0.3f, -13.3138f), 90.0f);
            chests.push_back(chest21);
        }

        if (dis(gen) < 0.25) {
            Chest chest22(glm::vec3(18.2704f, 0.3f, -12.1454f), 0.0f);
            chests.push_back(chest22);
        }

        if (dis(gen) < 0.25) {
            Chest chest23(glm::vec3(18.2621f, 0.3f, -4.07328f), 0.0f);
            chests.push_back(chest23);
        }

        if (dis(gen) < 0.25) {
            Chest chest24(glm::vec3(2.98643f, 0.3f, -24.4461f), 90.0f);
            chests.push_back(chest24);
        }

        if (dis(gen) < 0.25) {
            Chest chest25(glm::vec3(7.19058f, 0.3f, -23.2428f), 0.0f);
            chests.push_back(chest25);
        }

        if (dis(gen) < 0.25) {
            Chest chest26(glm::vec3(-8.078f, 0.3f, -24.4193f), 90.0f);
            chests.push_back(chest26);
        }

        if (dis(gen) < 0.25) {
            Chest chest27(glm::vec3(-9.88308f, 0.3f, -20.2373f), 0.0f);
            chests.push_back(chest27);
        }

        if (dis(gen) < 0.25) {
            Chest chest28(glm::vec3(-11.067f, 0.3f, -19.0355f), -90.0f);
            chests.push_back(chest28);
        }

        if (dis(gen) < 0.25) {
            Chest chest29(glm::vec3(-12.2597f, 0.3f, -20.234f), 180.0f);
            chests.push_back(chest29);
        }

        if (dis(gen) < 0.25) {
            Chest chest30(glm::vec3(-14.4506f, 0.3f, -24.4312f), 90.0f);
            chests.push_back(chest30);
        }

        if (dis(gen) < 0.25) {
            Chest chest33(glm::vec3(-25.1724f, 0.3f, 19.8899f), 90.0f);
            chests.push_back(chest33);
        }

        if (dis(gen) < 0.25) {
            Chest chest34(glm::vec3(-29.3303f, 0.3f, 9.98386f), 180.0f);
            chests.push_back(chest34);
        }

        if (dis(gen) < 0.25) {
            Chest chest35(glm::vec3(-26.9758f, 0.3f, -6.17842f), 0.0f);
            chests.push_back(chest35);
        }

        if (dis(gen) < 0.25) {
            Chest chest36(glm::vec3(-34.118f, 0.3f, -7.95768f), -90.0f);
            chests.push_back(chest36);
        }

        if (dis(gen) < 0.25) {
            Chest chest37(glm::vec3(-35.3373f, 0.3f, -9.14862f), 180.0f);
            chests.push_back(chest37);
        }

        if (dis(gen) < 0.25) {
            Chest chest38(glm::vec3(-34.1369f, 0.3f, -10.3523f), 90.0f);
            chests.push_back(chest38);
        }

        if (dis(gen) < 0.25) {
            Chest chest39(glm::vec3(-29.3188f, 0.3f, -20.2508f), 180.0f);
            chests.push_back(chest39);
        }

        if (dis(gen) < 0.25) {
            Chest chest40(glm::vec3(-29.3179f, 0.3f, -26.2363f), 180.0f);
            chests.push_back(chest40);
        }

        if (dis(gen) < 1.0) {
            Chest chest41(glm::vec3(1.1842f, 0.3f, 7.01446f), 0.0f);
            chests.push_back(chest41);
        }

    }

    // load models
    // -----------
    Model mapModel("Assets/Map/Map.obj");
    Model chestModel("Assets/Chest/Chest.obj");
    Model enemyModel("Assets/Skeleton/skeleton.obj");
    Model swordModel("Assets/Blade/Blade.obj");
    Model mainMenuModel("Assets/Menu/mainMenu.obj");
    Model deathMenuModel("Assets/Menu/deathMenu.obj");
    Model skeletonHandModel("Assets/SkeletonHand/handSkeleton.obj");

    std::vector<glm::vec3> vertices1;
    std::vector<glm::vec3> normals1;
    std::vector<unsigned int> indices1;

    loadOBJ("Assets/Map/Map.obj", vertices1, normals1, indices1);
    AABBNode* mapAABBRoot = buildMapAABBTree();

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
    glm::vec3 previousPlayerPosition = player.GetPosition(); // Zapamiêtaj poprzedni¹ pozycjê gracza
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

        // start the ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // input
        // -----
        processInput(window, camera, deltaTime, player, menu, died);
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
                mainMenuModel.Draw(Shader);
            }

            if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
            {
                camera.Position = glm::vec3(-0.000978f, 0.9f, 15.233716f);
                menu = false;
                game = true;
            }


            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
        else if (game)
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

                // render the chests models
                for (const Chest& chest : chests)
                {
                    glm::mat4 model = glm::mat4(1.0f);
                    model = glm::translate(model, chest.GetPosition());
                    model *= glm::mat4_cast(chest.Rotation);
                    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
                    model = glm::rotate(model, glm::radians(chest.getRadians()), glm::vec3(0.0f, 1.0f, 0.0f));
                    Shader.setMat4("model", model);
                    chestModel.Draw(Shader);
                }

                //Enemy collision and damage
                for (Enemy& enemy : enemies)
                {
                    player.CheckDistanceAndModifyHealth(enemy.GetPosition(), 2.0f, enemy.Damage, enemy.canDamage, enemy);
                    enemy.isAnimating = true;
                    enemy.playHandAnimation(deltaTime);
                }

                // Chest interaction
                for (Chest& chest : chests) {
                    chest.CheckInteraction(player);
                }

                // render the skeleton hand model
                for (const Enemy& enemy : enemies) {
                    // Renderowanie rêki wroga
                    glm::mat4 model = glm::mat4(1.0f);
                    model = glm::translate(model, enemy.GetPosition() + skeletonHandOffset);
                    model *= glm::mat4_cast(enemy.Rotation); // Rêka obraca siê tak samo jak wróg
                    model = glm::rotate(model, glm::radians(enemy.hand.rotationAngle), glm::vec3(1.0f, 0.0f, 0.0f));
                    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
                    Shader.setMat4("model", model);
                    skeletonHandModel.Draw(Shader); // Zak³adam, ¿e handModel jest modelem rêki wroga
                }

                //Collision with map
                bool playerMapCollision = checkPlayerMapCollisionAndMoveBack(camera.Position, mapAABBRoot, previousPlayerPosition);
                if (playerMapCollision) {
                    // Gracz koliduje z map¹, przywracamy poprzedni¹ pozycjê gracza
                    camera.SetPosition(previousPlayerPosition.x, previousPlayerPosition.y, previousPlayerPosition.z);
                    std::cout << "Kolizja z map¹! Przywrócono poprzedni¹ pozycjê gracza: (" << previousPlayerPosition.x << ", " << previousPlayerPosition.y << ", " << previousPlayerPosition.z << ")" << std::endl;
                    std::cout << "Aktualna pozycja gracza: (" << player.Position.x << ", " << player.Position.y << ", " << player.Position.z << ")" << std::endl;
                } else {
					// Gracz nie koliduje z map¹, aktualizujemy poprzedni¹ pozycjê gracza
					previousPlayerPosition = camera.Position;
				}

                //Update Sword Animation
                player.UpdateSwordAnimation(deltaTime, enemies);

                //Update Enemies
                for (Enemy& enemy : enemies)
                {
                    enemy.Update(deltaTime, camera);
                    enemy.Position = enemy.GetPosition();
                }

                //Remove dead enemies
                enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](const Enemy& enemy) {
                    return enemy.GetHealth() <= 0.0f;
                    }), enemies.end());


                //Remove opened chests
                chests.erase(std::remove_if(chests.begin(), chests.end(), [](const Chest& chest) {
                    return chest.opened == true;
                    }), chests.end());

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


                //Debugging
                //printf("Distance: %f\n", (glm::distance(player.GetPosition(), enemy1.GetPosition())-0.9));
                //std::cout<<"(" << player.Position.x << "f," << "0.0f," << player.Position.z << "f" <<")" << std::endl;
                //printf("Enemy position: (%f, %f, %f)\n", enemy1.GetPosition().x, enemy1.GetPosition().y, enemy1.GetPosition().z);
                //printf("Camera position: (%f, %f, %f)\n", camera.Position.x, camera.Position.y, camera.Position.z);
                //std::cout << glm::distance(player.GetPosition(), enemyEO.GetPosition()) << std::endl;

                //Regeneration
                player.RegenerateHealth();

                //Change game state if player dies
                if(player.GetHealth() <= 0.0f)
                {
					game = false;
					died = true;
				}

                //Render UI
	            {
                    ImGui::SetNextWindowPos(ImVec2( 50, SCR_HEIGHT-175));
                    ImGui::SetNextWindowSize(ImVec2(400, 125));
		            ImGui::Begin(" ");

                	ImGui::Text("Player Health: %.1f / %.1f", player.GetHealth(), player.GetMaxHealth());

                    ImGui::Text("Player Regeneration: %.3f", player.GetRegeneration());

                	ImGui::Text("Player Damage: %.1f", player.GetDamage());

                    ImGui::Text("Player Attack Speed: %.3f", player.GetAttackSpeed());

                    ImGui::Text("Player Attack Range: %.1f", player.GetAttackRange());

                	ImGui::End();

                    ImGui::SetNextWindowPos(ImVec2(50, 50));
                    ImGui::SetNextWindowSize(ImVec2(200, 75));
                    ImGui::Begin("Entities");
                    ImGui::Text("Enemies: %d", enemies.size());
                    ImGui::Text("Chests: %d", chests.size());
                    ImGui::End();
	            }

                //Render UI
                ImGui::Render();
                ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

                glfwSwapBuffers(window);
                glfwPollEvents();
            }
        } else if(died)
        {
            camera.Position = glm::vec3(5000.0f, 0.0f, -4998.0f);

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

			// render the died menu model
        	{
	        	glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(5000.0f, 0.0f, -5000.0f));
				model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
				Shader.setMat4("model", model);
                deathMenuModel.Draw(Shader);
			}

            camera.ResetCameraAngles();

			if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			{
                glfwTerminate();
                return 0;
			}

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			glfwSwapBuffers(window);
			glfwPollEvents();
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
        if(!died)
        {
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
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
