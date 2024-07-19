#include<chrono>
#include<thread>
#define _USE_MATH_DEFINES
#include<math.h>
#include"Robot.h"
//#include<ft2build.h>
//#include FT_FREETYPE_H

using std::cout, std::endl, std::vector, std::array;

//=====================================================================================================================
// Variables and Objects declaration-----------------------------------------------------------------------------------
//=====================================================================================================================


bool left_mouse_button;
bool right_mouse_button;

array<int, 2> windowSize{512, 512};
float g_xpos, g_ypos;
float wc_x;
float wc_y;

float Scale_x = 0.05f;
float Scale_y = Scale_x;

robot BotObj;
robot* bot{&BotObj};

struct KeyFlags 
{
	bool UpKeyPressed{ false };
	bool LeftKeyPressed{ false };
	bool RightKeyPressed{ false };
	bool DownKeyPressed{ false };
};


KeyFlags flags;

//=====================================================================================================================
// CallBacks ----------------------------------------------------------------------------------------------------------
//=====================================================================================================================
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void process_input(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

void cursor_pos_callBack(GLFWwindow* window, double xpos, double ypos)
{
	g_xpos = xpos;
	g_ypos = ypos;
	wc_x = (2 * (xpos / windowSize[0]) - 1) / Scale_x;
	wc_y = (-2 * (ypos / windowSize[1]) + 1) / Scale_y;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	
	if (!(flags.UpKeyPressed && flags.RightKeyPressed && flags.LeftKeyPressed && flags.DownKeyPressed))
	{
		if (key == GLFW_KEY_UP && action == GLFW_PRESS)
		{
			if (bot->move_up())
				bot->AddScore();
			flags.UpKeyPressed = true;
			cout << "Position Num: " << bot->getCurrentPosNum() << endl;
			cout << "Coordinates: x = " << bot->getPos()[0] << "y =" << bot->getPos()[1] << endl;
			cout << "Distance to Goal: " << bot->getDistance(bot->getCurrentPosNum(), 36) << endl;
			cout << "Your score till now: " << bot->GetScore();
		}
		else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
		{
			if(bot->move_left())
				bot->AddScore();
			flags.LeftKeyPressed = true;
			cout << "Position Num: " << bot->getCurrentPosNum() << endl;
			cout << "Coordinates: x = " << bot->getPos()[0] << "y =" << bot->getPos()[1] << endl;
			cout << "Distance to Goal: " << bot->getDistance(bot->getCurrentPosNum(), 36) << endl;
			cout << "Your score till now: " << bot->GetScore();
		}
		else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
		{
			if(bot->move_right())
				bot->AddScore();
			flags.RightKeyPressed = true;
			cout << "Position Num: " << bot->getCurrentPosNum() << endl;
			cout << "Coordinates: x = " << bot->getPos()[0] << "y =" << bot->getPos()[1] << endl;
			cout << "Distance to Goal: " << bot->getDistance(bot->getCurrentPosNum(), 36) << endl;
			cout << "Your score till now: " << bot->GetScore();
		}
		else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
		{
			if (bot->move_down())
				bot->AddScore();
			flags.DownKeyPressed = true;
			cout << "Position Num: " << bot->getCurrentPosNum() << endl;
			cout << "Coordinates: x = " << bot->getPos()[0] << "y =" << bot->getPos()[1] << endl;
			cout << "Distance to Goal: " << bot->getDistance(bot->getCurrentPosNum(), 36) << endl;
			cout << "Your score till now: " << bot->GetScore();
		}
	}
	else
	{
		if (key == GLFW_KEY_UP && action == GLFW_RELEASE)
		{
			flags.UpKeyPressed = false;
		}
		else if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE)
		{
			flags.LeftKeyPressed = false;
		}
		else if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE)
		{
			flags.RightKeyPressed = false;
		}
		else if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE)
		{
			flags.DownKeyPressed = false;
		}
	}
}

void mouse_clicked(GLFWwindow* window, int button, int action, int mod)
{
	left_mouse_button = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1);
	right_mouse_button = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2);
}



