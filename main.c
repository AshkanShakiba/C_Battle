#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

FILE *info;
FILE *users;

int bot;
int turn=1;
int showsteps=0;
//int height=10;
//int width=10;
#define height 10
#define width 10
int count[9];

char player[3][33];
char map[3][height][width];
char shipmap[3][height][width];

typedef struct{
    char username[33];
    char password[33];
    int score;
}user;
typedef struct{
    int x;
    int y;
}point;
typedef struct{
    int len;
    point p1;
    point p2;
    void *next;
    void *prev;
    //struct ship *next;
    //struct ship *prev;
}ship;

ship *head[3],*end[3];
ship *current,*previous;

void mainmenu();
void multiplayer();
void singleplayer();
void signin(int num);
void signup(int num);
void getuser(user *input);
int usersame(user x,user y);
void adduser(user *new);
void putshipM(int num);
void getpoint(point *p);
void getpoints(ship *s,int num);
void getpointsA(ship *s,int num);
void putshipA(int num);
void show(char map[height][width]);
void boundaryO(char map[height][width]);
void game();
void shoot(int num);
int change(int num);
void anykey();
void boundaryW(char map[height][width]);

int main(){
    count[1]=4;
    count[2]=3;
    count[3]=2;
    count[4]=0;
    count[5]=1;
    count[6]=0;
    count[7]=0;
    count[8]=0;
    time_t t = time(NULL);
    srand(t);
    info=fopen("info.bin","r+");
    users=fopen("users.bin","r+");
    int i,j,k;
    for(i=1;i<3;i++){
        for(j=0;j<height;j++){
            for(k=0;k<width;k++){
                map[i][j][k]=' ';
                shipmap[i][j][k]=' ';
            }
        }
    }
    mainmenu();
}

