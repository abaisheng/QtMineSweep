#include <QDebug>
#include <QPainter>
#include <QMouseEvent>
#include <QTimer>
#include <QMessageBox>
#include <QLabel>
#include "game_window.h"
#include "ui_maingamewindow.h"
#include "game_model.h"


// ---------全局变量------------ //
const int blockSize = 20;
const int offsetX = 5; // 横向边距
const int offsetY = 5; // 纵向边距
const int spaceY = 60; // 上方空白用于绘制记分板
//GameLevel basic = {8, 10, 15};
//GameLevel medium = {15, 20, 50};
//GameLevel hard = {20, 30, 99};
// ------------------------------//

MainGameWindow::MainGameWindow(QWidget *parent) :
   QMainWindow(parent),
   ui(new Ui::MainGameWindow)
{
   ui->setupUi(this);
   this->setWindowTitle("MineSweeper");
   // 创建计时数字标签
   timeLabel = new QLabel(this);
   mineLabel = new QLabel(this);

   // 关联信号槽
   connect(ui->actionStart, SIGNAL(triggered(bool)), this, SLOT(onStartGameClicked()));
   connect(ui->actionBasic, SIGNAL(triggered(bool)), this, SLOT(onLevelChooseClicked()));
   connect(ui->actionMedium, SIGNAL(triggered(bool)), this, SLOT(onLevelChooseClicked()));
   connect(ui->actionHard, SIGNAL(triggered(bool)), this, SLOT(onLevelChooseClicked()));
   connect(ui->actionCustom, SIGNAL(triggered(bool)), this, SLOT(onLevelChooseClicked()));
   connect(ui->actionQuit, SIGNAL(triggered(bool)), this, SLOT(onQuitClicked()));
   connect(ui->actionAbout, SIGNAL(triggered(bool)), this, SLOT(onAboutClicked()));

   timer = new QTimer(this);
   connect(timer, SIGNAL(timeout()), this, SLOT(updateTimer()));

   // 创建游戏初始化游戏，设置好参数，默认是中级,启动计时器
   // 定义窗口大小(必须放在游戏创建之后后面，该函数设置后大小不可变动，窗口强制重绘)
   game = new GameModel;
   game->createGame();
}

MainGameWindow::~MainGameWindow()
{
   // 清理
   delete game;
   game = NULL;
   delete ui;
}

void MainGameWindow::paintEvent(QPaintEvent *)
{
   QPainter painter(this);
   QPixmap bmpBlocks(":/res/blocks.bmp");
   QPixmap bmpFaces(":/res/faces.bmp");
   QPixmap bmpFrame(":/res/frame.bmp");
   QPixmap bmpNumber(":/res/timenumber.bmp");

//   setAutoFillBackground(true);//必须有这条语句
//   setPalette(QPalette(QColor(100,100,100)));

   //绘制框架
   painter.drawPixmap( offsetX, ui->menuBar->height(), bmpFrame, 0, 0, 20, 40);
   for (int n = 1; n < game->mCol - 1; n++)
      painter.drawPixmap( offsetX + n * blockSize, ui->menuBar->height(), bmpFrame, 5, 0, 20, 40);
   painter.drawPixmap((game->mCol * blockSize + offsetX * 2) / 2 - 20, ui->menuBar->height(), bmpFrame, 80, 0, 40, 40);
   painter.drawPixmap( offsetX + (game->mCol - 1) * blockSize, ui->menuBar->height(), bmpFrame, 180, 0, 20, 40);

   //绘制时间
   setFixedSize(game->mCol * blockSize + offsetX * 2, game->mRow * blockSize + offsetY * 2 + spaceY);
   timeLabel->setGeometry(game->mCol * blockSize + offsetX * 2 - 80, (spaceY + offsetY) / 2, 80, 20);
   int minutes = game->timerSeconds/60;
   int seconds = game->timerSeconds%60;
   QString strTime = QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0'));
   timeLabel->setText("Time:" + strTime);


   // 绘制笑脸
   painter.drawPixmap((game->mCol * blockSize + offsetX * 2) / 2 - 12, ui->menuBar->height() + 7, bmpFaces, game->gameState * 24, 0, 24, 24);

   // 绘制雷数
   mineLabel->setGeometry(15, (spaceY + offsetY) / 2, 80, 20);
   QString strMine = QString("%1/%2").arg(game->markNumber, 2, 10, QChar('0')).arg(game->totalMineNumber, 2, 10, QChar('0'));
   mineLabel->setText("Mines:" + strMine);
   // 绘制雷区
   for(int i = 0; i < game->mRow; i++)
   {
      for(int j = 0; j < game->mCol; j++)
      {
         if (game->gameMap[i][j].curState == WRONG_BOMB)
         {
             if(game->gameState == PLAYING || game->gameState == FAULT)
             {
                // 如果还在游戏中就显示旗子
                painter.drawPixmap(j * blockSize + offsetX, i * blockSize + ui->menuBar->height() + 40, bmpBlocks, blockSize * 11, 0, blockSize, blockSize);
             }
             else if(game->gameState == OVER)
             {
                // 如果游戏已经结束，就显示标错了
                painter.drawPixmap(j * blockSize + offsetX, i * blockSize + ui->menuBar->height() + 40, bmpBlocks, blockSize * 12, 0, blockSize, blockSize);
             }
         }
         else painter.drawPixmap(j * blockSize + offsetX, i * blockSize + ui->menuBar->height() + 40, bmpBlocks, blockSize * game->gameMap[i][j].curState, 0, blockSize, blockSize);
      }
   }

}

