#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <stack>
//TESTING
//TESTING

// OLC PIXEL FIXES
//Draw(x * (mnPathWidth+1) + px, y* (m_nPathWidth +1) + py, olc::WHITE);
//Draw(x * (mnPathWidth+1) + px, y* (m_nPathWidth +1) + py, olc::BLUE);



class OLC_Maze : public olc::PixelGameEngine
{
public:
	OLC_Maze()
	{
		sAppName = "Example";
	}
private: 
    int m_nMazeWidth;
    int m_nMazeHeight;
    int *m_maze;

    enum
    {
        CELL_PATH_N = 0x01,
        CELL_PATH_E = 0x02,
        CELL_PATH_S = 0x04,
        CELL_PATH_W = 0x08,
        CELL_VISITED = 0x10,
    };
    
    int m_nVisitedCells;
    
    std::stack<std::pair<int, int>> m_stack; 

    int m_nPathWidth;

public:
	bool OnUserCreate() override
	{
        // Maze parameters
		// Called once at the start, so create things here
		m_nMazeWidth = 40;
        m_nMazeHeight = 25;

        m_maze = new int[m_nMazeWidth * m_nMazeHeight];

        memset(m_maze, 0x00, m_nMazeWidth * m_nMazeHeight * sizeof(int));

        m_stack.push(std::make_pair(0,0));
        m_maze[0] = CELL_VISITED;
        m_nVisitedCells = 1;

        m_nPathWidth = 3;
        return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{

        auto offset = [&](int x, int y)
        {
            return (m_stack.top().second + y) * m_nMazeWidth + (m_stack.top().first + x);
        };
        // Maze Algorithm
        if (m_nVisitedCells < m_nMazeWidth * m_nMazeHeight)
        {
            // Create a set of unvisted neighbours
			std::vector<int> neighbours;

			// North neighbour
			if (m_stack.top().second > 0 && (m_maze[offset(0, -1)] & CELL_VISITED) == 0)
            {
				neighbours.push_back(0);
            }
			// East neighbour
			if (m_stack.top().first < m_nMazeWidth - 1 && (m_maze[offset(1, 0)] & CELL_VISITED) == 0)
				neighbours.push_back(1);
			// South neighbour
			if (m_stack.top().second < m_nMazeHeight - 1 && (m_maze[offset(0, 1)] & CELL_VISITED) == 0)
				neighbours.push_back(2);
			// West neighbour
			if (m_stack.top().first > 0 && (m_maze[offset(-1, 0)] & CELL_VISITED) == 0)
				neighbours.push_back(3);

            // Are there any neighbours available?
            if (!neighbours.empty())
            {
                // choose available neighbor at random
                int next_cell_dir = neighbours[rand() % neighbours.size()];

                // create path between neighbor and current cell
                switch (next_cell_dir) 
                {
                    case 0: //north
                        m_maze[offset(0, 0)] |= CELL_PATH_N;
                        m_maze[offset(0, -1)] |= CELL_PATH_S;
                        m_stack.push(std::make_pair((m_stack.top().first + 0), (m_stack.top().second - 1)));
                        break;
                    case 1: //east
                        m_maze[offset(0, 0)] |= CELL_PATH_E;
                        m_maze[offset(+1, 0)] |= CELL_PATH_W;
                        m_stack.push(std::make_pair((m_stack.top().first + 1), (m_stack.top().second + 0)));
                        break;

                    case 2: //south
                        m_maze[offset(0, 0)] |= CELL_PATH_S;
                        m_maze[offset(0, +1)] |= CELL_PATH_N;
                        m_stack.push(std::make_pair((m_stack.top().first + 0), (m_stack.top().second + 1)));
                        break;

                    case 3: //west
                        m_maze[offset(0, 0)] |= CELL_PATH_W;
                        m_maze[offset(-1, 0)] |= CELL_PATH_E;
                        m_stack.push(std::make_pair((m_stack.top().first -1), (m_stack.top().second + 0)));
                        break;
                        
                }

                // new cell
                m_maze[offset(0, 0)] |= CELL_VISITED;
                m_nVisitedCells++;
            }
            else 
            {
            m_stack.pop();
            }
			
        }
		// called once per frame
        FillRect(0, 0, ScreenWidth(), ScreenHeight(), olc::BLACK);

        // Draw maze
        for (int x=0; x<m_nMazeWidth; x++)
        {
            for (int y=0; y<m_nMazeHeight; y++)
            {
                for (int py = 0; py<m_nPathWidth; py++)
                {
                    for (int px=0; px<m_nPathWidth; px++) 
                    {
                        if (m_maze[y * m_nMazeWidth + x] & CELL_VISITED)
                            Draw(x * (m_nPathWidth +1) + px, y * (m_nPathWidth +1) + py, olc::WHITE);
                        else 
                            Draw(x * (m_nPathWidth +1) + px, y * (m_nPathWidth +1) + py , olc::BLUE);  
                    }
                }

                // Draw passageways between cells
				for (int p = 0; p < m_nPathWidth; p++)
				{
					if (m_maze[y * m_nMazeWidth + x] & CELL_PATH_S)
						Draw(x * (m_nPathWidth + 1) + p, y * (m_nPathWidth + 1) + m_nPathWidth); // Draw South Passage

					if (m_maze[y * m_nMazeWidth + x] & CELL_PATH_E)
						Draw(x * (m_nPathWidth + 1) + m_nPathWidth, y * (m_nPathWidth + 1) + p); // Draw East Passage
				}
                
            }
        }


		return true;
	}
};


int main()
{
	OLC_Maze maze;
	if (maze.Construct(160, 100, 8, 8))
		maze.Start();

	return 0;
}
