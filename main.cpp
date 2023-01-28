
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <vector>


using namespace std;


class Car;
class City;
class TrafficLight;
void ticker(int i, City c);
void ps(string s, int x, int y);
void CreateRoad();
bool atEndEW(Car c);
bool atEndNS(Car c);
int srl;
int srml;
int srm;
int srr;
int srmr;
int tlTick;
vector<TrafficLight> trafficLightList;




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
        if (nsDirection>0){down();}
        if (nsDirection<0){up();}
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
        cy--;
        ps(" ",cx ,cy+1);
        ps("0",cx , cy);
    }
    void down(){
        cy++;
        ps(" ",cx ,cy-1);
        ps("0",cx , cy);
    }
    void del(){
        ps(" ",cx,cy);
    }
    int maxS(){
        return max;
    }
};

class TrafficLight{
    bool isNS = false;
    POINT startPos;

    void switchLights() {
        isNS = !isNS;
    }
public:
    TrafficLight(POINT setPos){
        startPos=setPos;
    }
    bool canGo(Car tc){
        int posX=startPos.x;
        int posY=startPos.y;
        int cx=tc.cx;
        int cy=tc.cy;
        if(!isNS){
            if((tc.cx==startPos.x+1&&tc.cy==startPos.y)||(tc.cx==startPos.x+3&&tc.cy==startPos.y+7)){
                return false;
            }
        }else{
            if((cx==posX+4&&cy==posY+1)||(cx==posX+4&&cy==posY+2)||(cx==posX+4&&cy==posY+3)||(cx==posX&&cy==posY+5)||(cx==posX&&cy==posY+6)||(cx==posX&&cy==posY+7)){
                return false;
            }
        }
        return true;
    }
    void changeLight(){
        if (tlTick==300){
            switchLights();
            tlTick=0;
        }else{
            tlTick++;
        }
    }

};




class City{
    vector<Car> carslist;
    bool atEndEW(Car c){
        if (c.cx == 0 || c.cx==250){
            return true;
        }
        return false;
    }
    bool atEndNS(Car c){
        if (c.cy == 0 || c.cy==15){
            return true;
        }
        return false;
    }

public:
    void city(int tick){
        vector<Car> tmplist;
        for (int i=0; i<carslist.size(); i++){
            if(atEndEW(carslist.at(i))== false && atEndNS(carslist.at(i))==false){
                tmplist.push_back(carslist.at(i));
            }else{
                carslist.at(i).del();

            }

        }
        carslist = tmplist;                         // check every car and move if allowed
        for (int i=0; i<carslist.size(); i++){


            if (carslist.at(i).ewDirection!=0){         // check car against all cars for allowed movement
                int maxsEW = carslist.at(i).maxS();
                int s1 = canGoEW(i,maxsEW);
                // if the car has a speed of 4 it can update every tick, if the car has a speed of 2 it cna update on the 2 and 4 ticks
                if ((s1 == 4 || ((tick==2 || tick==4)  && s1 == 2) || (tick==1 && s1==1))){
                    carslist.at(i).car();
                }
            }else{                                  // up & down
                int maxsNS= carslist.at(i).maxS();
                int s2 = canGoNS(i,maxsNS);
                // if the car has a speed of 4 it can update every tick, if the car has a speed of 2 it cna update on the 2 and 4 ticks
                if ((s2 == 4 || ((tick==2 || tick==4)  && s2 == 2) || (tick==1 && s2==1))){
                    carslist.at(i).car();
                }
            }
        }

    }
    void newCar(int m,int x,int y,int vx,int vy){
        Car c = Car(m,vx,vy);
        c.cx=x;
        c.cy=y;
        carslist.push_back(c);
    }


