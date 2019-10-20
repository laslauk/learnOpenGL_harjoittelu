#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp";

#include "stb_image.h"

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
glm::mat4 lookAtCustom(const glm::vec3 &right, const glm::vec3 &up, const glm::vec3 dir, const glm::vec3 pos);


//settings 
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float mixValue = 0.2f;
float camPosZ = 0.1f;
float camPosX = 0.1f;

const float  FOV_MIN = 15.0f;
const float  FOV_MAX = 110.0f;
float fov = 50.0f;


float deltaTime = 0.0f;
float lastFrame = 0.0f;

float lastX = 400.0f, lastY = 300.0f;

float yaw = -90.0f;
float pitch = 0.0f;

bool firstMouse = true;


glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

int main() {

	// GLFW: Init ja config
	//---------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	// GLFW WINDOW CREATION
	// ---------------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	
	//set callbacks
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);


	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	

	Shader ourShader("./vertex.vs", "./fragment.fr");

	float firstTriangle[] = {
		//position			//colors
		-0.9f, -0.5f, 0.0f,         1.0f, 0.0f, 0.0f, // left 
		-0.0f, -0.5f, 0.0f,			0.0f, 1.0f, 0.0f,	// right
		-0.45f, 0.5f, 0.0f,			0.0f, 0.0f, 1.0f	// top 
	};

	/*
	float vertices[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	};*/

	/*
	float vertices[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	};
	*/
	
	float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

	glm::vec3 cubePositions[] = {
  glm::vec3(0.0f,  0.0f,  0.0f),
  glm::vec3(2.0f,  5.0f, -15.0f),
  glm::vec3(-1.5f, -2.2f, -2.5f),
  glm::vec3(-3.8f, -2.0f, -12.3f),
  glm::vec3(2.4f, -0.4f, -3.5f),
  glm::vec3(-1.7f,  3.0f, -7.5f),
  glm::vec3(1.3f, -2.0f, -2.5f),
  glm::vec3(1.5f,  2.0f, -2.5f),
  glm::vec3(1.5f,  0.2f, -1.5f),
  glm::vec3(-1.3f,  1.0f, -1.5f)
	};
	

	float secondTriangle[] = {
		0.0f, -0.5f, 0.0f,  // left
		0.9f, -0.5f, 0.0f,  // right
		0.45f, 0.5f, 0.0f   // top 
	};

	float textCoords[] = {
		0.0f, 0.0f, //lower left corner
		1.0f, 0.0f, //lower right corner
		0.5f, 1.0f //top cetner
	};


		//j�rjestys miss� j�rjestyksess� rendataan
	unsigned int indices[] = {  // note that we start from 0!
			0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};


			//koko paska 36 tavua
		
			/*
			The position data is stored as 32-bit (4 byte) floating point values.
			Each position is composed of 3 of those values.
			There is no space (or other values) between each set of 3 values. The values are tightly packed in the array.
			The first value in the data is at the beginning of the buffer.
			*/


	//With this knowledge we can tell OpenGL how it should interpret the vertex data (per vertex attribute) using glVertexAttribPointer: 


	/* glVertexAttribPointer parametrit
	1. which vertex attribute we want to config. layout (location = 0) asettaa shaderiss� position vertex attribuutin
	lokaation 0, ja me halutaan sy�tt�� dataa t�h�n atribuuttiin, funktiossa sy�tet��n 0
	
	2. vertex atribuutin koko, vertex attribute on vec3 joten sill� on 3 valuee

	3.datan tyyppi GL_FLOAT vec* consist of floats

	4. halutaanko data normalisoituna v�lille 0 1, nyt ei haluta

	5. STRIDE = space between consecutive vertex attributes. Nyt jokaisen position datan v�lill� on aina 3 * float datan koko, niin asetetaan 3*floatinkoko
		- jos on tightly packed niin voi laittaa 0 jolloin openGL p��tt��

	6. void* tyyppi, joten castataan (void*) 0. T�m� on offsetti mist� data alkaa bufferissa, nyt se on 0:ssa joten 0sta aloitetaan
	*/





	//As soon as we want to draw an object, we simply bind the VAO with the prefered settings before drawing the object and that is it. In code this would look a bit like this: 


	// Usually when you have multiple objects you want to draw, 
	// 1. you first generate/configure all the VAOs (and thus the required VBO and attribute pointers) and store those for later use. 
	// 2. The moment we want to draw one of our objects, we take the corresponding VAO, bind it, then draw the object and unbind the VAO again. 
	/* <<<<<<<<<<<<<<<<<<<< VAO END >>>>>>>>>>>>>>>>>>>>>>>>>>*/
	/* ------------------------ SHADER---------------------- */

	

	unsigned int VBOs[2], VAOs[2];
	glGenVertexArrays(2, VAOs); //we can also generate multiple VOAs on buffers at the same time
	glGenBuffers(2, VBOs);

	//FIRST TRIANGLE SETUP
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
				//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	//POSITION ATTRIBUTE
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); //nyt 6 kertaa float koko
	glEnableVertexAttribArray(0);
	//COLOR attribute, on attribute location 1, eli layout 1 shaderissä asetettu
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1); 

	//Since we now have two vertex attributes, we need to calculate STRIDE value, the value to get next same data for exxample x0 --> x1 --> x2
	//in the array we have to move 6 floats to the right eli 6*sizeof(float) = 24 tavua, ja nyt pitää definee offsetti, eli 
	//position vertexin offsetti on datassa 0
	//color vertexin datassa  3*sizeof(float) (12 tavua, koska yks on 4 tavua)

	//SECOND TRIANGLE SETUP
	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Neliö

	unsigned int VBO, VAO, EBO;
	int width, height, nrChannels;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	//bindataan VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	/*
	//asetetaan attribute pointerit mitä on VAOssa
	//----------------------------------
	//position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//texture coords attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
	glEnableVertexAttribArray(2);

	*/

	//position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	//texture coords attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);


	//Luodaan texture
	stbi_set_flip_vertically_on_load(true);
	unsigned int texture1, texture2;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	unsigned char *data = stbi_load("C:/Users/Mulkerokala/Documents/OmatTiedostot/Projektit/OpenGL/Projektit/opengl_tutorial_website/container.jpg", &width, &height, &nrChannels, 0);


	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "FAILED TO LOAD TEXTURE" << std::endl;
	}
	//Free memory
	stbi_image_free(data);

	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//ladataan toka textuuri

	//const char* path = std::string("C:/Users/Mulkerokala/Documents/OmatTiedostot/Projektit/OpenGL/Projektit/opengl_tutorial_website/awesomeface.png").c_str();
	data = 0;
	data = stbi_load("C:/Users/Mulkerokala/Documents/OmatTiedostot/Projektit/OpenGL/Projektit/opengl_tutorial_website/awesomeface.png", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "FAILED TO LOAD second TEXTURE" << std::endl;
	}


	// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
	// -------------------------------------------------------------------------------------------
	ourShader.use();
	// either set it manually like so:
	glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);
	// or set it via the texture class
	ourShader.setInt("texture2", 1);


	/*  |||||||||||||||||||||| GLM */
	/*
	glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f); //define vector vec 
	glm::mat4 trans = glm::mat4(1.0f); //create identity matrix, define its diagonals as 1.0f;
	 // create translation matrix with identity matrix trans as base, and create t
	//the translation matrix that is used to multiple, with transform of x +1, y +1, z +0;
	trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
	vec = trans * vec; //multiply (translate) 
	//calculation is original = (1,0,0) translated with +(1.0, 1.0f, 0) = (2,1,0,0)
	std::cout << vec.x << vec.y << vec.z << std::endl;
	*/


	//lets rotate the container object

	//ensin querytään uniform variablen locaatio
	//sitten lähetetään dataa uniformiin joka on tyyppi Matrix4fv
	//1. Uniform locaatio joka on queraty
	//2. kuinka monta matriisia lähetetään
	//3. transponoinaanko matrix, IE,
	//4. itse data, mutta GLM tyyppion outo ja ptiää muuttaa value_ptr

		/*  |||||||||||||||||||||| GLM */
	unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
	
	//MODEL MATRIX, model matrixissa on trans, scale, rotat otska halutaan
	//applyta transformaamaan kaikkia objectin vertiseitä 


	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	

		//input
		//-----


		//rendering 
		//-----
	
		//bind texture
		glActiveTexture(GL_TEXTURE0);	
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		ourShader.use();

		//Create transformations 
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		ourShader.setMat4("projection", projection);

		//View

		//lookAtCustom()

		glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::mat4 view = lookAtCustom(glm::cross(cameraFront, cameraUp), cameraUp, glm::normalize(cameraPos - cameraTarget), cameraPos);

		//glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		ourShader.setMat4("view", view);

	//	view = glm::translate(view, glm::vec3(1.0f*camPosX, 0.0f, -1.0*camPosZ));
		//projection
		
		glBindVertexArray(VAO);

		for (unsigned int i = 0; i < 10; ++i) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			ourShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}


		//pass transformation matrices to the shader





		// render container
	//	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


		ourShader.setFloat("mixValue", mixValue);

		// check and call events and swap the buffers
			glfwPollEvents(); //poll events n�pp�imist�t yms
			glfwSwapBuffers(window); //Swapataan COLOR bufferit
	}


	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}


