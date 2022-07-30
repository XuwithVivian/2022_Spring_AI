#include<bits/stdc++.h>
using namespace std;

int day[7][10] = {0};     // 每天的工人
int length_day[7] = {0}; // 每天工人的数量
int work[10][7] = {0};    // 每位工人的工作天
int length_work[10] = {0};   // 每位工人工作的天数
int n1 = 7, n2 = 10;
int worker1[7] = {1, 1, 0, 0, 0, 0, 0};     // 1表示senior,2表示junior
int worker2[10] = {1, 1, 0, 0, 0, 0, 0, 1, 0, 1};

// 判断长度为len的数组a中是否有n元素
bool find_value(int a[], int n, int len)
{
    for (int i = 0; i < len; i++)
    {
        if (a[i] == n)
            return true;
    }
    return false;
}

// 判断车间1将工人worker加入日期date是否违反约束条件
bool check_1(int date, int worker)
{
    for (int j = 0; j < length_day[date]; j++)
    {
        if (day[date][j] == worker + 1) // 工人已在这一天工作
            return false;
    }
    day[date][length_day[date]++] = worker + 1;
    work[worker][length_work[worker]++] = date + 1;
    int flag1 = 0, flag2 = 0, flag3 = 0, flag4 = 0, flag6 = 0;
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < length_day[i]; j++)
        {
            if (day[i][j] == 1)
                flag1 = 1;
            if (day[i][j] == 2)
                flag2 = 1;
            if (day[i][j] == 3)
                flag3 = 1;
            if (day[i][j] == 4)
                flag4 = 1;
            if (day[i][j] == 6)
                flag6 = 1;
        }
        // 存在矛盾关系
        if ((flag1 && flag4) || (flag2 && flag3) || (flag3 && flag6)){
            day[date][--length_day[date]] = 0;
            work[worker][--length_work[worker]] = 0;
            return false;
        }
        flag1 = 0;
        flag2 = 0;
        flag3 = 0;
        flag4 = 0;
        flag6 = 0;
    }
    for (int i = 0; i < n1; i++)
    {
        if (length_work[i] > 5){    // 存在工人休息时间少于两天
            day[date][--length_day[date]] = 0;
            work[worker][--length_work[worker]] = 0;
            return false;
        }   //  存在工人连续休息三天
        for (int j = 1; j <= date - 2; j++)
        {
            if (!find_value(work[i], j, length_work[i]) && !find_value(work[i], j + 1, length_work[i]) && !find_value(work[i], j + 2, length_work[i]))
            {
                day[date][--length_day[date]] = 0;
                work[worker][--length_work[worker]] = 0;
                return false;
            }
        }
    }
    int n = n1;
    for (int i = 0; i < n1; i++)
    {
        if (length_work[i] == 5)
            n--;
    }
    if (n < 4)      // 一天中凑不齐4个人
    {
        day[date][--length_day[date]] = 0;
        work[worker][--length_work[worker]] = 0;
        return false;
    }
    day[date][--length_day[date]] = 0;
    work[worker][--length_work[worker]] = 0;
    return true;
}

