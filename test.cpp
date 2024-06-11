#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <cmath>
using namespace std;



class piece;

typedef struct sq{
	int rank;
	int file;
	piece* Piece;
	sq* top;
	sq* down;
	sq* right;
	sq* left;
	sq* top_l;
	sq* top_r;
	sq* down_l;
	sq* down_r;
}sq;

sq* board[8][8];
sq* prev = NULL;
sq* wk_pos;
sq* bk_pos;
int turn;

class piece{
	public:
		char colour;
		char type;
		int last_moved;
		piece(){
			this->colour = '\0';
			this->type = '\0';
			this->last_moved = 0;
		}
		void toString(){
			cout << this->colour << this->type;
		}
		virtual bool vision(sq* start,sq* end) = 0;
};

class pawn:public piece{
	public:
		pawn(int a,int b,char colour){
			this->colour = colour;
			this->type = 'P';
			board[7-a][b]->Piece = this;
		}
		bool vision(sq* start,sq* end){
			if(start->Piece->colour == 'W'){
				if(start->top == end and start->top->Piece == NULL){
					return true;
				}
				if(start->top_r->Piece != NULL){
					if(start->top_r == end and start->top_r->Piece->colour == 'B'){
						return true;
					}
				}
				if(start->top_l->Piece != NULL){
					if(start->top_l == end and start->top_l->Piece->colour == 'B'){
						return true;
					}
				}
				if(start->rank == 2){
					if(start->top->top == end and start->top->Piece == NULL and start->top->top->Piece == NULL){
						return true;
					}
				}
				if(start->rank == 5 and (end == start->top_r or end == start->top_l)){
					if(start->top_r == end){
						if(start->right->Piece != NULL){
							if(start->right->Piece->type == 'P' and start->right->Piece->last_moved == turn-1 and start->right->Piece->colour == 'B'){
								end->Piece = start->right->Piece;
								start->right->Piece = NULL;
								return true;
							}
						}
					}
					else{
						if(start->left->Piece != NULL){
							if(start->left->Piece->type == 'P' and start->left->Piece->last_moved == turn-1 and start->left->Piece->colour == 'B'){
								end->Piece == start->left->Piece;
								start->left->Piece = NULL;
								return true;
							}
						}
					}
				}
			}
			else{
				if(start->down == end and start->down->Piece == NULL){
					return true;
				}
				if(start->down_r->Piece != NULL){
					if(start->down_r == end and start->down_r->Piece->colour == 'W'){
						return true;
					}
				}
				if(start->down_l->Piece != NULL){
					if(start->down_l == end and start->down_l->Piece->colour == 'W'){
						return true;
					}
				}
				if(start->rank == 7){
					if(start->down->down == end and start->down->Piece == NULL and start->down->down->Piece == NULL){
						return true;
					}
				}
				if(start->rank == 4 and (end == start->down_r or end == start->down_l)){
					if(start->down_r == end){
						if(start->right->Piece != NULL){
							if(start->right->Piece->type == 'P' and start->right->Piece->last_moved == turn-1 and start->right->Piece->colour == 'W'){
								end->Piece = start->right->Piece;
								start->right->Piece = NULL;
								return true;
							}
						}
					}
					else{
						if(start->left->Piece != NULL){
							if(start->left->Piece->type == 'P' and start->left->Piece->last_moved == turn-1 and start->left->Piece->colour == 'W'){
								end->Piece = start->left->Piece;
								start->left->Piece = NULL;
								return true;
							}
						}
					}
				}
			}
			return false;
		}
};

