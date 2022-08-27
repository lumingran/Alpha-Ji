#pragma once
#include <graphics.h>
#include <vector>
using namespace std;
typedef enum {
	CHESS_WHITE = -1,//��
	CHESS_BLACK = 1//��
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
	bool clickBoard(int x, int y, ChessPos* pos);//�ж������Ƿ���Ч
	void chessDown(ChessPos* pos, chess_kind_t kind);
	int getGradeSize();//���̴�С
	int getChessData(ChessPos* pos);//��ȡָ��λ�õ�������Ϣ
	int getChessData(int row, int col);
	bool checkOver();//ʤ��check
private:
	IMAGE chessBlackImg;
	IMAGE chessWhiteImg;
	IMAGE chessBlackImg2;
	IMAGE chessWhiteImg2;
	int gradeSize;
	int margin_x;//��߽�
	int margin_y;//�ϱ߽�
	float chessSize;
	vector<vector<int>> chessMap;
	bool playerFlag;//��ǰ�ж�����1���� 0����
	void updateGameMap(ChessPos* pos);
	bool checkWin();//������� return true
	int fst;
	ChessPos lastPos;//�������
};

