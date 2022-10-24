#include<vector>
#include<algorithm>
#include<queue>
#include<unordered_map>
#include<iostream>

struct Point{
    int x, y;
    int f = 0, g = 0, h = 0;
    Point* parentNode = nullptr;

    void setFGH(int _f, int _g, int _h){
        this->f = _f;
        this->g = _g;
        this->h = _h;
    }

    int operator - (const Point& a) const {
        int _x = abs(this->x - a.x);
        int _y = abs(this->y - a.y);
        return _x + _y;
    }

    bool operator < (Point &a) const {
        return this->f < a.f;
    }

    bool operator == (Point &a) const {
        return a.x == this->x && a.y == this->y;
    }

    bool operator != (Point &a) const {
        return !(*this == a);
    }

    Point(int _x, int _y) : x(_x), y(_y){};
};

struct cmp_Point{
    bool operator()(const Point* a, const Point* b){
        return a->f > b->f;
    }
};

struct hash_Point
{
    size_t operator()(const Point& a){
        auto hash1 = std::hash<int>{}(a.x);
        auto hash2 = std::hash<int>{}(a.y);
        return hash1 ^ hash2;
    }
};

// 1标识障碍物
class AStar{
public:
    std::vector<Point*> operator()(const std::vector<std::vector<int>> map, Point* begin, Point* end){
        int m = map.size();
        if(m < 1) return {};
        int n = map[0].size();
        if(n < 1) return {};

        if(begin->x < 0 || begin->x >= m || begin->y < 0 || begin->y >= n || end->x < 0 || end->x >= m || end->y < 0 || end->y >= n){
            std::cout << "起点或终点不合法，寻路失败。" << std::endl;
            return {};
        }

        if(map[begin->x][begin->y] == 1 || map[end->x][end->y] == 1){
            std::cout << "起点或终点为障碍物，寻路失败。" << std::endl;
            return {};
        }

        // 未遍历过节点的优先队列
        std::priority_queue<Point*, std::vector<Point*>, cmp_Point> openList;
        // 存储此处的节点信息
        std::vector<std::vector<Point*>> visited(m, std::vector<Point*>(n, nullptr));

        int around[8][3] = {{-1,0,10},{-1,1,14},{0,1,10},{1,1,14},{1,0,10},{1,-1,14},{0,-1,10},{-1,-1,14}};// 左 左上 上 右上 右 右下 下 左下
        
        int g = 0, h = (*begin - *end) * 10;
        int f = g + h;
        begin->setFGH(f, g, h);
        visited[begin->x][begin->y] = begin;

        // 用于退出的标识
        bool seek = true;
        openList.push(begin);
        Point* cur = nullptr;
        while(seek && !openList.empty()){
            cur = openList.top();
            openList.pop();

            for(int i = 0; seek && i < sizeof(around) / sizeof(around[0]); ++i){
                int newX = cur->x + around[i][0];
                int newY = cur->y + around[i][1];
                // 找没访问过的节点
                if(newX < 0 || newX >= m || newY < 0 || newY >= n || map[newX][newY] == 1 || visited[newX][newY] != nullptr) continue;

                Point* newPoint = new Point(newX, newY);
                int g = cur->g + around[i][2];
                int h = (*newPoint - *end) * 10;
                int f = g + h;
                int minF = f;

                // 找最近的父节点计算f
                for(int j = 0; j < sizeof(around) / sizeof(around[0]); ++j){
                    int _newX = newPoint->x + around[j][0];
                    int _newY = newPoint->y + around[j][1];
                    // 找访问过的节点作为newPoint的父节点 'newPoint'的'g'表示为'父节点的g + 走一格的g'
                    if(_newX < 0 || _newX >= m || _newY < 0 || _newY >= n || map[_newX][_newY] == 1 || visited[_newX][_newY] == nullptr) continue;

                    // 找总路径最小的作为父节点
                    Point* pNode = visited[_newX][_newY];
                    g = pNode->g + around[j][2];
                    f = g + h;
                    if(f <= minF){
                        minF = f;
                        newPoint->parentNode = pNode;
                        newPoint->setFGH(f, g, h);
                    }
                }

                // 记录已有节点信息，并压入待遍历队列
                visited[newPoint->x][newPoint->y] = newPoint;
                openList.push(newPoint);

                // 终点可以退出
                if(*newPoint == *end){
                    seek = false;
                }
            }
        }

        // 从终点往回找父节点，然后输出
        Point *p = visited[end->x][end->y];
        std::vector<Point*> path = {};
        while(true){
            if(p == nullptr){
                std::cout << "寻路失败" << std::endl;
                return {};
            }
            path.push_back(p);

            std::cout << "p: " << p->x << " " << p->y << std::endl;
            if(*p == *begin){
                std::reverse(path.begin(), path.end());
                return path;
            }
            p = p->parentNode;
        }

        return {};
    }

    int visitCount;
};

void printPath(std::vector<Point*> path){
    for(Point* point:path){
        std::cout << "(" << point->x << "," << point->y << ")" << " -> ";
    }

    std::cout << "end" << std::endl;
}

int main(){
    std::vector<std::vector<int>> arr =
    {
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,1,0,0,0},
        {0,0,0,0,1,0,0,0},
        {0,0,0,0,1,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
    };
 
    Point* s = new Point(1, 1);
    Point* e = new Point(2, 6);

    AStar as;
    std::vector<Point*> ans = as(arr, s, e);
    printPath(ans);
    return 0;
}