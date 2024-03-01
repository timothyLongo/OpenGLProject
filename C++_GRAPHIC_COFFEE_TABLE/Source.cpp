//////////////////////////////////////////////////////////////////
// 
// 
//     TIMOTHY LONGO
//     CS-330
//     ASSIGNMENT 7-1: Final Project
//          
//     12 / 1 / 2023
//
//
/////////////////////////////////////////////////////////////////




#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"

#include <iostream>

#include "cylinder.h"

#include "Sphere.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

// camera
glm::vec3 cameraPos = glm::vec3(25.0f, 5.0f, -35.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraDown = glm::vec3(0.0f, -1.0f, 0.0f);

bool firstMouse = true;
float yaw = 160.0f;	// yaw is initialized to -90.0 degrees
                    // since a yaw of 0.0 results in a direction
                    // vector pointing to the right so
                    // we initially rotate a bit to the left.
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Timothy Longo FINAL", NULL, NULL);
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

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile our shader zprogram
	// ------------------------------------
	Shader ourShader("shaderfiles/7.3.camera.vs", "shaderfiles/7.3.camera.fs");
	Shader lightCubeShader("shaderfiles/6.light_cube.vs", "shaderfiles/6.light_cube.fs");

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	
	float vertices[] = {

		-0.4f, -0.2f, -0.5f,  0.0f, 0.0f,
		 0.4f, -0.2f, -0.5f,  1.0f, 0.0f,
		 0.4f,  0.2f, -0.5f,  1.0f, 1.0f,
		 0.4f,  0.2f, -0.5f,  1.0f, 1.0f,
		-0.4f,  0.2f, -0.5f,  0.0f, 1.0f,
		-0.4f, -0.2f, -0.5f,  0.0f, 0.0f,

		-0.4f, -0.2f,  0.5f,  0.0f, 0.0f,
		 0.4f, -0.2f,  0.5f,  1.0f, 0.0f,
		 0.4f,  0.2f,  0.5f,  1.0f, 1.0f,
		 0.4f,  0.2f,  0.5f,  1.0f, 1.0f,
		-0.4f,  0.2f,  0.5f,  0.0f, 1.0f,
		-0.4f, -0.2f,  0.5f,  0.0f, 0.0f,

		-0.4f,  0.2f,  0.5f,  1.0f, 0.0f,
		-0.4f,  0.2f, -0.5f,  1.0f, 1.0f,
		-0.4f, -0.2f, -0.5f,  0.0f, 1.0f,
		-0.4f, -0.2f, -0.5f,  0.0f, 1.0f,
		-0.4f, -0.2f,  0.5f,  0.0f, 0.0f,
		-0.4f,  0.2f,  0.5f,  1.0f, 0.0f,

		 0.4f,  0.2f,  0.5f,  1.0f, 0.0f,
		 0.4f,  0.2f, -0.5f,  1.0f, 1.0f,
		 0.4f, -0.2f, -0.5f,  0.0f, 1.0f,
		 0.4f, -0.2f, -0.5f,  0.0f, 1.0f,
		 0.4f, -0.2f,  0.5f,  0.0f, 0.0f,
		 0.4f,  0.2f,  0.5f,  1.0f, 0.0f,

		-0.4f, -0.2f, -0.5f,  0.0f, 1.0f,
		 0.4f, -0.2f, -0.5f,  1.0f, 1.0f,
		 0.4f, -0.2f,  0.5f,  1.0f, 0.0f,
		 0.4f, -0.2f,  0.5f,  1.0f, 0.0f,
		-0.4f, -0.2f,  0.5f,  0.0f, 0.0f,
		-0.4f, -0.2f, -0.5f,  0.0f, 1.0f,

		-0.4f,  0.2f, -0.5f,  0.0f, 1.0f,
		 0.4f,  0.2f, -0.5f,  1.0f, 1.0f,
		 0.4f,  0.2f,  0.5f,  1.0f, 0.0f,
		 0.4f,  0.2f,  0.5f,  1.0f, 0.0f,
		-0.4f,  0.2f,  0.5f,  0.0f, 0.0f,
		-0.4f,  0.2f, -0.5f,  0.0f, 1.0f,






		// NUMERO DOSE
		-0.4f, -0.2f, -0.5f,  0.0f, 0.0f,
		 0.4f, -0.2f, -0.5f,  1.0f, 0.0f,
		 0.4f,  0.2f, -0.5f,  1.0f, 1.0f,
		 0.4f,  0.2f, -0.5f,  1.0f, 1.0f,
		-0.4f,  0.2f, -0.5f,  0.0f, 1.0f,
		-0.4f, -0.2f, -0.5f,  0.0f, 0.0f,

		-0.4f, -0.2f,  0.5f,  0.0f, 0.0f,
		 0.4f, -0.2f,  0.5f,  1.0f, 0.0f,
		 0.4f,  0.2f,  0.5f,  1.0f, 1.0f,
		 0.4f,  0.2f,  0.5f,  1.0f, 1.0f,
		-0.4f,  0.2f,  0.5f,  0.0f, 1.0f,
		-0.4f, -0.2f,  0.5f,  0.0f, 0.0f,

		-0.4f,  0.2f,  0.5f,  1.0f, 0.0f,
		-0.4f,  0.2f, -0.5f,  1.0f, 1.0f,
		-0.4f, -0.2f, -0.5f,  0.0f, 1.0f,
		-0.4f, -0.2f, -0.5f,  0.0f, 1.0f,
		-0.4f, -0.2f,  0.5f,  0.0f, 0.0f,
		-0.4f,  0.2f,  0.5f,  1.0f, 0.0f,

		 0.4f,  0.2f,  0.5f,  1.0f, 0.0f,
		 0.4f,  0.2f, -0.5f,  1.0f, 1.0f,
		 0.4f, -0.2f, -0.5f,  0.0f, 1.0f,
		 0.4f, -0.2f, -0.5f,  0.0f, 1.0f,
		 0.4f, -0.2f,  0.5f,  0.0f, 0.0f,
		 0.4f,  0.2f,  0.5f,  1.0f, 0.0f,

		-0.4f, -0.2f, -0.5f,  0.0f, 1.0f,
		 0.4f, -0.2f, -0.5f,  1.0f, 1.0f,
		 0.4f, -0.2f,  0.5f,  1.0f, 0.0f,
		 0.4f, -0.2f,  0.5f,  1.0f, 0.0f,
		-0.4f, -0.2f,  0.5f,  0.0f, 0.0f,
		-0.4f, -0.2f, -0.5f,  0.0f, 1.0f,

		-0.4f,  0.2f, -0.5f,  0.0f, 1.0f,
		 0.4f,  0.2f, -0.5f,  1.0f, 1.0f,
		 0.4f,  0.2f,  0.5f,  1.0f, 0.0f,
		 0.4f,  0.2f,  0.5f,  1.0f, 0.0f,
		-0.4f,  0.2f,  0.5f,  0.0f, 0.0f,
		-0.4f,  0.2f, -0.5f,  0.0f, 1.0f,


		// NUMERO TRES
		-0.4f, -0.2f, -0.5f,  0.0f, 0.0f,
		 0.4f, -0.2f, -0.5f,  1.0f, 0.0f,
		 0.4f,  0.2f, -0.5f,  1.0f, 1.0f,
		 0.4f,  0.2f, -0.5f,  1.0f, 1.0f,
		-0.4f,  0.2f, -0.5f,  0.0f, 1.0f,
		-0.4f, -0.2f, -0.5f,  0.0f, 0.0f,

		-0.4f, -0.2f,  0.5f,  0.0f, 0.0f,
		 0.4f, -0.2f,  0.5f,  1.0f, 0.0f,
		 0.4f,  0.2f,  0.5f,  1.0f, 1.0f,
		 0.4f,  0.2f,  0.5f,  1.0f, 1.0f,
		-0.4f,  0.2f,  0.5f,  0.0f, 1.0f,
		-0.4f, -0.2f,  0.5f,  0.0f, 0.0f,

		-0.4f,  0.2f,  0.5f,  1.0f, 0.0f,
		-0.4f,  0.2f, -0.5f,  1.0f, 1.0f,
		-0.4f, -0.2f, -0.5f,  0.0f, 1.0f,
		-0.4f, -0.2f, -0.5f,  0.0f, 1.0f,
		-0.4f, -0.2f,  0.5f,  0.0f, 0.0f,
		-0.4f,  0.2f,  0.5f,  1.0f, 0.0f,

		 0.4f,  0.2f,  0.5f,  1.0f, 0.0f,
		 0.4f,  0.2f, -0.5f,  1.0f, 1.0f,
		 0.4f, -0.2f, -0.5f,  0.0f, 1.0f,
		 0.4f, -0.2f, -0.5f,  0.0f, 1.0f,
		 0.4f, -0.2f,  0.5f,  0.0f, 0.0f,
		 0.4f,  0.2f,  0.5f,  1.0f, 0.0f,

		-0.4f, -0.2f, -0.5f,  0.0f, 1.0f,
		 0.4f, -0.2f, -0.5f,  1.0f, 1.0f,
		 0.4f, -0.2f,  0.5f,  1.0f, 0.0f,
		 0.4f, -0.2f,  0.5f,  1.0f, 0.0f,
		-0.4f, -0.2f,  0.5f,  0.0f, 0.0f,
		-0.4f, -0.2f, -0.5f,  0.0f, 1.0f,

		-0.4f,  0.2f, -0.5f,  0.0f, 1.0f,
		 0.4f,  0.2f, -0.5f,  1.0f, 1.0f,
		 0.4f,  0.2f,  0.5f,  1.0f, 0.0f,
		 0.4f,  0.2f,  0.5f,  1.0f, 0.0f,
		-0.4f,  0.2f,  0.5f,  0.0f, 0.0f,
		-0.4f,  0.2f, -0.5f,  0.0f, 1.0f
		
	};
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);







	// load and create a texture 
	// -------------------------
	unsigned int texture1, texture2, texture3, texture4, texture5, texture6,
		texture7, texture8, texture9, texture10, texture11, texture12, texture13, texture14;
	// texture 1
	// ---------
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char* data = stbi_load("images/canvas.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	// texture 2
	// ---------
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	data = stbi_load("images/CoffeeLogo.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		// note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// texture 3
    // ---------
	glGenTextures(1, &texture3);
	glBindTexture(GL_TEXTURE_2D, texture3);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char* data2 = stbi_load("images/wood.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data2);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data2);

	// texture 4 (coffee can lid)
	// ---------
	glGenTextures(1, &texture4);
	glBindTexture(GL_TEXTURE_2D, texture4);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	data = stbi_load("images/CoffeeCanLid.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		// note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);


	
	// texture 5 (coffee can bottom)
	// ---------
	glGenTextures(1, &texture5);
	glBindTexture(GL_TEXTURE_2D, texture5);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	data = stbi_load("images/chrome.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		// note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	
	// texture 6 cookie container
	// ---------
	glGenTextures(1, &texture6);
	glBindTexture(GL_TEXTURE_2D, texture6);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	data = stbi_load("images/cookie.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		// note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);


	// texture 7 coffee cup (black texture)
	// ---------
	glGenTextures(1, &texture7);
	glBindTexture(GL_TEXTURE_2D, texture7);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	data = stbi_load("images/marble.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		// note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// texture 8 matchbox outer
	// ---------
	glGenTextures(1, &texture8);
	glBindTexture(GL_TEXTURE_2D, texture8);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	data = stbi_load("images/matchBox.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		// note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// texture 9
	// ---------
	glGenTextures(1, &texture9);
	glBindTexture(GL_TEXTURE_2D, texture9);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	data = stbi_load("images/matches.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		// note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// texture 10
	// ---------
	glGenTextures(1, &texture10);
	glBindTexture(GL_TEXTURE_2D, texture10);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	data = stbi_load("images/coffeeLiquid.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		// note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// texture 11
	// ---------
	glGenTextures(1, &texture11);
	glBindTexture(GL_TEXTURE_2D, texture11);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char* data3 = stbi_load("images/amber.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data3);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data3);

	// texture 12
	// ---------
	glGenTextures(1, &texture12);
	glBindTexture(GL_TEXTURE_2D, texture12);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	data = stbi_load("images/map.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		// note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// texture 13
	// ---------
	glGenTextures(1, &texture13);
	glBindTexture(GL_TEXTURE_2D, texture13);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char* data4 = stbi_load("images/marble.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data4);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data4);

	// texture 14
	// ---------
	glGenTextures(1, &texture14);
	glBindTexture(GL_TEXTURE_2D, texture14);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	data = stbi_load("images/wax.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		// note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);



	// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
	// -------------------------------------------------------------------------------------------
	ourShader.use();
	ourShader.setInt("texture1", 0);
	ourShader.setInt("texture2", 1);
	ourShader.setInt("texture3", 2);

	glm::mat4 model;
	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// activate shader
		ourShader.use();

		// pass projection matrix to shader (note that in this case it could change every frame)
		glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		ourShader.setMat4("projection", projection);

		// camera/view transformation
		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		ourShader.setMat4("view", view);



		////////////////////////////////////////////////////////////////////////////
		//////////////////////
		//////////
		////
		//
		//                        DRAWS THE VERTEX ARRAY OBJ 1 HERE
		//
		glBindVertexArray(VAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture8);
		// calculate the model matrix for each object and pass it to shader before drawing
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
		model = glm::translate(model, glm::vec3(10.0f, -4.0f, -20.0f));
		model = glm::scale(model, glm::vec3(3.0f));
		ourShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		//
		//
		//
		/////
		///////////
		//////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////
		//////////////////////
		//////////
		////
		//
		//                        DRAWS THE VERTEX ARRAY OBJ 2 HERE
		//
		glBindVertexArray(VAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		// calculate the model matrix for each object and pass it to shader before drawing
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
		model = glm::translate(model, glm::vec3(10.0f, -4.0f, -22.0f));
		model = glm::scale(model, glm::vec3(2.9f));
		ourShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 36, 72);
		//
		//
		//
		/////
		///////////
		//////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////
		//////////////////////
		//////////
		////
		//
		//                        DRAWS THE VERTEX ARRAY OBJ 3 HERE
		//
		glBindVertexArray(VAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture9);
		// calculate the model matrix for each object and pass it to shader before drawing
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
		model = glm::translate(model, glm::vec3(10.0f, -3.975f, -22.0f));
		model = glm::scale(model, glm::vec3(2.8f));
		ourShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 72, 108);
		//
		//
		//
		/////
		///////////
		//////////////////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////////////////////////
		///                               BELOW IS ATTEMPT FOR A PLANE
		///////////////////////////////////////////////////////////////////////////////////
		glActiveTexture(GL_TEXTURE0); // I suppose this 'loads' the texture into place
		glBindTexture(GL_TEXTURE_2D, texture3); // tells the machine, use the texture 3 on this 
												// next render, which is the wood texture we've added
												// to illustrate the wooden table from the picture
		// glBindVertexArray(VAO2); I'm not sure we need this line, since we didn't use a vertex array
								 // to create this table top
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		// I suppose the above line 'loads' the model of the object
		model = glm::translate(model, glm::vec3(0.0f, -5.0f, -25.0f));
				// above positions object (x, y, z)
		ourShader.setMat4("model", model);
				// tell machine to use shader on this specific object/ model

		static_meshes_3D::Cylinder CoffeeTable(20, 100, 1, true, true, true);
		// above: create the cylinder using outsourced code from 'staticMesh3D.cpp'
		CoffeeTable.render(); // render it -- display it to the screen
		////////////////////////////////////////////////////////////////////
  





		////////////////////////////////////////////////////////////////////////////////
		///                     >---------BELOW IS ATTEMPT FOR COFFEE CAN
		//////////////////////////////////////////////////////////////////////////////////////////////////
		glActiveTexture(GL_TEXTURE0); // I suppose this 'loads' the texture into place
		glBindTexture(GL_TEXTURE_2D, texture2); // tells the machine, use the texture 3 on this 
		// next render, which is the wood texture we've added
		// to illustrate the wooden table from the picture
        // glBindVertexArray(VAO2); I'm not sure we need this line, since we didn't use a vertex array
						 // to create this table top
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		// I suppose the above line 'loads' the model of the object
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, -15.0f));
		// above positions object (x, y, z)
		ourShader.setMat4("model", model);
		// tell machine to use shader on this specific object/ model

		static_meshes_3D::Cylinder CoffeeCan(5, 100, 8, true, true, true);
		// above: create the cylinder using outsourced code from 'staticMesh3D.cpp'
		CoffeeCan.render(); // render it -- display it to the screen
		//////////////////////////////////////////////////////////////////////////////////////////////////


		////////////////////////////////////////////////////////////////////////////////
		///                     >---------BELOW IS ATTEMPT FOR COFFEE CAN LID
		///////////////////////////////////////////////////////////////////////////////////
		glActiveTexture(GL_TEXTURE0); // I suppose this 'loads' the texture into place
		glBindTexture(GL_TEXTURE_2D, texture4); // tells the machine, use the texture 3 on this 
		// next render, which is the wood texture we've added
		// to illustrate the wooden table from the picture
		// glBindVertexArray(VAO2); I'm not sure we need this line, since we didn't use a vertex array
						 // to create this table top
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		// I suppose the above line 'loads' the model of the object
		model = glm::translate(model, glm::vec3(0.0f, 3.0f, -15.0f));
		// above positions object (x, y, z)
		ourShader.setMat4("model", model);
		// tell machine to use shader on this specific object/ model

		static_meshes_3D::Cylinder CoffeeCanLid(5.12, 100, 0.45, true, true, true);
		// above: create the cylinder using outsourced code from 'staticMesh3D.cpp'
		CoffeeCanLid.render(); // render it -- display it to the screen
		//////////////////////////////////////////////////////////////////////////////////////////////////

		
		////////////////////////////////////////////////////////////////////////////////
		///                     >---------BELOW IS ATTEMPT FOR COFFEE CAN BOTTOM GOLD TEXTURE
		///////////////////////////////////////////////////////////////////////////////////
		glActiveTexture(GL_TEXTURE0); // I suppose this 'loads' the texture into place
		glBindTexture(GL_TEXTURE_2D, texture5); // tells the machine, use the texture 3 on this 
		// next render, which is the wood texture we've added
		// to illustrate the wooden table from the picture
		// glBindVertexArray(VAO2); I'm not sure we need this line, since we didn't use a vertex array
						 // to create this table top
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		// I suppose the above line 'loads' the model of the object
		model = glm::translate(model, glm::vec3(0.0f, -4.5f, -15.0f));
		// above positions object (x, y, z)
		ourShader.setMat4("model", model);
		// tell machine to use shader on this specific object/ model

		static_meshes_3D::Cylinder CoffeeCanGold(5.05, 100, 0.3, true, true, true);
		// above: create the cylinder using outsourced code from 'staticMesh3D.cpp'
		CoffeeCanGold.render(); // render it -- display it to the screen
		//////////////////////////////////////////////////////////////////////////////////////////////////
		



		///////////////////////////////////////////////////////////////////////
		//////////////
		//////                 COOKIE CONTAINERS
		// 
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture6);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-8.0f, -3.0f, -25.0f));
		ourShader.setMat4("model", model);
		static_meshes_3D::Cylinder cookie1(5.05, 100, 3.0, true, true, true);
		cookie1.render();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture6);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-8.0f, 0.0f, -25.0f));
		ourShader.setMat4("model", model);
		static_meshes_3D::Cylinder cookie2(5.05, 100, 3.0, true, true, true);
		cookie2.render();
		//
		/////
		//////////////
		////////////////////////////////////////////////////////////////////////
		/////////////
		////                BOTTOM & TOP & MID PIECE
		//   
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture5);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-8.0f, -4.4f, -25.0f));
		ourShader.setMat4("model", model);
		static_meshes_3D::Cylinder cookieBottom(5.15, 100, 0.3, true, true, true);
		cookieBottom.render();
		//
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture5);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-8.0f, -1.4f, -25.0f));
		ourShader.setMat4("model", model);
		static_meshes_3D::Cylinder cookieMid(5.15, 100, 0.3, true, true, true);
		cookieMid.render();
		// 
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture5);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-8.0f, 0.0f, -25.0f));
		ourShader.setMat4("model", model);
		static_meshes_3D::Cylinder cookieTop(5.0, 100, 3.2, true, true, true);
		cookieTop.render();
		/////
		////////////// wonderful
		////////////////////////////////////////////////////////////////////////
		/////               
		///                 NOW COFFE CUP
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture8);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-6.0f, -1.0f, -37.0f));
		ourShader.setMat4("model", model);
		static_meshes_3D::Cylinder coffeeCup(2.0, 100, 7.5, true, true, true);
		coffeeCup.render();
		//                BOTTOM PIECE
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture5);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-6.0f, -4.2f, -37.0f));
		ourShader.setMat4("model", model);
		static_meshes_3D::Cylinder coffeeBottom(2.01, 100, 0.85, true, true, true);
		coffeeBottom.render();
		//                    TOP PIECE
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture5);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-6.0f, 2.3f, -37.0f));
		ourShader.setMat4("model", model);
		static_meshes_3D::Cylinder coffeeTop(2.01, 100, 1.0, true, true, true);
		coffeeTop.render();
		//
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture10);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-6.0f, 2.31f, -37.0f));
		ourShader.setMat4("model", model);
		static_meshes_3D::Cylinder coffeeLiquid(2.0, 100, 1.0, true, true, true);
		coffeeLiquid.render();
		//
		//
		///
		////
		/////////
		///////////////////////////////////////////////////////////////////////////////////////
		/////////////
		////////            
		/////                   -==  AND NOW FOR A CANDLE
		///
		//
		// BASE

		ourShader.use();
		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture13);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(6.0f, -4.3f, -29.0f));
		ourShader.setMat4("model", model);
		static_meshes_3D::Cylinder candleBase(4.5, 100, 0.5, true, true, true);
		candleBase.render();

		// CANDLE WAX PART

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture14);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(6.0f, -1.85f, -29.0f));
		ourShader.setMat4("model", model);
		static_meshes_3D::Cylinder candleWax(0.75, 100, 4.5, true, true, true);
		candleWax.render();

		// CANDLE WICK

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(6.0f, 0.55f, -29.0f));
		ourShader.setMat4("model", model);
		static_meshes_3D::Cylinder candleWick(0.05, 100, 0.5, true, true, true);
		candleWick.render();

		//
		//
		///
		////
		////////
		/////////////////
		//////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////
		/////////////
		////////            
		/////                   -==  AND NOW FOR A GLOBE LIGHT SOURCE
		///
		//

		lightCubeShader.use();
		lightCubeShader.setMat4("projection", projection);
		lightCubeShader.setMat4("view", view);
		model = glm::mat4(1.0f);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(6.0f, 21.7f, -29.0f));

		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		lightCubeShader.setMat4("model", model);
		Sphere light(3, 20, 20);
		light.Draw();

		//
		//
		///
		////
		////////
		/////////////////
		//////////////////////////////////////////////////////////////////////////////////



		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);


	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// let's declare this in the global space
// to then easily manipulate it with another function
// I'm sure there's a better way to do this
// but this is easy solution
float cameraSpeed = 0.02;


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	 // float cameraSpeed = 2.5 * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////     QE KEYS COMPLETE
	/////////////     Q GOES UP
	/////////////     E GOES DOWN
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraUp;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraDown;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////     
	/////////////     PRESS 'P' TO TOGGLE PERSPECTIVE OR ORTHO VIEW: FAIL
	/////////////     
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
	   // FAIL
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f; // change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{

	//////////////////////////////////////////////////////////////////////////////////////
	////////// MOUSE SCROLL - CAMERA CONTROLS COMPLETE
	////////// SCROLL UP TO SPEED UP (INCREMENTALLY)
	////////// SCROLL DOWN TO SLOW DOWN (INCREMENTALLY) as opposed to only two set speeds
	///////////////////////////////////////////////////////////////////////////////////////

	cameraSpeed += (float)yoffset*(0.005);
	if (cameraSpeed < 0.02) {
		cameraSpeed = 0.02;
	}
	if (cameraSpeed > 1.1) {
		cameraSpeed = 1.1;
	}
}
