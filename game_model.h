#ifndef GAMEMODEL_H
#define GAMEMODEL_H

// 游戏逻辑模型类，逻辑与界面分离
#include <vector>

// ---------------- 全局定义变量 ---------------- //
// 方块的状态，未挖掘，翻开，标记，雷出现，错误标记
enum BlockState
{
   DIGED_0,
   DIGED_1,
   DIGED_2,
   DIGED_3,
   DIGED_4,
   DIGED_5,
   DIGED_6,
   DIGED_7,
   DIGED_8,
   BOMB,
   UN_DIG,
   MARKED,
   WRONG_BOMB
};
// 雷方块类
struct MineBlock
{
   BlockState curState; // 当前状态
   int valueFlag; // 数值，0到8， -1表示雷
};

// 游戏状态，分为未完，有错误标记、输、赢四种
enum GameState
{
   OVER,
   PLAYING,
   WIN,
   FAULT,
};

// 游戏难度，有低级、中级、高级
enum GameLevel
{
   BASIC,
   MEDIUM,
   HARD,
   CUSTOM
};

//extern struct GameLevel
//{
//    int mRow; // 地图行数
//    int mCol; // 地图列数
//    int mCount; //雷数
//}basic, medium, hard;


// 游戏默认参数
const int kRow = 15;
const int kCol = 20;
const int kMineCount = 50;
const int kTime = 0;
//-----------------------------------------------

//int kRow = 15;
//int kCol = 20;
//int kMineCount = 50;
//int kTime = 0;

class GameModel
{
public:
   GameModel();
   virtual ~GameModel();
public:
   void digMine(int m, int n); //挖雷, m是行， n是列
   void markMine(int m, int n); // 标记雷
   void leftRight(int m, int n); //鼠标左右同时按下
   void createGame(int row = kRow, int col = kCol, int mineCount = kMineCount, GameLevel level = MEDIUM); // 初始化游戏
//   void createGame(GameLevel gameLevel = medium);
   void restartGame();  // 按当前参数重新开始游戏
   void checkGame(); // 检查游戏输赢
public:
   std::vector<std::vector<MineBlock> > gameMap; // 游戏地图

   int mRow; //行数
   int mCol; //列数
   int totalMineNumber; // 雷数
   int curMineNumber;   // 当前雷数（仅用于显示）
   int markNumber;      //标记数
   int timerSeconds;   // 计时（秒）
   GameState gameState; // 当前游戏状态
   GameLevel gameLevel; // 当前游戏难度
};

#endif // GAMEMODEL_H
