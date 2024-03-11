#include <bits/stdc++.h>

using namespace std;
const int n = 200;
const int robot_num = 10;
const int berth_num = 10;
const int N = 210;
//int ck = 0;
//double w_berth_1 = 10000, w_berth_2 = -10, w_berth_3 = 0.5, w_berth_4 = 0.5, w_berth_5 = 100;
////w_berth_1:该泊位是否停靠船只 w_berth_2:该泊位货物量减船容量 w_berth_3:该泊位性价比 w_berth_4:该泊位货物量 w_berth_5:到达泊位的距离
//double w_good_1 = 1000, w_good_2 = 1000, w_good_3 = 0.5;


double w_berth_size = 0.5, w_berth_speed = 0.5, w_berth_dis = 0.5;  // w_berth_size > w_berth_dis > w_berth_speed
double w_good_val = 0.5, w_good_dis = 0.5;
double w_boat_speed = 0.5, w_boat_size = 0.5;

//w_good_1:货物价值 w_good_2:货物距离
struct Robot {
    int x, y, goods;
//    int status;
//    int mbx, mby;

    Robot() {}

//    Robot(int startX, int startY) {
//        x = startX;
//        y = startY;
//    }
} robot[robot_num + 10];

struct Berth {
    int x;
    int y;
    int transport_time;
    int loading_speed;
    int size;

    Berth() {}

//    Berth(int x, int y, int transport_time, int loading_speed) {
//        this->x = x;
//        this->y = y;
//        this->transport_time = transport_time;
//        this->loading_speed = loading_speed;
//    }
} berth[berth_num + 10];

struct Boat {
    int num, pos, status, berth, flag;
} boat[10];
//struct Goods {
//    int x, y, val, time;
//} good;

struct Node {
    int x, y, gn;
    double hn;

    bool operator>(const Node &W) const {
        return gn + hn > W.gn + W.hn;
    }
};

//int goods_num;
int money, boat_capacity, id;
char ch[N][N];//地图
int good_time[N][N];//货物出现时间
int vis[N][N]; //是否访问过
//0 right 1 left 2 up 3 down
int dx[] = {0, 0, -1, 1};
int dy[] = {1, -1, 0, 0};
int fa[N][N][2];
//int robot_move[robot_num + 10];
int vis_good[N][N];//货物是否被锁定
int vis_ban[N][N];//该位置是否被锁定
int vis_berth[berth_num];//泊位是否被锁定
int flag_berth[N][N];//泊位ID
int good_value[N][N];
int vis_search_berth[10];
int latest_near_good_time[N][N];  // 附近最近刷新货物时间
int highest_near_good_val[N][N];  // 附近货物最高价值
//double pre_dis_berth[N][N];
//int virtual_berth[N][N];//虚拟泊位
//int id_virtual_berth[N][N];//虚拟泊位ID
int ban_boot[10][2];

