#include"Robot.h"

void robot::Init() 
{
	bot.ShaderSetup(RobotShader, "./Textures/GlowDotFilled.png");
	bot.VertexUpdate(&robotVertices, nullptr);
	
	move_down();
	move_down();
	move_left();
	move_left();
	
	CurrentPosNum = 1;
	cout << "bot positioned at 1" << endl;
}

void robot::render()
{
	bot.transform({ 1.0f,1.0f,1.0f }, { CurrentPos[0], CurrentPos[1], 0.0f });
	bot.DrawShape(color);
}

bool robot::move_right()
{
	if ((CurrentPosNum % 6 != 0) && std::find(grid.BlockedCells.begin(), grid.BlockedCells.end(), CurrentPosNum + 1) == grid.BlockedCells.end())
	{
		CurrentPos[0] += GRID_STEP;
		CurrentPosNum++;
		return true;
	}
	else
		return false;
}

bool robot::move_left()
{
	if ((CurrentPosNum % 6 != 1) && std::find(grid.BlockedCells.begin(), grid.BlockedCells.end(), CurrentPosNum - 1) == grid.BlockedCells.end())
	{
		CurrentPos[0] -= GRID_STEP;
		CurrentPosNum--;
		return true;
	}
	else
		return false;
}

bool robot::move_up()
{
	if ((CurrentPosNum < 31) && std::find(grid.BlockedCells.begin(), grid.BlockedCells.end(), CurrentPosNum + 6) == grid.BlockedCells.end())
	{
		CurrentPos[1] += GRID_STEP;
		CurrentPosNum += 6;
		return true;
	}
	else
		return false;
}

bool robot::move_down()
{
	if ((CurrentPosNum >= 7) && std::find(grid.BlockedCells.begin(), grid.BlockedCells.end(), CurrentPosNum - 6) == grid.BlockedCells.end())
	{
		CurrentPos[1] -= GRID_STEP;
		CurrentPosNum -= 6;
		return true;
	}
	else
		return false;
	
}

bool robot::check_right(int PosToCheck)
{
	if ((PosToCheck % 6 != 0) && std::find(grid.BlockedCells.begin(), grid.BlockedCells.end(), PosToCheck + 1) == grid.BlockedCells.end())
	{
		return true;
	}
	else
		return false;
}

bool robot::check_left(int PosToCheck)
{
	if ((PosToCheck % 6 != 1) && std::find(grid.BlockedCells.begin(), grid.BlockedCells.end(), PosToCheck - 1) == grid.BlockedCells.end())
	{
		return true;
	}
	else
		return false;
}

bool robot::check_up(int PosToCheck)
{
	if ((PosToCheck < 31) && std::find(grid.BlockedCells.begin(), grid.BlockedCells.end(), PosToCheck + 6) == grid.BlockedCells.end())
	{
		return true;
	}
	else
		return false;
}

bool robot::check_down(int PosToCheck)
{
	if ((PosToCheck >= 7) && std::find(grid.BlockedCells.begin(), grid.BlockedCells.end(), PosToCheck - 6) == grid.BlockedCells.end())
	{
		return true;
	}
	else
		return false;

}

void robot::move_to_begin()
{
	CurrentPosNum = 1;
	CurrentPos = { -0.666f, -0.666f };
}

void robot::setColor(glm::vec3 NewColor)
{
	color = NewColor;
}

array<float, 2> robot::getPos() 
{
	return CurrentPos;
}

int robot::getCurrentPosNum()
{
	return CurrentPosNum;
}

vector<int> robot::getValidNeighors(int PosToCheck)
{
	vector<int> neighbors;

	if (check_right(PosToCheck))
		neighbors.push_back(PosToCheck + 1);
	if (check_left(PosToCheck))
		neighbors.push_back(PosToCheck - 1);
	if (check_up(PosToCheck))
		neighbors.push_back(PosToCheck + 6);
	if (check_down(PosToCheck))
		neighbors.push_back(PosToCheck - 6);

	std::sort(neighbors.begin(), neighbors.end());
	return neighbors;
}

float robot::getDistance(int pos1, int pos2) 
{
	if (pos1 == -1)
	{
		pos1 = CurrentPosNum;
		pos2 = 36;
	}
	
	float x1, y1, x2, y2;
	x1 = GRID_STEP * ((pos1-1) % grid.GridSize - 2); 
	y1 = GRID_STEP * (static_cast<int>((pos1-1) / grid.GridSize) - 2);
	x2 = GRID_STEP * ((pos2 - 1) % grid.GridSize - 2);
	y2 = GRID_STEP * (static_cast<int>((pos2 - 1) / grid.GridSize) - 2);
	//cout << "Distance to Goal: x = " << x << ", y = " << y <<endl;
	return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
	
	return -1.0f;
}

