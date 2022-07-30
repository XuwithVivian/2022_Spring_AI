#include<iostream>
#include<vector>
#include<string.h>
#include<fstream>
#include<bits/stdc++.h>
#include<time.h>

using namespace std;

// �ĸ����ĺ���
void A_h1(const vector<vector<int> > &start, const vector<vector<int> > &target);
void A_h2(const vector<vector<int> > &start, const vector<vector<int> > &target);
void IDA_h1(const vector<vector<int> > &start, const vector<vector<int> > &target);
void IDA_h2(const vector<vector<int> > &start, const vector<vector<int> > &target);

#define MAX 0x7FFFFFFF

// ����ṹ��
struct node
{
    int gx;     // �Ѿ��߹���·������
    int hx;     // ��������
    vector<vector<int> > status;    // 5*5����
    bool operator < (const node &x) const   // ���������
	{
		return gx + hx > x.gx + x.hx;
	}
    string path;    // ����ý���·��
};

int success = 0;    // �ж��Ƿ�õ����
priority_queue<node> open;      // ���ȶ���
set<vector<vector<int> > > close;       // �����Ѿ��߹��ķ���
set<vector<vector<int> > > open_set;    // �������ȶ����еķ���
string path = "";
vector<vector<int> > after_move;
stack<node> ida_stack;      // ջ
int row = 0, column = 0;    // ���������ڷ����е��к���

// ȡ��Сֵ
int min(int a, int b)
{
    if (a <= b)
        return a;
    else
        return b;
}

// �����پ���
int distant(int i, int j, int x, int y)
{
    int distance = 0;
    if (i > x)
        distance += i - x;
    else
        distance += x - i;

    if (j > y)
        distance += j - y;
    else
        distance += y - j;
    return distance;
}

// �ҵ�num��status�е��к���
void getpos(const vector<vector<int> > &status, int num)
{
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
            if (status[i][j] == num)
            {
                row = i;
                column = j;
            }
    }
}

// ��������1
int h1(const vector<vector<int> > &start, const vector<vector<int> > &target)
{
    int num = 0;
    for(int i = 0; i < 5; i++)
        for(int j = 0; j < 5; j++)
        {
            if (target[i][j] != 0 && start[i][j] != target[i][j])
                num++;
        }
    return num;
}

// ��������2
int h2(const vector<vector<int> > &start, const vector<vector<int> > &target)
{
    int distance = 0, road[5], min;
    for(int i = 0; i < 5; i++)
        for(int j = 0; j < 5; j++)
        {
            if (target[i][j] != 0 && start[i][j] != target[i][j])
            {
                min = 20;
                getpos(start, target[i][j]);
                road[0] = distant(i, j, row, column);
                road[1] = distant(i, j, 2, 0) + distant(2, 4, row, column) + 1;
                road[2] = distant(i, j, 2, 4) + distant(2, 0, row, column) + 1;
                road[3] = distant(i, j, 0, 2) + distant(4, 2, row, column) + 1;
                road[4] = distant(i, j, 4, 2) + distant(0, 2, row, column) + 1;
                for (int i = 0; i < 5; i++)
                {
                    if (min > road[i])
                        min = road[i];
                }
                distance += min;
            }
        }
    return distance;
}

// �ҵ�0��status�е��к���
void getzeropos(const vector<vector<int> > &status)
{
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
            if (status[i][j] == 0)
            {
                row = i;
                column = j;
            }
    }
}

// �����ƶ�
int movetoup(const vector<vector<int> > &status)
{
    after_move = status;
    getzeropos(after_move);
    int swap;
    if (row != 0 && after_move[row - 1][column] > 0)
    {
        swap = after_move[row][column];
        after_move[row][column] = after_move[row - 1][column];
        after_move[row - 1][column] = swap;
    }
    if (row == 0 && column == 2 && after_move[4][column] > 0)
    {
        swap = after_move[row][column];
        after_move[row][column] = after_move[4][column];
        after_move[4][column] = swap;
    }
    if (after_move == status)
        return 0;
    else
        return 1;
}

// �����ƶ�
int movetodown(const vector<vector<int> > &status)
{
    after_move = status;
    getzeropos(after_move);
    int swap;
    if (row != 4 && after_move[row + 1][column] > 0)
    {
        swap = after_move[row][column];
        after_move[row][column] = after_move[row + 1][column];
        after_move[row + 1][column] = swap;
    }
    if (row == 4 && column == 2 && after_move[0][column] > 0)
    {
        swap = after_move[row][column];
        after_move[row][column] = after_move[0][column];
        after_move[0][column] = swap;
    }
    if (after_move == status)
        return 0;
    else
        return 1;
}

