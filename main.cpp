#include <bits/stdc++.h>
using namespace std;

const int n = 200;
const int robot_num = 10;
const int berth_num = 10;
const int N = 210;
int ck=0;
struct Robot
{
    int x, y, goods;
    int status;
    int mbx, mby;
    Robot() {}
    Robot(int startX, int startY) {
        x = startX;
        y = startY;
    }
}robot[robot_num + 10];

struct Berth
{
    int x;
    int y;
    int transport_time;
    int loading_speed;
    int size;
    Berth(){}
    Berth(int x, int y, int transport_time, int loading_speed) {
        this -> x = x;
        this -> y = y;
        this -> transport_time = transport_time;
        this -> loading_speed = loading_speed;
    }
}berth[berth_num + 10];
int cmp_berth(Berth a, Berth b) {
    return a.transport_time < b.transport_time;
}
struct Boat
{
    int num, pos, status,berth,flag;
}boat[10];
struct Goods
{
    int x, y, val, time;
}good;
//list<good> goods; 
int goods_num;
int money, boat_capacity, id;
char ch[N][N];
int good_time[N][N];
// int gds[N][N];
int vis[N][N]; 
//0 right 1 left 2 up 3 down
int dx[] = {0, 0, -1, 1};
int dy[] = {1, -1,0, 0};
int fa[N][N][2]; 
int robot_move[robot_num + 10]; 
char temp_ch[robot_num + 10];
int vis_good[N][N];
int vis_ban[N][N];
int berth_size[N];
int vis_berth[berth_num];
int vis_dest[N][N];
int flag_berth[N][N];
void Init()
{
    for(int i = 1; i <= n; i ++)
        scanf("%s", ch[i] + 1);//地图
    for(int i = 0; i < berth_num; i ++)
    {
        int id;
        scanf("%d", &id);//泊位ID
        scanf("%d%d%d%d", &berth[id].x, &berth[id].y, &berth[id].transport_time, &berth[id].loading_speed);
        berth[id].x++;
        berth[id].y++;
        //坐标，运输时间，装卸速度
        for(int t=1;t<=3;t++)
            for(int j=1;j<=3;j++)
                flag_berth[berth[id].x+t][berth[id].y+j]=id;
    }
    sort(berth , berth +  berth_num, cmp_berth);
    scanf("%d", &boat_capacity);//船的容量
    char okk[100];
    scanf("%s", okk);
    printf("OK\n");
    fflush(stdout);
}
int Input(int zhen)
{
    scanf("%d%d", &id, &money);//帧序号和金钱
    int num;
    //物品1000帧消失
    scanf("%d", &num);//场上新增货物数量
    for(int i = 1; i <= num; i ++)
    {
        int x, y, val;
        scanf("%d%d%d", &x, &y, &val);//货物坐标和价值<100
        x++,y++;
        if(ch[x][y] != 'B'&&ch[x][y] != '*'&&ch[x][y] != '#')
        {
            good_time[x][y] = zhen;
        }
    }
    for(int i = 0; i < robot_num; i ++)
    {
        int sts;
        scanf("%d%d%d%d", &robot[i].goods, &robot[i].x, &robot[i].y, &sts);
        robot[i].x++;
        robot[i].y++;
        //是否携带物品，坐标，状态
    }
    for(int i = 0; i < 5; i ++){
        scanf("%d%d\n", &boat[i].status, &boat[i].pos);
        //船的状态和ID
        if(boat[i].status == 1&&boat[i].flag==0){
            for(int t=0;t<berth_num;t++){
                if(vis_berth[t]==0){
                    vis_berth[t]=1;
                    printf("ship %d %d\n", i,t);
                    boat[i].num=boat_capacity;
                    boat[i].berth=t;
                    boat[i].flag=1;
                    break;
                }
            }
        }
         else if(boat[i].status == 1&&boat[i].flag==1){
         	
            if(boat_capacity-boat[i].num>=10){
                printf("go %d\n", i);
                berth[boat[i].berth].size=boat_capacity;
                vis_berth[boat[i].berth]=0;
                boat[i].flag==0;
                continue;
             }
            int temp=min(berth[boat[i].berth].size,berth[boat[i].berth].loading_speed);
            temp=min(temp,boat[i].num);
//            printf("move %d %d\n", temp,boat[i].num);
            boat[i].num-=temp;
            berth[boat[i].berth].size-=temp;
         }
    }
        
    char okk[100];
    scanf("%s", okk);
    return id;
}
void bfs_good(int x, int y,int zhen,int id)
{
    int check_move=0;
    for(int i = 1; i <= n; i ++)
        for(int j = 1; j <= n; j ++)
            vis[i][j] = 0;
    queue<pair<int, int> > q;
    q.push(make_pair(x, y));
    vis[x][y]=1;
    while(!q.empty())
    {
		pair<int, int> now = q.front();
        q.pop();
        if(good_time[now.first][now.second] >0&&zhen-good_time[now.first][now.second]<1000&&vis_good[now.first][now.second]==0)
        {
            vis_good[now.first][now.second]=1;
            int temp_x=now.first;
            int temp_y=now.second;
            while(fa[temp_x][temp_y][0]!=x||fa[temp_x][temp_y][1]!=y )
            {
				int x1=fa[temp_x][temp_y][0];
                int y1=fa[temp_x][temp_y][1];
                if(fa[x1][y1][0]==x&&fa[x1][y1][1]==y)
                {
                    vis_ban[temp_x][temp_y]=1;
                }
                temp_x=x1;
                temp_y=y1;
            }
            
            int di_x = temp_x - x;
            int di_y = temp_y - y;
            for(int i = 0; i < 4; i ++)
            {
                if(di_x == dx[i] && di_y == dy[i])
                {
                    check_move=1;
                    printf("move %d %d\n", id,i);
//                    robot_move[id]=i;
                    temp_ch[id]=ch[temp_x][temp_y];
                    vis_ban[temp_x][temp_y]=1;
                    robot_move[id]=i;
                    return;
                    
                }
            }
        }
        for(int i = 0; i < 4; i ++)
        {
            int nx = now.first + dx[i];
            int ny = now.second + dy[i];
            if(ch[nx][ny] == '*' || ch[nx][ny] == '#'||ch[nx][ny]=='B') continue;
            if(nx < 1 || nx > n || ny < 1 || ny > n) continue;
            if(vis[nx][ny]==1) continue;
            if(vis_ban[nx][ny]) continue;
            fa[nx][ny][0]=now.first;
            fa[nx][ny][1]=now.second;
            q.push(make_pair(nx, ny));
            vis[nx][ny]=1;
        }
    }
    if(check_move==0){
        vis_ban[x][y]=1;
    }
}
void bfs_berth(int x, int y,int zhen,int id)
{
    int check_move=0;
    for(int i = 1; i <= n; i ++)
        for(int j = 1; j <= n; j ++)
            vis[i][j] = 0;
    queue<pair<int, int> > q;
    q.push(make_pair(x, y));
    vis[x][y]=1;
    while(!q.empty())
    {
		pair<int, int> now = q.front();
        q.pop();
        if(ch[now.first][now.second] == 'B')
        {
            int temp_x=now.first;
            int temp_y=now.second;
            while(fa[temp_x][temp_y][0]!=x||fa[temp_x][temp_y][1]!=y )
            {
				int x1=fa[temp_x][temp_y][0];
                int y1=fa[temp_x][temp_y][1];
                if(fa[x1][y1][0]==x&&fa[x1][y1][1]==y)
                {
                    vis_ban[temp_x][temp_y]=1;
                }
                temp_x=x1;
                temp_y=y1;
            }
            
            int di_x = temp_x - x;
            int di_y = temp_y - y;
            for(int i = 0; i < 4; i ++)
            {
                if(di_x == dx[i] && di_y == dy[i])
                {
                    check_move=1;
                     printf("move %d %d\n", id,i);
//                    robot_move[id]=i;
                    temp_ch[id]=ch[temp_x][temp_y]; 
                    vis_ban[temp_x][temp_y]=1;
                    robot_move[id]=i;
                    return;
                }
            }
        }
        for(int i = 0; i < 4; i ++)
        {
            int nx = now.first + dx[i];
            int ny = now.second + dy[i];
            if(ch[nx][ny] == '*' || ch[nx][ny] == '#') continue;
            if(nx < 1 || nx > n || ny < 1 || ny > n) continue;
            if(vis[nx][ny]==1) continue;
            if(vis_ban[nx][ny]) continue;
            fa[nx][ny][0]=now.first;
            fa[nx][ny][1]=now.second;
            q.push(make_pair(nx, ny));
            vis[nx][ny]=1;
        }
    }
    if(check_move==0){
        vis_ban[x][y]=1;
    }
}