class rook:public piece{
	public:
		rook(int a,int b,char colour){
			this->colour = colour;
			this->type = 'R';
			board[7-a][b]->Piece = this;
		}
		bool vision(sq* start,sq* end){
			sq* tmp;
			if(start->rank == end->rank){
				if(start->file > end->file){
					tmp = start;
					while(tmp != NULL){
						if(tmp == end){
							return true;
						}
						else if(tmp->left->Piece == NULL){
							tmp = tmp->left;
						}
						else if(tmp->left->Piece->colour != start->Piece->colour){
							if(tmp->left == end){
								return true;
							}
							else{
								return false;
							}
						}
						else{
							return false;
						}
					}
					return false;
				}
				else{
					tmp = start;
					while(tmp != NULL){
						if(tmp == end){
							return true;
						}
						else if(tmp->right->Piece == NULL){
							tmp = tmp->right;
						}
						else if(tmp->right->Piece->colour != start->Piece->colour){
							if(tmp->right == end){
								return true;
							}
							else{
								return false;
							}
						}
						else{
							return false;
						}
					}
					return false;
				}
			}
			else if(start->file == end->file){
				if(start->rank > end->rank){
					tmp = start;
					while(tmp != NULL){
						if(tmp == end){
							return true;
						}
						else if(tmp->down->Piece == NULL){
							tmp = tmp->down;
						}
						else if(tmp->down->Piece->colour != start->Piece->colour){
							if(tmp->down == end){
								return true;
							}
							else{
								return false;
							}
						}
						else{
							return false;
						}
					}
					return false;
				}
				else{
					tmp = start;
					while(tmp != NULL){
						if(tmp == end){
							return true;
						}
						else if(tmp->top->Piece == NULL){
							tmp = tmp->top;
						}
						else if(tmp->top->Piece->colour != start->Piece->colour){
							if(tmp->top == end){
								return true;
							}
							else{
								return false;
							}
						}
						else{
							return false;
						}
					}
					return false;
				}
			}
			else{
				return false;
			}
		}
};

class knight:public piece{
	public:
		knight(int a,int b,char colour){
			this->colour = colour;
			this->type = 'N';
			board[7-a][b]->Piece = this;
		}
		bool vision(sq* start,sq* end){
			if(end->rank == start->rank + 2 or end->rank == start->rank - 2){
				if(end->file == start->file + 1 or end->file == start->file - 1){
					if(end->Piece == NULL){
						return true;
					}
					else if(end->Piece->colour != start->Piece->colour){
						return true;
					}
					return false;	
				}		
			}
			else if(end->file == start->file + 2 or end->file == start->file - 2){
				if(end->rank == start->rank + 1 or end->rank == start->rank - 1){
					if(end->Piece == NULL){
						return true;
					}
					else if(end->Piece->colour != start->Piece->colour){
						return true;
					}
					return false;
				}
			}
			return false;
		}
};

class bishop:public piece{
	public:
		bishop(int a,int b,char colour){
			this->colour = colour;
			this->type = 'B';
			board[7-a][b]->Piece = this;
		}
		bool vision(sq* start,sq* end){
			if(abs(end->rank - start->rank) == abs(end->file - start->file)){
				sq* tmp = start;
				if(end->rank > start->rank and end->file >start->file){
					while(tmp != NULL){
						if(tmp == end){
							return true;
						}
						else if(tmp->top_r->Piece == NULL){
							tmp = tmp->top_r;
						}
						else if(tmp->top_r->Piece->colour != start->Piece->colour){
							if(tmp->top_r == end){
								return true;
							}
							else{
								return false;
							}
						}
						else{
							return false;
						}
					}
					return false;
				}
				else if(end->rank > start->rank and end->file < start->file){
					while(tmp != NULL){
						if(tmp == end){
							return true;
						}
						else if(tmp->top_l->Piece == NULL){
							tmp = tmp->top_l;
						}
						else if(tmp->top_l->Piece->colour != start->Piece->colour){
							if(tmp->top_l == end){
								return true;
							}
							else{
								return false;
							}
						}
						else{
							return false;
						}
					}
					return false;
				}
				else if(end->rank < start->rank and end->file > start->file){
					while(tmp != NULL){
						if(tmp == end){
							return true;
						}
						else if(tmp->down_r->Piece == NULL){
							tmp = tmp->down_r;
						}
						else if(tmp->down_r->Piece->colour != start->Piece->colour){
							if(tmp->down_r == end){
								return true;
							}
							else{
								return false;
							}
						}
						else{
							return false;
						}
					}
					return false;
				}
				else{
					while(tmp != NULL){
						if(tmp == end){
							return true;
						}
						else if(tmp->down_l->Piece == NULL){
							tmp = tmp->down_l;
						}
						else if(tmp->down_l->Piece->colour != start->Piece->colour){
							if(tmp->down_l == end){
								return true;
							}
							else{
								return false;
							}
						}
						else{
							return false;
						}
					}
					return false;
				}
			}
			return false;
		}
};