// �����ƶ�
int movetoleft(const vector<vector<int> > &status)
{
    after_move = status;
    getzeropos(after_move);
    int swap;
    if (column != 0 && after_move[row][column - 1] > 0)
    {
        swap = after_move[row][column];
        after_move[row][column] = after_move[row][column - 1];
        after_move[row][column - 1] = swap;
    }
    if (column == 0 && row == 2 && after_move[row][4] > 0)
    {
        swap = after_move[row][column];
        after_move[row][column] = after_move[row][4];
        after_move[row][4] = swap;
    }
    if (after_move == status)
        return 0;
    else
        return 1;
}

// �����ƶ�
int movetoright(const vector<vector<int> > &status)
{
    after_move = status;
    getzeropos(after_move);
    int swap;
    if (column != 4 && after_move[row][column + 1] > 0)
    {
        swap = after_move[row][column];
        after_move[row][column] = after_move[row][column + 1];
        after_move[row][column + 1] = swap;
    }
    if (column == 4 && row == 2 && after_move[row][0] > 0)
    {
        swap = after_move[row][column];
        after_move[row][column] = after_move[row][0];
        after_move[row][0] = swap;
    }
    if (after_move == status)
        return 0;
    else
        return 1;
}

// ��ӡ�������ڵ���
void printvector(const vector<vector<int> > &status)
{
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
            cout << status[i][j] << " ";
        cout << endl;
    }
}

// ���ĺ���A_h1
void A_h1(const vector<vector<int> > &start, const vector<vector<int> > &target)
{
    node start_node, pass_node, next_node;
    start_node.gx = 0;
    start_node.hx = h1(start, target);
    start_node.status = start;
    start_node.path = "";
    open.push(start_node);
    while (!open.empty())
    {
        pass_node = open.top();
        if (pass_node.status == target)
        {
            path = pass_node.path;
            success = 1;
            return;
        }
        open.pop();
        if (close.find(pass_node.status) == close.end())
        {
            close.insert(pass_node.status);
            for (int i = 0; i < 4; i++)
            {
                if (i == 0)
                {
                    next_node = pass_node;
                    if (movetoup(next_node.status) == 1);{
                        next_node.status = after_move;
                        next_node.gx++;
                        next_node.hx = h1(next_node.status, target);
                        next_node.path += 'U';
                        open.push(next_node);
                    }
                }
                else if (i == 1)
                {
                    next_node = pass_node;
                    if (movetodown(next_node.status) == 1);{
                        next_node.status = after_move;
                        next_node.gx++;
                        next_node.hx = h1(next_node.status, target);
                        next_node.path += 'D';
                        open.push(next_node);
                    }
                }
                else if (i == 2)
                {
                    next_node = pass_node;
                    if (movetoleft(next_node.status) == 1);{
                        next_node.status = after_move;
                        next_node.gx++;
                        next_node.hx = h1(next_node.status, target);
                        next_node.path += 'L';
                        open.push(next_node);
                    }
                }
                else
                {
                    next_node = pass_node;
                    if (movetoright(next_node.status) == 1);{
                        next_node.status = after_move;
                        next_node.gx++;
                        next_node.hx = h1(next_node.status, target);
                        next_node.path += 'R';
                        open.push(next_node);
                    }
                }
            }
        }
    }
    success = 0;
    return;
}

// ���ĺ���A_h2
void A_h2(const vector<vector<int> > &start, const vector<vector<int> > &target)
{
    node start_node, pass_node, next_node;
    start_node.gx = 0;
    start_node.hx = h2(start, target);
    start_node.status = start;
    start_node.path = "";
    open.push(start_node);
    while (!open.empty())
    {
        pass_node = open.top();
        if (pass_node.status == target)
        {
            path = pass_node.path;
            success = 1;
            return;
        }
        open.pop();
        if (close.find(pass_node.status) == close.end())
        {
            close.insert(pass_node.status);
            for (int i = 0; i < 4; i++)
            {
                if (i == 0)
                {
                    next_node = pass_node;
                    if (movetoup(next_node.status) == 1);{
                        next_node.status = after_move;
                        next_node.gx++;
                        next_node.hx = h2(next_node.status, target);
                        next_node.path += 'U';
                        open.push(next_node);
                    }
                }
                else if (i == 1)
                {
                    next_node = pass_node;
                    if (movetodown(next_node.status) == 1);{
                        next_node.status = after_move;
                        next_node.gx++;
                        next_node.hx = h2(next_node.status, target);
                        next_node.path += 'D';
                        open.push(next_node);
                    }
                }
                else if (i == 2)
                {
                    next_node = pass_node;
                    if (movetoleft(next_node.status) == 1);{
                        next_node.status = after_move;
                        next_node.gx++;
                        next_node.hx = h2(next_node.status, target);
                        next_node.path += 'L';
                        open.push(next_node);
                    }
                }
                else
                {
                    next_node = pass_node;
                    if (movetoright(next_node.status) == 1);{
                        next_node.status = after_move;
                        next_node.gx++;
                        next_node.hx = h2(next_node.status, target);
                        next_node.path += 'R';
                        open.push(next_node);
                    }
                }
            }
        }
    }
    success = 0;
    return;
}

