#include <iostream>
#include <vector>
#include <queue>
#include <utility>

using namespace std;

const int ROWS = 5;    // Set maze size
const int COLS = 5;
int maze[ROWS][COLS] = { // Define maze as a 2D grid (0 = path, 1 = wall)
    {0, 1, 0, 0, 0},
    {0, 1, 0, 1, 0},
    {0, 0, 0, 1, 0},
    {0, 1, 1, 0, 1},
    {0, 0, 0, 0, 0}
};

struct Node {
    int x, y, dist;
};

// Direction vectors for movement (up, down, left, right)
int dx[] = {-1, 1, 0, 0};
int dy[] = {0, 0, -1, 1};

// BFS function for shortest pathfinding in maze
bool bfs(vector<vector<int>>& maze, int startX, int startY, int endX, int endY) {
    vector<vector<bool>> visited(ROWS, vector<bool>(COLS, false));
    queue<Node> q;
    q.push({startX, startY, 0});
    visited[startX][startY] = true;

    while (!q.empty()) {
        Node current = q.front();
        q.pop();

        // Check if we reached the destination
        if (current.x == endX && current.y == endY) {
            cout << "Shortest path distance: " << current.dist << endl;
            return true;
        }

        // Explore neighbors
        for (int i = 0; i < 4; i++) {
            int nx = current.x + dx[i];
            int ny = current.y + dy[i];

            if (nx >= 0 && ny >= 0 && nx < ROWS && ny < COLS && maze[nx][ny] == 0 && !visited[nx][ny]) {
                visited[nx][ny] = true;
                q.push({nx, ny, current.dist + 1});
            }
        }
    }

    cout << "No path found!" << endl;
    return false;
}

int main() {
    int startX = 0, startY = 0;
    int endX = 4, endY = 4;
    
    if (bfs(maze, startX, startY, endX, endY)) {
        cout << "Path found in the maze." << endl;
    } else {
        cout << "Path does not exist." << endl;
    }

    return 0;
}
