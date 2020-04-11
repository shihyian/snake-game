#include <stdio.h>
#include <stdlib.h>
#include <conio.h> 
#include <graphics.h>
#include <time.h>
#include <math.h>
#include <windows.h>
#include <iostream>
#include "mmsystem.h" 
#include<pthread.h>

#define SCREEN_HEIGHT 500 //�]�w�C���������� 
#define SCREEN_WIDTH 500 //�]�w�C�������e��
#define GRID_SIDE 40 //�]�w�C����}�C���l�ƶq 
#define LEFT_MARGINE 30 //�]�w����� 
#define TOP_MARGINE 40 //�]�w�W��� 
#define FOOD_AMOUT 1 //�]�w�C�����ͭ����ƶq 
#define INIT_SPEED  80 //�]�w��l�g���D���ʳt�� 


//�ŧi�e�i��V�C�|��� 
enum Direction {
	RIGHT, 
	LEFT, 
	UP, 
	DOWN 
};

//�ŧi�C�����X�{����C�|��� 
enum Object {
	BACKGROUND, //�I�� 
	WALL, //��λ�ê�� 
	FOOD, //����
	BACKGROUND2,//�I����
	SPEED,//�Y���ܺC 
 	POINTT,//�Y��[�Q�� 
 	LENGTH//�Y��h�T�` 
 
};

//�ŧi�g���D����`�I���c 
struct Node {
	int row; //�`�I��b�ĴX�� 
	int col; //�`�I��b�ĴX�C 
	Direction direct; //�Ӹ`�I���e�i��V 
	struct Node *next;	//���V�U�@�Ӹ`�I 
}; 

//�w�q���V�`�I���c�������ܼ� 
typedef struct Node *NodePointer;

void openWindow(); //�}�Ҵ������� 
void closeGame(NodePointer snake); //�B�z�C�������޿� 
int playGame(int field[][GRID_SIDE], NodePointer snake); //�C���i���޿� 
bool IsGameOver(NodePointer snake, int field[][GRID_SIDE]); //�g���D�O�_���`(���`����G����M����ۤv����) 
int showGameOverMsg(); //�C�������T��
void showInfo(); //��ܹC��������T 
void drawGameField(int field[][GRID_SIDE]); //ø�s�C���ϰ� 
void drawSquare(int row, int col, int color); //ø�s��� 
void controlSnakeDirection(int field[][GRID_SIDE], NodePointer snake); //Ū����L��V��J�A�ó]�w��Ҧ��g���D�`�I 
void moveNext(NodePointer snake); //ø�s�g���D�C�e�i�@�B���������  
void createFood(int field[][GRID_SIDE], NodePointer snake); //���ͭ��� 
bool IsAtWall(int field[][GRID_SIDE], int row, int col); //�P�_�O�_������  
bool IsAtSnake(NodePointer snake, int row, int col); //�P�_�O�_����g���D�ۤv������
void eatFood(int field[][GRID_SIDE], NodePointer snake); //�B�z�Y�쭹���޿� 
void addNode(NodePointer snake); //�W�[�g���D����`�I 
void playmusic();
void introduction(NodePointer snake);
int original();
Direction AI(int field[][GRID_SIDE], NodePointer snake);//��@�q������g���D�޿� 
void load(int x,int y) ;
void loading();
int playcombat(int field[][GRID_SIDE], NodePointer snake,NodePointer snake2);
void controlSnakeDirection2(int field[][GRID_SIDE], NodePointer snake);
void closeGame2(NodePointer snake,NodePointer snake2);

void createSpeed(int field[][GRID_SIDE], NodePointer snake); //���ʹ�t���� 
void eatSpeed(int field[][GRID_SIDE], NodePointer snake); //�B�z�Y���t�����޿�
void createPoint(int field[][GRID_SIDE], NodePointer snake); //���ͥ[������ 
void eatPoint(int field[][GRID_SIDE], NodePointer snake); //�B�z�Y��[�������޿�
void createLength(int field[][GRID_SIDE], NodePointer snake); //���ͼW������ 
void eatLength(int field[][GRID_SIDE], NodePointer snake); //�B�z�Y��[���׭����޿�
//void * pthread_func2(void * data_ptr);
//void * pthread_func1(void * data_ptr);
void combat();