float robot::getCostToGoal()
{
	// Todo
	return 0.0f;
}

//-----------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------- Search Algorithms -----------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------
// Disclaimers:
// 
// * All following algorithms were developed by Me (Emil) and no pseudocode or code snippets were used in the development process 
// * These algorithms are far far away from being optimised and are only for demonstration purposes


// Depth-First Search Algorithm
vector<int> robot::SolveDepthFirst(int begin, int end)
{
	// Depth-First search algorithm assumptions: 
	// * Each time in the intersections the neighboring cell with the lowest cell number is chosen
	// * Upon reaching the end goal the search operation is terminated and alternate paths are not considered although implemenation of the said system is trivial

	SearchScore = 0;
	score = 0;

	int next_cell{ begin }, i{ 0 }, n{ 0 }, counter{ 0 }, SearchCost{ 0 }, temp_cell, crnt_cell;
	vector<int> neighbors, PassedCells, path;
	bool EndCellReached{ false }, stuck{ false };

	cout << endl << "--------------------------------------------------------------------------" << endl;
	cout << "Depth-First Algorithm Results:" << endl << endl;
	
	while (SearchCost < 200 && !EndCellReached)
	{
		crnt_cell = next_cell;
		neighbors = getValidNeighors(crnt_cell);	
		std::sort(neighbors.begin(), neighbors.end());
		
		if (neighbors.size() == 1)
		{
			stuck = true;
			PassedCells.push_back(crnt_cell);
		}

			else if (!stuck)
			{
				for (int n : neighbors)
				{
						if (n == end)
						{
							if (std::find(PassedCells.begin(), PassedCells.end(), crnt_cell) == PassedCells.end())
								PassedCells.push_back(crnt_cell);
							next_cell = n;
							path.push_back(next_cell);
							EndCellReached = true;
							counter = 0;
							break;
						}

						else if (std::find(PassedCells.begin(), PassedCells.end(), n) == PassedCells.end())
						{
							if (std::find(PassedCells.begin(), PassedCells.end(), crnt_cell) == PassedCells.end())
								PassedCells.push_back(crnt_cell);
							next_cell = n;
							path.push_back(next_cell);
							counter = 0;
							break;
						}
				}
				if (crnt_cell == next_cell) 
				{
					stuck = true;
					if (std::find(PassedCells.begin(), PassedCells.end(), crnt_cell) == PassedCells.end())
						PassedCells.push_back(crnt_cell);
				}
			}

			if (stuck)
			{
				for (int n : neighbors)
				{
					if (std::find(PassedCells.begin(), PassedCells.end(), n) == PassedCells.end())
					{
						stuck = false;
						if (std::find(PassedCells.begin(), PassedCells.end(), crnt_cell) == PassedCells.end())
							PassedCells.push_back(crnt_cell);
						next_cell = n;
						path.push_back(next_cell);
						break;
					}
				
				}
			
				if (counter <= PassedCells.size() && stuck)
				{
					next_cell = *(PassedCells.end() - (counter + 2));
					counter++;
					path.pop_back();
				}
			
			}

			// Printing each step for debugging purposes
			cout << "Current Cell: " << crnt_cell << " Next Cell: " << next_cell << endl;
			cout << "Passed Cells: ";
			for (int i : PassedCells) cout << i << " ";
			cout << endl << endl;

			// Calculating the search score
			SearchCost++;
		}

		SearchScore = SearchCost;

		cout << "Found Path: ";
		for (int num : path) cout << " -> " << num;
		cout << endl;
		return path;
	}