class queen:public piece{
	public:
		queen(int a,int b,char colour){
			this->colour = colour;
			this->type = 'Q';
			board[7-a][b]->Piece = this;
		}
		bool vision(sq* start,sq* end){
			sq* tmp;
			if(start->rank == end->rank){
				if(start->file > end->file){
					tmp = start;
					while(tmp != NULL){
						if(tmp == end){
							return true;
						}
						else if(tmp->left->Piece == NULL){
							tmp = tmp->left;
						}
						else if(tmp->left->Piece->colour != start->Piece->colour){
							if(tmp->left == end){
								return true;
							}
							else{
								return false;
							}
						}
						else{
							return false;
						}
					}
					return false;
				}
				else{
					tmp = start;
					while(tmp != NULL){
						if(tmp == end){
							return true;
						}
						else if(tmp->right->Piece == NULL){
							tmp = tmp->right;
						}
						else if(tmp->right->Piece->colour != start->Piece->colour){
							if(tmp->right == end){
								return true;
							}
							else{
								return false;
							}
						}
						else{
							return false;
						}
					}
					return false;
				}
			}
			else if(start->file == end->file){
				if(start->rank > end->rank){
					tmp = start;
					while(tmp != NULL){
						if(tmp == end){
							return true;
						}
						else if(tmp->down->Piece == NULL){
							tmp = tmp->down;
						}
						else if(tmp->down->Piece->colour != start->Piece->colour){
							if(tmp->down == end){
								return true;
							}
							else{
								return false;
							}
						}
						else{
							return false;
						}
					}
					return false;
				}
				else{
					tmp = start;
					while(tmp != NULL){
						if(tmp == end){
							return true;
						}
						else if(tmp->top->Piece == NULL){
							tmp = tmp->top;
						}
						else if(tmp->top->Piece->colour != start->Piece->colour){
							if(tmp->top == end){
								return true;
							}
							else{
								return false;
							}
						}
						else{
							return false;
						}
					}
					return false;
				}
			}
			else if(abs(end->rank - start->rank) == abs(end->file - start->file)){
				tmp = start;
				if(end->rank > start->rank and end->file >start->file){
					while(tmp != NULL){
						if(tmp == end){
							return true;
						}
						else if(tmp->top_r->Piece == NULL){
							tmp = tmp->top_r;
						}
						else if(tmp->top_r->Piece->colour != start->Piece->colour){
							if(tmp->top_r == end){
								return true;
							}
							else{
								return false;
							}
						}
						else{
							return false;
						}
					}
					return false;
				}
				else if(end->rank > start->rank and end->file < start->file){
					while(tmp != NULL){
						if(tmp == end){
							return true;
						}
						else if(tmp->top_l->Piece == NULL){
							tmp = tmp->top_l;
						}
						else if(tmp->top_l->Piece->colour != start->Piece->colour){
							if(tmp->top_l == end){
								return true;
							}
							else{
								return false;
							}
						}
						else{
							return false;
						}
					}
					return false;
				}
				else if(end->rank < start->rank and end->file > start->file){
					while(tmp != NULL){
						if(tmp == end){
							return true;
						}
						else if(tmp->down_r->Piece == NULL){
							tmp = tmp->down_r;
						}
						else if(tmp->down_r->Piece->colour != start->Piece->colour){
							if(tmp->down_r == end){
								return true;
							}
							else{
								return false;
							}
						}
						else{
							return false;
						}
					}
					return false;
				}
				else{
					while(tmp != NULL){
						if(tmp == end){
							return true;
						}
						else if(tmp->down_l->Piece == NULL){
							tmp = tmp->down_l;
						}
						else if(tmp->down_l->Piece->colour != start->Piece->colour){
							if(tmp->down_l == end){
								return true;
							}
							else{
								return false;
							}
						}
						else{
							return false;
						}
					}
					return false;
				}
			}
			return false;
		}
};