int main()
{
    Init();
    for(int zhen = 1; zhen <= 15000; zhen ++)
    {
//    	 if(zhen==2000){
//    	 	printf("move %d %d\n", 10,1);
//    	 	puts("OK");
//         	fflush(stdout);
//         	continue;
//		 } 
        int id = Input(zhen);
        for(int i=0;i<robot_num;i++){
            robot_move[i]=-1;
        }
        for(int i=1;i<=n;i++){
            for(int j=1;j<=n;j++){
                vis_good[i][j]=0;
                vis_ban[i][j]=0;
            }
        }
        for(int i = 0; i < robot_num; i ++){
            if(robot[i].goods == 0){
                if(good_time[robot[i].x][robot[i].y] > 0&&zhen-good_time[robot[i].x][robot[i].y]<1000){
                    printf("get %d\n", i);
                    vis_ban[robot[i].x][robot[i].y]=1;
                }
            }
            else if(robot[i].goods == 1){
                if(ch[robot[i].x][robot[i].y] == 'B'){
                    printf("pull %d\n", i);
                    vis_ban[robot[i].x][robot[i].y]=1;
                    berth[flag_berth[robot[i].x][robot[i].y]].size++;
                }
            }
        }
        for(int i = 0; i < robot_num; i ++){
            if(robot[i].goods == 0){
                if(good_time[robot[i].x][robot[i].y] > 0&&zhen-good_time[robot[i].x][robot[i].y]<1000){
                    good_time[robot[i].x][robot[i].y] = 0;
                }
                else{
                	bfs_good(robot[i].x, robot[i].y,zhen,i);
			 	} 
            }
            else if(robot[i].goods == 1){
                if(ch[robot[i].x][robot[i].y] == 'B') continue;
                else bfs_berth(robot[i].x, robot[i].y,zhen,i);
            }
        }
        for(int i=1;i<=n;i++){
            for(int j=1;j<=n;j++){
                vis_dest[i][j]=0;
            }
        }
        vector<pair<int,int>> dest[robot_num+5];
        for(int i=0;i<robot_num;i++){
            int x=robot[i].x+dx[robot_move[i]];
            int y=robot[i].y+dy[robot_move[i]];
            vis_dest[x][y]=1;
        }
        puts("OK");
        fflush(stdout);
    }

    return 0;
}
