#include <iostream>
#include <vector>
#include <io.h>
#include <fcntl.h>

using namespace std;

enum facing
{
    LEFT,
    RIGHT,
    UP,
    DOWN,
    STAY
};

int puzzle[3][3], posX, posY, target, deep;

class state
{
public:
    int arr[3][3], x, y, f;
    string way;
    facing canFace;

    state(int a[3][3], string way, facing canFace, int x, int y, int f)
    {
        this->f = f;
        this->way = way;
        this->canFace = canFace;
        this->x = x;
        this->y = y;

        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                arr[i][j] = a[i][j];
    }

    bool canMoveLeft()
    {
        return canFace != LEFT && y > 0 && f <= deep;
    }
    bool canMoveRight()
    {
        return canFace != RIGHT && y < 2 && f <= deep;
    }
    bool canMoveUp()
    {
        return canFace != UP && x > 0 && f <= deep;
    }
    bool canMoveDown()
    {
        return canFace != DOWN && x < 2 && f <= deep;
    }

    bool backTrack()
    {
        return f == deep;
    }

    void moveLeft()
    {
        swap(arr[x][y], arr[x][y - 1]);
        y--;
        canFace = RIGHT;
        way += "l";
        f++;
    }
    void moveRight()
    {
        swap(arr[x][y], arr[x][y + 1]);
        y++;
        canFace = LEFT;
        way += "r";
        f++;
    }
    void moveUp()
    {
        swap(arr[x][y], arr[x - 1][y]);
        x--;
        canFace = DOWN;
        way += "u";
        f++;
    }
    void moveDown()
    {
        swap(arr[x][y], arr[x + 1][y]);
        x++;
        canFace = UP;
        way += "d";
        f++;
    }

    bool checkFinish()
    {
        if (target == 1)
        {
            for (int i = 0; i <= 2; i++)
                if (arr[0][i] != i + 1 || arr[2][i] != 7 - i)
                    return false;

            return arr[1][0] != 8 || arr[1][2] != 4 ? false : true;
        }
        else
        {
            for (int i = 0; i <= 2; i++)
                if (arr[0][i] != i || arr[1][i] != i + 3 || arr[2][i] != i + 6)
                    return false;

            return true;
        }
    }
};

void moveLeft()
{
    swap(puzzle[posX][posY], puzzle[posX][posY - 1]);
    posY--;
}
void moveRight()
{
    swap(puzzle[posX][posY], puzzle[posX][posY + 1]);
    posY++;
}
void moveUp()
{
    swap(puzzle[posX][posY], puzzle[posX - 1][posY]);
    posX--;
}
void moveDown()
{
    swap(puzzle[posX][posY], puzzle[posX + 1][posY]);
    posX++;
}

void initPuzzle()
{
    _setmode(_fileno(stdout), _O_U16TEXT);

    wcout << L"=== Nhập 9 giá trị khởi đầu cho trò chơi (1 - 8)\n";
    wcout << L"=== 0 là vị trí bắt đầu:\n -> ";
    cin >> puzzle[0][0] >> puzzle[0][1] >> puzzle[0][2];
    cin >> puzzle[1][0] >> puzzle[1][1] >> puzzle[1][2];
    cin >> puzzle[2][0] >> puzzle[2][1] >> puzzle[2][2];
    wcout << L"Nhập độ sâu của thuật toán: ";
    cin >> deep;

    bool checked = true;
    int sum = 0;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
        {
            sum += puzzle[i][j];
            if (puzzle[i][j] > 8)
            {
                checked = false;
                break;
            }
        }

    if (sum != 36 || checked == false)
    {
        wcout << L"! Nhập sai dữ liệu... Vui lòng nhập lại !\n\n";
        return initPuzzle();
    }

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (puzzle[i][j] == 0)
            {
                posX = i;
                posY = j;
                return;
            }
}

void printPuzzle()
{
	wcout << "\n    .-----------.\n   ";
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
			if (puzzle[i][j] != 0)
				wcout << " | " << puzzle[i][j];
			else
				wcout << " | " << " ";
		if (i != 2)
			wcout << " |\n    |---+---+---|\n   ";
		else
			wcout << " |\n    '-----------'\n";
	}
}

bool checkFinish()
{
    int counter1 = 0, counter2 = 0;
    for (int i = 0; i < 3; i++)
        if (puzzle[0][i] == i + 1)
            counter1++;

    for (int i = 0; i < 3; i++)
        if (puzzle[2][i] == 7 - i)
            counter1++;

    if (puzzle[1][0] == 8)
        counter1++;
    if (puzzle[1][2] == 4)
        counter1++;

    if (counter1 == 8)
        return true;

    for (int i = 0; i < 3; i++)
    {
        if (puzzle[0][i] == i)
            counter2++;
        if (puzzle[1][i] == i + 3)
            counter2++;
        if (puzzle[2][i] == i + 6)
            counter2++;
    }

    if (counter2 == 8)
        return true;

    return false;
}