void Init() {
    for (int i = 1; i <= n; i++)
        scanf("%s", ch[i] + 1);//地图
    for (int i = 0; i < berth_num; i++) {
        int id;
        scanf("%d", &id);//泊位ID
        scanf("%d%d%d%d", &berth[id].x, &berth[id].y, &berth[id].transport_time, &berth[id].loading_speed);
        berth[id].x++;
        berth[id].y++;
        //坐标，运输时间，装卸速度
        for (int t = 1; t <= 3; t++)
            for (int j = 1; j <= 3; j++)
                flag_berth[berth[id].x + t][berth[id].y + j] = id;
    }
    //初始化虚拟泊位
//    for (int j = 2; j <= 200; j += 2) {
//        for (int i = 0; i < berth_num; i++) {
//            for (int l = 0; l < 4; l++) {
//                int nx = berth[i].x + dx[l] * j;
//                int ny = berth[i].y + dy[l] * j;
//                if (ch[nx][ny] != '*' && ch[nx][ny] != '#' && ch[nx][ny] != 'B') {
//                    if (nx >= 1 && nx <= n && ny >= 1 && ny <= n) {
//                        virtual_berth[nx][ny] = j;
//                        id_virtual_berth[nx][ny] = i;
//                    }
//                }
//            }
//        }
//    }
//    std::ofstream file("output.txt");
//
//    for (int i = 0; i < n; i++) {
//        for (int j = 0; j < n; j++) {
//            file << virtual_berth[i][j] << ' ';
//        }
//        file << '\n';
//    }

//    file.close();
//    for (int i = 1; i <= n; i++)
//        for (int j = 1; j <= n; j++)
//            pre_dis_berth[i][j] = 1e9;
//    for (int i = 0; i < berth_num; i++) {
//        for (int j = 1; j <= n; j++) {
//            for (int k = 1; k <= n; k++) {
//                pre_dis_berth[j][k] = min(pre_dis_berth[j][k], (abs(j - berth[i].x) + abs(k - berth[i].y)) * 1.0);
//            }
//        }
//    }
    scanf("%d", &boat_capacity);//船的容量
    char okk[100];
    scanf("%s", okk);
    printf("OK\n");
    fflush(stdout);
}

int Input(int zhen) {
    scanf("%d%d", &id, &money);//帧序号和金钱
    int num;
    //物品1000帧消失
    scanf("%d", &num);//场上新增货物数量
    for (int i = 1; i <= num; i++) {
        int x, y, val;
        scanf("%d%d%d", &x, &y, &val);//货物坐标和价值<100
        x++, y++;
        if (ch[x][y] != 'B' && ch[x][y] != '*' && ch[x][y] != '#') {
            good_time[x][y] = zhen;
            good_value[x][y] = val;
            for (int dx = -4; dx <= 4; ++dx)
                for (int dy = -4; dy <= 4; ++dy) {
                    int nx = dx + x, ny = dy + y;
                    if (nx >= 1 && nx <= n && ny >= 1 && ny <= n && ch[nx][ny] != 'B' && ch[nx][ny] != '*' &&
                        ch[nx][ny] != '#') {
                        latest_near_good_time[nx][ny] = max(latest_near_good_time[nx][ny], zhen);
                        highest_near_good_val[nx][ny] = max(highest_near_good_val[nx][ny], val);
                    }
                }
        }
    }
    for (int i = 0; i < robot_num; i++) {
        int sts;
        scanf("%d%d%d%d", &robot[i].goods, &robot[i].x, &robot[i].y, &sts);
        robot[i].x++;
        robot[i].y++;
        //是否携带物品，坐标，状态
    }
    for (int i = 0; i < 5; i++) {
        scanf("%d%d\n", &boat[i].status, &boat[i].pos);
        //船的状态和ID
        //如果处于空闲状态，前往泊位
        if (boat[i].status == 1 && boat[i].flag == 0) {
            //寻找最佳泊位
            double mx = -1e9;
            int pos;
            for (int t = 0; t < berth_num; t++) {
                if (vis_berth[t] == 0) {
                    //计算总性价比
                    double w = berth[t].size * w_boat_size +
                               berth[t].loading_speed * 1.0 / berth[t].transport_time * w_boat_speed;
//                    double w = berth[t].loading_speed * 1.0 / berth[t].transport_time * w_berth_3 +
//                               berth[t].size * 1.0 / boat_capacity * w_berth_2;
                    if (w > mx) {
                        mx = w;
                        pos = t;
                    }
                }
            }
            vis_berth[pos] = 1;
            printf("ship %d %d\n", i, pos);
            boat[i].num = boat_capacity;
            boat[i].berth = pos;
            boat[i].flag = 1;
        }
            //如果处于装货状态，装货
        else if (boat[i].status == 1 && boat[i].flag == 1) {
            //如果时间不够
            if (14980 - berth[boat[i].berth].transport_time - zhen < 0) {
                printf("go %d\n", i);
                berth[boat[i].berth].size = boat_capacity;
                vis_berth[boat[i].berth] = 0;
                boat[i].flag = 0;
                continue;
            }
            if (boat[i].num == 0) {
                printf("go %d\n", i);
                berth[boat[i].berth].size = boat_capacity;
                vis_berth[boat[i].berth] = 0;
                boat[i].flag = 0;
                continue;
            }
            int temp = min(berth[boat[i].berth].size, berth[boat[i].berth].loading_speed);
            temp = min(temp, boat[i].num);
//            printf("move %d %d\n", temp,boat[i].num);
            boat[i].num -= temp;
            berth[boat[i].berth].size -= temp;
        }
    }

    char okk[100];
    scanf("%s", okk);
    return id;
}


