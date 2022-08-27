#include "Chess.h"
#include <math.h>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")
#include <conio.h>

void putimagePNG(int x, int y, IMAGE* picture) 
{
    DWORD* dst = GetImageBuffer();    
    DWORD* draw = GetImageBuffer();
    DWORD* src = GetImageBuffer(picture); 
    int picture_width = picture->getwidth(); 
    int picture_height = picture->getheight(); 
    int graphWidth = getwidth();       
    int graphHeight = getheight();   
    int dstX = 0;   
    for (int iy = 0; iy < picture_height; iy++)
    {
        for (int ix = 0; ix < picture_width; ix++)
        {
            int srcX = ix + iy * picture_width;
            int sa = ((src[srcX] & 0xff000000) >> 24);
            int sr = ((src[srcX] & 0xff0000) >> 16);
            int sg = ((src[srcX] & 0xff00) >> 8);  
            int sb = src[srcX] & 0xff;
            if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight)
            {
                dstX = (ix + x) + (iy + y) * graphWidth; 
                int dr = ((dst[dstX] & 0xff0000) >> 16);
                int dg = ((dst[dstX] & 0xff00) >> 8);
                int db = dst[dstX] & 0xff;
                draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16) 
                    | ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)      
                    | (sb * sa / 255 + db * (255 - sa) / 255);           
            }
        }
    }
}
Chess::Chess(int gradeSize, int marginX, int marginY, float chessSize)
{
    this->gradeSize = gradeSize;
    this->margin_x = marginX;
    this->margin_y = marginY;
    this->chessSize = chessSize;
    playerFlag = CHESS_BLACK;
    for (int i = 0; i < gradeSize; i++)
    {
        vector<int> row;
        for (int j = 0; j < gradeSize; j++)
        {
            row.push_back(0);
        }
        chessMap.push_back(row);
    }
}

void Chess::init()
{
    initgraph(897, 895);
    loadimage(0, "res/棋盘2.jpg");
    mciSendString("play res/start.wav", 0, 0, 0);
    loadimage(&chessBlackImg, "res/black.png", chessSize, chessSize, true);
    loadimage(&chessWhiteImg, "res/white.png", chessSize, chessSize, true);
    loadimage(&chessBlackImg2, "res/black2.png", chessSize, chessSize, true);
    loadimage(&chessWhiteImg2, "res/white2.png", chessSize, chessSize, true);
    for (int i = 0; i < gradeSize; i++)
    {
        for (int j = 0; j < gradeSize; j++)
        {
            chessMap[i][j] = 0;
        }
    }
    playerFlag = true;
    fst = 0;

}

bool Chess::clickBoard(int x, int y, ChessPos* pos)
{
    int col = (x - margin_x) / chessSize;
    int row = (y - margin_y) / chessSize;
    int leftTopPosX = margin_x + chessSize * col;
    int leftTopPosY = margin_y + chessSize * row;
    int offset = chessSize * 0.4;
    int len;
    bool ret = false;
    do
    {
        //左上
        len = sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY) * (y - leftTopPosY));
        if (len < offset)
        {
            pos->row = row;
            pos->col = col;
            if (chessMap[pos->row][pos->col] == 0)
            {
                ret = true;
            }
            break;
        }
        //右上
        int x2 = leftTopPosX + chessSize;
        int y2 = leftTopPosY;
        len = sqrt((x - x2) * (x - x2) + (y - y2) * (y - y2));
        if (len < offset)
        {
            pos->row = row;
            pos->col = col + 1;
            if (chessMap[pos->row][pos->col] == 0)
            {
                ret = true;
            }
            break;
        }
        //左下
        x2 = leftTopPosX;
        y2 = leftTopPosY + chessSize;
        len = sqrt((x - x2) * (x - x2) + (y - y2) * (y - y2));
        if (len < offset)
        {
            pos->row = row + 1;
            pos->col = col;
            if (chessMap[pos->row][pos->col] == 0)
            {
                ret = true;
            }
            break;
        }
        //右下
        x2 = leftTopPosX + chessSize;
        y2 = leftTopPosY + chessSize;
        len = sqrt((x - x2) * (x - x2) + (y - y2) * (y - y2));
        if (len < offset)
        {
            pos->row = row + 1;
            pos->col = col + 1;
            if (chessMap[pos->row][pos->col] == 0)
            {
                ret = true;
            }
            break;
        }
    } while (0);
    
    return ret;
}