// Breadth-First Search Algorithm
vector<int> robot::SolveBreadthFirst(int begin, int end, bool DebugPrintingOn)
{
	int i{ 0 }, j{ 0 }, CellParent;
	vector<int> neighbors, path, tempPath, PassedCells;
	vector<vector<int>> AllPaths;
	vector<array<int,2>> NextLayer, CrntLayer, flaggedCells;
	vector<vector<array<int, 2>>> AllLayers;
	bool EndCellReached{ false }, AllPathsNotFound{ true };

	NextLayer.push_back({ begin, begin });
	PassedCells.push_back(begin);

	if (DebugPrintingOn)
		cout << endl << "--------------------------------------------------------------------------" << endl << "Breadth-First Algorithm Results:" << endl << endl;

	while (!EndCellReached)
	{
		CrntLayer = NextLayer;

		NextLayer.clear();
		for (array<int,2> cell: CrntLayer)
		{
			neighbors = getValidNeighors(cell[0]);
			//std::sort(neighbors.begin(), neighbors.end());

			for (int n : neighbors)
			{
				if (cell[0] == end)
				{
					EndCellReached = true;
				
					// Extracting all paths
					PassedCells.clear();
					CrntLayer.clear();
					NextLayer.clear();
					CellParent = end;
					path.insert(path.begin(), end);

					for (i = AllLayers.size() - 1; i >= 0; i--)
					{
						CrntLayer = AllLayers.at(i);

						for (array<int, 2> cell : CrntLayer)
						{
							if (cell[0] == CellParent && std::find(flaggedCells.begin(), flaggedCells.end(), cell) == flaggedCells.end())
							{
								CellParent = cell[1];
								path.insert(path.begin(), CellParent);
							}

							SearchScore++;
						}
					}
					// For debugging

					if (DebugPrintingOn)
					{
						cout << endl << "Found Path: ";
						for (int num : path) cout << " -> " << num;
						cout << endl;
					}
					
					return path;
				}

				if (std::find(PassedCells.begin(), PassedCells.end(), n) == PassedCells.end() && std::find(NextLayer.begin(), NextLayer.end(), array { n,cell[0]}) == NextLayer.end())
					NextLayer.push_back({ n,cell[0]});
			}

			AllLayers.push_back(NextLayer);
		}

		for (array<int,2> n : NextLayer)
		{
			if (n[0] > 0)
			{
				if (std::find(PassedCells.begin(), PassedCells.end(), n[0]) == PassedCells.end())
				{
					PassedCells.push_back(n[0]);
				}
			}
		}

		// For debugging
		if (DebugPrintingOn)
		{
			for (array<int, 2> n : NextLayer)
				cout << n[0] << "," << n[1] << " ";
			cout << endl;
		}
		
	}

	
}

// Greedy Search Algorithm
vector<int> robot::SolveGreedy(int begin, int end)
{
	SearchScore = 0;
	score = 0;
	int tries{0}, oldNum{ 0 }, nextNum;
	float dist{ 0.0f }, LowestDist{ 100.0f };
	vector<int> steps;

	cout << endl << "--------------------------------------------------------------------------" << endl;
	cout << "Greedy Algorithm Results:" << endl << endl;

	while (CurrentPosNum != 36 || tries > 100) 
	{
		vector neighbors = getValidNeighors(CurrentPosNum);
		tries++;

		for (int i : neighbors)
		{
			dist = getDistance(i, end);

			if (dist < LowestDist)
			{
				LowestDist = dist;
				nextNum = i;
			}
			else if (dist == LowestDist)
			{
				nextNum = i > oldNum ? i : oldNum;
			}
			SearchScore++;
		}

		steps.push_back(nextNum);

		if (nextNum - CurrentPosNum == 1)
		{
			move_right();
		}
		else if (nextNum - CurrentPosNum == -1)
		{
			move_left();
		}
		else if (nextNum - CurrentPosNum == 6)
		{
			move_up();
		}
		else if (nextNum - CurrentPosNum == -6)
		{
			move_down();
		}

		if (CurrentPosNum == 36)
		{
			cout << "Found Path: ";
			for (int num : steps) cout << " -> " << num;
			cout << endl;
			return steps;
		}
		
	}
}

vector<int> robot::SolveAStar(int begin, int end) 
{

	int CrntCell{ begin }, NextNum, NextCell{ begin }, counter{ 0 };
	float g{ 0 }, h{ 0 }, TotalScore{ 1000 };
	vector<int> neighbors, pathtillnow, path, PassedCells;
	bool GoalReached{ false };

	// Notice! The heuristic only considers right and up moves in estimating valid paths
	cout << endl << "--------------------------------------------------------------------------" << endl;
	cout << "A* Algorithm Results:" << endl << endl;

	while (!GoalReached && counter < 100) 
	{
		CrntCell = NextCell;
		neighbors = getValidNeighors(CrntCell);

		for (int cell : neighbors)
		{
			if (std::find(PassedCells.begin(), PassedCells.end(), cell)==PassedCells.end())
			{
				if (cell == end)
				{
					NextCell = cell;
					path.push_back(NextCell);
					cout << "Found Path: ";
					for (int num : path) cout << " -> " << num;
					return path;
				}
				
				CalcScore(SolveBreadthFirst(cell, end));
				h = this->score;

				path.push_back(cell);
				CalcScore(path);
				g = score;
				path.pop_back();

				if (TotalScore > g + h)
				{
					TotalScore = g + h;
					NextCell = cell;
				}

				PassedCells.push_back(cell);
			}
			score = 0;
		}

		TotalScore = 1000;
		path.push_back(NextCell);
		counter++;
	}
}

