# A*寻路算法

定义：

1. *g*=起始点到当前点的距离
2. *h*=当前点到终点的距离
3. *f=g+h*
4. *s*=起点，*e*=终点
5. `map[][]`=地图，1=障碍物，0=可走的点
6. `around[][]`=当前节点能到达的下一个节点及附加*g*值的集合；`around[][0]`=x偏移，`around[][1]`=*y*偏移，`around[][2]`=附加*g*值
7. `visited[][]`=遍历过的点的信息，包括*fgh*和*xy*和*parentNode*
7. `openList`=小顶堆（按*f*排序），记录待遍历的点

原理：

1. 每次从*openList*里取*f*最小的点出发，遍历可到达的下一个点（必须是没走过的点）的集合
   1. 遍历到的点*next*的*g*和*h*更新方式：`g=parent.g+around[][2]` `h=(e-next)*10`;*parent*为*around*里**遍历过的节点**里面***g*值最小**的那个，即使*f*最小；并更新`next.parentNode=parent`

2. 记录`visited[next.x][next.y]=next`，将*next*压入优先队列*openList*里
3. 循环直到`next==e`时停止遍历
4. 如果*e*没被遍历过，则寻路失败；否则，从*e*出发，不断寻找*parentNode*并压入结果数组，直到找到*s*。如果*parentNode*不存在，也视为寻路失败。
