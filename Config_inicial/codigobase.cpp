// Proyecto Zocalo


#include <iostream>
#include <cmath>
#include <vector>
#include <string>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes

#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Skybox.h"

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
void Animation();

void DrawSidewalk(
	Shader shader,
	GLuint VAO,
	glm::vec3 position,
	glm::vec3 size,
	glm::vec3 color
);

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(1.0f, -0.8f, 7.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.0f,2.0f, 0.0f),
	glm::vec3(0.0f,0.0f, 0.0f),
	glm::vec3(0.0f,0.0f,  0.0f),
	glm::vec3(0.0f,0.0f, 0.0f)
};

float vertices[] = {
	 -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

glm::vec3 Light1 = glm::vec3(0);
//Anim
float rotBall = 0.0f;
bool AnimBall = false;
bool AnimDog = false;
float rotDog = 0.0f;
int dogAnim = 0;
float FLegs = 0.0f;
float RLegs = 0.0f;
float head = 0.0f;
float tail = 0.0f;

float walkTime = 0.0f;



// Posicion y orientacion del perro
glm::vec3 dogPos(0.0f, -1.5f, 0.0f);
float dogYaw = 0.0f;
float dogRot = 0.0f;

// NPC

float npcPosX = -4.0f;
float npcPosZ = 2.0f;

float npcVelocidad = 1.5f;

float npcLimiteIzquierdo = -4.0f;
float npcLimiteDerecho = 4.0f;

int npcDireccion = 1;
// Camara de tercera persona
float cameraDistance = 4.0f;
float cameraHeight = 1.0f;

bool step = false;

// Deltatime
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

GLuint LoadWallTexture(const char* path)
{
	GLuint textureID;
	glGenTextures(1, &textureID);

	int width, height;

	unsigned char* image = SOIL_load_image(
		path, &width, &height, 0, SOIL_LOAD_RGB
	);

	if (!image)
	{
		cout << "No se pudo cargar la imagen: " << path << endl;
		glDeleteTextures(1, &textureID);
		return 0;
	}

	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGB,
		width, height, 0, GL_RGB,
		GL_UNSIGNED_BYTE, image
	);

	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	return textureID;
}


// Plano del muro

GLuint wallVAO, wallVBO, wallEBO;

GLuint texNorte, texSur, texEste, texOeste;