double cal_h(int x, int y, int zhen) {
    // todo
//    highest_near_good_val up
//    latest_near_good_time up
//    zhen - latest_near_good_time[x][y] down
    return highest_near_good_val[x][y] * 0.2 + 0.8 / (zhen - latest_near_good_time[x][y]);  // score 126589
//    return 1.0 * highest_near_good_val[x][y] / (zhen - latest_near_good_time[x][y]);   // score 113190
//    return -1.0 * highest_near_good_val[x][y] / (zhen - latest_near_good_time[x][y]);  // score 115640
}

double cal_h_berth(int x, int y) {
    // return 1.0 * pre_dis_berth[x][y];
    double minn = 1e9;
    for (int i = 0; i < berth_num; i++) {
        if (vis_berth[i] == 1) {
            minn = min(minn, (abs(x - berth[i].x) + abs(y - berth[i].y)) * 0.9);
        } else minn = min(minn, (abs(x - berth[i].x) + abs(y - berth[i].y)) * 1.0);
    }
    return minn;
}


void bfs_good(int x, int y, int zhen, int id) {
    int check_move = 0;
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++)
            vis[i][j] = 0;

    // queue<pair<int, int> > q;

    priority_queue<Node, vector<Node>, greater<> > q;
    // queue<int> distance;

    q.push({x, y, 0, cal_h(x, y, zhen)}); // 机器人初始位置

    vis[x][y] = 1;
    double mx = -1e9;
    pair<int, int> pos;
    pos = {-1, -1};
    int re = 0;
    while (!q.empty()) {
        re++;
        if (pos.first != -1 && re > 800) break;
        if (zhen < 100 && re > 1e3) break;
        Node node = q.top();
        pair<int, int> now = {node.x, node.y};
        q.pop();
        int dis = node.gn;

        // 扩展到一个货物了
        if (good_time[now.first][now.second] > 0 && zhen - good_time[now.first][now.second] < 1000 &&
            vis_good[now.first][now.second] == 0) {
            double w = good_value[now.first][now.second] * w_good_val + w_good_dis / dis;
//            double w = good_value[now.first][now.second] * w_good_1 + w_good_2 / dis;
            if (w > mx) {
                mx = w;
                pos = now;
            }
        }
        for (int i = 0; i < 4; i++) {
            int nx = now.first + dx[i];
            int ny = now.second + dy[i];
            if (ch[nx][ny] == '*' || ch[nx][ny] == '#') continue;
            if (nx < 1 || nx > n || ny < 1 || ny > n) continue;
            if (vis[nx][ny] == 1) continue;
            if ((vis_ban[nx][ny] == 1 && (abs(nx - x) + abs(ny - y)) < 3) || vis_ban[nx][ny] >= 2) continue;
            if (nx == ban_boot[id][0] && ny == ban_boot[id][1]) continue;
            fa[nx][ny][0] = now.first;
            fa[nx][ny][1] = now.second;
            q.push({nx, ny, dis + 1, cal_h(nx, ny, zhen)});
            vis[nx][ny] = 1;
        }
    }
    if (pos.first == -1) return;
    vis_good[pos.first][pos.second] = 1;
    latest_near_good_time[pos.first][pos.second] = 0;

    // todo
//    highest_near_good_val[pos.first][pos.second] = 0;

    int temp_x = pos.first;
    int temp_y = pos.second;
