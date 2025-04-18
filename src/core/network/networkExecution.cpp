//
//  networkExecution.cpp
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 5.02.2025.
//

#include "networkExecution.hpp"

const unsigned int SCR_WIDTH = 400;
const unsigned int SCR_HEIGHT = 400;

int networkRunWithoutGraphics(const std::string& networkConfigPath)
{
    // Use relative paths for cross-platform compatibility
    auto network = SimulationNetworkLoader::loadFromYAML(networkConfigPath);
    
    // Bu kısımda normalde number of iterations kere itere ettiriyodum ama baştaki particle sayılarını yazabilmek için ikiye ayırdım. Önce bir kere itere ettiriyorum sonra alive particle count alıyorum ondan sonra geri kalanını çalıştırıyorum. Debug için var sonra değiştirebilirim eski haline. Eski hali şuydu: network->iterateNetwork(NUMBER_OF_ITERATIONS,0);
    network->iterateNetwork(1,0);
    std::cout << "Particles in the Beginning: " << network->getAliveParticleCountInNetwork() << std::endl;
    network->iterateNetwork(NUMBER_OF_ITERATIONS-1,1);
    
    
    network->simulationsWrite("Output/Outputs");
    std::cout << "Particles Remaining Unused at the End: " << network->getAliveParticleCountInNetwork() << std::endl;
    std::cout << "Particles in Sinks: " << network->getParticlesInSinks() << std::endl;
    
    return 0;
}

int networkRunWithGraphics(const std::string& networkConfigPath){
    // Initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Particle Simulation", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
#if defined(_WIN32) || defined(_WIN64) || (defined(__unix__) && !defined(__APPLE__))
    // Initialize GLAD on Windows and Linux
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        glfwTerminate();
        return -1;
    }
#endif
    
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    // Compile shaders using relative paths
    Shader particleShader("shaders/vertexshader.txt", "shaders/fragmentshader.txt");
    
    // For transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Particle square vertices (centered at origin, unit size)
    float squareVertices[] = {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f,
        -0.5f,  0.5f
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };
    
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertices), squareVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    // Generate circle vertices
    const int circleSegments = 30;
    std::vector<float> circleVertices = generateCircleVertices(1.0f, circleSegments);

    unsigned int circleVAO, circleVBO;
    glGenVertexArrays(1, &circleVAO);
    glGenBuffers(1, &circleVBO);

    glBindVertexArray(circleVAO);
    glBindBuffer(GL_ARRAY_BUFFER, circleVBO);
    glBufferData(GL_ARRAY_BUFFER, circleVertices.size() * sizeof(float), circleVertices.data(), GL_STATIC_DRAW);

    // Set vertex attributes
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    double particleSize = 0.01;
    
    
    // Initialize the simulation with relative path
    auto network = SimulationNetworkLoader::loadFromYAML(networkConfigPath);
    Simulation* firstSimulation = network->getFirstSimulation();
//    Simulation* secondSimulation = network->getSecondSimulation();

    
    // Render loop
    int totalFrames = NUMBER_OF_ITERATIONS / ITERATIONS_PER_FRAME;
    int currentFrame = 0;
    
    // FPS calculation variables
    double lastTime = glfwGetTime();
    int frameCount = 0;
    
    while (!glfwWindowShouldClose(window) && currentFrame < totalFrames)
    {
        // Calculate FPS
        double currentTime = glfwGetTime();
        frameCount++;
        
        // If a second has passed, calculate and display FPS
        if (currentTime - lastTime >= 1.0) {
            double fps = frameCount / (currentTime - lastTime);
            
            // Update window title with FPS information
            std::string title = "Particle Simulation - FPS: " + std::to_string(fps);
            glfwSetWindowTitle(window, title.c_str());
            
            frameCount = 0;
            lastTime = currentTime;
        }
        
        // Input
        processInput(window);

        // Render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        //Iterate the simulation
        network->iterateNetwork(ITERATIONS_PER_FRAME, currentFrame);
        
        //Draw the receiver

        const std::vector<std::unique_ptr<Receiver>>& receivers = firstSimulation->getReceivers();
       
        for (int i = 0; i < receivers.size(); ++i) {
            if (receivers[i]) {
                // Try to cast to SphericalReceiver
                SphericalReceiver* sphericalReceiver = dynamic_cast<SphericalReceiver*>(receivers[i].get());
                
                // Only draw if it's a SphericalReceiver
                if (sphericalReceiver) {
                    glm::dvec3 receiverPosition = sphericalReceiver->getPosition();
                    double receiverRadius = sphericalReceiver->getRadius();
                    receiverPosition *= GRAPHICS_ZOOM_MULTIPLIER;
                    receiverRadius *= GRAPHICS_ZOOM_MULTIPLIER;
                   
                    particleShader.setVec2("objectPos", glm::dvec2(receiverPosition.z, receiverPosition.y));
                    particleShader.setFloat("objectSize", static_cast<float>(receiverRadius));
                    particleShader.setVec3("objectColor", glm::dvec3(0.0, 0.7, 0.7)); // Blue for receiver

                    glBindVertexArray(circleVAO);
                    glDrawArrays(GL_TRIANGLE_FAN, 0, circleSegments + 2);
                }
            }
        }
        
        // Draw the particles
        particleShader.use();
        glBindVertexArray(VAO);
        
        std::vector<glm::dvec3> particlePositions = firstSimulation->getAliveParticlePositions();
        
        for (int i = 0; i < particlePositions.size(); ++i) {
            glm::dvec3 partcilePosition = particlePositions[i] * GRAPHICS_ZOOM_MULTIPLIER;
            particleShader.setVec2("objectPos", glm::dvec2(partcilePosition.z, partcilePosition.y));
            particleShader.setFloat("objectSize", particleSize);
            particleShader.setVec3("objectColor", glm::dvec3(1.0, 0.5, 0.0));
            
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }
        

        glfwSwapBuffers(window);
        glfwPollEvents();
        
        currentFrame++;
    }
    
    // Clean up
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    
    network->simulationsWrite("Output/Outputs");
    
    return 0;
}