void MainGameWindow::handleGameState(GameModel *game)
{
   if(game->gameState == OVER)
   {
      timer->stop();
      qDebug() << "you lose!";
//      ui->statusBar->showMessage(QString("Game over!"), 2000);
//      QMessageBox::information(this, "game over!", "You lose the game in " + QString::number(game->timerSeconds) + " s.");

   }
   else if(game->gameState == WIN)
   {
      timer->stop();
      qDebug() << "you win!";
//      ui->statusBar->showMessage(QString("Win!"), 2000);
      // 可以选择弹窗告知游戏胜利成绩
//      QMessageBox::information(this, "win!", "You win in " + QString::number(game->timerSeconds) + " s.");

   }
   update();
}

void MainGameWindow::mousePressEvent(QMouseEvent *event)
{
   if(event->y() < spaceY + offsetY)
   {
      int x = event->x();
      int y = event->y();
      // 此时判断是否点击笑脸
      if(x >= (game->mCol * blockSize + offsetX * 2) / 2 - 12
      && x <= (game->mCol * blockSize + offsetX * 2) / 2 + 12
      && y >= spaceY / 2
      && y <= spaceY / 2 + 24)
      {
         game->restartGame(); // 重玩
         timer->stop();
         update();
      }
   }
   else if(game->gameState != OVER && game->gameState != WIN)
   {
      // 游戏没输或没赢才接受点击
      // 此时判断点击的是哪个方块
      // 获得点击坐标
      int px = event->x() - offsetX;
      int py = event->y() - offsetY - spaceY;
      // 换算成方格索引
      int row = py / blockSize;
      int col = px / blockSize;

      // 根据不同情况响应
      if ( py < (game->mRow *blockSize) && 0 <= px && px < (game->mCol * blockSize) )
      {
         if (event->buttons() == (Qt::LeftButton|Qt::RightButton))
         {
            game->leftRight(row, col);
            update();// 每次点击都要重绘
         }
         else if(event->button() == Qt::LeftButton)
         {
            if (!timer->isActive()) timer->start(1000);
            game->digMine(row, col);
            update();// 每次点击都要重绘
         }
         else if(event->button() == Qt::RightButton)
         {
            game->markMine(row, col);
            update();// 每次点击都要重绘
         }
         // 处理游戏状态
         handleGameState(game);
      }
   }

}

void MainGameWindow::onStartGameClicked()
{
   qDebug()<<"game started";
   game->restartGame(); // 重新开始
   timer->stop();
   update();
}

void MainGameWindow::onLevelChooseClicked()
{
   QAction *actionSender = (QAction *)dynamic_cast<QAction *>(sender());
   if(actionSender == ui->actionBasic)
   {
      qDebug() << "basic";
      // 先设置游戏模型
      game->createGame(8, 10, 15, BASIC);
//      game->createGame(basic);
   }
   else if(actionSender == ui->actionMedium)
   {
      qDebug() << "medium";
      game->createGame(15, 20, 50, MEDIUM);
//      game->createGame(medium);
   }
   else if(actionSender == ui->actionHard)
   {
      qDebug() << "hard";
      game->createGame(20, 30, 99, HARD);
//      game->createGame(hard);
   }
   else if(actionSender == ui->actionCustom)
   {
      qDebug() << "custom";
      customDialog *cus_ui = new customDialog;

      cus_ui->row = game->mRow;
      cus_ui->col = game->mCol;
      cus_ui->mines = game->totalMineNumber;
      cus_ui->show();
      cus_ui->exec();

      if (cus_ui->Accepted &&
          cus_ui->row >= 8 && cus_ui->row <= 30 &&
          cus_ui->col >= 10 && cus_ui->col <= 50 &&
          cus_ui->mines >= 15 && cus_ui->mines <= 200)
          game->createGame(cus_ui->row, cus_ui->col, cus_ui->mines, CUSTOM);
      else
      {
          if (cus_ui->row < 8) cus_ui->row = 8;
          if (cus_ui->row > 30) cus_ui->row = 30;
          if (cus_ui->col < 10) cus_ui->col = 10;
          if (cus_ui->col > 50) cus_ui->col = 50;
          if (cus_ui->mines < 15) cus_ui->mines = 15;
          if (cus_ui->mines > 200) cus_ui->mines = 200;
          game->createGame(cus_ui->row, cus_ui->col, cus_ui->mines, CUSTOM);
      }
//          QMessageBox::information(this, "error!", "Input error.");
   }
   // 重新计时
   // 再刷新UI，窗口大小改变会强制重绘
   timer->stop();
   update();
}

void MainGameWindow::updateTimer()
{
   if(game->gameState != OVER && game->gameState != WIN)
   {
      game->timerSeconds++;
      update();
      qDebug() << game->timerSeconds;
   }
}

void MainGameWindow::onAboutClicked()
{
    QMessageBox::information(this, "About MineSweep", "hello qt!");
}

void MainGameWindow::onQuitClicked()
{
   // 退出
   QCoreApplication::quit();
}