vector<int> robot::SolveLocalSearch(int begin, int end) 
{
	vector<int> path, neighbors, PassedCells, NonPassedNeighbors;
	vector<float> costs, chances;
	int NextCell{ begin }, counter{ 0 }, backprop{ 1 };
	float sum{ 0 }, p{ 0 };
	bool CellFound{ false };

	cout << endl << "--------------------------------------------------------------------------" << endl;
	cout << "Local Search Algorithm Results:" << endl << endl;

	PassedCells.push_back(begin);

	while(counter < 100)
	{
		NonPassedNeighbors.clear();
		costs.clear();
		chances.clear();
		p = 0;
		sum = 0;
		CellFound = false;

		if (NextCell == end) 
		{
			cout << "Found Path: ";
			for (int num : path) cout << " -> " << num;
			return path;
		}

		neighbors = getValidNeighors(NextCell);

		for (int cell : neighbors)
		{
			if(std::find(PassedCells.begin(), PassedCells.end(), cell)==PassedCells.end())
			{
				score = 0;
				CalcScore({ cell });
				cout << "Cell: " << cell << " Score: " << score << endl;
				costs.push_back(float(1.0 / score));
				sum += float(1.0 / score);
				NonPassedNeighbors.push_back(cell);
				CellFound = true;
				backprop = 1;
			}
			SearchScore++;
		}

		if(!CellFound)
		{
			if (std::find(PassedCells.begin(), PassedCells.end(), NextCell) == PassedCells.end())
				PassedCells.push_back(NextCell);
			NextCell = *(PassedCells.rbegin() + backprop);
			path.push_back(NextCell);
			counter++;
			backprop++;
			continue;
		}

		for (int i = 0; i < costs.size(); i++)
		{
			if (std::find(NonPassedNeighbors.begin(), NonPassedNeighbors.end(), neighbors.at(i)) != NonPassedNeighbors.end())
			{				
				chances.push_back((1.0f / costs[i]) / sum);
			}
			SearchScore++;
		}
		
		std::mt19937 gen(std::random_device{}());		
		std::discrete_distribution<> d{ chances.begin(), chances.end() };

		PassedCells.push_back(NextCell);
		NextCell = NonPassedNeighbors.at(d(gen));

		path.push_back(NextCell);
		counter++;
	}
}

void robot::PlaySequence(vector<int> seq) 
{
	using namespace std::this_thread;
	using namespace std::chrono;

	double time{ 0.0 };
	std::chrono::duration<double> duration{0.0};

	for(int i: seq)
	{

		//auto start = std::chrono::high_resolution_clock::now();
		
		if (duration.count() > 1)
		{
			if (i - CurrentPosNum == 1)
			{
				move_right();
			}
			else if (i - CurrentPosNum == -1)
			{
				move_left();
			}
			else if (i - CurrentPosNum == 6)
			{
				move_up();
			}
			else if (i - CurrentPosNum == -6)
			{
				move_down();
			}

		}

		//auto end = std::chrono::high_resolution_clock::now();
		//duration = end - start;

		//sleep_for(seconds(1));
	}

}

void robot::CalcScore(vector<int> seq) 
{
	int TempScore{ 0 };

	if (seq.size() > 0)
	{
		for (int i : seq)
		{
			if (std::find(grid.PenalizedCells.begin(), grid.PenalizedCells.end(), i) == grid.PenalizedCells.end())
				TempScore++;
			else
				TempScore+=2;
		}
		score += TempScore;
	}
}

int robot::GetScore()
{
	return score;
}

int robot::GetUserScore()
{
	return UserScore;
}

int robot::GetSearchScore()
{
	return SearchScore;
}

void robot::AddScore()
{
	if (std::find(grid.PenalizedCells.begin(), grid.PenalizedCells.end(), CurrentPosNum) == grid.PenalizedCells.end())
		UserScore++;
	else
		UserScore+=2;
}

void robot::ResetScore()
{
	SearchScore = 0;
	score = 0;
}

void robot::ResetUserScore()
{
	UserScore = 0;
}