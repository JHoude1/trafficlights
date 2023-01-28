
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <vector>


using namespace std;


class Car;
class City;
void ticker(int i, City c);
void ps(string s, int x, int y);
void CreateRoad();
bool atEnd(Car c);


class Car{

public:
    int cx;
    int cy;
    int ewDirection;
    int nsDirection;
    int max;
    int speed;
    Car(int m,int ewd,int nsd){
        max = m;
        ewDirection=ewd;
        nsDirection=nsd;
    }
    void car(){
        if (ewDirection>0){right();}
        if (ewDirection<0){left();}
    }
    void left(){
        cx--;
        ps(" ",cx + 1,cy);
        ps("0",cx , cy);
    }
    void right(){
        cx++;
        ps(" ",cx - 1,cy);
        ps("0",cx , cy);
    }
    void up(){

    }
    void down(){

    }
    void del(){
        ps(" ",cx,cy);
    }
    int maxS(){
        return max;
    }
};
class City{
    vector<Car> carslist;
    bool atEnd(Car c){
        if (c.cx == 0 || c.cx==250){
            return true;
        }
        return false;
    }
public:
    void city(int tick){
        vector<Car> tmplist;
        for (int i=0; i<carslist.size(); i++){
            if(atEnd(carslist.at(i))== false){
                tmplist.push_back(carslist.at(i));
            }else{
                carslist.at(i).del();
            }

        }
        carslist = tmplist;
        for (int i=0; i<carslist.size(); i++){
            int maxs = carslist.at(i).maxS();
            int s = canGo(i,maxs);
            // if the car has a speed of 4 it can update every tick, if the car has a speed of 2 it cna update on the 2 and 4 ticks
            if (s == 4 || ((tick==2 || tick==4)  && s == 2) || (tick==1 && s==1)){
                carslist.at(i).car();
            }
        }

    }
    void newCar(int m,int x,int y,int vx,int vy){
        Car c = Car(m,vx,vy);
        c.cx=x;
        c.cy=y;
        carslist.push_back(c);
    }


    int canGo(int curCar, int reqSpd){
        int tx,ty;
        tx=carslist.at(curCar).cx;
        ty=carslist.at(curCar).cy;
        return canGoR(0,reqSpd,curCar, reqSpd,tx,ty);

    }
    int canGoR(int i,int reqSpd, int curCar,int maxSpd, int tx, int ty){
        int tc1,tc2;
        if(carslist.at(curCar).ewDirection>0){
            tc1=i;
            tc2=curCar;
        }else{
            tc1=curCar;
            tc2=i;
        }

        if(i==carslist.size()){
            return maxSpd;
        }

        if (carslist.at(tc2).cx<carslist.at(tc1).cx && carslist.at(tc2).cy==carslist.at(tc1).cy){
            int tempDist = ewDist(carslist.at(tc1),carslist.at(tc2))-1;
            if(tempDist>reqSpd){
                return canGoR(i+1,reqSpd,curCar,maxSpd,tx,ty);
            }
            else if(tempDist<maxSpd && tempDist != 0){
                return canGoR(i+1,reqSpd,curCar,tempDist,tx,ty);
            }else{
                return 0;

            }
        }else{
            return canGoR(i+1,reqSpd,curCar,maxSpd,tx,ty);
        }

    }

    int ewDist(Car c1,Car c2){
        return abs(c1.cx-c2.cx);
    }




};



void ps(string s, int x, int y) {
    static HANDLE h = NULL;
    h = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD c;
    c.X = x;
    c.Y = y;
    SetConsoleCursorPosition(h, c);
    cout << s;
}
void hidecursor(){
    static HANDLE h = NULL;
    h =  GetStdHandle(STD_OUTPUT_HANDLE);
    bool showCursor = false;
    HANDLE std_out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(h, &cursorInfo);
    cursorInfo.bVisible = showCursor;
    SetConsoleCursorInfo(h, &cursorInfo);
    return;
}



int main() {
    hidecursor();
    CreateRoad();
    usleep(10000);
    City c;

    c.newCar(2,230,4,-2,0);
    c.newCar(4,240,4,-4,0);
    c.newCar(2,240,5,-2,0);
    c.newCar(4,230,5,-4,0);
    c.newCar(2,2,7,2,0);
    c.newCar(4,3,7,4,0);
    c.newCar(4,2,8,4,0);
    c.newCar(2,40,8,2,0);
    ticker(1,c);
    system("pause");
}



void CreateRoad(){
    for (int i=0; i<255; i++){
        ps("_",i,2);
    }
    for (int i=0; i<255; i++){
        ps("=",i,6);
    }
    for (int i=0; i<255; i++){
        ps("_",i, 10);
    }
    ps("",1,20);
}

// ticker gives the ticks for the cars to run on.
void ticker(int i, City c){
    i++;
    if(i>4){
        i=0;
    }
    c.city(i);
    usleep(10000);
    ticker(i,c);
}






