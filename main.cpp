/*
«Миссионеры и людоеды». Три миссионера и три людоеда находятся на левом берегу
реки и им нужно переправиться на правый берег, однако у них имеется только одна
лодка, в которую могут сесть лишь 2 человека. Поэтому необходимо определить план,
соблюдая который и курсируя несколько раз туда и обратно, можно переправить всех
шестерых. Однако если на любом берегу реки число миссионеров будет меньше, чем
число людоедов, то миссионеры будут съедены. Решения принимают миссионеры,
людоеды их выполняют. Алгоритмы: BFS, DFS, IDS.
*/

#include <iostream>
#include <random>
#include <deque>
#include <time.h>
#include <string>
#include <queue>
#include <windows.h>
#include <ctime>
#include <cassert>
#include <iomanip>   
#include <iterator>
#include <ctime>
#include <time.h>

using namespace std;

enum banks {Left, Right};

string carry[5] = {"01","02","11","10","20" }; // cannibals | missioners



struct State {
	int cannibals_left;
	int cannibals_right;

	int missioners_left;
	int missioners_right;

	banks boat_side;
	
	State() {}

	State(int cl, int cr, int ml, int mr, banks bs) : 
    cannibals_left(cl), cannibals_right(cr), missioners_left(ml), missioners_right(mr), boat_side(bs) {}

	State(pair<int, pair<int, int>> p) {
		int c = p.second.first;
		int m = p.second.second;
		int b = p.first;
		boat_side = banks(b);
		cannibals_left = c;
		cannibals_right = 3 - c;
		missioners_left = m;
		missioners_right = 3 - m;
	}

	banks get_direction() {
		return banks((boat_side + 1) % 2);
	}

	friend bool operator==(State & s1, State & s2) {
		return (s1.cannibals_left == s2.cannibals_left && s1.missioners_left == s2.missioners_left && s1.boat_side == s2.boat_side);
	}

	//=====================Move is valid
	bool is_valid(banks direction, int c, int m) {
		if (direction == Right)
		{
			if ( (boat_side == Right) ||
				(c + m > 2) || 
				!(c + m) || 
				(c > cannibals_left) ||
				(m > missioners_left) ||
				(cannibals_left - c > missioners_left - m && ((missioners_left - m) != 0) ) ||
				( (cannibals_right + c) > (missioners_right + m) && ( (missioners_right + m) !=0) )  )
				return false;
		}
		else //to the left
		{
			if ((boat_side == Left) ||
				(c + m > 2) ||
				(c > cannibals_right) ||
				(m > missioners_right) ||
				!(c+m) ||
				((cannibals_left + c > missioners_left + m) && (missioners_left + m != 0)  )||
				((cannibals_right - c > missioners_right - m) && (missioners_right - m!=0)  )  )
				return false;
		}
		return true;
	}

	//=====================Check if goal
	bool is_goal() {
		return missioners_right == 3 && cannibals_right == 3;
	}

	//=====================Print
	void print() {
		printf("Left bank: %d missioners, %d cannibals. \n", missioners_left, cannibals_left);
		printf("Right bank: %d missioners, %d cannibals. \n",missioners_right, cannibals_right);
		if (boat_side == Right)
			cout << "Boat in on the right bank \n";
		else
			cout << "Boat in on the left bank \n";
	}


	void cool_print(vector<State> moves) {
		cout << endl;
		for (int i = moves.size() - 1; i>=0;i--) {
			string cl, cr, ml, mr, b = "";
			for (int j = 1; j <= moves[i].cannibals_left; j++)
				cl+='C';
			for (int j = 1; j <= moves[i].missioners_left; j++)
				ml+='M';
			if (moves[i].boat_side == Right)
				b= "______B_";
			else
				b= "_B______";
			for (int j = 1; j <= moves[i].cannibals_right; j++)
				cr+='C';
			for (int j = 1; j <= moves[i].missioners_right; j++)
				mr+='M';
			printf("%3s %3s %s %3s %3s \n", cl.c_str(), ml.c_str(), b.c_str(),cr.c_str(),mr.c_str());
		}
		cout << endl;
	}

	//=====================Move boat
	State MoveBoat(banks direction, int c, int m ) {
		//cout << "C: " << c << "M: " << m << "Dir" << direction << endl;
		//assert(is_valid(direction, c, m));
		State new_state(*this);
		if (is_valid(direction, c, m))
		{
			if (direction == Right)
			{
				new_state.cannibals_left -= c;
				new_state.missioners_left -= m;
				new_state.cannibals_right += c;
				new_state.missioners_right += m;
			}
			else
			{
				new_state.cannibals_left += c;
				new_state.missioners_left += m;
				new_state.cannibals_right -= c;
				new_state.missioners_right -= m;
			}
			new_state.boat_side = direction;
			//new_state.print();
			//Sleep(1000);
			return new_state;
		}
		return *this;
	}

};