void checkTarget()
{
    int sum = 0;

    for (int p = 0; p < 9; p++)
    {
        int row = p / 3;
        int col = p % 3;
        int counter = puzzle[row][col];

        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                if (row < i && puzzle[i][j] < counter && puzzle[i][j] != 0)
                    sum++;
                else if (row == i && col < j && puzzle[i][j] < counter && puzzle[i][j] != 0)
                    sum++;
    }

    target = sum % 2;
}

main()
{
    _setmode(_fileno(stdout), _O_U16TEXT);

    int step = 0;
    long long int numOfState = 0;
    bool check = checkFinish();

    initPuzzle();
    checkTarget();
    const clock_t beginTime = clock();
    string way = "";
    state stt(puzzle, "", STAY, posX, posY, 0);
    vector<state> vt, vtBackTrack;
    vt.push_back(stt);

    if (target == 1)
		wcout << L"=== Xác định trạng thái đích 1\n";
	else
		wcout << L"=== Xác định trạng thái đích 2\n";

	wcout << "======================================================\n";
	wcout << L"   Trạng thái ban đầu";
	printPuzzle();

    while (!check)
    {
        if (vt.size() == 0)
        {
            for (int i = 0; i < vtBackTrack.size(); i++)
                vt.push_back(vtBackTrack.at(i));

            deep += 10;
        }

        vector<state> open;
        
        int i = vt.size() - 1;
        if (vt.at(i).checkFinish())
        {
            way = vt.at(i).way;
            check = true;
            break;
        }
        else
        {
            if (vt.at(i).canMoveUp())
            {
                state stt = vt.at(i);
                stt.moveUp();
                open.push_back(stt);
                if (stt.backTrack())
                    vtBackTrack.push_back(stt);
            }
            if (vt.at(i).canMoveDown())
            {
                state stt = vt.at(i);
                stt.moveDown();
                open.push_back(stt);
                if (stt.backTrack())
                    vtBackTrack.push_back(stt);
            }
            if (vt.at(i).canMoveRight())
            {
                state stt = vt.at(i);
                stt.moveRight();
                open.push_back(stt);
                if (stt.backTrack())
                    vtBackTrack.push_back(stt);
            }
            if (vt.at(i).canMoveLeft())
            {
                state stt = vt.at(i);
                stt.moveLeft();
                open.push_back(stt);
                if (stt.backTrack())
                    vtBackTrack.push_back(stt);
            }
        }

        vt.pop_back();

        for (int i = 0; i < open.size(); i++)
            vt.push_back(open.at(i));

        numOfState++;
    }

    for (int i = 0; i < way.length(); i++)
    {
        if (way[i] == 'l')
        {
            moveLeft();
            wcout << "=========================\n";
            wcout << " " << i + 1 << L".Di chuyển sang trái";
            printPuzzle();
        }
        else if (way[i] == 'r')
        {
            moveRight();
            wcout << "=========================\n";
            wcout << " " << i + 1 << L".Di chuyển sang phải";
            printPuzzle();
        }
        else if (way[i] == 'u')
        {
            moveUp();
            wcout << "=========================\n";
            wcout << " " << i + 1 << L".Di chuyển lên trên";
            printPuzzle();
        }
        else if (way[i] == 'd')
        {
            moveDown();
            wcout << "=========================\n";
            wcout << " " << i + 1 << L".Di chuyển xuống dưới";
            printPuzzle();
        }
    }
    wcout << L"======================================================\n";
    wcout << L"Thuật toán IDS (Tìm kiếm theo chiều sâu lặp dần):\n";
    wcout << L"Số bước đi: " << way.length() << endl;
    wcout << L"Số phép toán đã thực hiện: " << numOfState << endl;
    wcout << L"Độ sâu đã tăng dần: " << deep << endl;
    wcout << L"Thời gian tính toán: " << float(clock() - beginTime) / CLOCKS_PER_SEC << "s\n";
    wcout << L"======================================================\n";
    wcout << L"=== Trí tuệ nhân tạo - Nhóm 9\n";
    wcout << L" 2020601638 - Vũ Hồng Phương\n";
    wcout << L" 2020601369 - Đặng Quang Trung\n";
    wcout << L" 2020601349 - Nguyễn Xuân Trường\n";
    wcout << L" 2020600692 - Nguyễn Anh Tú\n";
    wcout << L" 2020600313 - Cao Văn Sơn\n";
    getchar();
    wcout << L"=== Xin cảm ơn!";
    getchar();
}