// 判断车间2将工人worker加入日期date是否违反约束条件
bool check_2(int date, int worker)
{
    for (int j = 0; j < length_day[date]; j++)
    {
        if (day[date][j] == worker + 1) // 工人已在这一天工作
            return false;
    }
    day[date][length_day[date]++] = worker + 1;
    work[worker][length_work[worker]++] = date + 1;
    int flag1 = 0, flag2 = 0, flag5 = 0, flag6 = 0, flag8 = 0, flag10 = 0;
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < length_day[i]; j++)
        {
            if (day[i][j] == 1)
                flag1 = 1;
            if (day[i][j] == 2)
                flag2 = 1;
            if (day[i][j] == 5)
                flag5 = 1;
            if (day[i][j] == 6)
                flag6 = 1;
            if (day[i][j] == 8)
                flag8 = 1;
            if (day[i][j] == 10)
                flag10 = 1;
        }
        // 存在矛盾关系
        if ((flag1 && flag5) || (flag2 && flag6) || (flag8 && flag10)){
            day[date][--length_day[date]] = 0;
            work[worker][--length_work[worker]] = 0;
            return false;
        }
        flag1 = 0;
        flag2 = 0;
        flag5 = 0;
        flag6 = 0;
        flag8 = 0;
        flag10 = 0;
    }
    for (int i = 0; i < n2; i++)
    {
        if (length_work[i] > 5){    // 存在工人休息时间少于两天
            day[date][--length_day[date]] = 0;
            work[worker][--length_work[worker]] = 0;
            return false;
        }   //  存在工人连续休息三天
        for (int j = 1; j <= date - 2; j++)
        {
            if (!find_value(work[i], j, length_work[i]) && !find_value(work[i], j + 1, length_work[i]) && !find_value(work[i], j + 2, length_work[i]))
            {
                day[date][--length_day[date]] = 0;
                work[worker][--length_work[worker]] = 0;
                return false;
            }
        }
    }
    int n = n2;
    for (int i = 0; i < n2; i++)
    {
        if (length_work[i] == 5)
            n--;
    }
    if (n < 5)      // 一天中凑不齐5个人
    {
        day[date][--length_day[date]] = 0;
        work[worker][--length_work[worker]] = 0;
        return false;
    }
    day[date][--length_day[date]] = 0;
    work[worker][--length_work[worker]] = 0;
    return true;
}

// 判断车间1是否满足
bool complete_1()
{
    int flag1 = 0, flag2 = 0, flag3 = 0, flag4 = 0, flag6 = 0;
    for (int i = 0; i < 7; i++) {
        int senior = 0;
        if (length_day[i] < 4)      // 每天至少要有4个人
            return false;
        for (int j = 0; j < length_day[i]; j++)
        {
            if (worker1[day[i][j] - 1] == 1)
                senior = 1;
            if (day[i][j] == 1)
                flag1 = 1;
            if (day[i][j] == 2)
                flag2 = 1;
            if (day[i][j] == 3)
                flag3 = 1;
            if (day[i][j] == 4)
                flag4 = 1;
            if (day[i][j] == 6)
                flag6 = 1;
        }
        // 不存在个人矛盾且至少有一名senior
        if ((flag1 && flag4) || (flag2 && flag3) || (flag3 && flag6) || senior == 0)
            return false;
        flag1 = 0;
        flag2 = 0;
        flag3 = 0;
        flag4 = 0;
        flag6 = 0;
        senior = 0;
    }
    for (int i = 0; i < n1; i++)
    {
        if (length_work[i] > 5)     // 每个工人每周必须休息2天或2天以上
            return false;
        // 每个工人每周不可以连续休息3天
        for (int j = 1; j <= 5; j++)
        {
            if (!find_value(work[i], j, length_work[i]) && !find_value(work[i], j + 1, length_work[i]) && !find_value(work[i], j + 2, length_work[i]))
                return false;
        }
    }
    return true;
}

// 判断车间2是否满足
bool complete_2()
{
    int flag1 = 0, flag2 = 0, flag5 = 0, flag6 = 0, flag8 = 0, flag10 = 0;
    for (int i = 0; i < 7; i++) {
        int senior = 0;
        if (length_day[i] < 5)      // 每天至少要有5个人
            return false;
        for (int j = 0; j < length_day[i]; j++)
        {
            if (worker2[day[i][j] - 1] == 1)
                senior = 1;
            if (day[i][j] == 1)
                flag1 = 1;
            if (day[i][j] == 2)
                flag2 = 1;
            if (day[i][j] == 5)
                flag5 = 1;
            if (day[i][j] == 6)
                flag6 = 1;
            if (day[i][j] == 8)
                flag8 = 1;
            if (day[i][j] == 10)
                flag10 = 1;
        }
        // 不存在个人矛盾且至少有一名senior
        if ((flag1 && flag5) || (flag2 && flag6) || (flag8 && flag10) || senior == 0)
            return false;
        flag1 = 0;
        flag2 = 0;
        flag5 = 0;
        flag6 = 0;
        flag8 = 0;
        flag10 = 0;
        senior = 0;
    }
    for (int i = 0; i < n2; i++)     // 每个工人每周必须休息2天或2天以上
    {
        if (length_work[i] > 5)
            return false;
        // 每个工人每周不可以连续休息3天
        for (int j = 1; j <= 5; j++)
        {
            if (!find_value(work[i], j, length_work[i]) && !find_value(work[i], j + 1, length_work[i]) && !find_value(work[i], j + 2, length_work[i]))
                return false;
        }
    }
    return true;
}

