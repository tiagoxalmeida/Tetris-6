// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// GLM header file
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

// shaders header file
#include <common/shader.cpp>

// Vertex array object (VAO)
GLuint VertexArrayID;

// Vertex buffer object (VBO)
GLuint vertexbuffer;

// color buffer object (CBO)
GLuint colorbuffer;

// GLSL program from the shaders
GLuint programID;


GLint WindowWidth = 600;
GLint WindowHeight = 600;
GLint LastWindowWidth = 600;
GLint LastWindowHeight = 600;
float t = 0;
float l = 0;
float w = WindowWidth;
float h = WindowHeight;
float mouse_x = 0;
float mouse_y = 0;

//vamos ter flags para identificar o que estamos a ver no momento
bool is_menu_view = true;
bool is_game_view = false;
bool is_options_view = false; // talvez numa fase mais avançada poderemos implementar opções para definir a velocidade das peças a cairem, sons e etc
bool is_scores_view = false;
bool is_name_view = false;


void transferDataToGPUMemory(void) //functio to transfer data for GPU
{
	// VAO
	glGenVertexArrays(1, &VertexArrayID); //definition of vertex array object
	glBindVertexArray(VertexArrayID); //bind the array object

	// Create and compile our GLSL program from the shaders
	programID = LoadShaders("shader.vertexshader", "shader.fragmentshader");//load shaders

	//Definition of the vertices

	// Move vertex data to video memory; specifically to VBO called vertexbuffer
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	// Move color data to video memory; specifically to CBO called colorbuffer
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

}

void cleanupDataFromGPU()
{
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &colorbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);
}

bool is_CTRL_down;
bool is_F_down;
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT)) { // clicar na tecla UP e/ou continuar a clicar
		//fazer movimentos com as peças
	}

	if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT)) {// clicar na tecla DOWN e/ou continuar a clicar
		//fazer movimentos com as peças
	}

	if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {// clicar na tecla LEFT e/ou continuar a clicar
		//fazer movimentos com as peças
	}

	if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {// clicar na tecla RIGHT e/ou continuar a clicar
		//fazer movimentos com as peças
	}
	if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		//fazer a peça rodar 
	}
	if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		//fazer a peça rodar 
	}
	if (key == GLFW_KEY_ESCAPE && (action == GLFW_PRESS)) {//clicar na tecla ESCAPE para 
		int width_mm, height_mm;
		glfwGetMonitorPhysicalSize(glfwGetPrimaryMonitor(), &width_mm, &height_mm);
		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		glfwSetWindowMonitor(window, NULL, (mode->width - LastWindowWidth) / 2, (mode->height - LastWindowHeight) / 2, LastWindowWidth, LastWindowHeight, 0);
		glViewport(l, t, w, h);
	}
	if (key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_RIGHT_CONTROL)
		is_CTRL_down = action == GLFW_PRESS;
	if (key == GLFW_KEY_F) 
		is_F_down = action == GLFW_PRESS;
	if (is_CTRL_down && is_F_down)
	{
		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		glfwSetWindowPos(window, (mode->width - WindowWidth) / 2, (mode->height - WindowHeight) / 2);

		glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, mode->refreshRate);
		glViewport(l, t, w, h);
	}


}

void character_callback(GLFWwindow* window, unsigned int codepoint)
{
	if (is_name_view) { // se estiver na view de escrever o nome então escrevemos o nome

	}
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	mouse_x = xpos;
	mouse_y = ypos;
	printf("%f,%f\n", mouse_x, mouse_y);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	//use yoffset to move up and down in menus
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		//if mouse right button clicked
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		//if mouse left button clicked
	}
}

//TODO fazer com que as proporções fiquem iguais caso façamos o resize
void framebuffer_size_callback(GLFWwindow* window, int width, int height) //para fazer o jogo responsivo
{
	t = t*height / (float)WindowHeight;
	l = l*width / (float)WindowWidth;
	w = w*width / (float)WindowWidth;
	h = h*height / (float)WindowHeight;
	glViewport(l, t, w, h);
	LastWindowHeight = WindowHeight;
	WindowHeight = h;
	LastWindowWidth = WindowWidth;
	WindowWidth = w;
}

void mainProgram() {
	// render scene for each frame
	do {
		glClear(GL_COLOR_BUFFER_BIT);
		//left top
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//draw(); //draw function

		// Swap buffers
		glfwSwapBuffers(window);
		// looking for events
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwWindowShouldClose(window) == 0);
}

int main(void)
{
	// Initialise GLFW
	glfwInit();

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window
	window = glfwCreateWindow(WindowWidth, WindowHeight, "6TURN Tetris -The Game", NULL, NULL);

	// Create window context
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	glewInit();

	//key input callback
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCharCallback(window, character_callback);

	//mouse input callback
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// White background
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	// transfer my data (vertices, colors, and shaders) to GPU side
	transferDataToGPUMemory();


	GLuint FramebufferName = 0;
	glGenFramebuffers(1, &FramebufferName);
	//glBindFramebuffer(GL_FRAMEBUFFER, FramebufvisualerName);


	mainProgram();

	// delete framebuffer
	glDeleteFramebuffers(1, &FramebufferName);

	// Cleanup VAO, VBOs, and shaders from GPU
	cleanupDataFromGPU();

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}