// CopsAndRobbers.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <algorithm>
#include <string>
#include <functional>
#include <iostream>
#include <iomanip>
#include <vector>
#include <list>
#include <stack>
#include <queue>
#include <cmath>

using namespace std;

typedef pair<int, int> II;

#define FORR(i, a, b) for(int i = a; i >= b; i--)
#define FOR(i, a, b)  for(int i = a; i <= b; i++)
#define REP(i, n)     FOR(i, 0, n-1)

char grid[200][200];

int n, k, m, movesMax; // multiplicity, m = [1, 9]

// clock-wise from top-left ( = 0 )
int side[][2] = {-1,-1, -1,0, -1,1, 0,1, 1,1, 1,0, 1,-1, 0,-1};

void Display()
{
	REP(i, n) { cout << "\n"; REP(j, k) cout << grid[i][j]; }
	cout << "\t\t";	system("pause");
}

int Remove(char c)
{
	list<II> l, endanger;

	REP(i, n) REP(j, k) if(grid[i][j] == c) l.push_back(II(i, j));
	int count = l.size();

	while(!l.empty())
	{
		int foe = 0, ally = 1; endanger.clear();

		REP(s, 8)
		{
			int i = l.front().first + side[s][0], j = l.front().second + side[s][1];
			if(0 <= i && i < n && 0 <= j && j < k)
				if(grid[i][j] == c)
				{
					ally++; endanger.push_back(II(i, j));
				}
				else if(grid[i][j] != '.') foe++;
		}

		if(c == 'C') ally *= m; else foe *= m;

		if(foe > ally)
		{
			grid[l.front().first][l.front().second] = '.'; --count;

			for(list<II>::iterator i = endanger.begin(); i != endanger.end(); i++)
				if(find(l.begin(), l.end(), *i) == l.end()) l.push_back(*i);
		}

		l.pop_front();
	}

	return count;
}

void Move(char c)
{
	bool canMove = false;

	// find weakest
	II weakestFoe = II(-1, -1); int count = 9;
	REP(i, n) REP(j, k)
	{
		int tCount = 0;
		if(grid[i][j] != c && grid[i][j] != '.') 
		{
			REP(s, 8)
			{
				int p = i + side[s][0], q = j + side[s][1];
				if(0 <= p && p < n && 0 <= q && q < k && grid[p][q] != c && grid[p][q] != '.') tCount++;
			}
			if(count > tCount) { weakestFoe = II(i, j); count = tCount; }
		}
	}

	if(weakestFoe == II(-1, -1)) return; // no foe remaining

	REP(i, n) REP(j, k)	if(grid[i][j] == c)
	{
		//int d = abs(weakestFoe.first - i) + abs(weakestFoe.second - j);
		int d = max(abs(weakestFoe.first - i), abs(weakestFoe.second - j));

		REP(s, 8) // move toward the weakest
		{
			int p = i + side[s][0], q = j + side[s][1];
			if(0 <= p && p < n && 0 <= q && q < k && grid[p][q] == '.'
				&& max(abs(weakestFoe.first - p), abs(weakestFoe.second - q)) < d)
			{
				grid[i][j] = '.'; grid[p][q] = '*'; canMove = true; break;
			}
		}

		if(grid[i][j] == c && !canMove) REP(s, 8) // cant move toward weakest foe - so move somehow
		{
			int p = i + side[s][0], q = j + side[s][1];
			if(0 <= p && p < n && 0 <= q && q < k && grid[p][q] == '.')
			{
				grid[i][j] = '.'; grid[p][q] = '*'; canMove = true; break;
			}
		}
	}

	REP(i, n) REP(j, k) if(grid[i][j] == '*') grid[i][j] = c;

	// none can move
	if(!canMove) REP(i, n) REP(j, k) if(grid[i][j] == c) grid[i][j] = '.';
}

bool Play(string gridStr)
{
	REP(i, n) REP(j, k) grid[i][j] = gridStr[i * k + j];
	//cout << "\n" << gridStr << " m = " << m << "\trunning ... ";

	int moveCount = 0, r = 0, c = 0;
	while(true)
	{
		c = Remove('C'); //display();
		if(c) Move('R'); //display();
		r = Remove('R'); //display();
		if(r) Move('C'); //display();

		moveCount++;
		if(moveCount == movesMax)
		{ 
			//cout << "\nCant Resolve";
			return false;
		}

		if(r == 0)
		{
			//cout << "\nCops Win";
			return true;
		}
		if(c == 0)
		{
			//cout << "\nRobbers Win";
			return false; 
		}
	}
}

bool CheckMultiplicityForAllPermutaion(int r, int c)
{
	string gridStr;

	REP(i, (n * k) - r - c) gridStr += '.';
	REP(i, c) gridStr += 'C';
	REP(i, r) gridStr += 'R';

	int count = 0;
	do
	{
		if (!Play(gridStr)) return false;

	}while(next_permutation(gridStr.begin(), gridStr.end()));

	return true;
}
int RobberRequired(int i, int j)
{

}
bool CheckMultiplicityForWorstcase(int r, int c)
{
	// find worst case
	int temp[200][200];

	int empty = (n*k)-c;
	int deadC = m >= 5 ? (k-1)/2 : (k+1)/2;

	int attackCol = 0, nonAttackCol = 0;

	// Equation 1: n = na + b + e; where na = attacking col, b = non-attacking col, e = empty lines = b+1, b, b-1 
	// Equation 2: na*k + a*deadC <= C
	if (m==1 || m==2)
	{
		attackCol = ((n+1)*k - c) / (2*k - deadC);
		if (attackCol*deadC < c) attackCol = (n*k - c) / (2*k - deadC);
		nonAttackCol = ceil((c - (attackCol * deadC)) / k);
	}
	if (nonAttackCol >= 2 || nonAttackCol == 0)	attackCol = ((n-1)*k - c) / (2*k - deadC);

	return false;

	//string gridStr; // display and make string to for Play
	//REP(i, n) {	cout << "\n"; REP(j, k) { gridStr += temp[i][j]; cout << temp[i][j]; } }

	//return Play(gridStr); // check
}

int FindMinM(int r, int c)
{
	// 3
	if(r >= (2*k) + 1) return 3;
	if(r == 2*k && c > k && c%k <= k/2)  return 3;

	// 2
	if(r >= 8) return 2; // diagonal

	//if(r == k && c - (k+1)/2 <= k)  return 2; // may be solved by above func.
	if(r == k && c > k && c%k <= (k+1)/2)  return 2;
	if(r >= k + 1) return 2;

	// 1
	return 1;
}

int main()
{
	cout << "n = "; cin >> n; 
	cout << "k = "; cin >> k;
	//int r = 8, c = 4; n = 4; k = 4; cout << "n = " << n << "\tk = " << k;

	if (n<k) swap(n,k); // k must be always smaller than n

	movesMax = 2 * (n + k); cout << "\tmax move = " << movesMax;

	FOR(r, 1, (n * k) - 2)
	{
		int mMax = min(r, 8) + 1; // (max number of robber that can surround a cop) + 1 (to overpower the robbers)
		m = 1;

		FORR(c, (n * k) - r - 1, 1)
		{
			m = max(FindMinM(r, c), m);

			// m -> min to max
			//while(m < mMax && !CheckMultiplicityForAllPermutaion(r, c)) ++m;

			while(m < mMax && !CheckMultiplicityForWorstcase(r, c)) ++m;

			cout << "\nR = " << r << "\tC = " << c << "\tm = " << m;
		}
	}

	cout << "\nfinish..\t\t"; system("pause");
	return 0;
}
