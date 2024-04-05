#include <iostream>  //подключаем заголовочный файл для работы с вводом/выводом через стандартный поток ввода-вывода
#include <windows.h>  //подключаем заголовочный файл для работы с функциями и структурами Windows API
#include <conio.h>  //подключаем заголовочный файл для работы с функциями для работы с консольным вводом-выводом и управлением курсором
#include <ctime>  //подключаем аголовочный файл для работы с функциями для работы с временем и датой
#include <vector>  //подключаем заголовочный файл для работы с динамическими массивами и стандартными контейнерами вектор
#include <locale>  //подключаем заголовочный файл для работы с локализацией и локальными функциями
#include <string>  //подключаем заголовочный файл для работы со строками
#include <stack>  //подключаем заголовочный файл для работый с контейнером стек
using namespace std;  //используем пространство имен std

//Игра Сапёр
//В этой игре есть такие классы как: Map(игровое поле), Game(Игра), Console(настройка консоли), Cell(клетка), Player(игрок)

enum NumberColor {BLUE = 1, GREEN = 2, RED = 3, DARKBLUE = 4, BROWN = 5, TURQUOISE = 6, BLACK = 7, WHITE = 8}; //раскрас игровых цифр
//enum NumberColor { BLUE , GREEN, RED, DARKBLUE, BROWN, TURQUOISE, BLACK, WHITE };
enum KeyCode {ENTER = 13, SPACE = 32,LEFT = 75, RIGHT = 77, DOWN = 80, UP = 72 }; //управление клавиатурой
enum SapperObject {WALL = 100, HALL = 0, MINE = 10}; //

//класс для работы с консолью
class Console
{
public:
	//метод для установки позиции курсора на экране
	void Options(int x, int y)
	{
		COORD p = { x, y };
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), p);
	}

	//метод, который выводит логотип игры и просит нажать ENTER чтобы начать игру
	void ShowLogo()
	{
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
		int widthConsole = csbi.dwSize.X;

		int x = (widthConsole - 6) / 2; // Центрируем текст "Sapper"
		int y = 9;

		//не забыть разкоментировать
		/*Options(x, y);
		cout << "Sapper" << "\n";
		cout << "\n\n\t\t\t\t\tPlease, press ENTER to start ";
		while (_getch() != ENTER);
		system("cls");*/
	}

	//метод, который скрывает курсор
	void HideCursor(bool vision)
	{
		HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_CURSOR_INFO info;
		info.bVisible = !vision;
		info.dwSize = 100;
		SetConsoleCursorInfo(h, &info);
	}

	//метод, который настравивает курсор
	//void SetCursor(int x, int y, int color, string text) 
	//{
	//	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	//	COORD position;
	//	position.X = x; // настройка конкретных координат
	//	position.Y = y;
	//	SetConsoleCursorPosition(h, position); // установка курсора в заданную точку
	//	SetConsoleTextAttribute(h, color); // смена цвета
	//	cout << text; // вывод строки текста
	//	cout << "\n";
	//}

	void SetCursor(int x, int y)
	{
		HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
		COORD position;
		position.X = x; // Настройка координат X
		position.Y = y; // Настройка координат Y
		SetConsoleCursorPosition(h, position); // Установка курсора в заданную точку
	}

	//метод для настройки параметров консоли (размер окна, название окна)
	void OptionsConsole()
	{
		HWND hwnd = GetConsoleWindow();

		int screen_width = GetSystemMetrics(SM_CXSCREEN);
		int screen_height = GetSystemMetrics(SM_CYSCREEN);

		int window_width = 1360;
		int window_height = 768;

		MoveWindow(hwnd,
			(screen_width - window_width) / 2,
			(screen_height - window_height) / 2,
			window_width,
			window_height,
			1);
		system("title Sapper");
	}
};

