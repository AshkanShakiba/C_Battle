#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

FILE *info;
FILE *users;
FILE *load[7];

int bot;
int turn=1;
int showsteps=0;
#define height 10
#define width 10
int count[9];
int freeOk=0;
int score[3];
int sship;

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

char RED[9]="\x1b[31m";
char GREEN[9]="\x1b[32m";
char YELLOW[9]="\x1b[33m";
char BLUE[9]="\x1b[34m";
char MAGENTA[9]="\x1b[35m";
char CYAN[9]="\x1b[36m";
char WHITE[9]="\x1b[0m";
char RESET[9]="\x1b[0m";

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
void boundaryO(char map[height][width]);
void game();
void shoot(int num);
int change(int num);
void anykey();
void boundaryW(char map[height][width]);
int CE(int num);
void show(char map[height][width]);
void settings();
void theme();
void textcolor();
void defaulttheme();
void reversetheme();
void simpletheme();
void scoreboard();
void reset();
void save();
void Load();
int shipcount(int num);

int main(){
    /*
    sship=4;
    count[1]=0;
    count[2]=0;
    count[3]=0;
    count[4]=1;
    count[5]=0;
    count[6]=0;
    count[7]=0;
    count[8]=0;
    score[1]=0;
    score[2]=0;
    */
    sship=21;
    count[1]=4;
    count[2]=3;
    count[3]=2;
    count[4]=0;
    count[5]=1;
    count[6]=0;
    count[7]=0;
    count[8]=0;
    score[1]=0;
    score[2]=0;
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
    system("cls");
    printf("1. Play with a Friend\n2. Play with bot\n3. Load game\n");
    printf("4. Load last game\n5. Settings\n6. Score Board\n7. Reset Data\n8. Exit\n");
    scanf("%d",&choice);
    switch(choice){
        case 1:
            multiplayer();
            break;
            case 2:
                singleplayer();
                break;
                case 3:
                    Load();
                    break;
                case 4:
                    //loadlast();
                    break;
                case 5:
                    settings();
                    break;
                case 6:
                    scoreboard();
                    break;
                case 7:
                    reset();
                    break;
                case 8:
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
    printf("Second player:\n");
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
    game();
}
void signin(int num){
    user input,avlb;
    int choice;
    getuser(&input);
    while(num==2 && strcmp(input.username,player[1])==0){
        printf("User has been chosen before, Try again\n");
        printf("1. Sign in\n2. Sign up\n");
        scanf("%d",&choice);
        switch(choice){
            case 1:
                signin(num);
                break;
            case 2:
                signup(num);
        }
        return;
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
            printf("1. Sign in\n2. Sign up\n");
            scanf("%d",&choice);
            switch(choice){
                case 1:
                    signin(num);
                    break;
                case 2:
                    signup(num);
            }
            return;
        }
    }
    adduser(&input);
    strcpy(player[num],input.username);
    printf("Successfully signed up\n");
}
void getuser(user *input){
    //char c='X';
    printf("Username: ");
    scanf("%s",input->username);
    printf("Password: ");
    scanf("%s",input->password);
    /*while(c!='\n'){
        c=getchar();

    }*/
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
}
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
    printf("First point of ship:\n");
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

    /*
    current=head[num];
    while(current!=end[num]){
        printf("len=%d  (%d,%d)...(%d,%d)\n",current->len,current->p1.x,current->p1.y,current->p2.x,current->p2.y);
        current=current->next;
    }
    printf("len=%d  (%d,%d)...(%d,%d)\n",current->len,current->p1.x,current->p1.y,current->p2.x,current->p2.y);
    anykey();
    */
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
    score[1]=0;
    score[2]=0;
    while(CE(1)<sship && CE(2)<sship){
        system("cls");
        show(map[1]);
        if(!bot){
            printf("%s's turn\n",player[1]);
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
            case 3:
                //save();
                break;
            default:
                shoot(1);
        }
        boundaryW(map[1]);
        system("cls");
        show(map[1]);
        anykey();
        if(CE(1)<sship){
            if(bot){
                shoot(2);
                boundaryW(map[2]);
            }
            else{
                system("cls");
                show(map[2]);
                printf("%s's turn\n",player[2]);
                printf("1. Shoot\n2. Rocket\n3. Save\n");
                scanf("%d",&choice);
                switch(choice) {
                    case 1:
                        shoot(2);
                        break;
                    case 2:
                        //rocket(1);
                        break;
                    case 3:
                        //save();
                        break;
                    default:
                        shoot(2);
                }
                boundaryW(map[2]);
                system("cls");
                show(map[2]);
                anykey();
            }
        }
    }
    system("cls");
    if(CE(1)==sship){
        printf("%s wins!\n",player[1]);
        user avlb;
        fseek(users,0,SEEK_SET);
        while(fread(&avlb,sizeof(user),1,users)==1){
            if(strcmp(avlb.username,player[1])==0){
                avlb.score+=score[1];
                break;
            }
        }
        printf("%s%d%s score added\n",YELLOW,score[1],RESET);
        fseek(users,0,SEEK_SET);
        while(fread(&avlb,sizeof(user),1,users)==1){
            if(strcmp(avlb.username,player[2])==0){
                avlb.score+=score[2]/2;
                break;
            }
        }
    }
    if(CE(2)==sship){
        printf("%s wins!\n",player[2]);
        user avlb;
        fseek(users,0,SEEK_SET);
        while(fread(&avlb,sizeof(user),1,users)==1){
            if(strcmp(avlb.username,player[2])==0){
                avlb.score+=score[2];
                break;
            }
        }
        printf("%s%d%s score added\n",YELLOW,score[2],RESET);
        fseek(users,0,SEEK_SET);
        while(fread(&avlb,sizeof(user),1,users)==1){
            if(strcmp(avlb.username,player[1])==0){
                avlb.score+=score[1]/2;
                break;
            }
        }
    }
    printf("Press any key to back to main menu\n");
    getch();
    mainmenu();
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
            map[num][p.y][p.x]='E';
            /*
            for(i=p.x-1;i<=p.x+1;i++){
                for(j=p.y-1;j<=p.y+1;j++){
                    if(0<=i && i<height && 0<=j && j<width){
                        if(shipmap[change(num)][i][j]=='#' && map[num][i][j]==' '){
                            map[num][p.y][p.x]='E';
                        }
                    }
                }
            }
             */
            boundaryW(map[num]);
            system("cls");
            if(num==1 || !bot){
                show(map[num]);
            }
            if(CE(num)<sship) shoot(num);
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
    int min,max;
    int i,j,ii,jj;
    int complete,done;
    ship *tmp;
    //convert E to C
    for(i=1;i<3;i++){
        current=head[i];
        while(current!=end[i]){
            done=0;
            complete=1;
            if(current!=NULL && current->p1.y==current->p2.y){
                min=min(current->p1.x,current->p2.x);
                max=max(current->p1.x,current->p2.x);
                for(j=min;j<=max;j++){
                    if(map[current->p1.y][j]!='E'){
                        complete=0;
                    }
                }
                if(complete){
                    for(j=min;j<=max;j++){
                        map[current->p1.y][j]='C';
                    }
                    if(current==head[i]){
                        head[i]=current->next;
                        head[i]->prev=NULL;
                        if(freeOk);
                            free(current);
                        current=head[i];
                    }
                    else{
                        tmp=current->prev;
                        tmp->next=current->next;
                        tmp=current->next;
                        tmp->prev=current->prev;
                        previous=current;
                        current=current->next;
                        if(freeOk);
                            free(previous);
                    }
                }
                else{
                    current=current->next;
                }
                done=1;
            }
            if(current!=NULL && current->p1.x==current->p2.x && !done){
                min=min(current->p1.y,current->p2.y);
                max=max(current->p1.y,current->p2.y);
                for(j=min;j<=max;j++){
                    if(map[j][current->p1.x]!='E'){
                        complete=0;
                    }
                }
                if(complete){
                    for(j=min;j<=max;j++) {
                        map[j][current->p1.x]='C';
                    }
                    if(current==head[i]){
                        head[i]=current->next;
                        head[i]->prev=NULL;
                        if(freeOk);
                            free(current);
                        current=head[i];
                    }
                    else{
                        tmp=current->prev;
                        tmp->next=current->next;
                        tmp=current->next;
                        tmp->prev=current->prev;
                        previous=current;
                        current=current->next;
                        if(freeOk);
                            free(previous);
                    }
                }
                else{
                    current=current->next;
                }
            }
        }
        done=0;
        complete=1;
        if(current!=NULL && current->p1.y==current->p2.y){
            min=min(current->p1.x,current->p2.x);
            max=max(current->p1.x,current->p2.x);
            for(j=min;j<=max;j++){
                if(map[current->p1.y][j]!='E'){
                    complete=0;
                }
            }
            if(complete){
                for(j=min;j<=max;j++){
                    map[current->p1.y][j]='C';
                }
                if(current==head[i]){
                    head[i]=end[i]=NULL;
                    if(freeOk);
                        free(current);
                }
                else{
                    end[i]=current->prev;
                    end[i]->next=NULL;
                    if(freeOk);
                        free(current);
                }
            }
            done=1;
        }
        if(current!=NULL && current->p1.x==current->p2.x && !done){
            min=min(current->p1.y,current->p2.y);
            max=max(current->p1.y,current->p2.y);
            for(j=min;j<=max;j++){
                if(map[j][current->p1.x]!='E'){
                    complete=0;
                }
            }
            if(complete){
                for(j=min;j<=max;j++){
                    map[j][current->p1.x]='C';
                }
                if(current==head[i]){
                    head[i]=end[i]=NULL;
                    if(freeOk);
                        free(current);
                }
                else{
                    end[i]=current->prev;
                    end[i]->next=NULL;
                    if(freeOk);
                        free(current);
                }
            }
        }
    }
    //convert <space> to W
    for(i=0;i<height;i++){
        for(j=0;j<width;j++){
            if(map[i][j]=='C'){
                for(ii=i-1;ii<=i+1;ii++){
                    for(jj=j-1;jj<=j+1;jj++){
                        if(0<=ii && ii<height && 0<=jj && jj<width && map[ii][jj]==' '){
                                map[ii][jj]='W';
                        }
                    }
                }
            }
        }
    }
}
int CE(int num){
    int i,j,res=0;
    for(i=0;i<height;i++){
        for(j=0;j<width;j++){
            if(map[num][i][j]=='E' || map[num][i][j]=='C') res++;
        }
    }
    return res;
}
void show(char map[height][width]){
    int i,j,k;
    printf("%s\\x",MAGENTA);
    for(k=0;k<width;k++){
        printf("  %d ",k);
    }
    printf("\ny\\%s",CYAN);
    for(k=0;k<4*width+1;k++){
        printf("-");
    }
    for(i=0;i<height;i++){
        printf("\n%s%d ",MAGENTA,i);
        for(j=0;j<width;j++){
            //printf("| %c ",map[i][j]);
            printf("%s| ",CYAN);
            switch(map[i][j]){
                case 'W':
                    printf("%s%c",BLUE,map[i][j]);
                    break;
                case 'E':
                    printf("%s%c",YELLOW,map[i][j]);
                    break;
                case 'C':
                    printf("%s%c",RED,map[i][j]);
                    break;
                case '#':
                    printf("%s%c",GREEN,map[i][j]);
                    break;
                case 'O':
                    printf("%s%c",BLUE,map[i][j]);
                    break;
                default:
                    printf("%c",map[i][j]);
            }
            printf(" ");
        }
        printf("%s|\n  ",CYAN);
        for(k=0;k<4*width+1;k++) {
            printf("-");
        }
    }
    printf("\n%s",RESET);
}
void settings(){
    int choice;
    system("cls");
    printf("1. Choose theme\n2. Choose text color\n3. Back to main menu\n");
    scanf("%d",&choice);
    switch(choice){
        case 1:
            theme();
            break;
        case 2:
            textcolor();
            break;
        case 3:
            mainmenu();
            break;
        default:
            printf("Invalid input, Try again\n");
            settings();
    }
}
void theme(){
    int choice;
    system("cls");
    printf("1. Default theme\n2. Reverse theme\n3. Simple theme\n4. Back to main menu\n");
    scanf("%d",&choice);
    switch(choice){
        case 1:
            defaulttheme();
            break;
        case 2:
            reversetheme();
            break;
        case 3:
            simpletheme();
            break;
        case 4:
            mainmenu();
            break;
        default:
            printf("Invalid input, Try again\n");
            theme();
    }
}
void textcolor(){
    int choice;
    system("cls");
    printf("%s1. Red\n%s2. Yellow\n%s3. Green\n%s4. Cyan\n",RED,YELLOW,GREEN,CYAN);
    printf("%s5. Blue\n%s6. Magenta\n%s7. White\n%s",BLUE,MAGENTA,WHITE,RESET);
    scanf("%d",&choice);
    switch(choice){
        case 1:
            strcpy(RESET,"\x1b[31m");
            break;
        case 2:
            strcpy(RESET,"\x1b[33m");
            break;
        case 3:
            strcpy(RESET,"\x1b[32m");
            break;
        case 4:
            strcpy(RESET,"\x1b[36m");
            break;
        case 5:
            strcpy(RESET,"\x1b[34m");
            break;
        case 6:
            strcpy(RESET,"\x1b[35m");
            break;
        case 7:
            strcpy(RESET,"\x1b[0m");
            break;
        default:
            printf("Invalid input, Try again\n");
            textcolor();
    }
    printf("%sText color has been changed, press any key to back\n",RESET);
    getch();
    settings();
}
void defaulttheme(){
    strcpy(RED,"\x1b[31m");
    strcpy(GREEN,"\x1b[32m");
    strcpy(YELLOW,"\x1b[33m");
    strcpy(BLUE,"\x1b[34m");
    strcpy(MAGENTA,"\x1b[35m");
    strcpy(CYAN,"\x1b[36m");
    printf("Theme has been changed, press any key to back\n");
    getch();
    settings();
}
void reversetheme(){
    strcpy(RED,"\x1b[34m");
    strcpy(GREEN,"\x1b[35m");
    strcpy(YELLOW,"\x1b[36m");
    strcpy(BLUE,"\x1b[31m");
    strcpy(MAGENTA,"\x1b[32m");
    strcpy(CYAN,"\x1b[33m");
    printf("Theme has been changed, press any key to back\n");
    getch();
    settings();
}
void simpletheme(){
    strcpy(RED,"\x1b[0m");
    strcpy(GREEN,"\x1b[0m");
    strcpy(YELLOW,"\x1b[0m");
    strcpy(BLUE,"\x1b[0m");
    strcpy(MAGENTA,"\x1b[0m");
    strcpy(CYAN,"\x1b[0m");
    printf("Theme has been changed, press any key to back\n");
    getch();
    settings();
}
void scoreboard(){
    user avlb;
    system("cls");
    fseek(users,0,SEEK_SET);
    while(fread(&avlb,sizeof(user),1,users)==1){
        printf("%s %s%d%s\n",avlb.username,YELLOW,avlb.score,RESET);
    }
    printf("\nPress any key to back\n");
    getch();
    mainmenu();
}
void reset(){
    fclose(info);
    fclose(users);
    info=fopen("info.bin","w");
    users=fopen("users.bin","w");
    fclose(info);
    fclose(users);
    info=fopen("info.bin","r+");
    users=fopen("users.bin","r+");
}
void save(){
    int i,j;
    system("cls");
    printf("Choose your saving file\n");
    printf("1. Load 1\n2. Load 2\n3. Load 3\n4. Load 4\n5. Load 5\n6. Load 6\n");
    scanf("%d",&i);
    switch(i){
        case 1:
            load[1]=fopen("load1.bin","w+");
            break;
        case 2:
            load[2]=fopen("load2.bin","w+");
            break;
        case 3:
            load[3]=fopen("load3.bin","w+");
            break;
        case 4:
            load[4]=fopen("load4.bin","w+");
            break;
        case 5:
            load[5]=fopen("load5.bin","w+");
            break;
        case 6:
            load[6]=fopen("load6.bin","w+");
            break;
        default:
            printf("Invalid input, Try again\n");
            getch();
            save();
            return;
    }
    if(load[i]==NULL){
        printf("Unable to save\n");
        mainmenu();
        return;
    }
    fseek(load[i],0,SEEK_SET);
    for(j=1;j<3;j++){
        int shipCount=shipcount(j);
        fwrite(&shipCount,sizeof(int),1,load[i]);
        fwrite(shipmap[j],sizeof(shipmap[j]),1,load[i]);
        fwrite(map[j],sizeof(map[j]),1,load[i]);
        fwrite(player[j],sizeof(player[j]),1,load[i]);
    }
    if(strcmp(RED,"\x1b[31m")==0)
        fwrite(1,sizeof(int),1,load[i]);
    if(strcmp(RED,"\x1b[34m")==0)
        fwrite(2,sizeof(int),1,load[i]);
    if(strcmp(RED,"\x1b[0m")==0)
        fwrite(3,sizeof(int),1,load[i]);
    fclose(load[i]);
}
void Load(){
    int i,j;
    system("cls");
    printf("Choose your loading file\n");
    printf("1. Load 1\n2. Load 2\n3. Load 3\n4. Load 4\n5. Load 5\n6. Load 6\n");
    scanf("%d",&i);
    switch(i){
        case 1:
            load[1]=fopen("load1.bin","r+");
            break;
        case 2:
            load[2]=fopen("load2.bin","r+");
            break;
        case 3:
            load[3]=fopen("load3.bin","r+");
            break;
        case 4:
            load[4]=fopen("load4.bin","r+");
            break;
        case 5:
            load[5]=fopen("load5.bin","r+");
            break;
        case 6:
            load[6]=fopen("load6.bin","r+");
            break;
        default:
            printf("Invalid input, Try again\n");
            getch();
            Load();
            return;
    }
    if(load[i]==NULL){
        printf("Unable to load\n");
        mainmenu();
        return;
    }
    fseek(load[i],0,SEEK_SET);
    for(j=1;j<3;j++){
        int shipCount;
        fread(&shipCount,sizeof(int),1,load[i]);
        fread(shipmap[j],sizeof(shipmap[j]),1,load[i]);
        fread(map[j],sizeof(map[j]),1,load[i]);
        fread(player[j],sizeof(player[j]),1,load[i]);
    }
    int theme;
    fread(&theme,sizeof(int),1,load[i]);
    switch(theme){
        case 1:
            defaulttheme();
            break;
        case 2:
            reversetheme();
            break;
        case 3:
            simpletheme();
    }
    fclose(load[i]);
}
int shipcount(int num){
    ship *curr;
    int count=1;
    curr=head[num];
    while(curr!=end[num]){
        curr=curr->next;
        count++;
    }
    return count;
}