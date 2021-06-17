#include <stdio.h>
#include <iostream>
#include <graphics.h>

#define height 10
#define width 10
#define wid 600

using namespace std;

void showG(char map[height][width]);

int main(){
	FILE *file;
	file=fopen("map.bin","r");
	//file=fopen("..\\C Battle\\cmake-build-debug\\map.bin","r");
	if(file==NULL){
		cout<<"Can't open the map!";
		getch();
		return 0;
	}
	char map[height][width];
	fread(map,sizeof(map),1,file);
	showG(map);
	fclose(file);
}

void showG(char map[height][width]){
	int i,j;
	initwindow(wid*1.017,wid*1.02,"C Battle");
	setcolor(3);
	setfillstyle(1,3);
	bar(0,0,wid,wid);
	for(i=0;i<height;i++){
		for(j=0;j<width;j++){
			if(map[i][j]==' '){
				setcolor(1);
				rectangle(j*(wid/width),i*(wid/height),(j+1)*(wid/width),(i+1)*(wid/height));
			}
			else{
				switch(map[i][j]){
					case 'W':
						setcolor(1);
						setfillstyle(6,1);
						break;
					case 'E':
						setcolor(14);
						setfillstyle(11,14);
						break;
					case 'C':
						setcolor(12);
						setfillstyle(9,12);
				}
				bar(j*(wid/width),i*(wid/height),(j+1)*(wid/width),(i+1)*(wid/height));
			}
		}
	}
	getch();
	closegraph();
}