void Chess::chessDown(ChessPos* pos, chess_kind_t kind)
{
    mciSendString("play res/down7.WAV", 0, 0, 0);
    int x = margin_x + chessSize * pos->col - 0.5 * chessSize;
    int y = margin_y + chessSize * pos->row - 0.5 * chessSize;
    if (kind == CHESS_WHITE)
    {
        //putimagePNG(x2, y2, &chessBlackImg);
        putimagePNG(x, y, &chessWhiteImg2);
     }
    else
    {
        //putimagePNG(x2, y2, &chessWhiteImg);
        putimagePNG(x, y, &chessBlackImg2);
    }
    
    
    updateGameMap(pos);
    fst++;
}

int Chess::getGradeSize()
{

    return gradeSize;
}

int Chess::getChessData(ChessPos* pos)
{
    return chessMap[pos->row][pos->col];
}

int Chess::getChessData(int row, int col)
{
    return chessMap[row][col];
}

bool Chess::checkOver()
{
    if (checkWin())
    {
        Sleep(1500);
        if (!playerFlag)//玩家胜利
        {
            mciSendString("play res/胜利.mp3", 0, 0, 0);
            loadimage(0, "res/胜利1.jpg");
        }
        else
        {
            mciSendString("play res/失败.mp3", 0, 0, 0);
            loadimage(0, "res/失败1.jpg");
        }
        _getch();
        return true;
    }
    return false;
}

void Chess::updateGameMap(ChessPos* pos)//更新数据
{

    int x2 = margin_x + chessSize * lastPos.col - 0.5 * chessSize;
    int y2 = margin_y + chessSize * lastPos.row - 0.5 * chessSize;
    if (fst)
    {
        if (fst%2==0)
        {
            putimagePNG(x2, y2, &chessWhiteImg);
            //putimagePNG(x, y, &chessWhiteImg2);
        }
        else
        {
            putimagePNG(x2, y2, &chessBlackImg);
            //putimagePNG(x, y, &chessBlackImg2);
        }
    }
    lastPos = *pos;
    chessMap[pos->row][pos->col] = playerFlag ? CHESS_BLACK : CHESS_WHITE;
    playerFlag = !playerFlag;//换对方落子
}

bool Chess::checkWin()
{
    int row = lastPos.row;
    int col = lastPos.col;
    for (int i = 0; i < 5; i++)
    {
        if (col - i >= 0 && col - i + 4 < gradeSize && chessMap[row][col - i] == chessMap[row][col - i + 1] && chessMap[row][col - i] == chessMap[row][col - i + 2] && chessMap[row][col - i] == chessMap[row][col - i + 3] && chessMap[row][col - i] == chessMap[row][col - i + 4])
        {
            return true;
        }
        if (row - i >= 0 && row - i + 4 < gradeSize && chessMap[row - i][col] == chessMap[row - i + 1][col] && chessMap[row - i][col] == chessMap[row - i + 2][col] && chessMap[row - i][col] == chessMap[row - i + 3][col] && chessMap[row - i][col] == chessMap[row - i + 4][col])
        {
            return true;
        }
        if (col - i >= 0 && col - i + 4 < gradeSize && row + i - 4 >= 0 && row + i < gradeSize && chessMap[row + i][col - i] == chessMap[row + i - 1][col - i + 1] && chessMap[row + i][col - i] == chessMap[row + i - 2][col - i + 2] && chessMap[row + i][col - i] == chessMap[row + i - 3][col - i + 3] && chessMap[row + i][col - i] == chessMap[row + i - 4][col - i + 4])
        {
            return true;
        }
        if (row - i >= 0 && row - i + 4 < gradeSize && col - i >= 0 && col - i + 4 < gradeSize && chessMap[row - i][col - i] == chessMap[row - i + 1][col - i + 1] && chessMap[row - i][col - i] == chessMap[row - i + 2][col - i + 2] && chessMap[row - i][col - i] == chessMap[row - i + 3][col - i + 3] && chessMap[row - i][col - i] == chessMap[row - i + 4][col - i + 4])
        {
            return true;
        }
    }
    return false;
}