// ���ĺ���IDA_h1
void IDA_h1(const vector<vector<int> > &start, const vector<vector<int> > &target)
{
    node start_node, pass_node, next_node;
    start_node.gx = 0;
    start_node.hx = h1(start, target);
    start_node.status = start;
    start_node.path = "";
    int d_limit = start_node.hx;
    while (d_limit < MAX) {
        int next_d_limit = MAX;
        ida_stack.push(start_node);
        open_set.insert(start);
        while (!ida_stack.empty())
        {
            close.clear();
            pass_node = ida_stack.top();
            if (pass_node.status == target)
            {
                path = pass_node.path;
                success = 1;
                return;
            }
            ida_stack.pop();
            open_set.erase(pass_node.status);
            if (pass_node.gx + pass_node.hx > d_limit) {
                next_d_limit = min(next_d_limit, pass_node.gx + pass_node.hx);
            }
            else {
                if (close.find(pass_node.status) == close.end())
                {
                    close.insert(pass_node.status);
                    for (int i = 0; i < 4; i++)
                    {
                        if (i == 0)
                        {
                            next_node = pass_node;
                            if (movetoup(next_node.status) == 1);{
                                if (close.find(after_move) == close.end() && open_set.find(after_move) == open_set.end()) {
                                    next_node.status = after_move;
                                    next_node.gx++;
                                    next_node.hx = h1(next_node.status, target);
                                    next_node.path += 'U';
                                    ida_stack.push(next_node);
                                    open_set.insert(next_node.status);
                                }
                            }
                        }
                        else if (i == 1)
                        {
                            next_node = pass_node;
                            if (movetodown(next_node.status) == 1);{
                                if (close.find(after_move) == close.end() && open_set.find(after_move) == open_set.end()) {
                                    next_node.status = after_move;
                                    next_node.gx++;
                                    next_node.hx = h1(next_node.status, target);
                                    next_node.path += 'D';
                                    ida_stack.push(next_node);
                                    open_set.insert(next_node.status);
                                }
                            }
                        }
                        else if (i == 2)
                        {
                            next_node = pass_node;
                            if (movetoleft(next_node.status) == 1);{
                                if (close.find(after_move) == close.end() && open_set.find(after_move) == open_set.end()) {
                                    next_node.status = after_move;
                                    next_node.gx++;
                                    next_node.hx = h1(next_node.status, target);
                                    next_node.path += 'L';
                                    ida_stack.push(next_node);
                                    open_set.insert(next_node.status);
                                }
                            }
                        }
                        else
                        {
                            next_node = pass_node;
                            if (movetoright(next_node.status) == 1);{
                                if (close.find(after_move) == close.end() && open_set.find(after_move) == open_set.end()) {
                                    next_node.status = after_move;
                                    next_node.gx++;
                                    next_node.hx = h1(next_node.status, target);
                                    next_node.path += 'R';
                                    ida_stack.push(next_node);
                                    open_set.insert(next_node.status);
                                }
                            }
                        }
                    }
                }
            }
        }
        d_limit = next_d_limit;
    }
    success = 0;
    return;
}