//класс для работы с игровым полем
class Map : public Console
{
public:
	int width = 10;
	int height = 10;
	bool isGameWon;  //переменная для отслеживания выиграша
	vector<vector<int>> map;  //двумерный вектор для игрового поля
	vector<vector<int>> mask;  //двумерный вектор для маски игрового поля
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);  //дескриптор для работы с консолью
public:
	/*Map(int w, int h) : width(w), height(h), h(GetStdHandle(STD_OUTPUT_HANDLE))
	{
		InitializeMap();
		InitializeMask();
	}*/

	Map() : isGameWon(false){}


	/*int getWidth() const { return width; }
	int getHeight() const { return height; }
	bool getIsGameWon() const { return isGameWon; }
	vector<vector<int>> getMap() const { return map; }
	vector<vector<int>> getMask() const { return mask; }
	HANDLE getHandle() const { return h; }

	void setWidth(int newWidth) { width = newWidth; }
	void setHeight(int newHeight) { height = newHeight; }
	void setIsGameWon(bool newValue) { isGameWon = newValue; }
	void setMap(const vector<vector<int>>& newMap) { map = newMap; }
	void setMask(const vector<vector<int>>& newMask) { mask = newMask; }
	void setHandle(HANDLE newHandle) { h = newHandle; }*/


	//метод для инициализации игрового поля
	void InitializeMap()
	{
		for (int i = 0; i < width; i++)
		{
			vector<int> temp;
			for (int j = 0; j < height; j++)
			{
				if (i == 0 || j == 0 || i == width - 1 || j == height - 1)
				{
					temp.push_back(WALL);
				}
				else
				{
					temp.push_back(HALL);
				}
			}
			map.push_back(temp);
		}
	}

	//метод для отображения игрового поля
	void PrintMap()
	{
		Options(0, 0);
		//gotoxy(0, 0);   //неплохо работает
		//system("cls");  //работает но не то что надо 
		for (int i = 0; i < width; i++)
		{
			for (int j = 0; j < height; j++)
			{
				if (mask[j][i] == HALL)
				{
					//SetConsoleTextAttribute(h, BLACK);
					cout << ".";
					continue;
				}

				if (map[j][i] == WALL)
				{
					SetConsoleTextAttribute(h, WHITE);
					cout << (char)178; // стена
				}
				else if (mask[j][i] == 2)
				{
					cout << "!";
				}
				else if (map[j][i] == HALL)
				{
					SetConsoleTextAttribute(h, BLACK);
					cout << " "; // коридор
				}
				else if (map[j][i] == MINE)
				{
					SetConsoleTextAttribute(h, GREEN);
					cout << "*"; //мина
				}
				else 
				{
					SetConsoleTextAttribute(h, BLACK);
					SetConsoleTextAttribute(h, WHITE);
					cout << map[j][i];
				}
			}
			cout << "\n";
		}
	}

	//метод для расстановки чисел на игровом поле
	void NumberSetting()
	{
		for (int i = 1; i < width - 1; i++)
		{
			for (int j = 1; j < height - 1; j++)
			{
				if (map[i][j] != MINE)
				{
					int count = 0;
					if (map[i - 1][j - 1] == MINE) count++;
					if (map[i - 1][j] == MINE) count++;
					if (map[i - 1][j + 1] == MINE) count++;
					if (map[i][j - 1] == MINE) count++;
					if (map[i][j + 1] == MINE) count++;
					if (map[i + 1][j - 1] == MINE) count++;
					if (map[i + 1][j] == MINE) count++;
					if (map[i + 1][j + 1] == MINE) count++;
					map[i][j] = count;
				}
			}
		}
	}

	//метод для инициализации маски (состояние ячеек)
	void InitializeMask()
	{
		for (int i = 0; i < width; i++)
		{
			vector<int> temp;
			for (int j = 0; j < height; j++)
			{
				if (i == 0 || j == 0 || i == width - 1 || j == height - 1)
				{
					temp.push_back(WALL);
				}
				else
				{
					temp.push_back(HALL);
				}
			}
			mask.push_back(temp);
		}
	}

	// Метод для открытия ячейки и обновления маски
	//поставить void 
	//void OpenCell(int x, int y)
	//{
	//	if (mask[x][y] == HALL)  // Если ячейка еще не открыта
	//	{
	//		if (map[x][y] == MINE)  // Если это мина, выводим звездочку
	//		{
	//			SetConsoleTextAttribute(h, GREEN);
	//			cout << "*";
	//		}
	//		else if (map[x][y] == HALL)  // Если это пустая ячейка, выводим пустоту
	//		{
	//			SetConsoleTextAttribute(h, BLACK);
	//			cout << " ";
	//		}
	//		else  // Иначе выводим значение ячейки (цифру)
	//		{
	//			SetConsoleTextAttribute(h, BLACK);
	//			SetConsoleTextAttribute(h, WHITE);
	//			cout << map[x][y];
	//		}
	//	}
	//}

	//int OpenCell(int x, int y)
	//{
	//	if (mask[y][x] == HALL)  // Проверяем, что ячейка еще не открыта
	//	{
	//		mask[y][x] = 1;  // Отмечаем ячейку как открытую

	//		if (map[y][x] == MINE)  // Если это мина, выводим звездочку
	//		{
	//			SetConsoleTextAttribute(h, GREEN);
	//			cout << "*";
	//			return MINE;
	//		}
	//		else if (map[y][x] == HALL)  // Если это пустая ячейка, запускаем заливку
	//		{
	//			Fill(x, y, *this); // Запускаем заливку из текущей ячейки
	//			return HALL;
	//		}
	//		else  // Иначе выводим значение ячейки (цифру)
	//		{
	//			SetConsoleTextAttribute(h, BLACK);
	//			SetConsoleTextAttribute(h, WHITE);
	//			cout << map[y][x];
	//			return map[y][x];
	//		}
	//	}
	//	//return 0;
	//	return -1;  // Возвращаем -1, чтобы обозначить ошибку
	//}

	//метод для открытия ячейки и обновления маски
	int OpenCell(int x, int y)
	{
		if (mask[x][y] == HALL)
		{
			mask[x][y] = 1;

			if (map[x][y] == MINE)
			{
				SetConsoleTextAttribute(h, GREEN);
				cout << "*";
				return MINE;
			}
			else if (map[x][y] == HALL)
			{
				Fill(x, y, *this);
				return HALL;
			}
			else
			{
				SetConsoleTextAttribute(h, BLACK);
				SetConsoleTextAttribute(h, WHITE);
				cout << map[x][y];
				return map[x][y];
			}
		}
		if (CheckGameWon())
		{
			isGameWon = true;
			return -2;
		}
		return -1;
	}

	//метод для проверки выиграша в игре
	bool CheckGameWon()
	{
		for (int i = 1; i < width - 1; i++)
		{
			for (int j = 1; j < height - 1; j++)
			{
				if (map[i][j] != MINE && mask[i][j] != 1) {
					return false; // Есть незамаскированные ячейки, игра не выиграна
				}
			}
		}
		return true; // Все незамаскированные ячейки без мин, игра выиграна
	}

	//метод для заливки пустой области на игровом поле
	//используется стек для хранения координат ячеек для заливки
	//обновляем маску поля и отображаем изменения на экране
	void Fill(int px, int py, Map& map)
	{
		stack<int> stk;
		stk.push(px);
		stk.push(py);

		int x = 0;
		int y = 0;
		while (!stk.empty())
		{
			y = stk.top();
			stk.pop();

			x = stk.top();
			stk.pop();

			if (map.map[x][y + 1] == HALL && map.mask[x][y + 1] == 0)
			{
				stk.push(x);
				stk.push(y + 1);
			}
			map.mask[x][y + 1] = 1;

			if (map.map[x][y - 1] == HALL && map.mask[x][y - 1] == 0)
			{
				stk.push(x);
				stk.push(y - 1);
			}
			map.mask[x][y - 1] = 1;

			if (map.map[x + 1][y + 1] == HALL && map.mask[x + 1][y + 1] == 0)
			{
				stk.push(x + 1);
				stk.push(y + 1);
			}
			map.mask[x + 1][y + 1] = 1;

			if (map.map[x + 1][y - 1] == HALL && map.mask[x + 1][y - 1] == 0)
			{
				stk.push(x + 1);
				stk.push(y - 1);
			}
			map.mask[x + 1][y - 1] = 1;

			if (map.map[x - 1][y + 1] == HALL && map.mask[x - 1][y + 1] == 0)
			{
				stk.push(x - 1);
				stk.push(y + 1);
			}
			map.mask[x - 1][y + 1] = 1;

			if (map.map[x - 1][y - 1] == HALL && map.mask[x - 1][y - 1] == 0)
			{
				stk.push(x - 1);
				stk.push(y - 1);
			}
			map.mask[x - 1][y - 1] = 1;

			if (map.map[x - 1][y] == HALL && map.mask[x - 1][y] == 0)
			{
				stk.push(x - 1);
				stk.push(y);
			}
			map.mask[x - 1][y] = 1;

			if (map.map[x + 1][y] == HALL && map.mask[x + 1][y] == 0)
			{
				stk.push(x + 1);
				stk.push(y);
			}
			map.mask[x + 1][y] = 1;

			if (stk.empty())
			{
				break;
			}
		}
		PrintMap(); // Печать обновленной карты
	}

	// метод для установки флага на игровом поле
	void PlaceFlag(int x, int y)
	{
		if (mask[y][x] == HALL) // Проверяем, что ячейка еще не открыта
		{
			mask[y][x] = 2; // Устанавливаем флаг
			SetConsoleTextAttribute(h, RED);
			PrintMap(); // Обновляем экран после установки флага
		}
		else if (mask[y][x] == 2) // Если в ячейке уже установлен флаг
		{
			mask[y][x] = HALL; // Убираем флаг, восстанавливаем маску
			PrintMap(); // Обновляем экран после удаления флага
		}
	}



};