//ALL the moves
vector<State> find_moves(State s) 
{
	vector<State> leafs;
	for (int i = 0; i < 5; i++)
	{
		if (s.is_valid(s.get_direction(), (int)carry[i][0] - 48, (int)carry[i][1] - 48))
		{
			State child = s.MoveBoat(s.get_direction(), (int)carry[i][0] - 48, (int)carry[i][1] - 48);
			for (int j = 0; j < leafs.size(); j++)
				if (leafs[j] == child)
					continue;
			leafs.push_back(child);
		}
	}
	return leafs;
}


vector <State> find_children(State s) {
	vector <State> children;
	int can_left = s.cannibals_left;
	int miss_left = s.missioners_left;
	int b = s.boat_side;
	State new_state;
	//left bank    //MoveBoat C M
	if (b == 0) {
		if (can_left >= 2 && miss_left == 3)
		    children.push_back(s.MoveBoat(Right,2, 0));

		if (can_left == 3 && miss_left == 0)
			children.push_back(s.MoveBoat(Right, 2, 0));

	    if (miss_left > can_left && can_left <= 1)
			children.push_back(s.MoveBoat(Right, 0, 2));

		if (can_left >= 2 && miss_left == 2)
		   children.push_back(s.MoveBoat(Right,0,2));

		if (children.empty()) //else
			children.push_back(s.MoveBoat(Right, 2, 0));
	}
	//right bank
	if (b == 1) {
		if (miss_left == 3 && miss_left > can_left)  // miss_right  == 3
			children.push_back(s.MoveBoat(Left, 1, 0)); 

		if (can_left >= 2 && miss_left == 0) //can_right <=1 miss_right = 3
			children.push_back(s.MoveBoat(Left, 1, 0));

		if (can_left >= 1 && miss_left >= can_left) 
			children.push_back(s.MoveBoat(Left, 1, 1));

		 if (can_left >= 1 && miss_left == 0)
			 children.push_back(s.MoveBoat(Left, 1, 0));
	}
	/*for (State ss : children) {
		ss.print();
		cout << endl;
	}
	cout << endl <<endl << endl;*/

	return children;
}



bool check_state(State s) {
	if (s.cannibals_left >=0 && s.missioners_right>=0 && s.cannibals_right>=0 &&
		s.missioners_left >=0 && ( s.cannibals_left <= s.missioners_left || (s.missioners_left == 0) )
		&& (s.cannibals_right <= s.missioners_right || (s.missioners_right == 0) ) )
		return true;
	else
		return false;
}



bool DLS(State s, int depth, vector<State> & moves) 
{
	//cout << endl << endl << "Depth: " << depth << "    State: " << endl;
	//s.print();
	//cout << "Children: " << endl;
	if (depth == 0 && s.is_goal()) 
		return false;
	if (s.is_goal())
		return true;
	//vector<State> leafs = find_moves(s);
	vector<State> leafs = find_children(s);
	for (int i = 0; i < leafs.size(); i++)
	{
		{
			if (DLS(leafs[i], depth+1, moves))
			{
				moves.push_back(leafs[i]);
				return true;
			}
		}
	}
}

void IDS(State s) {
	vector<State> moves;
	for (int i = 0; i <= 100; i++)
	{
		vector<State> v;
		if (DLS(s, i, v) == true){
			moves = v;
			break;
		}
		//Sleep(1000);
	}
	moves.push_back(s);
	s.cool_print(moves);
	for (int i = moves.size() - 1; i >= 0; i--)
	{
		moves[i].print();
		cout << endl;
	}
}


int main() {
	State init = State(3, 0, 3, 0, Left);
	//init = init.MoveBoat(Right, 2, 0);
	//init = init.MoveBoat(Left, 1, 0);
	//init = init.MoveBoat(Right, 2, 0);
	//init = init.MoveBoat(Left, 1, 0);
	//init = init.MoveBoat(Right, 0, 2);
	//init = init.MoveBoat(Left, 1, 1);
	//init = init.MoveBoat(Right, 0, 2);
	//init = init.MoveBoat(Left, 1, 0);
	//init = init.MoveBoat(Right, 2, 0);
	//init = init.MoveBoat(Left, 1, 0);
	//init = init.MoveBoat(Right, 2, 0);
	//init.print();

	int start_time = clock();
	IDS(init);
	int end_time = clock();
	cout << "Total time: " << (double)(end_time - start_time) / CLOCKS_PER_SEC << endl;

	system("pause");
}