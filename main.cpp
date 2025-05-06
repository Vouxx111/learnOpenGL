#include "Config.h"  // Includes GLFW, GLAD, GLM, and other config setup


// ------------------------------------------------------------- //
//                GLOBAL VARIABLES AND STATE                     //
// ------------------------------------------------------------- //


//Screen Data
int screenWidth = 800;
int screenHeight = 600;

// Global matrix and program handles for access in callbacks
glm::mat4 projection;
unsigned int projectionLoc;
unsigned int shaderProgram; // needed in callback for projection update
float mixValue;

//Cammera Variables
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

//Frame Data
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame


// ------------------------------------------------------------- //
//                       INPUT HANDLING                          //
// ------------------------------------------------------------- //


void processInput(GLFWwindow* window) {
	const float cameraSpeed = 7.5f * deltaTime; // adjust accordingly

	// Exit the program if ESC is pressed
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) 
	{
		glfwSetWindowShouldClose(window, true);
	}

	//Texture Mixing
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) 
	{
		mixValue += 0.005f; // change this value accordingly (might be too slow or too fast based on system hardware)
		if (mixValue >= 1.0f)
			mixValue = 1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) 
	{
		mixValue -= 0.005f; // change this value accordingly (might be too slow or too fast based on system hardware)
		if (mixValue <= 0.0f)
			mixValue = 0.0f;
	}

	//Camera Movement
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		cameraPos += cameraSpeed * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		cameraPos -= cameraSpeed * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
}


// ------------------------------------------------------------- //
//                    WINDOW RESIZE CALLBACK                     //
// ------------------------------------------------------------- //


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	screenWidth = width;
	screenHeight = height;
	glViewport(0, 0, width, height);
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

	glEnable(GL_DEPTH_TEST);

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
		// back face
		-1.0f, -1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f, -1.0f,  1.0f, 1.0f,
		 1.0f,  1.0f, -1.0f,  1.0f, 1.0f,
		-1.0f,  1.0f, -1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,  0.0f, 0.0f,

		// front face
		-1.0f, -1.0f,  1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f,  1.0f, 1.0f,
		 1.0f,  1.0f,  1.0f,  1.0f, 1.0f,
		-1.0f,  1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  1.0f,  0.0f, 0.0f,

		// left face
		-1.0f,  1.0f,  1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f, -1.0f,  1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  1.0f,  0.0f, 0.0f,
		-1.0f,  1.0f,  1.0f,  1.0f, 0.0f,

		// right face
		 1.0f,  1.0f,  1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f, -1.0f,  1.0f, 1.0f,
		 1.0f, -1.0f, -1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f, -1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f,  0.0f, 0.0f,
		 1.0f,  1.0f,  1.0f,  1.0f, 0.0f,

		 // bottom face
		 -1.0f, -1.0f, -1.0f,  0.0f, 1.0f,
		  1.0f, -1.0f, -1.0f,  1.0f, 1.0f,
		  1.0f, -1.0f,  1.0f,  1.0f, 0.0f,
		  1.0f, -1.0f,  1.0f,  1.0f, 0.0f,
		 -1.0f, -1.0f,  1.0f,  0.0f, 0.0f,
		 -1.0f, -1.0f, -1.0f,  0.0f, 1.0f,

		 // top face
		 -1.0f,  1.0f, -1.0f,  0.0f, 1.0f,
		  1.0f,  1.0f, -1.0f,  1.0f, 1.0f,
		  1.0f,  1.0f,  1.0f,  1.0f, 0.0f,
		  1.0f,  1.0f,  1.0f,  1.0f, 0.0f,
		 -1.0f,  1.0f,  1.0f,  0.0f, 0.0f,
		 -1.0f,  1.0f, -1.0f,  0.0f, 1.0f
	};


	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,   0.0f,   0.0f),
		glm::vec3(4.0f,  10.0f, -30.0f),
		glm::vec3(-3.0f,  -4.4f,  -5.0f),
		glm::vec3(-7.6f,  -4.0f, -24.6f),
		glm::vec3(4.8f,  -0.8f,  -7.0f),
		glm::vec3(-3.4f,   6.0f, -15.0f),
		glm::vec3(2.6f,  -4.0f,  -5.0f),
		glm::vec3(3.0f,   4.0f,  -5.0f),
		glm::vec3(3.0f,   0.4f,  -3.0f),
		glm::vec3(-2.6f,   2.0f,  -3.0f)
	};



	// ------------------------------------------------------------- //
	//                          SETUP BUFFERS                        //
	// ------------------------------------------------------------- //


	// Generate buffers for vertices and indices
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);  // bind VAO first

	glBindBuffer(GL_ARRAY_BUFFER, VBO);  // bind and fill VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Position attribute (location = 0)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

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
	ourShader.setInt("texture1", 0);	// either set it manually like so:
	ourShader.setInt("texture2", 1);	// or set it via the texture class


	// ------------------------------------------------------------- //
	//                          RENDER LOOP                          //
	// ------------------------------------------------------------- //


	while (!glfwWindowShouldClose(window)) {

		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;


		processInput(window);

		// Clear screen
		glClearColor(0.0f, 0.5f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::rotate(trans, glm::radians(-55.0f), glm::vec3(1.0, 0.0, 0.0));

		ourShader.setFloat("mixValue", mixValue);
		unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

		// Draw the square
		ourShader.use();


		glm::mat4 model = glm::mat4(1.0f); // No transformations
		glm::mat4 view = glm::mat4(1.0f); // Up direction
		glm::mat4 projection = glm::mat4(1.0f);

		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		projection = glm::perspective(glm::radians(90.0f), static_cast<float>(screenWidth) / screenHeight, 0.1f, 100.0f);

		// Get uniform locations
		unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
		unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
		unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");

		//Set Matrixes
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));


		glBindVertexArray(VAO);
		for (unsigned int x = 0; x < 10; x++)	{

			// calculate the model matrix for each object and pass it to shader before drawing
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[x]);

			if (x % 3 == 0)	{
				model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
			}
			else	{
				float angle = 20.0f * x;
				model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			}
			

			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}


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