//    int h = 0;
//    int ck_x = temp_x;
//    int ck_y = temp_y;
    while (fa[temp_x][temp_y][0] != x || fa[temp_x][temp_y][1] != y) {
        if (vis_ban[temp_x][temp_y] == 1) {
            for (int i = 0; i < berth_num; i++) {
                if (temp_x == berth[i].x && temp_y == berth[i].y) {
                    ban_boot[i][0] = temp_x;
                    ban_boot[i][1] = temp_y;
                    break;
                }
            }
            vis_ban[temp_x][temp_y]++;
        }
        int x1 = fa[temp_x][temp_y][0];
        int y1 = fa[temp_x][temp_y][1];
        if (fa[x1][y1][0] == x && fa[x1][y1][1] == y) {
            vis_ban[temp_x][temp_y] = 1;
        }
        temp_x = x1;
        temp_y = y1;
    }
    int di_x = temp_x - x;
    int di_y = temp_y - y;
    for (int i = 0; i < 4; i++) {
        if (di_x == dx[i] && di_y == dy[i]) {
            check_move = 1;
            printf("move %d %d\n", id, i);
            vis_ban[temp_x][temp_y] = 1;
//            robot_move[id] = i;
            break;
        }
    }
    if (check_move == 0) {
        vis_ban[x][y] = 1;
    }
}

void bfs_berth(int x, int y, int zhen, int id) {
    int check_move = 0;
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++)
            vis[i][j] = 0;
    for (int &i: vis_search_berth) {
        i = 0;
    }

    // queue<pair<int, int> > q;
    // queue<int> distance;
    // q.push(make_pair(x, y));
    // distance.push(0);
    // queue<pair<int, int> > q;

    priority_queue<Node, vector<Node>, greater<> > q;
    // queue<int> distance;


    q.push({x, y, 0, cal_h(x, y, zhen)}); // 机器人初始位置
    int ck = 0;

    vis[x][y] = 1;
    double mx = -1e9;
    pair<int, int> pos;
    pos = {-1, -1};
    int re = 0;
    int ck_berth = 0;
    int ck_virtual_berth = 0;
    while (!q.empty()) {
        re++;
        if (ck_berth != 0 && re > 1e3) break;
        Node node = q.top();
        pair<int, int> now = {node.x, node.y};
        q.pop();
        int dis = node.gn;
//        if(virtual_berth[now.first][now.second]>0){
//            if(ck_berth) continue;
//            int id = id_virtual_berth[now.first][now.second];
//            double w=0;
//            if(vis_berth[id] == 0) w-=1000;
//            w-=virtual_berth[now.first][now.second];
//            if(w>mx){
//                mx=w;
//                pos=now;
//                
//            }
//            // exit(0); 
//            ck_virtual_berth=1;
//        }
        if (ch[now.first][now.second] == 'B' && vis_search_berth[flag_berth[now.first][now.second]] == 0) {

            int id = flag_berth[now.first][now.second];
            if (vis_berth[id] == 0) continue;
            ck_berth = 1;
            vis_search_berth[id] = 1;
            double w = vis_berth[id] * (berth[id].size * 1.0 / boat_capacity) * w_berth_size + w_berth_dis / dis
                       + berth[id].loading_speed * w_berth_speed;
            // 有船才考虑泊位货物占比
//            double w = vis_berth[id] * w_berth_1 + berth[id].size * 1.0 / boat_capacity * w_berth_2 + w_berth_5 / dis;
            if (w > mx) {
                mx = w;
                pos = now;
            }
        }
        if (ch[now.first][now.second] == 'B') continue;
        for (int i = 0; i < 4; i++) {
            int nx = now.first + dx[i];
            int ny = now.second + dy[i];
            if (ch[nx][ny] == '*' || ch[nx][ny] == '#') continue;
            if (nx < 1 || nx > n || ny < 1 || ny > n) continue;
            if (vis[nx][ny] == 1) continue;
            if ((vis_ban[nx][ny] == 1 && (abs(nx - x) + abs(ny - y)) < 3) || vis_ban[nx][ny] >= 2) continue;
            if (nx == ban_boot[id][0] && ny == ban_boot[id][1]) continue;
            fa[nx][ny][0] = now.first;
            fa[nx][ny][1] = now.second;
            q.push({nx, ny, dis + 1, cal_h_berth(nx, ny)});
            vis[nx][ny] = 1;
        }
    }
    if (pos.first == -1) return;
    int temp_x = pos.first;
    int temp_y = pos.second;
