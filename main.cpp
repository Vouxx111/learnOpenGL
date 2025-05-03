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
float mixValue;


// ------------------------------------------------------------- //
//                       INPUT HANDLING                          //
// ------------------------------------------------------------- //


void processInput(GLFWwindow* window) {
	// Exit the program if ESC is pressed
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		mixValue += 0.005f; // change this value accordingly (might be too slow or too fast based on system hardware)
		if (mixValue >= 1.0f)
			mixValue = 1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		mixValue -= 0.005f; // change this value accordingly (might be too slow or too fast based on system hardware)
		if (mixValue <= 0.0f)
			mixValue = 0.0f;
	}
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

	Shader ourShader("C:/Users/iflyf/OneDrive/Documents/GitHub/learnOpenGL/vertexShader.vs", "C:/Users/iflyf/OneDrive/Documents/GitHub/learnOpenGL/fragmentShader.fs"); //Declare New External Shader	

	shaderProgram = ourShader.ID;

	// Set initial viewport and resize callback
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glViewport(0, 0, screenWidth, screenHeight);

	

	// ------------------------------------------------------------- //
	//                      DEFINE OBJECT SHAPE                      //
	// ------------------------------------------------------------- //


	// Vertex array (4 unique vertices for the square)
	float vertices[] = {
		// positions          // colors           // texture coords
		 1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f, // bottom right
		-1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	};

	// Indices to form two triangles from the 4 vertices
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};


	// ------------------------------------------------------------- //
	//                          SETUP BUFFERS                        //
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Color attribute (location = 1)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)( 3* sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// Good practice: unbind everything
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	// ------------------------------------------------------------- //
	//                          TEXTURING	                         //
	// ------------------------------------------------------------- //


	unsigned int texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load and generate the texture
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);

	unsigned char* data = stbi_load("C:/Users/iflyf/OneDrive/Documents/GitHub/learnOpenGL/container.jpg", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);


	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load and generate the texture
	data = stbi_load("C:/Users/iflyf/OneDrive/Documents/GitHub/learnOpenGL/awesomeface.png", &width, &height, &nrChannels, 0);
	if (data) {
		GLenum format = nrChannels == 4 ? GL_RGBA : GL_RGB;
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

		glGenerateMipmap(GL_TEXTURE_2D);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);


	ourShader.use(); // don't forget to activate/use the shader before setting uniforms!
	glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);	// either set it manually like so:
	ourShader.setInt("texture2", 1);	// or set it via the texture class


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
	unsigned int projectionLoc = glGetUniformLocation(ourShader.ID, "projection");

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

		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		ourShader.setFloat("mixValue", mixValue);

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