class Cell
{
private:
	bool isMine;
	bool isRevealed;
	int adjacentMines;
public:
	Cell() : isMine(false), isRevealed(false), adjacentMines(0) {}

	//метод, который рандомно расставляет мины
	void RandMines(int numMines, Map& map)
	{
		//проверка на количество мин, которые можно установить на поле
		if (numMines >= (map.width - 2) * (map.width - 2))
		{
			MessageBoxA(0, "Слишком много мин!", "Попробуйте заново!", MB_OK);
			return;
		}
		for (int i = 0; i < numMines; i++)
		{
			int x = 0;
			int y = 0;
			//поиск пустой ячейки, не занятой миной
			do
			{
				x = rand() % (map.width - 2) + 1;
				y = rand() % (map.height - 2) + 1;
			} while (map.map[x][y] == MINE);
			map.map[x][y] = MINE;
		}
	}
};


class Player
{

};

//класс отвечающий за управление игровым процессом и логикой игры
class Game
{
public:
	Map map;
	Cell cell;
	Console console;

	//метод проиграша
	void GameOver()
	{
		console.Options(40, 9);
		cout << "Game over";
		Sleep(2000);
		console.Options(0, 15);
		system("pause");
		exit(0);
	}

	//метод выиграша 
	void Won()
	{
		console.Options(40, 9);
		if (map.isGameWon) {
			cout << "Congratulations! You won!";
		}
		else {
			GameOver();
		}
		Sleep(2000);
		console.Options(0, 15);
		system("pause");
	}