//GLFW kutsuu t�t� t�ll� nimell�
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);


	/* VIEW OF VIEW*/

	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {

		fov += 0.1f;
		fov = glm::clamp(fov, FOV_MIN, FOV_MAX);
	}

	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
		fov -= 0.1f;
		fov = glm::clamp(fov, FOV_MIN, FOV_MAX);
	}
	

	float cameraSpeed = 3.5f * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}


	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	float sensitivity = 0.1f;
	
	lastX = xpos;
	lastY = ypos;
	
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	pitch += yoffset;

	yaw = glm::mod(yaw + xoffset, 360.0f);

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	/*
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
	*/

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);

}

glm::mat4 lookAtCustom(const glm::vec3 &right, const glm::vec3 &up, const glm::vec3 dir,  const glm::vec3 pos) {

	glm::mat4 matrix = glm::mat4(
		glm::vec4(right, 0.0f),
		glm::vec4(up, 0.0f),
		glm::vec4(dir, 0.0f),
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
	);

	glm::mat4 matrix2 = glm::mat4(
		glm::vec4(1.0f, 0.0f, 0.0f, -pos.x),
		glm::vec4(1.0f, 1.0f, 0.0f, -pos.y),
		glm::vec4(1.0f, 0.0f, 1.0f, -pos.z),
		glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)
	);

	return matrix * matrix2;
}
 