int speed; //�g���D���ʳt�� 
int scoreSum = 0; //�������� 
int totalTime = 0; //�����C���ɶ� 
int const scorePerFood = 1; //�C�@�������i�o���� 
bool meetSelfWillDie = false; //�M�w�g���D����ۤv����O�_�|���� 
bool IFPlayAI = false; //�O�_�}��AI�Ҧ� 
bool change = false;
int food_row;
int food_col;
int field[GRID_SIDE][GRID_SIDE] = {{3,3,3,3,3,3,3,3,3,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,3,3,3,3,3,3,3,3,3},
		   						   {3,3,3,3,3,3,3,3,1,0,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,0,1,3,3,3,3,3,3,3,3},
								   {3,3,3,3,3,3,3,1,0,0,0,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,0,0,0,1,3,3,3,3,3,3,3},
								   {3,3,3,3,3,3,1,0,0,0,0,0,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,0,0,0,0,0,1,3,3,3,3,3,3},
								   {1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
							       {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
							       {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
							       {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
							       {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
							   	   {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
								   {3,3,3,1,3,3,3,3,3,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,3,3,3,3,3,1,3,3,3},
								   {3,3,3,1,3,1,3,1,3,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,3,1,3,1,3,1,3,3,3},
								   {3,3,3,1,3,1,3,1,3,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,3,1,3,1,3,1,3,3,3},
								   {3,3,3,1,1,1,1,1,1,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,1,1,1,1,1,1,3,3,3}};	

int main(){
	openWindow();
	
	Node head = {10, 11, RIGHT, NULL}; //�]�w�D�Y��l��m�M��V 
	NodePointer snake = &head;
	Node head2 = {18, 11, RIGHT, NULL}; //�]�w�D�Y��l��m�M��V 
	NodePointer snake2 = &head2;
	
	introduction(snake);
}

void introduction(NodePointer snake){
 	cleardevice();
 	playmusic();
 	char msg1[70] = "Welcome to  Nyan cat world !!!           ";
 	char msg2[70] = "���C�����T�ؼҦ�:	Nyan, AI , Harder    ";
 	char msg3[70] = "press ' o ' for  Nyan       ";
 	char msg5[70] = "press ' a ' for  AI       ";
	char msg6[70] = "press ' f ' for  Harder       ";
	 	
	int i = 0;
    for(;;i++){
     	setcolor(13);
     	settextstyle(TRIPLEX_FONT, HORIZ_DIR , 3);
     	outtextxy(30, SCREEN_HEIGHT / 2, msg1);
     
     	setcolor(14);
     	moveto(5, 190);
     	lineto(225, 190);
     	moveto(335, 190);
     	lineto(550, 190);
     	moveto(5, 190);
     	lineto(5, 500);
     	moveto(550, 190);
     	lineto(550, 500);
     	moveto(5, 500);
     	lineto(550, 500);
		//�e����
     	moveto(220, 150);
     	lineto(280, 160);
     	moveto(220, 150);
     	lineto(230, 190);
     	moveto(340, 150);
     	lineto(280, 160);
     	moveto(340, 150);
     	lineto(330, 190);
     	ellipse(280,190,0,360,50,30); 
		//�e���� 
  		moveto(250, 175);
     	lineto(260, 185);
     	moveto(260, 175);
     	lineto(250, 185);
     
     	moveto(300, 175);
     	lineto(310, 185);
     	moveto(310, 175);
     	lineto(300, 185);
		//�e�L��
     	arc(275, 200, 180,360, 5);
        arc(285, 200, 180,360, 5);
     
     	setcolor(WHITE);
     	settextstyle(TRIPLEX_FONT, HORIZ_DIR , 1);
     	outtextxy(20, SCREEN_HEIGHT / 2 + 80, msg2);
     	settextstyle(TRIPLEX_FONT, HORIZ_DIR , 2);
     	outtextxy(70, SCREEN_HEIGHT / 2 + 100, msg3);
     	outtextxy(70, SCREEN_HEIGHT / 2 + 120, msg5);
     	outtextxy(70, SCREEN_HEIGHT / 2 + 140, msg6);
     
     	delay(700);
      
     	if(kbhit()) {
      		char key;
   			key = getch(); 
   			
			if (key == 'o' || key == 'O' ) {
    			original();     
			}
			else if(key== 'f'|| key == 'F' ){
    			combat();
   			}
			else if(key== 'a'|| key == 'A' ){
    			IFPlayAI = !IFPlayAI;
    			original();
   			}
  		}
 	}
}
void combat(){
	cleardevice();
	int field[GRID_SIDE][GRID_SIDE] = {{3,3,3,3,3,3,3,3,3,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,3,3,3,3,3,3,3,3,3},
		   						   {3,3,3,3,3,3,3,3,1,0,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,0,1,3,3,3,3,3,3,3,3},
								   {3,3,3,3,3,3,3,1,0,0,0,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,0,0,0,1,3,3,3,3,3,3,3},
								   {3,3,3,3,3,3,1,0,0,0,0,0,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,0,0,0,0,0,1,3,3,3,3,3,3},
								   {1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
							       {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
							       {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
							       {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
							       {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
							   	   {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
								   {3,3,3,1,3,3,3,3,3,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,3,3,3,3,3,1,3,3,3},
								   {3,3,3,1,3,1,3,1,3,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,3,1,3,1,3,1,3,3,3},
								   {3,3,3,1,3,1,3,1,3,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,3,1,3,1,3,1,3,3,3},
								   {3,3,3,1,1,1,1,1,1,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,1,1,1,1,1,1,3,3,3}};
								   
    								  
	while(TRUE){
		Node head = {10, 11, RIGHT, NULL}; //�]�w�D�Y��l��m�M��V 
		NodePointer snake = &head;
		Node head2 = {18, 11, RIGHT, NULL}; //�]�w�D�Y��l��m�M��V 
	    NodePointer snake2 = &head2;
		speed = INIT_SPEED;
		//�]�w�C�����M��ê��
		char key;
		key = playcombat(field,snake,snake2); //�i��C��
		if (key == 'q' || key == 'Q')
			closeGame2(snake,snake2); //�p�G���a��J'q'���}�C��	
		else if (key == 's' || key == 'S'){ 
			speed = INIT_SPEED;
   			scoreSum = 0;
   			totalTime = 0;
			introduction(snake); //�p�G���a��J's' �~��C��
		}		    
	}
//	pthread_t pth1;
//	pthread_t pth2;
// 	pthread_create(&pth1, NULL, pthread_func1, NULL);
// 	pthread_create(&pth2, NULL, pthread_func2, NULL);
// 	pthread_join(pth1,NULL);
// 	pthread_join(pth2,NULL);
}
//void * pthread_func1(void * data_ptr) {	
//	while(TRUE){
//		Node head = {10, 11, RIGHT, NULL}; //�]�w�D�Y��l��m�M��V 
//		NodePointer snake = &head;
//		speed = INIT_SPEED;
//		//�]�w�C�����M��ê��
//		char key;
//		key = playGame(field, snake); //�i��C��
//		if (key == 'q' || key == 'Q')
//			closeGame(snake); //�p�G���a��J'q'���}�C��	
//		else if (key == 's' || key == 'S'){ 
//			speed = INIT_SPEED;
//   			scoreSum = 0;
//   			totalTime = 0;
//			introduction(snake); //�p�G���a��J's' �~��C��
//		}
//	}
//}
//void * pthread_func2(void * data_ptr) {	
//	while(TRUE){
//		Node head = {20, 11, LEFT, NULL}; //�]�w�D�Y��l��m�M��V 
//		NodePointer snake2 = &head;
//		speed = INIT_SPEED;
//		//�]�w�C�����M��ê��
//		playcombat(field, snake2); //�i��C��	    
//	}
//}
int original( ){
	cleardevice();
	int field[GRID_SIDE][GRID_SIDE] = {{3,3,3,3,3,3,3,3,3,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,3,3,3,3,3,3,3,3,3},
		   						   {3,3,3,3,3,3,3,3,1,0,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,0,1,3,3,3,3,3,3,3,3},
								   {3,3,3,3,3,3,3,1,0,0,0,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,0,0,0,1,3,3,3,3,3,3,3},
								   {3,3,3,3,3,3,1,0,0,0,0,0,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,0,0,0,0,0,1,3,3,3,3,3,3},
								   {1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
							       {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
							       {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
							       {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
							       {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
							   	   {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
								   {3,3,3,1,3,3,3,3,3,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,3,3,3,3,3,1,3,3,3},
								   {3,3,3,1,3,1,3,1,3,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,3,1,3,1,3,1,3,3,3},
								   {3,3,3,1,3,1,3,1,3,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,3,1,3,1,3,1,3,3,3},
								   {3,3,3,1,1,1,1,1,1,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,1,1,1,1,1,1,3,3,3}};
	while(TRUE){
		Node head = {10, 11, RIGHT, NULL}; //�]�w�D�Y��l��m�M��V 
		NodePointer snake = &head;
		speed = INIT_SPEED;
		//�]�w�C�����M��ê��
		char key;
		key = playGame(field, snake); //�i��C��
		if (key == 'q' || key == 'Q')
			closeGame(snake); //�p�G���a��J'q'���}�C��	
		else if (key == 's' || key == 'S'){ 
			speed = INIT_SPEED;
   			scoreSum = 0;
   			totalTime = 0;
			introduction(snake); //�p�G���a��J's' �~��C��
		}		    
	}
}

void playmusic(){
	PlaySound(TEXT("Nyan.wav"), NULL, SND_ASYNC|SND_LOOP);
}

//�}�Ҵ�������
void openWindow(){
	initwindow(SCREEN_WIDTH + LEFT_MARGINE * 3, SCREEN_HEIGHT + TOP_MARGINE * 3, "Hungry Snake Game");
}

//�B�z�C�������޿� 
void closeGame(NodePointer snake){
	free(snake);
	exit(0);
}
void closeGame2(NodePointer snake,NodePointer snake2){
	free(snake);
	free(snake2);
	exit(0);
}

//�C���i���޿�
int playGame(int field[][GRID_SIDE], NodePointer snake){
	
	int i = 1;
	drawGameField(field); //ø�s�C���ϰ� 
	createFood(field, snake); //���ͲĤ@������ 
	
	while(true){
		controlSnakeDirection(field, snake); //Ū�����a��J��V��A�ñN�s��V�]�w��U�g���D�`�I 
		moveNext(snake); //�̾ڸ`�I����V�Aø�s�s���g���D��m 
		eatFood(field, snake); //�P�_�O�_���Y�쭹���A�p�G���s�W�`�I����� 
		eatSpeed(field, snake); //�P�_�O�_���Y��t�סA�p�G���t���ܺC 
  		eatPoint(field, snake); //�P�_�O�_���Y����ơA�p�G������ +5 
  		eatLength(field, snake); //�P�_�O�_���Y��A�p�G���t���ܺC
		showInfo(); //��ܮɶ��M���Ƹ�T 
		
		if(IsGameOver(snake, field)) //�P�_�O�_�ŦX�C����������A 
			return showGameOverMsg(); //��ܹC�������T���A�õ��ݪ��a��J�ﶵ 
		
		//���F�Y�쭹���|���ͷs�����A�t�Τ]�H�����ͷs���� 
		srand((unsigned)time(NULL));
		if(rand()*rand() % 30 == 0)
   			createFood(field, snake);
  		if(rand()*rand() % 30 == 1)
   			createSpeed(field, snake);
  		if(rand()*rand() % 30 == 2)
   			createPoint(field, snake);
  		if(rand()*rand() % 30 == 3)
   			createLength(field, snake);	
		
		i++;
  		if(i % 50 == 4){
   			if(speed>=6)
				speed -= 5;
   			//printf("%.2f\n", speed);
  		}
		
		delay(speed); //�M�w�g���D���ʳt�סAspeed�V�p���ʶV�� 
		
		//Ū���D��V�䪺��L��L��J 
	   	if(kbhit()) {
	   		char key;
			key = getch();	
			
			//�u����J�j�p�g��a, q �M s�A�t�Τ~�������ñN��J���G�ߨ�~�h���ݳB�z 
			if (key == 'q' || key == 'Q' || key == 's' || key == 'S')
				return key; 				
			else if (key == 'a') //�M�w�O�_���ܼҦ� �A�D�n�����a�Ҧ��M�q���ޱ���AI�Ҧ� 
				IFPlayAI = !IFPlayAI;
			//else if (key == 'f')
				//combat();
		}					
	}		
}
int playcombat(int field[][GRID_SIDE], NodePointer snake,NodePointer snake2){
	int i = 1;
	drawGameField(field); //ø�s�C���ϰ� 
	createFood(field, snake); //���ͲĤ@������ 
	
	while(true){
		controlSnakeDirection(field, snake); //Ū�����a��J��V��A�ñN�s��V�]�w��U�g���D�`�I 
		controlSnakeDirection(field, snake2);
		moveNext(snake);
		moveNext(snake2);  //�̾ڸ`�I����V�Aø�s�s���g���D��m 
		eatFood(field, snake); //�P�_�O�_���Y�쭹���A�p�G���s�W�`�I����� 
//		eatSpeed(field, snake); //�P�_�O�_���Y��t�סA�p�G���t���ܺC 
//  		eatPoint(field, snake); //�P�_�O�_���Y����ơA�p�G������ +5 
//  		eatLength(field, snake); //�P�_�O�_���Y��A�p�G���t���ܺC
  		eatFood(field, snake2); //�P�_�O�_���Y�쭹���A�p�G���s�W�`�I����� 
//		eatSpeed(field, snake2); //�P�_�O�_���Y��t�סA�p�G���t���ܺC 
//  		eatPoint(field, snake2); //�P�_�O�_���Y����ơA�p�G������ +5 
//  		eatLength(field, snake2); //�P�_�O�_���Y��A�p�G���t���ܺC
		showInfo(); //��ܮɶ��M���Ƹ�T 
		
		if(IsGameOver(snake, field)) //�P�_�O�_�ŦX�C����������A 
			return showGameOverMsg(); //��ܹC�������T���A�õ��ݪ��a��J�ﶵ 
		if(IsGameOver(snake2, field)) //�P�_�O�_�ŦX�C����������A 
			return showGameOverMsg(); //��ܹC�������T���A�õ��ݪ��a��J�ﶵ 
		
		
		delay(speed); //�M�w�g���D���ʳt�סAspeed�V�p���ʶV�� 
		
		//Ū���D��V�䪺��L��L��J 
	   	if(kbhit()) {
	   		char key;
			key = getch();	
			
			//�u����J�j�p�g��a, q �M s�A�t�Τ~�������ñN��J���G�ߨ�~�h���ݳB�z 
			if (key == 'q' || key == 'Q' || key == 's' || key == 'S')
				return key; 				
			else if (key == 'a') //�M�w�O�_���ܼҦ� �A�D�n�����a�Ҧ��M�q���ޱ���AI�Ҧ� 
				IFPlayAI = !IFPlayAI;
			else if (key == 'f')
				combat();
		}					
	}		
}

//ø�s�C���ϰ�A�̾ڹC�����x�}�]�wø�s���� 
void drawGameField(int field[][GRID_SIDE]){
	
   	int row = 0, col = 0;
   	
   	cleardevice(); //�M�z�ù��e�� 
	
	for(row = 0; row < GRID_SIDE; row ++){
   		for(col = 0; col <  GRID_SIDE; col++){
				switch(field[row][col])	{
					case WALL: //��b�x�}�����ȬO1 
						drawSquare(row, col, YELLOW);
						break;
					case FOOD: //�����b�x�}�����ȬO2
						drawSquare(row, col, GREEN);
						break;	
					case BACKGROUND://�I���b�x�}�����ȬO0
						drawSquare(row, col, LIGHTCYAN);
						break;
					case BACKGROUND2://�I���b�x�}�����ȬO4
						drawSquare(row, col, BLACK);
						break;
				}	
		   }
	}
}

//ø�s���
void drawSquare(int row, int col, int color){
	
	int squareHeight = SCREEN_HEIGHT / GRID_SIDE;
   	int SquareWidth = SCREEN_WIDTH / GRID_SIDE;
	int left = 0, right = 0, bottom = 0, top = 0;
	left = LEFT_MARGINE + col * SquareWidth + col;
   	top =  TOP_MARGINE + row * squareHeight + row;
	right = left + SquareWidth;
   	bottom = top + squareHeight;
   	 
	setfillstyle(SOLID_FILL,color); //�]�wø�s���󪺬���ߩM�C�� 
   	bar(left , top, right, bottom);	//�]�wø�s�������m 
}

//ø�s�g���D�C�e�i�@�B��������� 
void moveNext(NodePointer snake){
	int currRow, currCol;
	int count = 1;
	while (snake != NULL){
		currRow = snake->row;
		currCol = snake->col;
		
		//�̾ڸ`�I����V�ݩʡA�]�w���ʤU�@�B����m	
		switch(snake->direct){
			case RIGHT:
				snake->col++;
				break;			
			case LEFT:
				snake->col--;
				break;
			case UP:
				snake->row--;
				break;			
			case DOWN:
				snake->row++;
				break;
		}
		int color= 4;
		if(count%3==0){
			color = 6;
		}
		count++;
		
		drawSquare(snake->row, snake->col, color);
		
		if (!IsAtSnake(snake, currRow, currCol))
			drawSquare(currRow, currCol, LIGHTCYAN);
			
		snake = snake -> next;	
	}
}


//�g���D�O�_���`(���`����G����M����ۤv����) 
bool IsGameOver(NodePointer snake, int field[][GRID_SIDE]){
	NodePointer head = snake;
	
	//�P�_�O�_������ 
	if (IsAtWall(field, head->row, head->col))
		return true;
	
	//�P�_�O�_����g���D�ۤv 
	if(meetSelfWillDie)
		return IsAtSnake(snake->next, head->row, head->col);
	
	return false;		
}

//�P�_�O�_������
bool IsAtWall(int field[][GRID_SIDE], int row, int col){
		if (field[row][col] == WALL || field[row][col] == BACKGROUND2)
			return true;
		return false;
}

//�P�_�O�_����g���D�ۤv������
bool IsAtSnake(NodePointer snake, int row, int col){

	while(snake != NULL){
		if (row == snake->row && col == snake->col)
			return true;
		snake = snake -> next;		
	}
	return false;
}

//�C�������T��
int showGameOverMsg(){
   	cleardevice(); //�M�z�Ҧ��ù���ơA�p�G�Ʊ�u��ܰT���ɡA�������� 
   	PlaySound(NULL, NULL, SND_FILENAME);
	int i = 0;
	char msg1[70] = "Game      Over!!                             \n";
	char msg2[70] = "press [q] to quit or [s] to restart!!        ";
   	for(;;i++){
	   	setcolor(13);
	   	settextstyle(TRIPLEX_FONT, HORIZ_DIR , 6);
	   	outtextxy(0, SCREEN_HEIGHT / 2, msg1);
	   	
	   	setcolor(14);
	   	//�e���� 
	   	moveto(220, 250);
    	lineto(280, 260);
    	moveto(220, 250);
    	lineto(230, 290);
    	moveto(340, 250);
    	lineto(280, 260);
    	moveto(340, 250);
    	lineto(330, 290);
    	ellipse(280,290,0,360,50,30); 
    	//�e���� 
		moveto(250, 275);
    	lineto(260, 285);
    	moveto(260, 275);
    	lineto(250, 285);
    	
    	moveto(300, 275);
    	lineto(310, 285);
    	moveto(310, 275);
    	lineto(300, 285);
	   	//�e�L�� 
	   	arc(275, 300, 180,
            360, 5);
        arc(285, 300, 180,
            360, 5);
	   	
	   	setcolor(WHITE);
	   	settextstyle(TRIPLEX_FONT, HORIZ_DIR , 2);
	   	outtextxy(20, SCREEN_HEIGHT / 2 + 70, msg2);
	   	
	   	delay(700);
	   	
	   	
	   	setcolor(BLACK);
	   	settextstyle(TRIPLEX_FONT, HORIZ_DIR , 7);
	   	outtextxy(0, SCREEN_HEIGHT / 2- 20, msg1);	   
		   	
	   	if(kbhit()) {
	   		char key;
			key = getch();	
			if (key == 'q' || key == 'Q' || key == 's' || key == 'S') {
				return key; 				
			}
		}
	}	
}

//��ܹC��������T
void showInfo(){
	totalTime += speed;
	char timeMsg[45] = " Time:";
	char scoreMsg[45] = "Score:";
	char modeMsg[20] = "";
	char optMsg1[50] = "press [q] to quit, [s] to restart or";
	char optMsg2[50] = "press [a] to change mode.";
	
	char time[10];
	char score[10];
	
	sprintf(time, "%5d", totalTime/1000);
	strcat(timeMsg, time);
	strcat(timeMsg, " sec.");
	
	setcolor(WHITE); //�]�w��r�C�� 
   	settextstyle(COMPLEX_FONT, HORIZ_DIR , 1); //�]�w�r���A�����Ϋ����M�r���j�p 
   	outtextxy(0, 0, timeMsg); //�̾ڧ��п�X��r��ù�	
	
	sprintf(score, "%5d", scoreSum);
	strcat(scoreMsg, score);
	strcat(scoreMsg, " point.");

	setcolor(WHITE);
   	settextstyle(COMPLEX_FONT, HORIZ_DIR , 1);
   	outtextxy(0, 19, scoreMsg);	
   	
   	if(IFPlayAI){
   		strcat(modeMsg, "AI Mode    ");  		
	}else{
   		strcat(modeMsg, "Player Mode");  		
	}
			
   	setcolor(LIGHTMAGENTA);
   	settextstyle(COMPLEX_FONT, HORIZ_DIR , 1);
   	outtextxy(SCREEN_HEIGHT - LEFT_MARGINE * 2, 0, modeMsg);
   	
   	setcolor(LIGHTGREEN);
   	settextstyle(COMPLEX_FONT, HORIZ_DIR , 1);
   	outtextxy(0, TOP_MARGINE + (GRID_SIDE + 2) * SCREEN_HEIGHT / GRID_SIDE, optMsg1);
	outtextxy(0, TOP_MARGINE + (GRID_SIDE + 2) * SCREEN_HEIGHT / GRID_SIDE + 20, optMsg2);	      	
}

//Ū����L��V��J�A�ó]�w��Ҧ��g���D�`�I 
void controlSnakeDirection(int field[][GRID_SIDE], NodePointer snake){
	
	Direction snakeDirect = snake -> direct;
	
	//get key code by pressing keybord
	int key;
	if(kbhit()) 
		key = getch();
		
	//decide snake's moving direction	
	switch(key){
		case KEY_RIGHT:
			snakeDirect = RIGHT;
			break;			
		case KEY_LEFT:
			snakeDirect = LEFT;
			break;		
		case KEY_UP:
			snakeDirect = UP;
			break;				
		case KEY_DOWN:
			snakeDirect = DOWN;
			break;				
	}
	
	if (IFPlayAI) 
		snakeDirect = AI(field, snake);

	//���F�F�칳�D�@�˲��ʤ覡�A��D���ܤ�V�ɡA�ä��O��Ө���@�_���ܡA
	//�ӬO����ʦa�A�U�Ӹ`�I�v�@���ܡA��@�Ӹ`�I����s���e�i��V�ɡA
	//�|�N�ۤv�ثe����V�ǵ��U�@�ӡC 
	Direction temp;
	while(snake != NULL){
		temp = snake -> direct;
		snake -> direct = snakeDirect;
		snakeDirect = temp;
		snake = snake -> next;
	}
}
//Ū����L��V��J�A�ó]�w��Ҧ��g���D�`�I 
void controlSnakeDirection2(int field[][GRID_SIDE], NodePointer snake){
	
	Direction snakeDirect = snake -> direct;
	
	//get key code by pressing keybord
	int key;
	if(kbhit()) 
		key = _getch();
		
	//decide snake's moving direction	
	switch(key){
		case KEY_RIGHT:
			snakeDirect = RIGHT;
			break;			
		case KEY_LEFT:
			snakeDirect = LEFT;
			break;		
		case KEY_UP:
			snakeDirect = UP;
			break;				
		case KEY_DOWN:
			snakeDirect = DOWN;
			break;				
	}
	
	Direction temp;
	while(snake != NULL){
		temp = snake -> direct;
		snake -> direct = snakeDirect;
		snakeDirect = temp;
		snake = snake -> next;
	}
}
//���ͭ���
void createFood(int field[][GRID_SIDE], NodePointer snake){
	int row, col, i, foodAmount = FOOD_AMOUT;
	srand((unsigned)time(NULL)); //���ثe�t�ήɶ��@���üƺؤl 

	for(i=0; i< foodAmount; i++){
		//�p�G�üƲ��ͪ���m�O�b��M�D���魫�|�A�h���s���͡A����ŦX���󬰤� 
		do{
			row = rand() % GRID_SIDE;
			col = rand() % GRID_SIDE;	
		}while(IsAtWall(field, row, col) || IsAtSnake(snake, row, col));
	
		field[row][col] = FOOD;
		food_row= row;
		food_col= col;
		drawSquare(row,col, GREEN);			
	}
}

//�B�z�Y�쭹���޿�
void eatFood(int field[][GRID_SIDE], NodePointer snake){
	//�p�G�D�Y�P������m���|�A�N�O�Y�쭹�� 
	if(field[snake->row][snake->col] == FOOD){
		field[snake->row][snake->col] = BACKGROUND; //�N�ӭ����M�� 
		//printf("The snake has eaten food at row: %d, col: %d\n", snake->row, snake->col);
		addNode(snake); //�W�[�`�I 
		scoreSum = scoreSum + scorePerFood; //�������� 
		createFood(field, snake); //���ͷs������ 
	}
}

//�W�[�g���D����`�I
void addNode(NodePointer snake){
	NodePointer tail = NULL;
	NodePointer newNode = (NodePointer) malloc(sizeof(Node)); //��l�Ƥ@�ӷs�`�I 
	//�M��D������ 
	while(snake != NULL){
		tail = snake;
		snake = snake -> next;
	}
	//�N�D���ڪ���V�ݩʵ��s�`�I 
	newNode -> direct = tail -> direct;
	
	//�M�w�s�`�I����m�A�̾ڧ��ڪ����ʤ�V�ӱ��W�s�`�I 
	switch(tail->direct){
		case RIGHT:
			newNode -> row = tail -> row;
			newNode -> col = tail -> col - 1;
			break;			
		case LEFT:
			newNode -> row = tail -> row;
			newNode -> col = tail -> col + 1;			
			break;
		case UP:
			newNode -> row = tail -> row + 1;
			newNode -> col = tail -> col;			
			break;			
		case DOWN:
			newNode -> row = tail -> row - 1;
			newNode -> col = tail -> col;			
			break;
	}
	
	tail -> next = newNode; //�N���ڸ`�I�s����s�`�I 
	newNode -> next = NULL;		
}

//AI��@ 
Direction AI(int field[][GRID_SIDE], NodePointer snake){
 //自己找食物並避開牆 
 NodePointer temp = snake;
 Direction snakeDirect = temp -> direct;
 Direction currentDirect = snakeDirect;

 
 
 if(food_row > temp -> row){
   snakeDirect = DOWN; 
   if((IsAtWall(field, temp -> row+1, temp -> col)) && food_col == temp -> col){
    snakeDirect = Direction(rand()%2);
    change = true; 
    if(currentDirect == RIGHT || currentDirect == LEFT){
     snakeDirect = currentDirect;
      
    }
   
   return snakeDirect;
   }
   else if((!(IsAtWall(field, temp -> row+1, temp -> col)))&& change == true){
    snakeDirect = DOWN;
    change = false;
    goto HERE;
   }
   else if(change == true){
    
    return currentDirect;
   }
  
  }
  else if(food_row < temp -> row){
   snakeDirect = UP;
   if((IsAtWall(field, temp -> row-1, temp -> col)) && food_col == temp -> col){
    snakeDirect = Direction(rand()%2);
    change = true; 
    if(currentDirect == RIGHT || currentDirect == LEFT){
     snakeDirect = currentDirect;
      
    }
   
   return snakeDirect;
   }
   
   else if((!(IsAtWall(field, temp -> row-1, temp -> col))) && change == true){
    snakeDirect = UP;
    change = false;
    goto HERE;
   }
   else if(change == true){
    
    return currentDirect;
   }

  }
  else{;}

  
   
 
  if(food_col > temp -> col){
   snakeDirect = RIGHT;

  }
  else if(food_col < temp -> col){
   snakeDirect = LEFT;

  }
  else{;}
  
 

 
 HERE:
 
 
 switch(snakeDirect){
  case RIGHT:
   if(IsAtWall(field, temp -> row, temp -> col+1)){
    snakeDirect = Direction(rand()%2+2);
    if(currentDirect == UP || currentDirect == DOWN){
     snakeDirect = currentDirect; 
    }
    if(IsAtWall(field, temp -> row-1, temp -> col)) snakeDirect = DOWN;
    else if(IsAtWall(field, temp -> row+1, temp -> col)) snakeDirect = UP;
    
   }
   break;   
   
  case LEFT:
   if(IsAtWall(field, temp -> row, temp -> col-1)){ 
    snakeDirect = Direction(rand()%2+2);
    if(currentDirect == UP || currentDirect == DOWN){
     snakeDirect = currentDirect; 
    }
    if(IsAtWall(field, temp -> row-1, temp -> col)) snakeDirect = DOWN;
    else if(IsAtWall(field, temp -> row+1, temp -> col)) snakeDirect = UP;
    
   }
   break;
    
   
  case UP:
   break;   
   
  case DOWN:
   break; 
 }
 
 
 
 return snakeDirect;
}
//���ʹ�t���� 
void createSpeed(int field[][GRID_SIDE], NodePointer snake){
 	int row, col, i, foodAmount = FOOD_AMOUT;
 	srand((unsigned)time(NULL));

 	for(i = 0; i < foodAmount; i++){
  		do{
   			row = rand() % GRID_SIDE;
   			col = rand() % GRID_SIDE; 
  		}while(IsAtWall(field, row, col) || IsAtSnake(snake, row, col));
 
  		field[row][col] = SPEED;
  		food_row= row;
  		food_col= col;
  		drawSquare(row, col, BLUE);
 	}
}

//�B�z�Y���t�������޿� 
void eatSpeed(int field[][GRID_SIDE], NodePointer snake){ 
 	if(field[snake->row][snake->col] == SPEED){
		field[snake->row][snake->col] = BACKGROUND;
  		//printf("The snake has eaten speed at row: %d, col: %d\n", snake->row, snake->col);
		speed+=10;
  		createSpeed(field, snake);
 	}
}

//���ͥ[������ 
void createPoint(int field[][GRID_SIDE], NodePointer snake){
 	int row, col, i, foodAmount = FOOD_AMOUT;
 	srand((unsigned)time(NULL));
 	
 	for(i = 0; i < foodAmount; i++){
  		do{
   			row = rand() % GRID_SIDE;
   			col = rand() % GRID_SIDE; 
  		}while(IsAtWall(field, row, col) || IsAtSnake(snake, row, col));
 
  		field[row][col] = POINTT;
  		food_row= row;
		food_col= col;
  		drawSquare(row, col, BLACK);
 	}
}

//�B�z�Y�����������޿� 
void eatPoint(int field[][GRID_SIDE], NodePointer snake){ 
 	
	if(field[snake->row][snake->col] == POINTT){
  		field[snake->row][snake->col] = BACKGROUND;
  		//printf("The snake has eaten point at row: %d, col: %d\n", snake->row, snake->col);
  		scoreSum+=5;
		createPoint(field, snake);
 	}
}

//���ͼW�[���ת����� 
void createLength(int field[][GRID_SIDE], NodePointer snake){
 	int row, col, i, foodAmount = FOOD_AMOUT;
 	srand((unsigned)time(NULL));

 	for(i=0; i< foodAmount; i++){
 		
  	do{
   		row = rand() % GRID_SIDE;
   		col = rand() % GRID_SIDE; 
  	}while(IsAtWall(field, row, col) || IsAtSnake(snake, row, col));
 
  	field[row][col] = LENGTH;
  	food_row= row;
	food_col= col;
  	drawSquare(row,col, 12); 
 	}
}

//�B�z�Y����׼W�[�������޿� 
void eatLength(int field[][GRID_SIDE], NodePointer snake){
 
 	if(field[snake->row][snake->col] == LENGTH){
  		field[snake->row][snake->col] = BACKGROUND; 
  		//printf("The snake has eaten food at row: %d, col: %d\n", snake->row, snake->col);
  		for(int i = 0; i < 3; i++)
   			addNode(snake);
  		createFood(field, snake);
  	}
}