// 车间1的最少剩余值启发式
int mrv_1(int n)
{
    int next = 8;
    int min = 11;
    for (int i = 0; i < 7; i++)     // 找到满足约束的工人数最少的日期
    {
        int num = 0;
        for (int j = day[i][length_day[i] - 1]; j < n; j++)
        {
            if (check_1(i, j))
                num++;
        }
        if (num < min && num != 0)
        {
            min = num;
            next = i;
        }
    }
    return next;
}

// 车间2的最少剩余值启发式
int mrv_2(int n)
{
    int next = 8;
    int min = 11;
    for (int i = 0; i < 7; i++)     // 找到满足约束的工人数最少的日期
    {
        int num = 0;
        for (int j = day[i][length_day[i] - 1]; j < n; j++)
        {
            if (check_2(i, j))
                num++;
        }
        if (num < min && num != 0)
        {
            min = num;
            next = i;
        }
    }
    return next;
}

// 车间1的CSP回溯搜索
bool backtrack_1(int date)
{
    if (complete_1())
        return true;
    if (date == 8)
        return false;
    int next;
    // 对这一天还可能可以加入的工人进行遍历
    for (int i = day[date][length_day[date] - 1]; i < n1; i++)
    {
        if (check_1(date, i))   // 判断加入该工人是否满足约束
        {
            day[date][length_day[date]++] = i + 1;
            work[i][length_work[i]++] = date + 1;
            next = mrv_1(n1);       // 利用最小剩余值判断下一步遍历的日期
            if (backtrack_1(next))
                return true;
            day[date][--length_day[date]] = 0;
            work[i][--length_work[i]] = 0;
        }
    }
    return false;
}

// 车间2的CSP回溯搜索
bool backtrack_2(int date)
{
    if (complete_2())
        return true;
    if (date == 8)
        return false;
    int next;
    // 对这一天还可能可以加入的工人进行遍历
    for (int i = day[date][length_day[date] - 1]; i < n2; i++)
    {
        if (check_2(date, i))   // 判断加入该工人是否满足约束
        {
            day[date][length_day[date]++] = i + 1;
            work[i][length_work[i]++] = date + 1;
            next = mrv_2(n2);       // 利用最小剩余值判断下一步遍历的日期
            if (backtrack_2(next))
                return true;
            day[date][--length_day[date]] = 0;
            work[i][--length_work[i]] = 0;
        }
    }
    return false;
}

// 将数组置0
void transzero()
{
    for (int i = 0; i < 7; i++)
    {
        length_day[i] = 0;
        for (int j = 0; j < 10; j++)
            day[i][j] = 0;
    }
    for (int i = 0; i < 10; i++)
    {
        length_work[i] = 0;
        for (int j = 0; j < 7; j++)
            work[i][j] = 0;
    }
}

int main()
{
    FILE *fp1, *fp2;
    if((fp1 = fopen("../output/output1.txt","w"))==NULL)
    {
        printf("cannot open output1 file\n");
        exit(0);
    }
    if((fp2 = fopen("../output/output2.txt","w"))==NULL)
    {
        printf("cannot open output2 file\n");
        exit(0);
    }
    backtrack_1(0);
    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < length_day[i]; j++)
        {
            fprintf(fp1, "%d ", day[i][j]);
        }
        fprintf(fp1, "\n");
    }
    transzero();
    backtrack_2(0);
    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < length_day[i]; j++)
        {
            fprintf(fp2, "%d ", day[i][j]);
        }
        fprintf(fp2, "\n");
    }
    return 0;
}