class king:public piece{
	public:
		king(int a,int b,char colour){
			this->colour = colour;
			this->type = 'K';
			board[7-a][b]->Piece = this;
			if(colour == 'W'){
				wk_pos = board[7-a][b];
			}
			else{
				bk_pos = board[7-a][b];
			}
		}
		bool vision(sq* start,sq* end){
			if(abs(end->rank - start->rank) < 2 and abs(end->file - start->file) < 2){
				if(end->Piece == NULL){
					return true;
				}
				else if(end->Piece->colour != start->Piece->colour){
					return true;
				}
				else{
					return false;
				}
			}
			return false;
		}
		bool in_check(sq* start){
			sq* tmp = start->right;
			while(tmp != NULL){
				if(tmp->Piece == NULL){
					tmp = tmp->right;
				}
				else if(tmp->Piece->colour != start->Piece->colour){
					if(tmp->Piece->type == 'R' or tmp->Piece->type == 'Q'){
						return true;
					}
					else{
						break;
					}
				}	
				else{
					break;
				}
			}
			tmp = start->top;
			while(tmp != NULL){
				if(tmp->Piece == NULL){
					tmp = tmp->top;
				}
				else if(tmp->Piece->colour != start->Piece->colour){
					if(tmp->Piece->type == 'R' or tmp->Piece->type == 'Q'){
						return true;
					}
					else{
						break;
					}
				}	
				else{
					break;
				}
			}
			tmp = start->left;
			while(tmp != NULL){
				if(tmp->Piece == NULL){
					tmp = tmp->left;
				}
				else if(tmp->Piece->colour != start->Piece->colour){
					if(tmp->Piece->type == 'R' or tmp->Piece->type == 'Q'){
						return true;
					}
					else{
						break;
					}
				}
				else{
					break;
				}	
			}
			tmp = start->down;
			while(tmp != NULL){
				if(tmp->Piece == NULL){
					tmp = tmp->down;
				}
				else if(tmp->Piece->colour != start->Piece->colour){
					if(tmp->Piece->type == 'R' or tmp->Piece->type == 'Q'){
						return true;
					}
					else{
						break;
					}
				}
				else{
					break;
				}	
			}
			tmp = start->top_r;
			while(tmp != NULL){
				if(tmp->Piece == NULL){
					tmp = tmp->top_r;
				}
				else if(tmp->Piece->colour != start->Piece->colour){
					if(tmp->Piece->type == 'B' or tmp->Piece->type == 'Q'){
						return true;
					}
					if(start->top_r == tmp){
						if(tmp->right != NULL){
							if(tmp->right->Piece != NULL){
								if(tmp->right->Piece->type == 'N' and tmp->right->Piece->colour != start->Piece->colour){
									return true;
								}
							}
						}
						if(tmp->top != NULL){
							if(tmp->top->Piece != NULL){
								if(tmp->top->Piece->type == 'N' and tmp->top->Piece->colour != start->Piece->colour){
									return true;
								}
							}
						}
					}
				}
				else{
					if(start->top_r == tmp){
						if(tmp->right != NULL){
							if(tmp->right->Piece != NULL){
								if(tmp->right->Piece->type == 'N' and tmp->right->Piece->colour != start->Piece->colour){
									return true;
								}
							}
						}
						if(tmp->top != NULL){
							if(tmp->top->Piece != NULL){
								if(tmp->top->Piece->type == 'N' and tmp->top->Piece->colour != start->Piece->colour){
									return true;
								}
							}
						}
					}
					break;
				}	
			}
			tmp = start->top_l;
			while(tmp != NULL){
				if(tmp->Piece == NULL){
					tmp = tmp->top_l;
				}
				else if(tmp->Piece->colour != start->Piece->colour){
					if(tmp->Piece->type == 'B' or tmp->Piece->type == 'Q'){
						return true;
					}
					if(start->top_l == tmp){
						if(tmp->left != NULL){
							if(tmp->left->Piece != NULL){
								if(tmp->left->Piece->type == 'N' and tmp->left->Piece->colour != start->Piece->colour){
									return true;
								}
							}
						}
						if(tmp->top != NULL){
							if(tmp->top->Piece != NULL){
								if(tmp->top->Piece->type == 'N' and tmp->top->Piece->colour != start->Piece->colour){
									return true;
								}
							}
						}
					}
				}
				else{
					if(start->top_l == tmp){
						if(tmp->left != NULL){
							if(tmp->left->Piece != NULL){
								if(tmp->left->Piece->type == 'N' and tmp->left->Piece->colour != start->Piece->colour){
									return true;
								}
							}
						}
						if(tmp->top != NULL){
							if(tmp->top->Piece != NULL){
								if(tmp->top->Piece->type == 'N' and tmp->top->Piece->colour != start->Piece->colour){
									return true;
								}
							}
						}
					}
					break;
				}
			}
			tmp = start->down_r;
			while(tmp != NULL){
				if(tmp->Piece == NULL){
					tmp = tmp->down_r;
				}
				else if(tmp->Piece->colour != start->Piece->colour){
					if(tmp->Piece->type == 'B' or tmp->Piece->type == 'Q'){
						return true;
					}
					if(start->down_r == tmp){
						if(tmp->right != NULL){
							if(tmp->right->Piece != NULL){
								if(tmp->right->Piece->type == 'N' and tmp->right->Piece->colour != start->Piece->colour){
									return true;
								}
							}
						}
						if(tmp->down != NULL){
							if(tmp->down->Piece != NULL){
								if(tmp->down->Piece->type == 'N' and tmp->down->Piece->colour != start->Piece->colour){
									return true;
								}
							}
						}
					}
				}
				else{
					if(start->down_r == tmp){
						if(tmp->right != NULL){
							if(tmp->right->Piece != NULL){
								if(tmp->right->Piece->type == 'N' and tmp->right->Piece->colour != start->Piece->colour){
									return true;
								}
							}
						}
						if(tmp->down != NULL){
							if(tmp->down->Piece != NULL){
								if(tmp->down->Piece->type == 'N' and tmp->down->Piece->colour != start->Piece->colour){
									return true;
								}
							}
						}
					}
					break;
				}	
			}
			tmp = start->down_l;
			while(tmp != NULL){
				if(tmp->Piece == NULL){
					tmp = tmp->down_l;
				}
				else if(tmp->Piece->colour != start->Piece->colour){
					if(tmp->Piece->type == 'B' or tmp->Piece->type == 'Q'){
						return true;
					}
					if(start->down_l == tmp){
						if(tmp->left != NULL){
							if(tmp->left->Piece != NULL){
								if(tmp->left->Piece->type == 'N' and tmp->left->Piece->colour != start->Piece->colour){
									return true;
								}
							}
						}
						if(tmp->down != NULL){
							if(tmp->down->Piece != NULL){
								if(tmp->down->Piece->type == 'N' and tmp->down->Piece->colour != start->Piece->colour){
									return true;
								}
							}
						}
					}
				}
				else{
					if(start->down_l == tmp){
						if(tmp->left != NULL){
							if(tmp->left->Piece != NULL){
								if(tmp->left->Piece->type == 'N' and tmp->left->Piece->colour != start->Piece->colour){
									return true;
								}
							}
						}
						if(tmp->down != NULL){
							if(tmp->down->Piece != NULL){
								if(tmp->down->Piece->type == 'N' and tmp->down->Piece->colour != start->Piece->colour){
									return true;
								}
							}
						}
					}
					break;
				}	
			}
			return false;
		}
};

