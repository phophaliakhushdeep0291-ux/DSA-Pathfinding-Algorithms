#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <climits>
using namespace std;

// Node structure
struct Node {
    int x, y;
    Node* parent;
    int dist; // for Dijkstra
    int f, g, h; // for A*
    Node(int _x, int _y) : x(_x), y(_y), parent(nullptr), dist(INT_MAX), f(0), g(0), h(0) {}
};

int dx[4] = {-1, 1, 0, 0};
int dy[4] = {0, 0, -1, 1};

// Check if cell is valid
bool isValid(int x, int y, vector<vector<int>>& maze) {
    return x >= 0 && x < maze.size() && y >= 0 && y < maze[0].size() && maze[x][y] == 0;
}

// Print maze with optional path
void printMaze(vector<vector<int>>& maze, Node* start, Node* end, vector<Node*> path = {}) {
    for (int i = 0; i < maze.size(); i++) {
        for (int j = 0; j < maze[0].size(); j++) {
            if (i == start->x && j == start->y) cout << "S ";
            else if (i == end->x && j == end->y) cout << "E ";
            else {
                bool isPath = false;
                for (auto n : path) if (n->x == i && n->y == j) isPath = true;
                if (isPath) cout << "* ";
                else if (maze[i][j] == 1) cout << "# ";
                else cout << ". ";
            }
        }
        cout << endl;
    }
}

// Maze generation using DFS (Backtracking)
void generateMaze(vector<vector<int>>& maze) {
    int ROWS = maze.size();
    int COLS = maze[0].size();
    vector<vector<bool>> visited(ROWS, vector<bool>(COLS, false));
    stack<pair<int,int>> st;
    int startX = 1, startY = 1;
    visited[startX][startY] = true;
    maze[startX][startY] = 0;
    st.push({startX, startY});

    srand(time(0));

    while (!st.empty()) {
        pair<int,int> p = st.top();
            int x = p.first;
            int y = p.second;

        vector<pair<int,int>> neighbors;
        for (int i = 0; i < 4; i++) {
            int nx = x + dx[i]*2;
            int ny = y + dy[i]*2;
            if (nx > 0 && nx < ROWS-1 && ny > 0 && ny < COLS-1 && !visited[nx][ny])
                neighbors.push_back({nx, ny});
        }

        if (!neighbors.empty()) {
            pair<int,int> np = neighbors[rand() % neighbors.size()];
                int nx = np.first;
                int ny = np.second;

            maze[(x+nx)/2][(y+ny)/2] = 0; // remove wall
            maze[nx][ny] = 0;
            visited[nx][ny] = true;
            st.push({nx, ny});
        } else {
            st.pop();
        }
    }
}

// BFS Algorithm
vector<Node*> BFS(vector<vector<int>>& maze, Node* start, Node* end, int &visitedCount) {
    int ROWS = maze.size();
    int COLS = maze[0].size();
    visitedCount = 0;
    queue<Node*> q;
    vector<vector<bool>> visited(ROWS, vector<bool>(COLS, false));
    q.push(start);
    visited[start->x][start->y] = true;
    visitedCount++;

    while (!q.empty()) {
        Node* curr = q.front(); q.pop();
        if (curr->x == end->x && curr->y == end->y) {
            vector<Node*> path;
            while (curr) {
                path.push_back(curr);
                curr = curr->parent;
            }
            reverse(path.begin(), path.end());
            return path;
        }

        for (int i = 0; i < 4; i++) {
            int nx = curr->x + dx[i];
            int ny = curr->y + dy[i];
            if (isValid(nx, ny, maze) && !visited[nx][ny]) {
                Node* next = new Node(nx, ny);
                next->parent = curr;
                visited[nx][ny] = true;
                visitedCount++;
                q.push(next);
            }
        }
    }
    return {};
}

// DFS Algorithm
vector<Node*> DFS(vector<vector<int>>& maze, Node* start, Node* end, int &visitedCount) {
    int ROWS = maze.size();
    int COLS = maze[0].size();
    visitedCount = 0;
    stack<Node*> st;
    vector<vector<bool>> visited(ROWS, vector<bool>(COLS, false));
    st.push(start);

    while (!st.empty()) {
        Node* curr = st.top(); st.pop();
        if (visited[curr->x][curr->y]) continue;
        visited[curr->x][curr->y] = true;
        visitedCount++;
        if (curr->x == end->x && curr->y == end->y) {
            vector<Node*> path;
            while (curr) {
                path.push_back(curr);
                curr = curr->parent;
            }
            reverse(path.begin(), path.end());
            return path;
        }
        for (int i = 0; i < 4; i++) {
            int nx = curr->x + dx[i];
            int ny = curr->y + dy[i];
            if (isValid(nx, ny, maze) && !visited[nx][ny]) {
                Node* next = new Node(nx, ny);
                next->parent = curr;
                st.push(next);
            }
        }
    }
    return {};
}