	//метод, который запускает игру, инициализирует карту, расставляет мины,
	//позволяет игроку управлять персонажем и выполняет игровую логику
	void run()
	{
		console.OptionsConsole();
		console.ShowLogo();
		map.InitializeMap();
		map.InitializeMask();
		cell.RandMines(8, map);
		map.NumberSetting();
		map.PrintMap();

		COORD position;
		position.X = 1;
		position.Y = 1;
		console.SetCursor(1, 1);
		console.HideCursor(false);

		bool exit = false;
		while (!exit)
		{
			int code = _getch();
			if (code == 224)
			{
				code = _getch();
			}

			int tx = position.X;
			int ty = position.Y;

			console.Options(tx, ty);
			//console.SetCursor(position.X, position.Y, BLACK, " ");

			if (code == RIGHT && map.map[position.Y][position.X + 1] != WALL)
			{
				position.X++;
			}
			else if (code == LEFT && map.map[position.Y][position.X - 1] != WALL)
			{
				position.X--;
			}
			else if (code == DOWN && map.map[position.Y + 1][position.X] != WALL)
			{
				position.Y++;
			}
			else if (code == UP && map.map[position.Y - 1][position.X] != WALL)
			{
				position.Y--;
			}
			else if (code == SPACE)
			{
				map.PlaceFlag(position.Y, position.X);
			}
			else if (code == ENTER)
			{
				int result = map.OpenCell(position.X, position.Y);
				if (result == MINE)
				{
					GameOver();
					exit = true;
				}
				if (result == HALL)
				{
					map.Fill(position.X, position.Y, map);
					map.PrintMap();
				}
				//break;
				

				//if (map.mask[position.Y][position.X] == HALL)  // Проверяем, что ячейка еще не открыта
				//{
				//	if (map.map[position.Y][position.X] == MINE)  // Если это мина, завершаем игру
				//	{
				//		map.OpenCell(position.Y, position.X);
				//		//exit = true;
				//	}
				//	else if (map.map[position.Y][position.X] == HALL)  // Если это пустая ячейка, запускаем заливку
				//	{
				//		map.Fill(position.X, position.Y, map);
				//	}
				//	else  // Иначе открываем обычную ячейку
				//	{
				//		map.OpenCell(position.Y, position.X);
				//	}
				//}
			}

			console.Options(tx, ty); // Убираем предыдущий курсор
			console.Options(position.X, position.Y); // Перемещаем курсор
		}
	}
};

int main()
{
	srand(time(NULL));
	
	Game game;
	game.run();
}