// ���ĺ���IDA_h2
void IDA_h2(const vector<vector<int> > &start, const vector<vector<int> > &target)
{
    node start_node, pass_node, next_node;
    start_node.gx = 0;
    start_node.hx = h2(start, target);
    start_node.status = start;
    start_node.path = "";
    int d_limit = start_node.hx;
    while (d_limit < MAX) {
        int next_d_limit = MAX;
        ida_stack.push(start_node);
        open_set.insert(start);
        while (!ida_stack.empty())
        {
            close.clear();
            pass_node = ida_stack.top();
            if (pass_node.status == target)
            {
                path = pass_node.path;
                success = 1;
                return;
            }
            ida_stack.pop();
            open_set.erase(pass_node.status);
            if (pass_node.gx + pass_node.hx > d_limit) {
                next_d_limit = min(next_d_limit, pass_node.gx + pass_node.hx);
            }
            else {
                if (close.find(pass_node.status) == close.end())
                {
                    close.insert(pass_node.status);
                    for (int i = 0; i < 4; i++)
                    {
                        if (i == 0)
                        {
                            next_node = pass_node;
                            if (movetoup(next_node.status) == 1);{
                                if (close.find(after_move) == close.end() && open_set.find(after_move) == open_set.end()) {
                                    next_node.status = after_move;
                                    next_node.gx++;
                                    next_node.hx = h2(next_node.status, target);
                                    next_node.path += 'U';
                                    ida_stack.push(next_node);
                                    open_set.insert(next_node.status);
                                }
                            }
                        }
                        else if (i == 1)
                        {
                            next_node = pass_node;
                            if (movetodown(next_node.status) == 1);{
                                if (close.find(after_move) == close.end() && open_set.find(after_move) == open_set.end()) {
                                    next_node.status = after_move;
                                    next_node.gx++;
                                    next_node.hx = h2(next_node.status, target);
                                    next_node.path += 'D';
                                    ida_stack.push(next_node);
                                    open_set.insert(next_node.status);
                                }
                            }
                        }
                        else if (i == 2)
                        {
                            next_node = pass_node;
                            if (movetoleft(next_node.status) == 1);{
                                if (close.find(after_move) == close.end() && open_set.find(after_move) == open_set.end()) {
                                    next_node.status = after_move;
                                    next_node.gx++;
                                    next_node.hx = h2(next_node.status, target);
                                    next_node.path += 'L';
                                    ida_stack.push(next_node);
                                    open_set.insert(next_node.status);
                                }
                            }
                        }
                        else
                        {
                            next_node = pass_node;
                            if (movetoright(next_node.status) == 1);{
                                if (close.find(after_move) == close.end() && open_set.find(after_move) == open_set.end()) {
                                    next_node.status = after_move;
                                    next_node.gx++;
                                    next_node.hx = h2(next_node.status, target);
                                    next_node.path += 'R';
                                    ida_stack.push(next_node);
                                    open_set.insert(next_node.status);
                                }
                            }
                        }
                    }
                }
            }
        }
        d_limit = next_d_limit;
    }
    success = 0;
    return;
}

int main(int argc, char *argv[])
{
    int num;
    if (argc != 4)
    {
        printf("���������������");
    }
    FILE *fp1, *fp2, *fp3;
    ofstream os;
    clock_t begin, end;
    double duration;
    char path_input_1[30] = "../../data/";
    char path_input_2[30] = "../../data/";
    char path_output[30] = "../output/";
    strcat(path_input_1, argv[2]);
    strcat(path_input_2, argv[3]);
    vector<vector<int> > start;
    vector<vector<int> > target;
    vector<int> line;
    if((fp1 = fopen(path_input_1,"r"))==NULL)
    {
        printf("cannot open input file\n");
        exit(0);
    }
    if((fp2 = fopen(path_input_2,"r"))==NULL)
    {
        printf("cannot open target file\n");
        exit(0);
    }
    for (int i = 0; i < 5; i++)     // ����start
    {
        for (int j = 0; j < 5; j++) {
            fscanf(fp1, "%d", &num);
            line.push_back(num);
        }
        start.push_back(line);
        line.clear();
    }
    for (int i = 0; i < 5; i++)     // ����target
    {
        for (int j = 0; j < 5; j++) {
            fscanf(fp2, "%d", &num);
            line.push_back(num);
        }
        target.push_back(line);
        line.clear();
    }
    if (!strcmp(argv[1], "A_h1"))       // ����A_h1
    {
        begin = clock();
        A_h1(start, target);
        end = clock();
        os.open("../output/output_A_h1.txt", ios::app);
        duration = (double)(end - begin) / CLOCKS_PER_SEC;
        if (success == 1)
            os<<path<<","<<duration<<endl;
        else
            os<<"x,x"<<endl;
        os.close();
    }
    else if (!strcmp(argv[1], "A_h2"))       // ����A_h2
    {
        begin = clock();
        A_h2(start, target);
        end = clock();
        os.open("../output/output_A_h2.txt", ios::app);
        duration = (double)(end - begin) / CLOCKS_PER_SEC;
        if (success == 1)
            os<<path<<","<<duration<<endl;
        else
            os<<"x,x"<<endl;
        os.close();
    }
    else if (!strcmp(argv[1], "IDA_h1"))       // ����IDA_h1
    {
        begin = clock();
        IDA_h1(start, target);
        end = clock();
        os.open("../output/output_IDA_h1.txt", ios::app);
        duration = (double)(end - begin) / CLOCKS_PER_SEC;
        if (success == 1)
            os<<path<<","<<duration<<endl;
        else
            os<<"x,x"<<endl;
        os.close();
    }
    else if (!strcmp(argv[1], "IDA_h2"))       // ����IDA_h2
    {
        begin = clock();
        IDA_h2(start, target);
        end = clock();
        os.open("../output/output_IDA_h2.txt", ios::app);
        duration = (double)(end - begin) / CLOCKS_PER_SEC;
        if (success == 1)
            os<<path<<","<<duration<<endl;
        else
            os<<"x,x"<<endl;
        os.close();
    }
    fclose(fp1);
    fclose(fp2);
    return 0;
}