// Dijkstra Algorithm
vector<Node*> Dijkstra(vector<vector<int>>& maze, Node* start, Node* end, int &visitedCount) {
    int ROWS = maze.size();
    int COLS = maze[0].size();
    visitedCount = 0;
    vector<vector<int>> dist(ROWS, vector<int>(COLS, INT_MAX));
    dist[start->x][start->y] = 0;

    auto cmp = [](Node* a, Node* b){ return a->dist > b->dist; };
    priority_queue<Node*, vector<Node*>, decltype(cmp)> pq(cmp);
    start->dist = 0;
    pq.push(start);

    while (!pq.empty()) {
        Node* curr = pq.top(); pq.pop();
        visitedCount++;
        if (curr->x == end->x && curr->y == end->y) {
            vector<Node*> path;
            while (curr) {
                path.push_back(curr);
                curr = curr->parent;
            }
            reverse(path.begin(), path.end());
            return path;
        }

        for (int i = 0; i < 4; i++) {
            int nx = curr->x + dx[i];
            int ny = curr->y + dy[i];
            if (isValid(nx, ny, maze) && dist[curr->x][curr->y] + 1 < dist[nx][ny]) {
                Node* next = new Node(nx, ny);
                next->dist = dist[curr->x][curr->y] + 1;
                next->parent = curr;
                dist[nx][ny] = next->dist;
                pq.push(next);
            }
        }
    }
    return {};
}

// A* Algorithm
int heuristic(Node* a, Node* b) { return abs(a->x - b->x) + abs(a->y - b->y); }

vector<Node*> AStar(vector<vector<int>>& maze, Node* start, Node* end, int &visitedCount) {
    int ROWS = maze.size();
    int COLS = maze[0].size();
    visitedCount = 0;

    vector<vector<int>> gScore(ROWS, vector<int>(COLS, INT_MAX));
    gScore[start->x][start->y] = 0;

    auto cmp = [](Node* a, Node* b){ return a->f > b->f; };
    priority_queue<Node*, vector<Node*>, decltype(cmp)> openSet(cmp);
    start->g = 0;
    start->h = heuristic(start, end);
    start->f = start->g + start->h;
    openSet.push(start);

    while (!openSet.empty()) {
        Node* curr = openSet.top(); openSet.pop();
        visitedCount++;
        if (curr->x == end->x && curr->y == end->y) {
            vector<Node*> path;
            while (curr) { path.push_back(curr); curr = curr->parent; }
            reverse(path.begin(), path.end());
            return path;
        }

        for (int i = 0; i < 4; i++) {
            int nx = curr->x + dx[i];
            int ny = curr->y + dy[i];
            if (isValid(nx, ny, maze)) {
                int tentative_g = curr->g + 1;
                if (tentative_g < gScore[nx][ny]) {
                    Node* next = new Node(nx, ny);
                    next->g = tentative_g;
                    next->h = heuristic(next, end);
                    next->f = next->g + next->h;
                    next->parent = curr;
                    gScore[nx][ny] = tentative_g;
                    openSet.push(next);
                }
            }
        }
    }
    return {};
}

int main() {
    int rows, cols;
    cout << "Enter maze rows (odd number recommended): "; cin >> rows;
    cout << "Enter maze cols (odd number recommended): "; cin >> cols;
    vector<vector<int>> maze(rows, vector<int>(cols, 1));

    generateMaze(maze);
    Node* start = new Node(1, 1);
    Node* end = new Node(rows-2, cols-2);

    cout << "Choose algorithm: 1-BFS, 2-DFS, 3-Dijkstra, 4-A* : ";
    int choice; cin >> choice;

    int visitedCount = 0;
    vector<Node*> path;
    switch(choice) {
        case 1: path = BFS(maze, start, end, visitedCount); break;
        case 2: path = DFS(maze, start, end, visitedCount); break;
        case 3: path = Dijkstra(maze, start, end, visitedCount); break;
        case 4: path = AStar(maze, start, end, visitedCount); break;
        default: cout << "Invalid choice!"; return 0;
    }

    cout << "\nMaze:\n";
    printMaze(maze, start, end, path);
    cout << "\nPath length: " << path.size() << endl;
    cout << "Visited nodes: " << visitedCount << endl;

    return 0;
}