void mainmenu(){
    int choice;
    printf("1. Play with a Friend\n2. Play with bot\n3. Load game\n");
    printf("4.Load last game\n5. Settings\n6. Score Board\n7. Exit\n");
    scanf("%d",&choice);
    switch(choice){
        case 1:
            multiplayer();
            break;
            case 2:
                singleplayer();
                break;
                case 3:
                    //load();
                    break;
                case 4:
                    //loadlast();
                    break;
                case 5:
                    //settings();
                    break;
                case 6:
                    //scoreboard();
                    break;
                case 7:
                    exit(0);
    }
}
void multiplayer(){
    bot=0;
    int choice;
    system("cls");
    printf("First player:\n");
    printf("1. Sign in\n2. Sign up\n");
    scanf("%d",&choice);
    switch(choice){
        case 1:
            signin(1);
            break;
        case 2:
            signup(1);
    }
    printf("How to put ships?\n1. Auto\n2. Manual\n");
    scanf("%d",&choice);
    switch(choice){
        case 1:
            putshipA(1);
            break;
        case 2:
            putshipM(1);
    }
    system("cls");
    show(shipmap[1]);
    anykey();
    system("cls");
    printf("Second player\n");
    printf("1. Sign in\n2. Sign up\n");
    scanf("%d",&choice);
    switch(choice){
        case 1:
            signin(2);
            break;
        case 2:
            signup(2);
    }
    printf("How to put ships?\n1. Auto\n2. Manual\n");
    scanf("%d",&choice);
    switch(choice){
        case 1:
            putshipA(2);
            break;
        case 2:
            putshipM(2);
    }
    system("cls");
    show(shipmap[2]);
    anykey();
    game();
}
void singleplayer(){
    bot=1;
    int choice;
    system("cls");
    printf("Player:\n");
    printf("1. Sign in\n2. Sign up\n");
    scanf("%d",&choice);
    switch(choice){
        case 1:
            signin(1);
            break;
        case 2:
            signup(1);
    }
    printf("How to put ships?\n1. Auto\n2. Manual\n");
    scanf("%d",&choice);
    switch(choice){
        case 1:
            putshipA(1);
            break;
        case 2:
            putshipM(1);
    }
    system("cls");
    show(shipmap[1]);
    anykey();
    strcpy(player[2],"Mr. bot");
    putshipA(2);
    system("cls");
    show(shipmap[2]);
    anykey();
    game();
}
void signin(int num){
    user input,avlb;
    int choice;
    getuser(&input);
    while(num==2 && strcmp(input.username,player[1])==0){
        printf("User has been chosen before, Try again\n");
        getuser(&input);
    }
    fseek(users,0,SEEK_SET);
    while(fread(&avlb,sizeof(user),1,users)==1){
        if(usersame(input,avlb)){
            strcpy(player[num],input.username);
            printf("Successfully signed in\n");
            return;
        }
    }
    printf("Username or password is not correct\n");
    printf("1. Sign in\n2. Sign up\n");
    scanf("%d",&choice);
    switch(choice){
        case 1:
            signin(num);
            break;
        case 2:
            signup(num);
    }
}
void signup(int num){
    user input,avlb;
    int choice;
    getuser(&input);
    fseek(users,0,SEEK_SET);
    while(fread(&avlb,sizeof(user),1,users)==1){
        if(strcmp(input.username,avlb.username)==0){
            printf("Username has been chosen before, Try again\n");
            signup(num);
            return;
        }
    }
    adduser(&input);
    strcpy(player[num],input.username);
    printf("Successfully signed up\n");
}
void getuser(user *input){
    printf("Username: ");
    scanf("%s",input->username);
    printf("Password: ");
    scanf("%s",input->password);
}
int usersame(user x,user y){
    if(strcmp(x.username,y.username)==0 && strcmp(x.password,y.password)==0)
        return 1;
    return 0;
}
void adduser(user *new){
    new->score=0;
    fseek(users,0,SEEK_END);
    fwrite(new,sizeof(user),1,users);
    fclose(users);
    users=fopen("users.bin","r+");
} //Fix needed
void putshipM(int num){
    int i,j;
    previous=NULL;
    for(i=8;i>=1;i--){
        for(j=1;j<=count[i];j++){
            system("cls");
            show(shipmap[num]);
            current=(ship *)(malloc(sizeof(ship)));
            current->len=i;
            if(head[num]==NULL){
                head[num]=current;
            }
            else{
                previous->next=current;
                current->prev=previous;
            }
            previous=current;
            printf("Put ship size %d num #%d\n",i,j);
            getpoints(current,num);
            boundaryO(shipmap[num]);
        }
    }
    end[num]=previous;
}
void getpoint(point *p){
    int x,y;
    printf("Enter x: ");
    scanf("%d",&(p->x));
    while(p->x<0 || p->x>=width){
        printf("Invalid Input, it must be between 0 and %d\n",width-1);
        printf("Enter x again: ");
        scanf("%d",&(p->x));
    }
    printf("Enter y: ");
    scanf("%d",&(p->y));
    while(p->y<0 || p->y>=height){
        printf("Invalid Input, it must be between 0 and %d\n",height-1);
        printf("Enter y again: ");
        scanf("%d",&(p->y));
    }
}
void getpoints(ship *s,int num){
    int i,min,max;
    int invalid=1;
    if(s->len==1){
        getpoint(&(s->p1));
        s->p2=s->p1;
        if(shipmap[num][s->p1.y][s->p1.x]!=' '){
            printf("Invalid input, Try again\n");
            getpoints(s,num);
        }
        else{
            printf("Ship has been put\n");
            shipmap[num][s->p1.y][s->p1.x]='#';
        }
        return;
    }
    printf("Fisrt point of ship:\n");
    getpoint(&(s->p1));
    printf("Second point of ship:\n");
    getpoint(&(s->p2));
    min=min(s->p1.x,s->p2.x);
    max=max(s->p1.x,s->p2.x);
    if(max-min==s->len-1 && s->p1.y==s->p2.y){
        for(i=min;i<=max;i++){
            if(shipmap[num][s->p1.y][i]!=' '){
                printf("Can't put ship in this coordinates, Try again\n");
                getpoints(s,num);
                return;
            }
        }
        printf("Ship has been put\n");
        for(i=min;i<=max;i++){
            shipmap[num][s->p1.y][i]='#';
        }
        invalid=0;
    }
    min=min(s->p1.y,s->p2.y);
    max=max(s->p1.y,s->p2.y);
    if(max-min==s->len-1 && s->p1.x==s->p2.x){
        for(i=min;i<=max;i++){
            if(shipmap[num][i][s->p1.x]!=' '){
                printf("Can't put ship in this coordinates, Try again\n");
                getpoints(s,num);
                return;
            }
        }
        printf("Ship has been put\n");
        for(i=min;i<=max;i++){
            shipmap[num][i][s->p1.x]='#';
        }
        invalid=0;
    }
    if(invalid){
        printf("Invalid input, Try again\n");
        getpoints(s,num);
    }
}
void putshipA(int num){
    int i,j;
    previous=NULL;
    for(i=8;i>0;i--){
        for(j=1;j<=count[i];j++){
            if(showsteps){
                system("cls");
                show(shipmap[num]);
            }
            current=(ship *)(malloc(sizeof(ship)));
            current->len=i;
            if(head[num]==NULL){
                head[num]=current;
            }
            else{
                previous->next=current;
                current->prev=previous;
            }
            previous=current;
            getpointsA(current,num);
            boundaryO(shipmap[num]);
            if(showsteps){
                anykey();
            }
        }
    }
    end[num]=previous;
}
void getpointsA(ship *s,int num){
    int i,min,max;
    int invalid=1;
    if(s->len==1){
        s->p1.y=rand()%height;
        s->p1.x=rand()%width;
        s->p2=s->p1;
        if(shipmap[num][s->p1.y][s->p1.x]!=' '){
            getpointsA(s,num);
        }
        else{
            shipmap[num][s->p1.y][s->p1.x]='#';
        }
        return;
    }
    s->p1.y=rand()%height;
    s->p1.x=rand()%width;
    s->p2.y=rand()%height;
    s->p2.x=rand()%width;
    min=min(s->p1.x,s->p2.x);
    max=max(s->p1.x,s->p2.x);
    if(max-min==s->len-1 && s->p1.y==s->p2.y){
        for(i=min;i<=max;i++){
            if(shipmap[num][s->p1.y][i]!=' '){
                getpointsA(s,num);
                return;
            }
        }
        for(i=min;i<=max;i++){
            shipmap[num][s->p1.y][i]='#';
        }
        invalid=0;
    }
    min=min(s->p1.y,s->p2.y);
    max=max(s->p1.y,s->p2.y);
    if(max-min==s->len-1 && s->p1.x==s->p2.x){
        for(i=min;i<=max;i++){
            if(shipmap[num][i][s->p1.x]!=' '){
                getpointsA(s,num);
                return;
            }
        }
        for(i=min;i<=max;i++){
            shipmap[num][i][s->p1.x]='#';
        }
        invalid=0;
    }
    if(invalid){
        getpointsA(s,num);
    }
}
void show(char map[height][width]){
    int i,j,k;
    printf("\\x");
    for(k=0;k<width;k++){
        printf("  %d ",k);
    }
    printf("\ny\\");
    for(k=0;k<4*width+1;k++){
        printf("-");
    }
    for(i=0;i<height;i++){
        printf("\n%d ",i);
        for(j=0;j<width;j++){
            printf("| %c ",map[i][j]);
        }
        printf("|\n  ");
        for(k=0;k<4*width+1;k++) {
            printf("-");
        }
    }
    printf("\n");
}
void boundaryO(char map[height][width]){
    int i,j,ii,jj;
    for(i=0;i<height;i++){
        for(j=0;j<width;j++){
            if(map[i][j]=='#'){
                for(ii=i-1;ii<=i+1;ii++){
                    for(jj=j-1;jj<=j+1;jj++){
                        if(0<=ii && ii<height && 0<=jj && jj<width && map[ii][jj]!='#')
                            map[ii][jj]='O';
                    }
                }
            }
        }
    }
}
void game(){
    int choice;
    printf("\nPress any key to start the game");
    while(head[1]!=NULL && head[2]!=NULL){
        system("cls");
        show(map[1]);
        if(!bot){
            printf("First player's turn\n");
        }
        printf("1. Shoot\n2. Rocket\n3. Save\n");
        scanf("%d",&choice);
        switch(choice) {
            case 1:
                shoot(1);
                break;
            case 2:
                //rocket(1);
                break;
                //case 3:
                //save();
        }
        boundaryW(map[1]);
        system("cls");
        show(map[1]);
        anykey();
        if(bot){
            shoot(2);
            boundaryW(map[2]);
        }
        else{
            system("cls");
            show(map[2]);
            printf("Second player's turn\n");
            printf("1. Shoot\n2. Rocket\n3. Save\n");
            scanf("%d",&choice);
            switch(choice) {
                case 1:
                    shoot(2);
                    break;
                case 2:
                    //rocket(1);
                    break;
                //case 3:
                    //save();
            }
            boundaryW(map[2]);
            system("cls");
            show(map[2]);
            anykey();
        }
    }
}
void shoot(int num){
    int i,j;
    point p;
    if(bot && num==2){
        p.y=rand()%height;
        p.x=rand()%width;
    }
    else{
        getpoint(&p);
    }
    if(map[num][p.y][p.x]==' '){
        if(shipmap[change(num)][p.y][p.x]=='#'){
            map[num][p.y][p.x]='C';
            for(i=p.x-1;i<=p.x+1;i++){
                for(j=p.y-1;j<=p.y+1;j++){
                    if(0<=i && i<height && 0<=j && j<width){
                        if(shipmap[change(num)][i][j]=='#' && map[num][i][j]==' '){
                            map[num][p.y][p.x]='E';
                        }
                    }
                }
            }
            boundaryW(map[num]);
            system("cls");
            show(map[num]);
            shoot(num);
        }
        else{
            map[num][p.y][p.x]='W';
        }
    }
    else{
        if(!bot)
            printf("You can't select this area, Try again\n");
        shoot(num);
    }

}
int change(int num){
    if(num==1) return 2;
    if(num==2) return 1;
}
void anykey(){
    printf("Press any key to continue... ");
    getch();
}
void boundaryW(char map[height][width]){
    int i,j,ii,jj;
    for(i=0;i<height;i++){
        for(j=0;j<width;j++){
            if(map[i][j]=='C'){
                for(ii=i-1;ii<=i+1;ii++){
                    for(jj=j-1;jj<=j+1;jj++){
                        if(0<=ii && ii<height && 0<=jj && jj<width){
                            if(map[ii][jj]==' ')
                                map[ii][jj]='W';
                            if(map[ii][jj]=='E')
                                map[ii][jj]='C';
                        }
                    }
                }
            }
        }
    }
}