//=====================================================================================================================
// Main ---------------------------------------------------------------------------------------------------------------
//=====================================================================================================================
int main()
{
	using namespace std::this_thread;
	using namespace std::chrono;

	// GLFW initialization --------------------------------------------------------------------------------------------
	glfwInit();

	// setting window hints aka OpenGL version and profile
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// setting up the window and error handling
	GLFWwindow* window = glfwCreateWindow(windowSize[0], windowSize[1], "Pathfinder", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "window failed to Initialize";
		return -1;
	}

	// setting the window as OpendGl's current context
	glfwMakeContextCurrent(window);

	//Turning VSync Off! :/
	glfwSwapInterval(0);

	// glad loading error handling
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// FreeType Init --------------------------------------------------------------------------------------------------
	//FT_Library ftlib;

	//if (FT_Init_FreeType(&ftlib))
	//{
	//	cout << "ERROR::FreeType: FreeType library failed to initialize." << endl;
	//	return -1;
	//}

	//FT_Face ftface;

	//if (FT_New_Face(ftlib, "fonts/arial.ttf", 0, &ftface))
	//{
	//	cout << "ERROR::FreeType: Failed to load font." << endl;
	//	return -1;
	//}

	// CallBacks ------------------------------------------------------------------------------------------------------
	// updating viewport size if window size is changed CallBack
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, cursor_pos_callBack);
	glfwSetMouseButtonCallback(window, mouse_clicked);
	glfwSetKeyCallback(window, key_callback);

	// Shader Compilation ---------------------------------------------------------------------------------------------
	Shader GridShader;

	// Objects Declaration --------------------------------------------------------------------------------------------
	GraphicalObj GridImage;
	GridImage.ShaderSetup(GridShader, "./Textures/Grid.png");
	
	Colors color;
	bool flag{true};
	
	bot->Init();
	bot->setColor(color.MayiBleu);

	// Program Loop ---------------------------------------------------------------------------------------------------
	vector<int> DepthFirstSteps = bot->SolveDepthFirst(1, 36);
	bot->CalcScore(DepthFirstSteps);
	cout << endl << "The path score is equal to: " << bot->GetScore() << endl;
	cout << "The search score is equal to: " << bot->GetSearchScore() << endl;
	bot->ResetScore();

	vector<int> BreadthFirstSteps = bot->SolveBreadthFirst(1, 36, true);
	bot->CalcScore(BreadthFirstSteps);
	cout << endl << "The path score is equal to: " << bot->GetScore() << endl;
	cout << "The search score is equal to: " << bot->GetSearchScore() << endl;
	bot->ResetScore();

	vector<int> GreedySteps = bot->SolveGreedy(1, 36);
	bot->move_to_begin();
	bot->CalcScore(GreedySteps);
	cout << endl << "The path score is equal to: " << bot->GetScore() << endl;
	cout << "The search score is equal to: " << bot->GetSearchScore() << endl;
	bot->ResetScore();

	vector<int> ASteps = bot->SolveAStar(1, 36);
	bot->move_to_begin();
	bot->CalcScore(ASteps);
	cout << endl << "The path score is equal to: " << bot->GetScore() << endl;
	cout << "The search score is equal to: " << bot->GetSearchScore() << endl;
	bot->ResetScore();

	vector<int> LocalSteps = bot->SolveLocalSearch(1, 36);
	bot->move_to_begin();
	bot->CalcScore(LocalSteps);
	cout << endl << "The path score is equal to: " << bot->GetScore() << endl;
	cout << "The search score is equal to: " << bot->GetSearchScore() << endl;

	vector<int> steps = LocalSteps;
	int posnum{ 0 }, num{ 0 }, currentnum{0};
	bool seqflag{ true };

	cout << endl;
	while (!glfwWindowShouldClose(window))
	{
		// GLFW background setup
		
		process_input(window);

		glfwSwapBuffers(window);
		glfwPollEvents();
		glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		
		// Grid Render
		GridImage.transform(glm::vec3(-2.0, 2.0, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), M_PI);
		GridImage.DrawShape();

		// bot
		bot->render();


		// Animating the bot with a given path sequence
		if (seqflag)
		{
			cout << "sequencing ..." << endl;
			posnum = steps.at(num);
			currentnum = bot->getCurrentPosNum();

			if (bot->getCurrentPosNum() == 36)
			{
				seqflag = false;
				bot->move_to_begin();
			}

			sleep_for(milliseconds(200));
			bot->render();
			if (posnum - currentnum == 1)
			{
				bot->move_right();
			}
			else if (posnum - currentnum == -1)
			{
				bot->move_left();
			}
			else if (posnum - currentnum == 6)
			{
				bot->move_up();
			}
			else if (posnum - currentnum == -6)
			{
				bot->move_down();
			}

			if (num < steps.size() - 1)
				++num;

		}

	}

	// Unbinding and closing all glfw windows and clearing opbjects
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glfwTerminate();
	return 0;
}