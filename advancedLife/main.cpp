#include <iostream>
#include <random>
#include <cstdlib>
#include <ctime>

using namespace std;

const int default_size = 64;
int currentRound;
int bornAll = 0;
int bornEvery10 = 0;
int diedEvery10 = 0;

struct lcell
{
	bool now = 0;
	bool next = 0;
};

int amountOfCells = 0, amountOfRounds = 0;
bool xShift = 0, yShift = 0;
int sizeX = default_size;
int sizeY = default_size;
lcell ** field;

void resize(int x, int y){
	if (x > sizeX || sizeY < y){
		lcell** tmp = new lcell*[sizeX];
		for (int i = 0; i < sizeX; i++){
			tmp[i] = new lcell[sizeY];
			for (int j = 0; j < sizeY; j++)
				tmp[i][j] = field[i][j];
		}
		delete[] field;
		int newx, newy;
		newx = sizeX;
		newy = sizeY;
		while (x >= newx)
			newx *= 2;
		while (y >= newy)
			newy *= 2;
		field = new lcell*[newx];
		for (int i = 0; i < newx; i++){
			field[i] = new lcell[newy];
			if (i < sizeX)
			for (int j = 0; j < newy; j++)
				field[i][j] = tmp[i][j];
		}
		delete[] tmp;
		sizeX = newx;
		sizeY = newy;
	}
}

int getCellNow(int x, int y){
	if (sizeX > x && x > 0){
		if (sizeY > y && y > 0)
		{
			return field[x][y].now;
		}
		else return 0;
	}
	else return 0;
}

int getCellSum(int x, int y){
	int sum = 0;
		sum = getCellNow(x + 1, y - 1) + getCellNow(x + 1, y) + getCellNow(x + 1, y + 1) + getCellNow(x, y + 1);
		sum += getCellNow(x - 1, y - 1) + getCellNow(x - 1, y) + getCellNow(x - 1, y + 1) + getCellNow(x, y - 1);
	return sum;
}

void checkRound()
{
	int sum = 0;
	for (int x = 0; x < sizeX; x++)
		for (int y = 0; y < sizeY; y++)
		{
			sum = getCellSum(x, y);
			if (field[x][y].now == 0 && sum == 3){
				bornAll++;
				bornEvery10++;
			}
			if (field[x][y].now == 1 && (sum > 3 || sum < 2))
				diedEvery10++;
			if ((sum == 3) || ((sum == 2) && (field[x][y].now == 1)))
			{
				field[x][y].next = 1;
				xShift = x == 0 ? 1 : 0;
				yShift = y == 0 ? 1 : 0;
				int nx = x == sizeX - 1 ? sizeX * 2 : sizeX;
				int ny = y == sizeX - 1 ? sizeY * 2 : sizeY;
				if (nx != sizeX || ny != sizeY)
					resize(nx, ny);
			}

		}
}

void generateRound()
{
	xShift = 0;
	yShift = 0;
	checkRound();
	for (int i = 0; i < sizeX; i++)
		for (int j = 0; j < sizeY; j++){
			field[i + xShift][j + yShift].now = field[i][j].next;
			field[i][j].next = 0;
		}
}

int main()
{
	srand(time(0));
	field = new lcell*[default_size];
	for (int i = 0; i < default_size; i++)
		field[i] = new lcell[default_size];

	cout << "Welcome to G.O.L. simulator" << endl;
	cout << "Type starting position height and width" << endl;
	int startX = 0;
	int startY = 0;
	cin >> startX >> startY;
	resize(startX, startY);
	cout << "Type amount of rounds" << endl;
	cin >> amountOfRounds;
	char ans = 'a';
	cout << "Do you want to type all cells by yourself(y) or generate by Random generator(n)?" << endl;
		cin >> ans;
	cout << "Type amount of generated cells" << endl;
	cin >> amountOfCells;
	if (ans == 'y'){
		cout << "Type x and y of cell" << endl;
		int x = 0;
		int y = 0;
		for (int i = 0; i < amountOfCells; i++){
			cin >> x >> y;
			resize(y + 1, x + 1);
			field[y][x].now = 1;
		}
	}

	if (ans == 'n')
	for (int i = 0; i < amountOfCells; i++){
		int x = rand() % startX + 1;
		int y = rand() % startY + 1;
		resize(x + 1, y + 1);
		field[x][y].now = 1;
	}
	cout << endl;

	for (currentRound = 1; currentRound <= amountOfRounds; currentRound++){
		generateRound();
		if (currentRound % 10 == 0){
			cout << "Cells born from " << (currentRound / 10 - 1) * 10 << " to " << (currentRound / 10) * 10 << ": " << bornEvery10 << endl;
			if (diedEvery10 != 0)
				cout << "Average born/dead ratio from " << (currentRound / 10 - 1) * 10 << " to " << (currentRound / 10) * 10 << ": " << (double) bornEvery10 / diedEvery10 << endl;
			else
				cout << "Average born/dead ratio from " << (currentRound / 10 - 1) * 10 << " to " << (currentRound / 10) * 10 << ": INFINITE(nobody died)" << endl;
			bornEvery10 = 0;
			diedEvery10 = 0;
			cout << endl;
		}
	}
	cout << "Cells born: " << bornAll << endl;

	int camx = 0; int camy = 0;
	cout << "Press l, r, u, d to see another 60*60 block in specified destination" << endl;
	while (true){
		cout << "Current left upper position: x=" << camx << " y=" << camy << endl;
		for (int i = camy; i < camy + 60; i++){
			for (int j = camx; j < camx + 60; j++)
				cout << getCellNow(i, j);
			cout << endl;
		}

		ans = 'p';
		while (!(ans == 'l' || ans == 'r' || ans == 'd' || ans == 'u'))
			cin >> ans;
		switch (ans)
		{
		case 'd':
			camy += 60;
			break;
		case 'r':
			camx += 60;
			break;
		case 'l':
			if (camx >= 60)
				camx -= 60;
			else
				cout << "Its pointless, there is nothing" << endl;
			break;
		case 'u':
			if (camy >= 60)
				camy -= 60;
			else
				cout << "Its pointless, there is nothing" << endl;
			break;
		}
	}
	return 0;
}