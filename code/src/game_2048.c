#include "game_2048.h"

int game_st = 0; // 0:继续游戏 1:获胜 -1:失败
int score = 0;
int b_score = 0;
int change = 0; // 一次操作是否产生变化
int d[4][4];
int w_x[20], w_y[20], w_sum; // 存储空白数字块的坐标
double x[4] = {208.2, 303.5, 400.8, 497.8};
double y[4] = {59.1, 154.5, 249.5, 344.5};

void game_2048_init()
{
    srand(time(NULL));
    change = 1;
    game_st = score = 0;

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            d[i][j] = 0;

    int x = rand() % 4, y = rand() % 4, cnt = 2; // 起始方格数目

    while (cnt)
    {
        if (!d[x][y])
        {
            d[x][y] = rand() % 2 + 1;
            cnt--;
        }
        x = rand() % 4, y = rand() % 4;
    }
    game_2048_show();
}

int game_2048_get_score()
{
    return score;
}

void game_2048_show()
{
    if (!change)
        return;
    bmp_show(0, 0, BT_game_2048.page_bmp_name, NULL, NULL);
    char bmp_name[110];
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            // 输出矩阵
            printf("%d ", d[i][j]);

            if (!d[i][j])
                continue;

            sprintf(bmp_name, "../../bmp/game_2048/2048-%d.bmp", d[i][j]);
            bmp_show((int)x[j], (int)y[i], bmp_name, NULL, NULL);
        }
        printf("\n");
    }
    bmp_num_show(55.7, 181.4, 95, 20, score, NULL, 0);
    b_score = max(b_score, score);
    bmp_num_show(55.7, 375.4, 95, 20, b_score, NULL, 0);
}

int dx[] = {0, 1, 0, -1}, dy[] = {1, 0, -1, 0};
int check()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                int x = i + dx[k], y = j + dy[k];
                if (x >= 4 || x < 0 || y >= 4 || y < 0)
                    continue;
                if (d[x][y] == d[i][j])
                    return 0; // 继续游戏
            }
        }
    }
    return -1; // 游戏结束
}

int game_2048_update(int flag) // 0 left  1 up 2 right 3 down
{
    int stack[10], size = 0;

    int l = 0, r = 4, index = 1;
    if (flag >= 2) // right down
    {
        l = 3;
        r = -1;
        index = -1;
    }

    w_sum = change = 0;

    for (int i = 0; i < 4; i++)
    {
        for (int j = 1; j <= 5; j++)
            stack[j] = 0;
        size = 0;
        int flag_mix = 0;
        for (int j = l; j != r; j += index)
        {
            if (d[i][j] && flag % 2 == 0)
            {
                stack[++size] = d[i][j];
                if (size >= 2 && !flag_mix)
                {
                    int r = stack[size--];
                    stack[size + 1] = 0;
                    if (stack[size] == r)
                    {
                        stack[size] = r + 1;
                        score += qmi(2, stack[size]);
                        if (r + 1 == 11)
                            game_st = 1;
                        flag_mix = 1;
                    }
                    else
                    {
                        stack[++size] = r;
                    }
                }
                else
                    flag_mix = 0;
            }
            else if (d[j][i] && flag % 2 == 1)
            {
                stack[++size] = d[j][i];
                if (size >= 2 && !flag_mix)
                {
                    int r = stack[size--];
                    stack[size + 1] = 0;
                    if (stack[size] == r)
                    {
                        stack[size] = r + 1;
                        score += qmi(2, stack[size]);
                        if (r + 1 == 11)
                            game_st = 1;
                        flag_mix = 1;
                    }
                    else
                    {
                        stack[++size] = r;
                    }
                }
                else
                    flag_mix = 0;
            }
        }
        // 空白格
        for (int j = l, k = 1; j != r && k < 6; j += index, k++)
        {
            if (flag % 2 == 0)
            {
                if (d[i][j] != stack[k])
                    change = 1;
                d[i][j] = stack[k];
                if (!d[i][j])
                {
                    w_x[++w_sum] = i;
                    w_y[w_sum] = j;
                }
            }
            else
            {
                if (d[j][i] != stack[k])
                    change = 1;
                d[j][i] = stack[k];
                if (!d[j][i])
                {
                    w_x[++w_sum] = j;
                    w_y[w_sum] = i;
                }
            }
        }
    }

    if (w_sum)
    {
        if (change)
        {
            int t = rand() % w_sum + 1;
            d[w_x[t]][w_y[t]] = rand() % 2 + 1;
            w_sum--;
        }
        game_st = 0;
    }
    else
        game_st = check();
    game_2048_show();

    return game_st;
}
