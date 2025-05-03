#include "Config.h"  // Includes GLFW, GLAD, GLM, and other config setup


// ------------------------------------------------------------- //
//                GLOBAL VARIABLES AND STATE                     //
// ------------------------------------------------------------- //


int screenWidth = 800;
int screenHeight = 600;


// Global matrix and program handles for access in callbacks
glm::mat4 projection;
unsigned int projectionLoc;
unsigned int shaderProgram; // needed in callback for projection update


// ------------------------------------------------------------- //
//                       INPUT HANDLING                          //
// ------------------------------------------------------------- //


void processInput(GLFWwindow* window) {
	// Exit the program if ESC is pressed
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}


// ------------------------------------------------------------- //
//                    WINDOW RESIZE CALLBACK                     //
// ------------------------------------------------------------- //


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	screenWidth = width;
	screenHeight = height;
	glViewport(0, 0, width, height);

	// Update projection matrix to account for new window size
	float aspect = static_cast<float>(width) / height;
	float fov = 45.0f; // Field of view (in degrees)
	float near = 0.1f;  // Near clipping plane
	float far = 100.0f; // Far clipping plane

	// Perspective projection (FOV, aspect ratio, near and far planes)
	projection = glm::perspective(glm::radians(fov), aspect, near, far);

	// Upload updated projection to shader
	glUseProgram(shaderProgram);
	glUniformMatrix4fv(projectionLoc, 1, GL_TRUE, glm::value_ptr(projection));
}


// ------------------------------------------------------------- //
//                            MAIN                               //
// ------------------------------------------------------------- //


int main() {
	// Initialize GLFW and set OpenGL version
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create window
	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Load OpenGL function pointers using GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	Shader ourShader("vertexShader.txt", "fragmentShader.txt"); //Declare New External Shader

	shaderProgram = ourShader.ID;

	// Set initial viewport and resize callback
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glViewport(0, 0, screenWidth, screenHeight);

	

	// ------------------------------------------------------------- //
	//                      DEFINE OBJECT SHAPE                      //
	// ------------------------------------------------------------- //


	// Vertex array (4 unique vertices for the square)
	float vertices[] = {
		1.0f,  1.0f, 0.0f,   0.0f, 0.0f, 0.0f,  // Top right
		-1.0f,  1.0f, 0.0f,   0.0f, 0.0f, 0.0f, // Top left
		-1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 0.0f, // Bottom left
		1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 0.0f   // Bottom right
	};

	// Indices to form two triangles from the 4 vertices
	unsigned int indices[] = {
		0, 1, 2,  // First triangle (top-right, top-left, bottom-left)
		0, 2, 3   // Second triangle (top-right, bottom-left, bottom-right)
	};


	// ------------------------------------------------------------- //
	//                          SETUP Buffers                        //
	// ------------------------------------------------------------- //


	// Generate buffers for vertices and indices
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);  // bind VAO first

	glBindBuffer(GL_ARRAY_BUFFER, VBO);  // bind and fill VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Bind and fill EBO (element buffer object)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute (location = 0)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Color attribute (location = 1)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)( 3* sizeof(float)));
	glEnableVertexAttribArray(1);

	// Good practice: unbind everything
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	// ------------------------------------------------------------- //
	//                       MATRIX SETUP                            //
	// ------------------------------------------------------------- //


	glm::mat4 model = glm::mat4(1.0f); // No transformations
	glm::mat4 view = glm::lookAt(
		glm::vec3(0.0f, 0.0f, 5.0f),  // Camera position
		glm::vec3(0.0f, 0.0f, 0.0f),  // Target point
		glm::vec3(0.0f, 1.0f, 0.0f)); // Up direction

	// Get uniform locations
	unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
	unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");

	// Call resize callback once manually to initialize projection matrix
	framebuffer_size_callback(window, screenWidth, screenHeight);


	// ------------------------------------------------------------- //
	//                          RENDER LOOP                          //
	// ------------------------------------------------------------- //


	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		// Clear screen
		glClearColor(0.0f, 0.5f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Set Matrixes
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// Draw the square
		ourShader.use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);  // Draw 2 triangles from the index buffer

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//De-allocate resources
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	// Exit cleanly
	glfwTerminate();
	return 0;
}