void SetupWalls()
{
	float vertices[] = {
		// Posición             Normal       Coordenadas de textura
		-0.5f, -0.5f, 0.0f,     0, 0, 1,     0, 1,
		 0.5f, -0.5f, 0.0f,     0, 0, 1,     1, 1,
		 0.5f,  0.5f, 0.0f,     0, 0, 1,     1, 0,
		-0.5f,  0.5f, 0.0f,     0, 0, 1,     0, 0
	};

	unsigned int indices[] = {
		0, 1, 2,
		0, 2, 3
	};

	glGenVertexArrays(1, &wallVAO);
	glGenBuffers(1, &wallVBO);
	glGenBuffers(1, &wallEBO);

	glBindVertexArray(wallVAO);

	glBindBuffer(GL_ARRAY_BUFFER, wallVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, wallEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	texNorte = LoadWallTexture("images/fachadas/norte.jpeg");
	texSur = LoadWallTexture("images/fachadas/sur.jpeg");
	texEste = LoadWallTexture("images/fachadas/este.jpeg");
	texOeste = LoadWallTexture("images/fachadas/oeste.jpg");
}


// Dibujar cada muro

void DrawWall(
	Shader shader,
	GLuint texture,
	glm::vec3 position,
	float width,
	float height,
	float rotation
)
{
	if (texture == 0) return;

	glm::mat4 model(1.0f);

	model = glm::translate(model, position);
	model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(width, height, 1.0f));

	glUniformMatrix4fv(
		glGetUniformLocation(shader.Program, "model"),
		1, GL_FALSE, glm::value_ptr(model)
	);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glUniform1i(glGetUniformLocation(shader.Program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(shader.Program, "material.specular"), 0);

	// Aumentar el brillo ambiental de las fachadas
	glUniform3f(
		glGetUniformLocation(shader.Program, "dirLight.ambient"),
		0.45f, 0.45f, 0.50f
	);

	glBindVertexArray(wallVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);
}

int main()
{
	// Init GLFW
	glfwInit();

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Zocalo", nullptr, nullptr);
	

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	//glfwSetCursorPosCallback(window, MouseCallback);

	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");

	Shader skyboxShader("skybox.vs", "skybox.frag");

	SetupWalls();

	GLint linked = GL_FALSE;



	vector<string> skyboxFaces = {
	"images/Skybox/right.jpg",
	"images/Skybox/left.jpg",
	"images/Skybox/top.jpg",
	"images/Skybox/bottom.jpg",
	"images/Skybox/front.jpg",
	"images/Skybox/back.jpg"
	};

	Skybox nightSky(skyboxFaces);

	// Models
	Model DogBody((char*)"Models/DogBody.obj");
	Model HeadDog((char*)"Models/HeadDog.obj");
	Model DogTail((char*)"Models/TailDog.obj");
	Model F_RightLeg((char*)"Models/F_RightLegDog.obj");
	Model F_LeftLeg((char*)"Models/F_LeftLegDog.obj");
	Model B_RightLeg((char*)"Models/B_RightLegDog.obj");
	Model B_LeftLeg((char*)"Models/B_LeftLegDog.obj");
	Model Piso((char*)"Models/piso/Mrf+Pavement+Set03.obj");
	//Model Ball((char*)"Models/ball.obj");

	Model Puesto((char*)"Models/puesto/asian food vendor.obj");
	Model Lampara((char*)"Models/lampara/Street_lamp_1.obj");
	Model puestoFruta((char*)"Models/puesto2/fruit shop.obj");
	Model Personas((char*)"Models/personas/Audience.obj");

	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);

	glm::mat4 projection = glm::perspective(glm::radians(60.0f), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		GLfloat currentFrame = glfwGetTime();

		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		if (deltaTime > 0.05f)
		{
			deltaTime = 0.05f;
		}

		glfwPollEvents();

		// actualizar NPC

		static float tiempoAnteriorNPC = (float)glfwGetTime();

		float tiempoActualNPC = (float)glfwGetTime();
		float deltaTimeNPC = tiempoActualNPC - tiempoAnteriorNPC;

		tiempoAnteriorNPC = tiempoActualNPC;

		npcPosX += npcVelocidad * npcDireccion * deltaTimeNPC;

		if (npcPosX >= npcLimiteDerecho)
		{
			npcPosX = npcLimiteDerecho;
			npcDireccion = -1;
		}
		else if (npcPosX <= npcLimiteIzquierdo)
		{
			npcPosX = npcLimiteIzquierdo;
			npcDireccion = 1;
		}

		DoMovement();
		Animation();


		glClearColor(0.008f, 0.012f, 0.045f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);

		glm::mat4 modelTemp = glm::mat4(1.0f);

		lightingShader.Use();

		glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

		// Directional light - Iluminacion nocturna
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"),-0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"),0.30f, 0.30f, 0.40f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"),0.45f, 0.45f, 0.50f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"),0.15f, 0.15f, 0.20f);


		// Point light 1
		glm::vec3 lightColor;
		lightColor.x = abs(sin(glfwGetTime() * Light1.x));
		lightColor.y = abs(sin(glfwGetTime() * Light1.y));
		lightColor.z = sin(glfwGetTime() * Light1.z);

		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"),
			lightColor.x * 0.08f, lightColor.y * 0.08f, lightColor.z * 0.08f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"),
			lightColor.x * 0.35f, lightColor.y * 0.35f, lightColor.z * 0.35f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 0.2f, 0.2f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.075f);

		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.3f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(18.0f)));

		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.0f);

		glm::mat4 view;
		view = camera.GetViewMatrix();

		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glm::mat4 model(1);

		// Explanada

		glDisable(GL_CULL_FACE);

		glUniform1i(
			glGetUniformLocation(lightingShader.Program, "transparency"),
			0
		);

		for (int i = -15; i <= 15; i++)
		{
			for (int j = -15; j <= 15; j++)
			{
				model = glm::mat4(1.0f);

				model = glm::translate(
					model,
					glm::vec3(i * 1.0f, -2.0f, j * 1.0f)
				);

				model = glm::scale(
					model,
					glm::vec3(0.002f, 0.002f, 0.002f)
				);

				glUniformMatrix4fv(
					modelLoc,
					1,
					GL_FALSE,
					glm::value_ptr(model)
				);

				Piso.Draw(lightingShader);
			}
		}

		// Calle continuacion piso

		for (int i = -4; i <= 4; i++)
		{
			for (int j = -40; j <= -16; j++)
			{
				model = glm::mat4(1.0f);

				model = glm::translate(
					model,
					glm::vec3(i * 1.0f, -2.0f, j * 1.0f)
				);

				model = glm::scale(
					model,
					glm::vec3(0.002f, 0.002f, 0.002f)
				);

				glUniformMatrix4fv(
					modelLoc,
					1,
					GL_FALSE,
					glm::value_ptr(model)
				);

				Piso.Draw(lightingShader);
			}
		}

		// BANUQETAS

		lampShader.Use();

		// Configurar la cámara para el shader de las banquetas
		GLint sidewalkViewLoc =
			glGetUniformLocation(lampShader.Program, "view");

		GLint sidewalkProjLoc =
			glGetUniformLocation(lampShader.Program, "projection");

		glUniformMatrix4fv(
			sidewalkViewLoc, 1, GL_FALSE, glm::value_ptr(view)
		);

		glUniformMatrix4fv(
			sidewalkProjLoc, 1, GL_FALSE, glm::value_ptr(projection)
		);

		// Banqueta izquierda
		DrawSidewalk(
			lampShader,
			VAO,
			glm::vec3(-5.0f, -1.85f, -28.0f),
			glm::vec3(2.0f, 0.30f, 24.0f),
			glm::vec3(0.5f, 0.5f, 0.5f)
		);

		// Banqueta derecha
		DrawSidewalk(
			lampShader,
			VAO,
			glm::vec3(5.0f, -1.85f, -28.0f),
			glm::vec3(2.0f, 0.30f, 24.0f),
			glm::vec3(0.5f, 0.5f, 0.5f)
		);

		lightingShader.Use();


		glEnable(GL_CULL_FACE);

		glUniform1i(
			glGetUniformLocation(lightingShader.Program, "transparency"),
			0
		);

		// Dimensiones de los muros
		float alturaMuro = 12.0f;
		float centroY = 4.0f;
		float anchoMuro = 32.0f;

		// MURO NORTE - LADO IZQUIERDO
		DrawWall(
			lightingShader,
			texNorte,
			glm::vec3(-10.0f, centroY, -16.0f),
			12.0f,
			alturaMuro,
			0.0f
		);

		// MURO NORTE - LADO DERECHO
		DrawWall(
			lightingShader,
			texNorte,
			glm::vec3(10.0f, centroY, -16.0f),
			12.0f,
			alturaMuro,
			0.0f
		);

		// MURO SUR
		DrawWall(
			lightingShader,
			texSur,
			glm::vec3(0.0f, centroY, 16.0f),
			anchoMuro,
			alturaMuro,
			180.0f
		);

		// MURO ESTE
		DrawWall(
			lightingShader,
			texEste,
			glm::vec3(16.0f, centroY, 0.0f),
			anchoMuro,
			alturaMuro,
			-90.0f
		);

		// MURO OESTE
		DrawWall(
			lightingShader,
			texOeste,
			glm::vec3(-16.0f, centroY, 0.0f),
			anchoMuro,
			alturaMuro,
			90.0f
		);

		// Fachada del izquierda
		DrawWall(
			lightingShader,
			texEste,
			glm::vec3(-7.5f, centroY, -28.0f),
			24.0f,
			alturaMuro,
			90.0f
		);

		// Fachada derecha
		DrawWall(
			lightingShader,
			texSur,
			glm::vec3(7.5f, centroY, -28.0f),
			24.0f,
			alturaMuro,
			-90.0f
		);

		// Edificio del fondo
		DrawWall(
			lightingShader,
			texNorte,
			glm::vec3(0.0f, centroY, -40.0f),
			15.0f,
			alturaMuro,
			0.0f
		);

		// Volver a activar el descarte de caras
		glEnable(GL_CULL_FACE);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);

		// Body
		glm::vec3 dogPosElevado = dogPos + glm::vec3(0.0f, 0.05f, 0.0f);
		modelTemp = model = glm::translate(glm::mat4(1.0f), dogPosElevado);
		modelTemp = model = glm::rotate(model, glm::radians(dogRot), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		DogBody.Draw(lightingShader);

		// Head
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.0f, 0.093f, 0.208f));
		model = glm::rotate(model, glm::radians(head), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		HeadDog.Draw(lightingShader);

		// Tail 
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.0f, 0.026f, -0.288f));
		model = glm::rotate(model, glm::radians(tail), glm::vec3(0.0f, 0.0f, -1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		DogTail.Draw(lightingShader);

		// Front Left Leg
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.112f, -0.044f, 0.074f));
		model = glm::rotate(model, glm::radians(FLegs), glm::vec3(-1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		F_LeftLeg.Draw(lightingShader);

		// Front Right Leg
		model = modelTemp;
		model = glm::translate(model, glm::vec3(-0.111f, -0.055f, 0.074f));
		model = glm::rotate(model, glm::radians(FLegs), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		F_RightLeg.Draw(lightingShader);

		// Back Left Leg
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.082f, -0.046f, -0.218f));
		model = glm::rotate(model, glm::radians(RLegs), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		B_LeftLeg.Draw(lightingShader);

		// Back Right Leg
		model = modelTemp;
		model = glm::translate(model, glm::vec3(-0.083f, -0.057f, -0.231f));
		model = glm::rotate(model, glm::radians(RLegs), glm::vec3(-1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		B_RightLeg.Draw(lightingShader);


		// Puesto
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(3.0f, -1.5f, 0.0f)); 
		model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));     
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		Puesto.Draw(lightingShader);

		// Lampara
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-2.0f, -2.0f, 0.0f)); 
		model = glm::scale(model, glm::vec3(3.2f, 3.2f, 3.2f));		
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		Lampara.Draw(lightingShader);

		// Puesto fruta
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-6.0f, -2.0f, -2.0f)); 
		model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));        
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		puestoFruta.Draw(lightingShader);

		// --- NPC 1: Cerca del poste ---
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, -2.0f));
		model = glm::scale(model, glm::vec3(0.01f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Personas.Draw(lightingShader);


		// --- NPC 2: Cerca del puesto de frutas ---
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(5.0f, -2.0f, -4.0f));
		model = glm::scale(model, glm::vec3(0.01f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0, 1, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Personas.Draw(lightingShader);


		// NPC 3: Caminando de un lado a otro

		model = glm::mat4(1.0f);

		model = glm::translate(
			model,
			glm::vec3(npcPosX, -2.0f, npcPosZ)
		);

		model = glm::scale(
			model,
			glm::vec3(0.01f, 0.01f, 0.01f)
		);

		if (npcDireccion == 1)
		{
			model = glm::rotate(
				model,
				glm::radians(90.0f),
				glm::vec3(0.0f, 1.0f, 0.0f)
			);
		}
		else
		{
			model = glm::rotate(
				model,
				glm::radians(-90.0f),
				glm::vec3(0.0f, 1.0f, 0.0f)
			);
		}

		glUniformMatrix4fv(
			modelLoc,
			1,
			GL_FALSE,
			glm::value_ptr(model)
		);

		Personas.Draw(lightingShader);


		// --- NPC 4: Cerca del centro ---
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(3.0f, -2.0f, 5.0f));
		model = glm::scale(model, glm::vec3(0.01f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0, 1, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Personas.Draw(lightingShader);


		// Lamp object
		lampShader.Use();
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		model = glm::mat4(1);
		model = glm::translate(model, pointLightPositions[0]);
		model = glm::scale(model, glm::vec3(0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);


		// Dibujar cielo nocturno
		nightSky.Draw(skyboxShader, view, projection);

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

// Dibujar una banqueta con un cubo
void DrawSidewalk(
	Shader shader,
	GLuint VAO,
	glm::vec3 position,
	glm::vec3 size,
	glm::vec3 color
)
{
	glm::mat4 model = glm::mat4(1.0f);

	model = glm::translate(model, position);
	model = glm::scale(model, size);

	glUniformMatrix4fv(
		glGetUniformLocation(shader.Program, "model"),
		1,
		GL_FALSE,
		glm::value_ptr(model)
	);

	glUniform3f(
		glGetUniformLocation(shader.Program, "objectColor"),
		color.r,
		color.g,
		color.b
	);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

void DoMovement()
{
	bool moving = false;

	// Velocidades independientes del numero de cuadros
	const float moveSpeed = 3.0f;
	const float turnSpeed = 100.0f;

	float dt = deltaTime;

	// A y D giran al perro
	if (keys[GLFW_KEY_A])
		dogYaw += turnSpeed * dt;

	if (keys[GLFW_KEY_D])
		dogYaw -= turnSpeed * dt;

	// Direccion hacia la que mira el perro.
	// El modelo del perro tiene la cabeza hacia +Z.
	float yawRad = glm::radians(dogYaw);

	glm::vec3 forward(
		sin(yawRad),
		0.0f,
		cos(yawRad)
	);

	// W avanza en la direccion del perro
	if (keys[GLFW_KEY_W])
	{
		dogPos += forward * moveSpeed * dt;
		moving = true;
	}

	// S retrocede
	if (keys[GLFW_KEY_S])
	{
		dogPos -= forward * moveSpeed * dt;
		moving = true;
	}

	// Orientacion usada al dibujar el modelo
	dogRot = dogYaw;

	// Activar o detener la animacion de las patas
	dogAnim = moving ? 1 : 0;

	// Limites laterales de la plaza
	float limiteX = 15.0f;

	// Permitir salir por la abertura norte
	dogPos.x = glm::clamp(dogPos.x, -limiteX, limiteX);

	if (dogPos.x >= -4.0f && dogPos.x <= 4.0f)
	{
		dogPos.z = glm::clamp(dogPos.z, -40.0f, 15.0f);
	}
	else
	{
		dogPos.z = glm::clamp(dogPos.z, -15.0f, 15.0f);
	}

	// Mantener el perro a la altura definida
	dogPos.y = -1.5f;

	// Posicion de la camara: atras y arriba del perro
	glm::vec3 cameraPos =
		dogPos
		- forward * cameraDistance
		+ glm::vec3(0.0f, cameraHeight, 0.0f);

	// Punto al que mira la camara
	glm::vec3 lookTarget =
		dogPos + glm::vec3(0.0f, 0.35f, 0.0f);

	camera.SetPositionAndLookAt(cameraPos, lookTarget);

	// Controles de luz opcionales
	if (keys[GLFW_KEY_T]) pointLightPositions[0].x += 0.01f;
	if (keys[GLFW_KEY_G]) pointLightPositions[0].x -= 0.01f;
	if (keys[GLFW_KEY_Y]) pointLightPositions[0].y += 0.01f;
	if (keys[GLFW_KEY_H]) pointLightPositions[0].y -= 0.01f;
	if (keys[GLFW_KEY_U]) pointLightPositions[0].z -= 0.1f;
	if (keys[GLFW_KEY_J]) pointLightPositions[0].z += 0.01f;
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	if (keys[GLFW_KEY_SPACE])
	{
		active = !active;
		if (active)
		{
			Light1 = glm::vec3(0.2f, 0.8f, 1.0f);
		}
		else
		{
			Light1 = glm::vec3(0);
		}
	}
	if (keys[GLFW_KEY_N])
	{
		AnimBall = !AnimBall;
	}
	if (keys[GLFW_KEY_B])
	{
		dogAnim = 1;
	}

}

void Animation()
{
	if (AnimBall)
	{
		rotBall += 0.4f;
	}

	if (AnimDog)
	{
		rotDog -= 0.6f;
	}

	// Animación de caminar
	if (dogAnim == 1)
	{
		// La animación depende del tiempo y no de los FPS
		walkTime += deltaTime;

		// Movimiento alternado de las patas
		float movement = sin(walkTime * 8.0f);

		// Amplitud del movimiento de las patas
		FLegs = movement * 28.0f;
		RLegs = movement * 28.0f;

		// Pequeños movimientos de cabeza y cola
		head = sin(walkTime * 8.0f) * 2.0f;
		tail = sin(walkTime * 10.0f) * 5.0f;
	}
	else
	{
		// Regresar a la posición inicial al detenerse
		FLegs = 0.0f;
		RLegs = 0.0f;
		head = 0.0f;
		tail = 0.0f;
		walkTime = 0.0f;
	}
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}