//    int h = 0;
//    int ck_x = temp_x;
//    int ck_y = temp_y;
    while (fa[temp_x][temp_y][0] != x || fa[temp_x][temp_y][1] != y) {
        if (vis_ban[temp_x][temp_y] == 1) {
            for (int i = 0; i < berth_num; i++) {
                if (temp_x == berth[i].x && temp_y == berth[i].y) {
                    ban_boot[i][0] = temp_x;
                    ban_boot[i][1] = temp_y;
                    break;
                }
            }
            vis_ban[temp_x][temp_y]++;
        }
        int x1 = fa[temp_x][temp_y][0];
        int y1 = fa[temp_x][temp_y][1];
        if (fa[x1][y1][0] == x && fa[x1][y1][1] == y) {
            vis_ban[temp_x][temp_y] = 1;
        }
        temp_x = x1;
        temp_y = y1;
    }
    int di_x = temp_x - x;
    int di_y = temp_y - y;
    for (int i = 0; i < 4; i++) {
        if (di_x == dx[i] && di_y == dy[i]) {
            check_move = 1;
            printf("move %d %d\n", id, i);
            vis_ban[temp_x][temp_y] = 1;
//            robot_move[id] = i;
            break;
        }
    }
    if (check_move == 0) {
        vis_ban[x][y] = 1;
    }
}

int main(int argc, char *argv[]) {
    Init();
    for (int zhen = 1; zhen <= 15000; zhen++) {
//    	 if(zhen==500){
//    	 	printf("move %d %d\n", 10,1);
//    	 	puts("OK");
//         	fflush(stdout);
//         	continue;
//		 } 
        for (auto &i: ban_boot) {
            i[0] = i[1] = 0;
        }

        int id = Input(zhen);
//        for (int i = 0; i < robot_num; i++) {
//            robot_move[i] = -1;
//        }
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                vis_good[i][j] = 0;
                vis_ban[i][j] = 0;
            }
        }

        for (int i = 0; i < robot_num; i++) {
            vis_ban[robot[i].x][robot[i].y] = 1;
            // 没拿着货物 而且在货物上
            if (robot[i].goods == 0) {
                if (good_time[robot[i].x][robot[i].y] > 0 && zhen - good_time[robot[i].x][robot[i].y] < 1000) {
                    printf("get %d\n", i);
                    vis_ban[robot[i].x][robot[i].y] = 1;
                } // 拿着货物 而且在码头
            } else if (robot[i].goods == 1) {
                if (ch[robot[i].x][robot[i].y] == 'B') {
                    printf("pull %d\n", i);
                    vis_ban[robot[i].x][robot[i].y] = 1;
                    berth[flag_berth[robot[i].x][robot[i].y]].size++;
                }
            }
        }
        for (int i = 0; i < robot_num; i++) {
            if (robot[i].goods == 0) {
                if (good_time[robot[i].x][robot[i].y] > 0 && zhen - good_time[robot[i].x][robot[i].y] < 1000) {
                    good_time[robot[i].x][robot[i].y] = 0;
                } else {
                    bfs_good(robot[i].x, robot[i].y, zhen, i);
                }
            } else if (robot[i].goods == 1) {
                if (ch[robot[i].x][robot[i].y] == 'B') continue;
                else bfs_berth(robot[i].x, robot[i].y, zhen, i);
            }
        }
        puts("OK");
        fflush(stdout);
    }

    return 0;
}
