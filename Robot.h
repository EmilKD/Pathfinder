#pragma once
#include<array>
#include<algorithm>
#include<math.h>
#include<chrono>
#include<thread>
#include"Graphics.h"
#include <random>

using std::array, std::cout, std::endl;

struct Grid
{
	int GridSize{ 6 };
	vector<int> BlockedCells{ 8,10,12,14,21,22,25,29 };
	vector<int> PenalizedCells{ 3,15,19,23,26,33 };
};

inline Grid grid;



#define GRID_STEP static_cast<float>(1.0 / grid.GridSize * 2.0)


class robot
{
public:

	//robot();
	//~robot();
	
	void Init();
	bool move_right();
	bool move_left();
	bool move_up();
	bool move_down();
	bool check_right(int PosToCheck);
	bool check_left(int PosToCheck);
	bool check_up(int PosToCheck);
	bool check_down(int PosToCheck);
	void move_to_begin();
	void render();
	void setColor(glm::vec3 color);
	array<float,2> getPos();
	int getCurrentPosNum();
	vector<int> getValidNeighors(int PosToCheck);
	float getDistance(int pos1 = -1, int pos2 = -1);
	float getCostToGoal();
	void PlaySequence(vector<int> seq);
	void CalcScore(vector<int> seq);
	int GetScore();
	int GetUserScore();
	int GetSearchScore();
	void AddScore();
	void ResetScore();
	void ResetUserScore();

	// Solvers
	vector<int> SolveDepthFirst(int begin, int end);
	vector<int> SolveBreadthFirst(int begin, int end, bool DebugPrintingOn = false);
	vector<int> SolveGreedy(int begin, int end);
	vector<int> SolveAStar(int begin, int end);
	vector<int> SolveLocalSearch(int begin, int end);

private:
	Shader RobotShader;
	GraphicalObj bot;
	glm::vec3 color{1.0f,1.0f,1.0f};

	vector<float> robotVertices{
		// Postition             Colors                   Texture Coords
		0.1f - GRID_STEP/2, 0.1f - GRID_STEP/2, 0.0f,        0.0f, 0.0f, 0.0f,        1.0f, 1.0f,          //top right
		0.1f - GRID_STEP/2, -0.1f - GRID_STEP/2, 0.0f,       0.0f, 0.0f, 0.0f,        1.0f, 0.0f,          //bottom right
		-0.1f - GRID_STEP/2, -0.1f - GRID_STEP/2, 0.0f,      0.0f, 0.0f, 0.0f,        0.0f, 0.0f,          //bottom left
		-0.1f - GRID_STEP/2, 0.1f - GRID_STEP/2, 0.0f,       0.0f, 0.0f, 0.0f,      0.0f, 1.0f           //top left
	};

	int score{ 0 };
	int SearchScore{ 0 };
	int UserScore{ 0 };
	int CurrentPosNum{ 15 };
	array<float, 2> CurrentPos{ 0.0f, 0.0f };
};

