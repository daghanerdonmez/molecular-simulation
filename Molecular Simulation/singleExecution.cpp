//
//  singleExecution.cpp
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 3.02.2025.
//

#include "singleExecution.hpp"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

int singleRunWithoutGraphics()
{
    Simulation simulation;
    simulation.iterateSimulation(NUMBER_OF_ITERATIONS, 0);
//    if (OUTPUT_RESULTS) {
//        const std::vector<std::unique_ptr<Receiver>>& receivers = simulation.getReceivers();
//        receivers[0].get()->writeOutput();
//        // TODO: THIS SHOULD CHANGE BECAUSE THIS ASSUMES THERE IS ONLY 1 RECEIVER AND TELLS IT TO WRITE IT'S OUTPUT. IT SHOULD BE AN EASY FIX BUT I IGNORE IT FOR NOW.
//    }
    return 0;
}

int singleRunWithGraphics()
{
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
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
#if defined(_WIN32) || defined(_WIN64) || !(defined(__APPLE__))
    // Initialize GLAD on Windows and Linux
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD\n";
        glfwTerminate();
        return -1;
    }
#endif
    
    // Compile shaders
    // Use relative paths for cross-platform compatibility
    std::string shaderBasePath = "./Shaders/";
    Shader particleShader((shaderBasePath + "vertexshader.txt").c_str(), (shaderBasePath + "fragmentshader.txt").c_str());
    
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

    
    //Initialize the simulation
    Simulation simulation;
    
//    std::vector<glm::vec3> positions = getParticlePositions();
//    std::vector<Receiver> receivers = getReceivers();
//    Receiver receiver = receivers[0];
//    glm::vec3 receiverPosition = receiver.getPosition();
//    std::cout << glm::length(positions[0] - receiverPosition) << std::endl;
//    std::cout << receiver.getRadius() << std::endl;
    
    // Render loop
    int totalFrames = NUMBER_OF_ITERATIONS / ITERATIONS_PER_FRAME;
    int currentFrame = 0;
    while (!glfwWindowShouldClose(window) && currentFrame < totalFrames)
    {
        // Input
        processInput(window);

        // Render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        //Iterate the simulation
        simulation.iterateSimulation(ITERATIONS_PER_FRAME, currentFrame);
        
        //Draw the receiver
        const std::vector<std::unique_ptr<Receiver>>& receivers = simulation.getReceivers();
        
        for (int i = 0; i < SINGLE_RECEIVER_COUNT; ++i) {
            SphericalReceiver *sphericalReceiver = dynamic_cast<SphericalReceiver*>(receivers[i].get());
            if (sphericalReceiver) {
                glm::dvec3 receiverPosition = sphericalReceiver->getPosition();
                float receiverRadius = sphericalReceiver->getRadius();
                receiverPosition *= GRAPHICS_ZOOM_MULTIPLIER;
                receiverRadius *= GRAPHICS_ZOOM_MULTIPLIER;
                
                particleShader.setVec2("objectPos", glm::dvec2(receiverPosition.x, receiverPosition.y));
                particleShader.setFloat("objectSize", receiverRadius);
                particleShader.setVec3("objectColor", glm::dvec3(0.0, 0.7, 0.7)); // Blue for receiver

                glBindVertexArray(circleVAO);
                glDrawArrays(GL_TRIANGLE_FAN, 0, circleSegments + 2);
            } else {
                throw std::runtime_error("When running in graphics mode on, there is a nonspherical receiver, not drawing it because it is not implemented.");
            }
            
        }
        
        // Draw the particles
        particleShader.use();
        glBindVertexArray(VAO);
        
        std::vector<glm::dvec3> particlePositions = simulation.getAliveParticlePositions();
        
        for (int i = 0; i < particlePositions.size(); ++i) {
            glm::dvec3 partcilePosition = particlePositions[i] * GRAPHICS_ZOOM_MULTIPLIER;
            particleShader.setVec2("objectPos", glm::dvec2(partcilePosition.x, partcilePosition.y));
            particleShader.setFloat("objectSize", particleSize);
            particleShader.setVec3("objectColor", glm::dvec3(1.0, 0.5, 0.0));
            
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }
        

        glfwSwapBuffers(window);
        glfwPollEvents();
        
        currentFrame++;
    }
    
//    if (OUTPUT_RESULTS) {
//        const std::vector<std::unique_ptr<Receiver>>& receivers = simulation.getReceivers();
//        receivers[0].get()->writeOutput();
//        // TODO: THIS SHOULD CHANGE BECAUSE THIS ASSUMES THERE IS ONLY 1 RECEIVER AND TELLS IT TO WRITE IT'S OUTPUT. IT SHOULD BE AN EASY FIX BUT I IGNORE IT FOR NOW.
//    }
    
    // Clean up
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}
