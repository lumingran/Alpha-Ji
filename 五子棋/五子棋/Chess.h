#pragma once
#include <graphics.h>
#include <vector>
using namespace std;
typedef enum {
	CHESS_WHITE = -1,//白
	CHESS_BLACK = 1//黑
}chess_kind_t;
struct ChessPos {
	int row;
	int col;
	ChessPos(int r=0,int c=0):row(r),col(c){}
};
class Chess
{
public:
	Chess(int gradeSize, int marginX, int marginY, float chessSize);
	void init();
	bool clickBoard(int x, int y, ChessPos* pos);//判断落子是否有效
	void chessDown(ChessPos* pos, chess_kind_t kind);
	int getGradeSize();//棋盘大小
	int getChessData(ChessPos* pos);//获取指定位置的棋子信息
	int getChessData(int row, int col);
	bool checkOver();//胜负check
private:
	IMAGE chessBlackImg;
	IMAGE chessWhiteImg;
	IMAGE chessBlackImg2;
	IMAGE chessWhiteImg2;
	int gradeSize;
	int margin_x;//左边界
	int margin_y;//上边界
	float chessSize;
	vector<vector<int>> chessMap;
	bool playerFlag;//当前行动方：1：黑 0：白
	void updateGameMap(ChessPos* pos);
	bool checkWin();//如果结束 return true
	int fst;
	ChessPos lastPos;//最后落子
};