king* white_king;
king* black_king;

void createsquare(int a,int b){
	sq* newsquare = (sq*)malloc(sizeof(sq));
	newsquare->rank = 8-a;
	newsquare->file = b+1;
	newsquare->top = NULL;
	newsquare->down = NULL;
	newsquare->right = NULL;
	newsquare->left = NULL;
	newsquare->top_l = NULL;
	newsquare->top_r = NULL;
	newsquare->down_l = NULL;
	newsquare->down_r = NULL;
	newsquare->Piece = NULL;
	board[a][b] = newsquare;
	if(prev == NULL){
		prev = newsquare;
	}
	else{
		if(prev->rank == 8-a){
			prev->right = newsquare;
			newsquare->left = prev;
			if(prev->top != NULL){
				newsquare->top_l = prev->top;
				newsquare->top_l->down_r = newsquare;
				newsquare->top = newsquare->top_l->right;
				newsquare->top->down = newsquare;
				if(newsquare->top->right != NULL){
					newsquare->top_r = newsquare->top->right;
					newsquare->top_r->down_l = newsquare;
				}
			}
			prev = prev->right;
		}
		else{
			prev = board[a-1][0];
			prev->down = newsquare;
			prev->right->down_l = newsquare;
			newsquare->top = prev;
			newsquare->top_r = prev->right;
			prev = prev->down;
		}
	}
}

void createboard(){
	int i,j;
	for(i = 0; i<8 ; i++){
		for(j = 0; j<8 ; j++){
			createsquare(i,j);
		}
	}
	piece* newpiece;
	for(i = 0;i<8;i++){
		newpiece = new pawn(1,i,'W');
		newpiece = new pawn(6,i,'B');
	 	if(i == 0 or i == 7){
			newpiece = new rook(0,i,'W');
			newpiece = new rook(7,i,'B');
		}
		else if(i == 1 or i == 6){
			newpiece = new knight(0,i,'W');
			newpiece = new knight(7,i,'B');
		}
		else if(i == 2 or i == 5){
			newpiece = new bishop(0,i,'W');
			newpiece = new bishop(7,i,'B');
		}
		else if(i == 3){
			newpiece = new queen(0,i,'W');
			newpiece = new queen(7,i,'B');
		}
		else{
			white_king = new king(0,i,'W');
			black_king = new king(7,i,'B');
		}
	}
	turn = 1;
}

