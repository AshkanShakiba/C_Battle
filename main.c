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
int score[3];
int sship=21;
int rock[3];
int firstscore=0;

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
void boundaryW(char map[height][width],int num);
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
void save(int turnnum);
void Load();
void loadlast();
int shipcount(int num);
void rocket(int num);
void about();
void sort(user *arr,int size);
void firstscoreset();
void freemap(char map[height][width]);
void showstepsset();
void savemap(int num);

int main(){
    rock[1]=1;
    rock[2]=1;
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
    end[1]=end[2]=NULL;
    head[1]=head[2]=NULL;
    previous=current=NULL;
    system("cls");
    printf("1. Play with a Friend\n2. Play with bot\n3. Load game\n4. Load last game\n");
    printf("5. Settings\n6. Score Board\n7. Reset Data\n8. About\n9. Exit\n");
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
            loadlast();
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
            about();
            break;
        case 9:
            exit(0);
    }
}
void multiplayer(){
    bot=0;
    int choice;
    system("cls");
    printf("First player:\n");
    printf("1. Sign in\n2. Sign up\n3. Back to main menu\n");
    scanf("%d",&choice);
    switch(choice){
        case 1:
            signin(1);
            break;
        case 2:
            signup(1);
            break;
        case 3:
            mainmenu();
            break;
        default:
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
    printf("1. Sign in\n2. Sign up\n3. Back to main menu\n");
    scanf("%d",&choice);
    switch(choice){
        case 1:
            signin(2);
            break;
        case 2:
            signup(2);
            break;
        case 3:
            mainmenu();
            break;
        default:
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
    printf("1. Sign in\n2. Sign up\n3. Back to main menu\n");
    scanf("%d",&choice);
    switch(choice){
        case 1:
            signin(1);
            break;
        case 2:
            signup(1);
            break;
        case 3:
            mainmenu();
            break;
        default:
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
    users=fopen("users.bin","r+");
    fseek(users,0,SEEK_SET);
    while(fread(&avlb,sizeof(user),1,users)==1){
        if(usersame(input,avlb)){
            strcpy(player[num],input.username);
            printf("Successfully signed in\n");
            return;
        }
    }
    fclose(users);
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
    users=fopen("users.bin","r+");
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
    fclose(users);
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
    new->score=firstscore;
    users=fopen("users.bin","r+");
    fseek(users,0,SEEK_END);
    fwrite(new,sizeof(user),1,users);
    fclose(users);
}
void putshipM(int num){
    freemap(map[1]);
    freemap(map[2]);
    freemap(shipmap[1]);
    freemap(shipmap[2]);
    int i,j;
    end[1]=end[2]=NULL;
    head[1]=head[2]=NULL;
    previous=current=NULL;
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
    freemap(map[1]);
    freemap(map[2]);
    freemap(shipmap[1]);
    freemap(shipmap[2]);
    int i,j;
    end[1]=end[2]=NULL;
    head[1]=head[2]=NULL;
    previous=current=NULL;
    for(i=8;i>0;i--){
        for(j=1;j<=count[i];j++){
            if(num==1 || !bot){
                if(showsteps){
                    system("cls");
                    show(shipmap[num]);
                }
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
            getpointsA(current,num); ///////////////////Segmentation
            boundaryO(shipmap[num]);
            if(num==1 || !bot){
                if(showsteps){
                    anykey();
                }
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
        if(turn==1){
            system("cls");
            show(map[1]);
            if(!bot){
                printf("%s's turn\n",player[1]);
            }
            printf("1. Shoot\n2. Rocket\n3. Save\n4. Back\n5. Save Map\n");
            scanf("%d",&choice);
            switch(choice) {
                case 1:
                    shoot(1);
                    break;
                case 2:
                    rocket(1);
                    break;
                case 3:
                    save(1);
                    break;
                case 4:
                    mainmenu();
                    break;
                case 5:
                    savemap(1);
                    printf("Map saved\n");
                    printf("You can see it by SeeMap.exe\n");
                    printf("Press any key to shoot\n");
                    getch();
                    shoot(1);
                    break;
                default:
                    shoot(1);
            }
            boundaryW(map[1],1);
            system("cls");
            show(map[1]);
            anykey();
        }
        if(CE(1)<sship){
            if(bot){
                shoot(2);
                boundaryW(map[2],2);
            }
            else{
                system("cls");
                show(map[2]);
                printf("%s's turn\n",player[2]);
                printf("1. Shoot\n2. Rocket\n3. Save\n4. Back\n5. Save Map\n");
                scanf("%d",&choice);
                switch(choice) {
                    case 1:
                        shoot(2);
                        break;
                    case 2:
                        rocket(2);
                        break;
                    case 3:
                        save(2);
                        break;
                    case 4:
                        mainmenu();
                        break;
                    case 5:
                        savemap(2);
                        printf("Map saved\n");
                        printf("You can see it by SeeMap.exe\n");
                        printf("Press any key to shoot\n");
                        getch();
                        shoot(2);
                        break;
                    default:
                        shoot(2);
                }
                boundaryW(map[2],2);
                system("cls");
                show(map[2]);
                anykey();
            }
        }
        turn=1;
    }
    system("cls");
    users=fopen("users.bin","r+");
    if(CE(1)==sship){
        printf("%s wins!\n",player[1]);
        user avlb;
        fseek(users,0,SEEK_SET);
        while(fread(&avlb,sizeof(user),1,users)==1){
            if(strcmp(avlb.username,player[1])==0){
                avlb.score=avlb.score+score[1];
                fseek(users,(-1)*(sizeof(user)),SEEK_CUR);
                fwrite(&avlb,sizeof(user),1,users);
                break;
            }
        }
        printf("%s%d%s score added\n",YELLOW,score[1],RESET);
        fseek(users,0,SEEK_SET);
        while(fread(&avlb,sizeof(user),1,users)==1){
            if(strcmp(avlb.username,player[2])==0){
                avlb.score=avlb.score+score[2]/2;
                fseek(users,(-1)*(sizeof(user)),SEEK_CUR);
                fwrite(&avlb,sizeof(user),1,users);
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
                avlb.score=avlb.score+score[2];
                fseek(users,(-1)*(sizeof(user)),SEEK_CUR);
                fwrite(&avlb,sizeof(user),1,users);
                break;
            }
        }
        printf("%s%d%s score added\n",YELLOW,score[2],RESET);
        fseek(users,0,SEEK_SET);
        while(fread(&avlb,sizeof(user),1,users)==1){
            if(strcmp(avlb.username,player[1])==0){
                avlb.score=avlb.score+score[1]/2;
                fseek(users,(-1)*(sizeof(user)),SEEK_CUR);
                fwrite(&avlb,sizeof(user),1,users);
                break;
            }
        }
    }
    fclose(users);
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
            boundaryW(map[num],num);
            system("cls");
            if(num==1 || !bot){
                show(map[num]);
            }
            score[num]++;
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
void boundaryW(char map[height][width],int num){
    int min,max;
    int i,j,ii,jj;
    int complete,done;
    ship *tmp;
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
                        switch(current->len){
                            case 5:
                                score[change(i)]+=5;
                                break;
                            case 3:
                                score[change(i)]+=8;
                                break;
                            case 2:
                                score[change(i)]+=12;
                                break;
                            case 1:
                                score[change(i)]+=25;
                                break;
                        }
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
                        switch(previous->len){
                            case 5:
                                score[change(i)]+=5;
                                break;
                            case 3:
                                score[change(i)]+=8;
                                break;
                            case 2:
                                score[change(i)]+=12;
                                break;
                            case 1:
                                score[change(i)]+=25;
                                break;
                        }
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
                        switch(current->len){
                            case 5:
                                score[change(i)]+=5;
                                break;
                            case 3:
                                score[change(i)]+=8;
                                break;
                            case 2:
                                score[change(i)]+=12;
                                break;
                            case 1:
                                score[change(i)]+=25;
                                break;
                        }
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
                        switch(previous->len){
                            case 5:
                                score[change(i)]+=5;
                                break;
                            case 3:
                                score[change(i)]+=8;
                                break;
                            case 2:
                                score[change(i)]+=12;
                                break;
                            case 1:
                                score[change(i)]+=25;
                                break;
                        }
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
                    switch(current->len){
                        case 5:
                            score[change(i)]+=5;
                            break;
                        case 3:
                            score[change(i)]+=8;
                            break;
                        case 2:
                            score[change(i)]+=12;
                            break;
                        case 1:
                            score[change(i)]+=25;
                            break;
                    }
                    free(current);
                }
                else{
                    end[i]=current->prev;
                    end[i]->next=NULL;
                    switch(current->len){
                        case 5:
                            score[change(i)]+=5;
                            break;
                        case 3:
                            score[change(i)]+=8;
                            break;
                        case 2:
                            score[change(i)]+=12;
                            break;
                        case 1:
                            score[change(i)]+=25;
                            break;
                    }
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
                    switch(current->len){
                        case 5:
                            score[change(i)]+=5;
                            break;
                        case 3:
                            score[change(i)]+=8;
                            break;
                        case 2:
                            score[change(i)]+=12;
                            break;
                        case 1:
                            score[change(i)]+=25;
                            break;
                    }
                    free(current);
                }
                else{
                    end[i]=current->prev;
                    end[i]->next=NULL;
                    switch(current->len){
                        case 5:
                            score[change(i)]+=5;
                            break;
                        case 3:
                            score[change(i)]+=8;
                            break;
                        case 2:
                            score[change(i)]+=12;
                            break;
                        case 1:
                            score[change(i)]+=25;
                            break;
                    }
                    free(current);
                }
            }
        }
    }
    for(i=0;i<height;i++){
        for(j=0;j<width;j++){
            if(map[i][j]=='C'){
                for(ii=i-1;ii<=i+1;ii++){
                    for(jj=j-1;jj<=j+1;jj++){
                        if(0<=ii && ii<height && 0<=jj && jj<width && map[ii][jj]==' '){
                            map[ii][jj]='W';
                        }
                        if(0<=ii && ii<height && 0<=jj && jj<width && map[ii][jj]=='E'){
                            map[ii][jj]='C';
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
    printf("1. Choose theme\n2. Choose text color\n3. Set first score\n");
    printf("4. Show steps\n5. Back to main menu\n");
    scanf("%d",&choice);
    switch(choice){
        case 1:
            theme();
            break;
        case 2:
            textcolor();
            break;
        case 3:
            firstscoreset();
            break;
        case 4:
            showstepsset();
            break;
        case 5:
            mainmenu();
            break;
        default:
            printf("Invalid input, Try again\n");
            getch();
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
            getch();
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
            getch();
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
    strcpy(CYAN,"\x1b[36m");
    printf("Theme has been changed, press any key to back\n");
    getch();
    settings();
}
void scoreboard(){
    int i=1,j;
    user avlb,*arr;
    system("cls");
    users=fopen("users.bin","r");
    fseek(users,0,SEEK_SET);
    arr=(user*)(malloc(sizeof(user)));
    fread(&avlb,sizeof(user),1,users);
    arr[i-1]=avlb;
    while(fread(&avlb,sizeof(user),1,users)==1){
        i++;
        arr=(user*)(realloc(arr,sizeof(user)*i));
        fread(&avlb,sizeof(user),1,users);
        arr[i-1]=avlb;
    }
    sort(arr,i);
    for(j=0;j<i;j++){
        printf("%s %s%d%s\n",arr[j].username,YELLOW,arr[j].score,RESET);
    }
    fclose(users);
    printf("\nPress any key to back\n");
    getch();
    mainmenu();
}
void reset(){
    int i,choice;
    system("cls");
    printf("Are you sure? all data will be removed, it can't be undone\n");
    printf("1. Yes, I'm sure\n2. Back to main menu\n");
    scanf("%d",&choice);
    switch(choice){
        case 1:
            info=fopen("info.bin","w");
            users=fopen("users.bin","w");
            fclose(info);
            fclose(users);
            load[1]=fopen("load1.bin","w");
            load[2]=fopen("load2.bin","w");
            load[3]=fopen("load3.bin","w");
            load[4]=fopen("load4.bin","w");
            load[5]=fopen("load5.bin","w");
            load[6]=fopen("load6.bin","w");
            for(i=1;i<7;i++){
                fclose(load[i]);
            }
            printf("All data removed, press any key to back\n");
            getch();
            mainmenu();
        case 2:
            mainmenu();
            break;
        default:
            printf("Invalid input, Try again\n");
            getch();
            reset();
    }
}
void save(int turnnum){
    int i,j,k,theme;
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
            save(turnnum);
            return;
    }
    if(load[i]==NULL){
        printf("Unable to save, press any key to continue\n");
        getch();
        mainmenu();
        return;
    }
    fseek(load[i],0,SEEK_SET);
    fwrite(&bot,sizeof(int),1,load[i]);
    fwrite(&turnnum,sizeof(int),1,load[i]);
    for(j=1;j<3;j++){ // j: player num, i: load num
        int shipCount=shipcount(j);
        fwrite(&rock[j],sizeof(int),1,load[i]);
        fwrite(&shipCount,sizeof(int),1,load[i]);
        fwrite(shipmap[j],sizeof(shipmap[j]),1,load[i]);
        fwrite(map[j],sizeof(map[j]),1,load[i]);
        fwrite(player[j],sizeof(player[j]),1,load[i]);
    }
    if(strcmp(RED,"\x1b[31m")==0)
        theme=1;
    if(strcmp(RED,"\x1b[34m")==0)
        theme=2;
    if(strcmp(RED,"\x1b[0m")==0)
        theme=3;
    fwrite(&theme,sizeof(int),1,load[i]);
    fclose(load[i]);
    info=fopen("info.bin","w+");
    fwrite(&i,sizeof(int),1,info);
    fclose(info);
    printf("Saved successfully, press any key to back\n");
    getch();
    mainmenu();
}
void Load(){
    int i,j,k,theme;
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
        printf("Press any key to back\n");
        getch();
        mainmenu();
        return;
    }
    fseek(load[i],0,SEEK_SET);
    if(fread(&bot,sizeof(int),1,load[i])<1){
        printf("Unable to load\n");
        printf("Press any key to back\n");
        getch();
        mainmenu();
        return;
    }
    fread(&turn,sizeof(int),1,load[i]);
    for(j=1;j<3;j++){
        int shipCount;
        fread(&rock[j],sizeof(int),1,load[i]);
        fread(&shipCount,sizeof(int),1,load[i]);
        fread(shipmap[j],sizeof(shipmap[j]),1,load[i]);
        fread(map[j],sizeof(map[j]),1,load[i]);
        fread(player[j],sizeof(player[j]),1,load[i]);
    }
    fread(&theme,sizeof(int),1,load[i]);
    switch(theme){
        case 1:
            strcpy(RED,"\x1b[31m");
            strcpy(GREEN,"\x1b[32m");
            strcpy(YELLOW,"\x1b[33m");
            strcpy(BLUE,"\x1b[34m");
            strcpy(MAGENTA,"\x1b[35m");
            strcpy(CYAN,"\x1b[36m");
            break;
        case 2:
            strcpy(RED,"\x1b[34m");
            strcpy(GREEN,"\x1b[35m");
            strcpy(YELLOW,"\x1b[36m");
            strcpy(BLUE,"\x1b[31m");
            strcpy(MAGENTA,"\x1b[32m");
            strcpy(CYAN,"\x1b[33m");
            break;
        case 3:
            strcpy(RED,"\x1b[0m");
            strcpy(GREEN,"\x1b[0m");
            strcpy(YELLOW,"\x1b[0m");
            strcpy(BLUE,"\x1b[0m");
            strcpy(MAGENTA,"\x1b[0m");
            strcpy(CYAN,"\x1b[0m");
    }
    fclose(load[i]);
    printf("Loaded successfully, press any key to start the game\n");
    getch();
    game();
}
void loadlast(){
    int i,j;
    system("cls");
    info=fopen("info.bin","r+");
    if(fread(&i,sizeof(int),1,info)<1){
        printf("Unable to load\n");
        printf("Press any key to back\n");
        getch();
        mainmenu();
        return;
    }
    fclose(info);
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
        printf("Press any key to back\n");
        getch();
        mainmenu();
        return;
    }
    fseek(load[i],0,SEEK_SET);
    if(fread(&bot,sizeof(int),1,load[i])<1){
        printf("Unable to load\n");
        printf("Press any key to back\n");
        getch();
        mainmenu();
        return;
    }
    fread(&turn,sizeof(int),1,load[i]);
    for(j=1;j<3;j++){
        int shipCount;
        fread(&rock[j],sizeof(int),1,load[i]);
        fread(&shipCount,sizeof(int),1,load[i]);
        fread(shipmap[j],sizeof(shipmap[j]),1,load[i]);
        fread(map[j],sizeof(map[j]),1,load[i]);
        fread(player[j],sizeof(player[j]),1,load[i]);
    }
    int theme;
    fread(&theme,sizeof(int),1,load[i]);
    switch(theme){
        case 1:
            strcpy(RED,"\x1b[31m");
            strcpy(GREEN,"\x1b[32m");
            strcpy(YELLOW,"\x1b[33m");
            strcpy(BLUE,"\x1b[34m");
            strcpy(MAGENTA,"\x1b[35m");
            strcpy(CYAN,"\x1b[36m");
            break;
        case 2:
            strcpy(RED,"\x1b[34m");
            strcpy(GREEN,"\x1b[35m");
            strcpy(YELLOW,"\x1b[36m");
            strcpy(BLUE,"\x1b[31m");
            strcpy(MAGENTA,"\x1b[32m");
            strcpy(CYAN,"\x1b[33m");
            break;
        case 3:
            strcpy(RED,"\x1b[0m");
            strcpy(GREEN,"\x1b[0m");
            strcpy(YELLOW,"\x1b[0m");
            strcpy(BLUE,"\x1b[0m");
            strcpy(MAGENTA,"\x1b[0m");
            strcpy(CYAN,"\x1b[0m");
    }
    fclose(load[i]);
    printf("Loaded successfully, press any key to start the game\n");
    getch();
    game();
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
void rocket(int num){
    if(rock[num]==0){
        printf("You've used your rocket this match\n");
        printf("Press any key to shoot\n");
        getch();
        shoot(num);
        return;
    }
    user avlb;
    users=fopen("users.bin","r+");
    fseek(users,0,SEEK_SET);
    while(fread(&avlb,sizeof(user),1,users)==1){
        if(strcmp(avlb.username,player[num])==0){
            if(avlb.score<100){
                printf("You don't have enough score to use this weapon\n");
                printf("Press any key to shoot\n");
                getch();
                shoot(num);
                return;
            }
            avlb.score=avlb.score-100;
            fseek(users,(-1)*(sizeof(user)),SEEK_CUR);
            fwrite(&avlb,sizeof(user),1,users);
            break;
        }
    }
    fclose(users);
    rock[num]=0;

    int choice,z,i;
    printf("1. Vertical rocket\n2. Horizontal rocket\n");
    scanf("%d",&choice);
    switch(choice){
        case 1:
            printf("Enter your column\n");
            scanf("%d",&z); // z is our x
            for(i=0;i<height;i++){ //i is our y
                if(map[num][i][z]==' '){
                    if(shipmap[change(num)][i][z]=='#'){
                        map[num][i][z]='E';
                        score[num]++;
                        break;
                    }
                    else{
                        map[num][i][z]='W';
                        boundaryW(map[num],num);
                        system("cls");
                        show(map[num]);
                        Sleep(500);
                    }
                }
            }
            break;
        case 2:
            printf("Enter your row\n");
            scanf("%d",&z); // z is our y
            for(i=0;i<width;i++){ //i is our x
                if(map[num][z][i]==' '){
                    if(shipmap[change(num)][z][i]=='#'){
                        map[num][z][i]='E';
                        score[num]++;
                        break;
                    }
                    else{
                        map[num][z][i]='W';
                        boundaryW(map[num],num);
                        system("cls");
                        show(map[num]);
                        Sleep(500);
                    }
                }
            }
    }

}
void about(){
    system("cls");
    printf("Designed By Ashkan Shakiba\n");
    printf("Developed By C\n");
    printf("2021 Winter\n");
    getch();
    mainmenu();
}
void sort(user *arr,int size){
    int i,j;
    user tmp;
    for(i=size-1;i>0;i--){
        for(j=0;j<i;j++){
            if(arr[j].score<arr[j+1].score){
                tmp=arr[j];
                arr[j]=arr[j+1];
                arr[j+1]=tmp;
            }
        }
    }
}
void firstscoreset(){
    system("cls");
    printf("Enter a number to set as the first score of each user\n");
    scanf("%d",&firstscore);
    printf("Changed successfully, press any key to back");
    getch();
    settings();
}
void freemap(char map[height][width]){
    int i,j;
    for(i=0;i<height;i++){
        for(j=0;j<width;j++){
            map[i][j]=' ';
        }
    }
}
void showstepsset(){
    int choice;
    system("cls");
    printf("Do you want to see steps while ships are being put automatically?\n");
    printf("1. Yes I want to see\n2. No I don't want\n");
    scanf("%d",&choice);
    switch(choice){
        case 1:
            showsteps=1;
            break;
        case 2:
            showsteps=0;
    }
    printf("Changed successfully, press any key to back");
    getch();
    settings();
}
void savemap(int num){
    FILE *mf;
    mf=fopen("map.bin","w");
    fwrite(map[num],sizeof(map[num]),1,mf);
    fclose(mf);
}

//By Ashkan Shakiba