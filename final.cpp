#include <iostream>
#include <queue>
#include <vector>
#include <stack>

using namespace std;

void printMaze(const vector<vector<char>>& maze) {
    for (const auto& row : maze) {
        for (char cell : row) {
            cout << cell << ' ';
        }
        cout << endl;
    }
    cout << endl;
}

void printEscapePath(const vector<vector<int>>& personTime, int x, int y, const vector<vector<char>>& maze) {
    stack<pair<int, int>> path;
    int time = personTime[x][y];

    while (time > 0) {
        path.push({ x, y });

        int dx[] = { 0, 0, 1, -1 };
        int dy[] = { 1, -1, 0, 0 };

        for (int i = 0; i < 4; ++i) {
            int nx = x + dx[i];
            int ny = y + dy[i];

            if (nx >= 0 && nx < maze.size() && ny >= 0 && ny < maze[0].size() && personTime[nx][ny] == time - 1) {
                x = nx;
                y = ny;
                --time;
                break;
            }
        }
    }

    cout << "Escape Path:" << endl;
    while (!path.empty()) {
        cout << "(" << path.top().first << ", " << path.top().second << ") -> ";
        path.pop();
    }
    cout << "escape" << endl;
}

int main() {
    int R, C;
    cin >> R >> C;

    vector<vector<char>> maze(R, vector<char>(C));
    vector<vector<int>> personTime(R, vector<int>(C, -1));
    vector<vector<int>> fireTime(R, vector<int>(C, -1));

    int startX, startY;

    // 미로 입력 받기
    for (int i = 0; i < R; ++i) {
        for (int j = 0; j < C; ++j) {
            cin >> maze[i][j];
            if (maze[i][j] == 'M') {
                startX = i;
                startY = j;
                personTime[i][j] = 0;
            }
            else if (maze[i][j] == 'F') {
                fireTime[i][j] = 0;
            }
        }
    }

    // 초기 상태 출력
    cout << "Initial State:" << endl;
    printMaze(maze);

    int dx[] = { 0, 0, 1, -1 };
    int dy[] = { 1, -1, 0, 0 };
    queue<pair<int, int>> fireQueue, personQueue;

    for (int i = 0; i < R; ++i) {
        for (int j = 0; j < C; ++j) {
            if (maze[i][j] == 'F') {
                fireQueue.push({ i, j });
            }
            else if (maze[i][j] == 'M') {
                personQueue.push({ i, j });
            }
        }
    }

    int time = 0;

    while (!personQueue.empty()) {
        cout << "Time: " << time << endl;

        // 불의 확산
        int fireQSize = fireQueue.size();
        for (int i = 0; i < fireQSize; ++i) {
            int x = fireQueue.front().first;
            int y = fireQueue.front().second;
            fireQueue.pop();

            for (int j = 0; j < 4; ++j) {
                int nx = x + dx[j];
                int ny = y + dy[j];

                if (nx >= 0 && nx < R && ny >= 0 && ny < C && maze[nx][ny] != '#' && fireTime[nx][ny] == -1) {
                    fireTime[nx][ny] = time + 1;
                    fireQueue.push({ nx, ny });
                    if (maze[nx][ny] == '1') {
                        fireQueue.push({ nx + 1, ny });
                        fireQueue.push({ nx - 1, ny });
                        fireQueue.push({ nx , ny + 1 });
                        fireQueue.push({ nx, ny - 1 });
                    }
                    maze[nx][ny] = 'F';
                }
            }
        }

        // 사람의 이동
        int personQSize = personQueue.size();
        for (int i = 0; i < personQSize; ++i) {
            int x = personQueue.front().first;
            int y = personQueue.front().second;
            personQueue.pop();

            for (int j = 0; j < 4; ++j) {
                int nx = x + dx[j];
                int ny = y + dy[j];

                if (nx >= 0 && nx < R && ny >= 0 && ny < C && maze[nx][ny] == '0' && personTime[nx][ny] == -1 &&
                    (fireTime[nx][ny] == -1 || fireTime[nx][ny] > personTime[x][y] + 1)) {
                    personTime[nx][ny] = time + 1;
                    personQueue.push({ nx, ny });
                    maze[nx][ny] = 'M';
                }
                else if (nx < 0 || nx >= R || ny < 0 || ny >= C) {
                    // 미로의 가장자리에 도달하면 탈출 가능
                    cout << "Person escapes at: (" << x << ", " << y << ")" << endl;
                    printEscapePath(personTime, x, y, maze);
                    return 0;
                }
            }
        }

        // 현재 상태 출력
        printMaze(maze);

        ++time;
    }

    // 탈출이 불가능한 경우
    cout << "IMPOSSIBLE" << endl;

    return 0;
}