void printboard(){
	int i,j;
	for(i = 0;i<8;i++){
		cout << " ______";
	}
	cout << endl;
	for(i = 0;i<8;i++){
		for(j = 0;j<8;j++){
			cout << "|      ";
		}
		cout << "|" << endl;
		for(j = 0;j<8;j++){
			if(board[i][j]->Piece == NULL){
				cout << "|      ";
			}
			else{
				cout << "|  ";
				board[i][j]->Piece->toString();
				cout << "  ";
			}
		}
		cout << "|" << endl;
		for(j = 0;j<8;j++){
			cout << "|______";
		}
		cout << "|" << endl;
	}
}

bool move(char start_file,char start_rank,char end_file,char end_rank){
	start_file = toupper(start_file);
	end_file = toupper(end_file);
	sq* start;
	sq* end;
	piece* temp = NULL;
	if(start_file >= 'A' and start_file <= 'H' and start_rank >= '1' and start_rank <= '8'){
		start = board[56 - start_rank][start_file - 65];
	}
	else{
		return false;
	}
	if(end_file >= 'A' and end_file <= 'H' and end_rank >= '1' and end_rank <= '8'){
		end = board[56 - end_rank][end_file - 65];
	}
	else{
		return false;
	}
	if(start == end or start->Piece == NULL){
		return false;
	}
	if(start->Piece->vision(start,end)){
		if((turn%2 == 1 and start->Piece->colour == 'W') or (turn%2 == 0 and start->Piece->colour == 'B')){
			if(start->Piece->type == 'K'){
				if(start->Piece->colour == 'W'){
					wk_pos = end;
				}
				else{
					bk_pos = end;
				}
			}
			temp = end->Piece;
			end->Piece = start->Piece;
			start->Piece = NULL;
			if((turn%2 == 1 and white_king->in_check(wk_pos)) or (turn%2 == 0 and black_king->in_check(bk_pos))){
				start->Piece = end->Piece;
				end->Piece = temp;
				if(start->Piece->type == 'K'){
					if(start->Piece->colour == 'W'){
						wk_pos = start;
					}
					else{
						bk_pos = start;
					}
				}
				return false;
			}
			else{
				if(end->Piece->type == 'P' and end->rank == 8 and end->Piece->colour == 'W'){
					char promo;
					piece* newpiece;
					do{
						cout << "Promote the pawn into?(R,B,N,Q) : ";
						cin >> promo;
						if(promo == 'R'){
							newpiece = new rook(end->rank - 1,end->file - 1,'W');
						}
						else if(promo == 'B'){
							newpiece = new bishop(end->rank - 1,end->file - 1,'W');
						}
						else if(promo == 'N'){
							newpiece = new knight(end->rank - 1,end->file - 1,'W');
						}
						else if(promo == 'Q'){
							newpiece = new queen(end->rank - 1,end->file - 1,'W');
						}
						else{
							cout << "Enter correct promotion position" << endl;
						}
					}while(promo != 'R' or promo != 'B' or promo != 'N' or promo != 'Q');	
				}
				else if(end->Piece->type == 'P' and end->rank == 1 and end->Piece->colour == 'B'){
					char promo;
					piece* newpiece;
					do{
						cout << "Promote the pawn into?(R,B,N,Q) : ";
						cin >> promo;
						if(promo == 'R'){
							newpiece = new rook(end->rank - 1,end->file - 1,'B');
							break;
						}
						else if(promo == 'B'){
							newpiece = new bishop(end->rank - 1,end->file - 1,'B');
							break;
						}
						else if(promo == 'N'){
							newpiece = new knight(end->rank - 1,end->file - 1,'B');
							break;
						}
						else if(promo == 'Q'){
							newpiece = new queen(end->rank - 1,end->file - 1,'B');
							break;
						}
						else{
							cout << "Enter correct promotion position" << endl;
						}
					}while(true);
				}
				end->Piece->last_moved = turn;
				turn = turn + 1;
				return true;
			}
		}
	}
	return false;
}

int main(){
	createboard();
	printboard();
	char s[2];
	char e[2];
	while(true){
		if(turn%2 == 1){
			cout << "White's turn " << endl;
		}
		else{
			cout << "Black's turn " << endl;
		}
		cout << "Enter start square: ";
		cin >> s;
		cout << "Enter end square: ";
		cin >> e;
		if(move(s[0],s[1],e[0],e[1])){
			printboard();
		}
		else{
			cout << "Illegal move" << endl;
		}
		if(white_king->in_check(wk_pos)){
			cout << "White in check"<< endl;
		}
		if(black_king->in_check(bk_pos)){
			cout << "Black in check" << endl;
		}
	}
	return 0;
}