    int canGoEW(int curCar, int reqSpd){
        int tx,ty;
        tx=carslist.at(curCar).cx;
        ty=carslist.at(curCar).cy;
        return canGoREW(0,reqSpd,curCar, reqSpd,tx,ty);

    }
    int canGoREW(int i,int reqSpd, int curCar,int maxSpd, int tx, int ty){
        int tc1,tc2;
        if(i<trafficLightList.size()) {
            if (!trafficLightList.at(i).canGo(carslist.at(curCar))) {
                return 0;
            }
        }

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
                return canGoREW(i+1,reqSpd,curCar,maxSpd,tx,ty);
            }
            else if(tempDist<maxSpd && tempDist != 0){
                return canGoREW(i+1,reqSpd,curCar,tempDist,tx,ty);
            }else{
                return 0;

            }
        }else{
            return canGoREW(i+1,reqSpd,curCar,maxSpd,tx,ty);
        }

    }

    int ewDist(Car c1,Car c2){
        return abs(c1.cx-c2.cx);
    }

    int canGoNS(int curCar, int reqSpd){
        int tx,ty;
        tx=carslist.at(curCar).cx;
        ty=carslist.at(curCar).cy;
        return canGoRNS(0,reqSpd,curCar, reqSpd,tx,ty);

    }
    int canGoRNS(int i,int reqSpd, int curCar,int maxSpd, int tx, int ty){
        int tc1,tc2;
        if(i<trafficLightList.size()) {
            if (!trafficLightList.at(i).canGo(carslist.at(curCar))) {
                return 0;
            }
        }
        if(carslist.at(curCar).nsDirection>0){
            tc1=i;
            tc2=curCar;
        }else{
            tc1=curCar;
            tc2=i;
        }

        if(i==carslist.size()){
            return maxSpd;
        }

        if (carslist.at(tc2).cy<carslist.at(tc1).cy && carslist.at(tc2).cx==carslist.at(tc1).cx){
            int tempDist = nsDist(carslist.at(tc1),carslist.at(tc2))-1;
            if(tempDist>reqSpd){
                return canGoRNS(i+1,reqSpd,curCar,maxSpd,tx,ty);
            }
            else if(tempDist<maxSpd && tempDist != 0){
                return canGoRNS(i+1,reqSpd,curCar,tempDist,tx,ty);
            }else{
                return 0;

            }
        }else{
            return canGoRNS(i+1,reqSpd,curCar,maxSpd,tx,ty);
        }

    }

    int nsDist(Car c1,Car c2){
        return abs(c1.cy-c2.cy);
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
    c.newCar(1,41,1,0,1);
    ticker(1,c);
    system("pause");
}




void createSideRoad(int startPos){
    POINT pos;
    pos.x=startPos;
    pos.y=2;
    TrafficLight t = TrafficLight(pos);
    trafficLightList.push_back(t);
    srl=startPos;
    srml=startPos+1;
    srm=startPos+2;
    srmr=startPos+3;
    srr=startPos+4;

    for (int i=0;i<3;i++){
        ps("|",srl,i);
    }
    usleep(2000);
    for (int i=0;i<3;i++){
        ps("\"",srm,i);
    }
    usleep(2000);
    for (int i=0;i<3;i++){
        ps("|",srr,i);
    }
    usleep(2000);
    for (int i=0;i<3;i++){
        ps(" ",srml,i);
    }
    usleep(2000);
    for (int i=0;i<3;i++){
        ps(" ",srmr,i);
    }
    usleep(2000);

    for (int i=10;i<14;i++){
        ps("|",srl,i);
    }
    usleep(2000);
    for (int i=10;i<14;i++){
        ps("\"",srm,i);
    }
    usleep(2000);
    for (int i=10;i<14;i++){
        ps("|",srr,i);
    }
    usleep(2000);
    for (int i=10;i<14;i++){
        ps(" ",srml,i);
    }
    usleep(2000);
    for (int i=10;i<14;i++){
        ps(" ",srmr,i);
    }
    usleep(2000);
    for (int i=startPos;i<startPos+5;i++){
        ps(" ",i,10);
        ps(" ",i,6);
    }
    usleep(2000);
    ps("",1,20);
}




void CreateRoad(){
    for (int i=0; i<250; i++){
        ps("_",i,2);
    }
    usleep(1000);
    for (int i=0; i<250; i++){
        ps("=",i,6);
    }
    usleep(1000);
    for (int i=0; i<250; i++){
        ps("_",i, 10);
    }
    usleep(1000);
    createSideRoad(40);
    createSideRoad(80);
    ps("",1,20);

}


// ticker gives the ticks for the cars to run on.
void ticker(int i, City c){
    ps("AAAAA",i,50);
    i++;
    if(i>4){
        i=0;
    }
    c.city(i);
    for(int i=0; i<trafficLightList.size();i++){
        trafficLightList.at(i).changeLight();
    }
    usleep(10000);
    ticker(i,c);
}






