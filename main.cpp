
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <fstream>





using namespace std;


class Car;
class City;
class TrafficLight;
void ticker(int i, City c,int j);
void ps(string s, int x, int y);
void CreateRoad();
bool atEndEW(Car c);
bool atEndNS(Car c);
bool canPlaceCar(int x, int y, vector<Car> cars);
int totalTime=0;
int srl;
int srml;
int srm;
int srr;
int srmr;
int tlTick=0;
int numOfCars=0;
int maxNumOfCars= 300;
bool isNS = false;
bool yellow = false;
vector<TrafficLight> trafficLightList;




class Car{
    int dist=0;
    int stop=0;
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
    void distance(){
        dist++;
    }
    void stopped(){
        stop++;
    }
    void recordData(){
        ofstream data;
        data.open("dataSheet.csv",data.app);
        data<<dist<<","<<stop<<","<<dist+stop<<","<<numOfCars<<","<<totalTime<<endl;
        data.close();
    };
};

class TrafficLight{

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

            if(((cx==posX+4&&cy==posY+1)||(cx==posX+4&&cy==posY+2)||(cx==posX+4&&cy==posY+3)||(cx==posX&&cy==posY+5)||(cx==posX&&cy==posY+6)||(cx==posX&&cy==posY+7))&&yellow){
                return false;
            }

            if((tc.cx==startPos.x+1&&tc.cy==startPos.y)||(tc.cx==startPos.x+3&&tc.cy==startPos.y+8)){
                return false;
            }
        }else if(isNS){

            if(((tc.cx==startPos.x+1&&tc.cy==startPos.y)||(tc.cx==startPos.x+3&&tc.cy==startPos.y+8))&&yellow){
                return false;
            }

            if((cx==posX+4&&cy==posY+1)||(cx==posX+4&&cy==posY+2)||(cx==posX+4&&cy==posY+3)||(cx==posX&&cy==posY+5)||(cx==posX&&cy==posY+6)||(cx==posX&&cy==posY+7)){
                return false;
            }
        }
        return true;
    }
    void changeLight(){
        if (tlTick==300 && !isNS){
            switchLights();
            yellow=true;



        }else if(tlTick==30 && isNS){
            switchLights();
            yellow=true;
        }else if(tlTick==110 && yellow){
            yellow=false;
            tlTick=0;
        }
        else if(tlTick==330){
            yellow=false;
            tlTick=0;
        }
        else{

            string s = to_string(tlTick);
            ps(s,10,50);
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

        ps(to_string(numOfCars),10,90);
        vector<Car> tmplist;
        for (int i=0; i<carslist.size(); i++){
            if(atEndEW(carslist.at(i))== false && atEndNS(carslist.at(i))==false){
                tmplist.push_back(carslist.at(i));
            }else{
                carslist.at(i).recordData();
                carslist.at(i).del();
                numOfCars--;

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
                    carslist.at(i).distance();
                }else if((carslist.at(i).max == 4 || ((tick==2 || tick==4)  && (carslist.at(i).max == 2) || (tick==1 && (carslist.at(i).max ==1))))){
                    carslist.at(i).stopped();
                }
            }else{                                  // up & down
                int maxsNS= carslist.at(i).maxS();
                int s2 = canGoNS(i,maxsNS);
                // if the car has a speed of 4 it can update every tick, if the car has a speed of 2 it cna update on the 2 and 4 ticks
                if ((s2 == 4 || ((tick==2 || tick==4)  && s2 == 2) || (tick==1 && s2==1))){
                    carslist.at(i).car();
                    carslist.at(i).distance();
                }else if((carslist.at(i).max == 4 || ((tick==2 || tick==4)  && (carslist.at(i).max == 2) || (tick==1 && (carslist.at(i).max ==1))))){
                    carslist.at(i).stopped();
                }
            }
        }
        randplace();

    }
    void randplace(){
        int i = 1+rand()%25;
        newCar(i);

    }
    void newCar(int m,int x,int y,int vx,int vy){
        Car c = Car(m,vx,vy);
        c.cx=x;
        c.cy=y;
        carslist.push_back(c);
    }
    void newCar(int i){
        if(numOfCars <= maxNumOfCars) {
            if (i == 1) {
                if (canPlaceCar(41, 1, carslist)) {
                    newCar(1, 41, 1, 0, 1);
                    numOfCars++;
                }
            } else if (i == 2) {
                if (canPlaceCar(81, 1, carslist)) {
                    newCar(1, 81, 1, 0, 1);
                    numOfCars++;
                }
            } else if (i == 3) {
                if (canPlaceCar(43, 14, carslist)) {
                    newCar(1, 43, 14, 0, -1);
                    numOfCars++;
                }
            } else if (i == 4) {
                if (canPlaceCar(83, 14, carslist)) {
                    newCar(1, 83, 14, 0, -1);
                    numOfCars++;
                }
            } else if (i == 5 || i == 13 || i == 25) {
                if (canPlaceCar(249, 3, carslist)) {
                    newCar(1, 249, 3, -1, 0);
                    numOfCars++;
                }
            } else if (i == 6 || i == 14 || i == 24) {
                if (canPlaceCar(249, 4, carslist)) {
                    newCar(2, 249, 4, -2, 0);
                    numOfCars++;
                }
            } else if (i == 7 || i == 15 || i == 23) {
                if (canPlaceCar(249, 5, carslist)) {
                    newCar(4, 249, 5, -4, 0);
                    numOfCars++;
                }
            } else if (i == 8 || i == 17 || i == 22) {
                if (canPlaceCar(1, 7, carslist)) {
                    newCar(4, 1, 7, 4, 0);
                    numOfCars++;
                }
            } else if (i == 9 || i == 18 || i == 21) {
                if (canPlaceCar(1, 8, carslist)) {
                    newCar(2, 1, 8, 2, 0);
                    numOfCars++;
                }
            } else if (i == 10 || i == 19 || i == 20) {
                if (canPlaceCar(1, 9, carslist)) {
                    newCar(1, 1, 9, 1, 0);
                    numOfCars++;
                }
            }else if (i==11){
                if (canPlaceCar(221, 1, carslist)) {
                    newCar(1, 221, 1, 0, 1);
                    numOfCars++;
                }
            }else if (i==12){
                if (canPlaceCar(223, 14, carslist)) {
                    newCar(1, 223, 14, 0, -1);
                    numOfCars++;
                }
            }
        }
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


    ticker(1,c,1);
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
        cout << "\033[47m";
        ps("|",srl,i);
        cout << "\033[0m";
        usleep(1000);
    }
    usleep(2000);
    for (int i=0;i<3;i++){
        cout << "\033[33m";
        ps("\"",srm,i);
        cout << "\033[0m";
        usleep(1000);
    }
    usleep(2000);
    for (int i=0;i<3;i++){
        cout << "\033[47m";
        ps("|",srr,i);
        cout << "\033[0m";
        usleep(1000);
    }
    usleep(2000);
    for (int i=0;i<3;i++){
        ps(" ",srml,i);
        usleep(1000);
    }
    usleep(2000);
    for (int i=0;i<3;i++){
        ps(" ",srmr,i);
        usleep(1000);
    }
    usleep(2000);

    for (int i=10;i<14;i++){
        cout << "\033[47m";
        ps("|",srl,i);
        cout << "\033[0m";
        usleep(1000);
    }
    usleep(2000);
    for (int i=10;i<14;i++){
        cout << "\033[33m";
        ps("\"",srm,i);
        cout << "\033[0m";
        usleep(1000);
    }
    usleep(2000);
    for (int i=10;i<14;i++){
        cout << "\033[47m";
        ps("|",srr,i);
        cout << "\033[0m";
        usleep(1000);
    }
    usleep(2000);
    for (int i=10;i<14;i++){
        ps(" ",srml,i);
        usleep(1000);
    }
    usleep(2000);
    for (int i=10;i<14;i++){
        ps(" ",srmr,i);
        usleep(1000);
    }
    usleep(2000);
    for (int i=startPos;i<startPos+5;i++){
        // ps(" ",i,10);
        ps(" ",i,6);
        usleep(1000);
    }
    usleep(2000);
    ps("",1,20);
}

void CreateRoad() {
    cout << "\033[47m";
    for (int i = 0; i < 250; i++) {
        ps("_", i, 2);
    }
    cout << "\033[0m";
    usleep(1000);
    cout << "\033[33m";
    for (int i=0; i<250; i++) {
        ps("=", i, 6);
    }
    cout << "\033[0m";
    usleep(1000);
    cout << "\033[47m";
    for (int i=0; i<250; i++) {
        ps("_", i, 10);
    }
    cout << "\033[0m";
    usleep(1000);
    createSideRoad(40);
    createSideRoad(80);
    createSideRoad(220);
    ps("",1,20);

}

bool canPlaceCar(int x, int y, vector<Car> cars){

    for (int i=0; i<cars.size(); i++){
        if(abs(x - cars.at(i).cx)<=0  && abs(y - cars.at(i).cy)<=0){
            return false;
        }

    }
    return true;
}
// ticker gives the ticks for the cars to run on.
void ticker(int i, City c,int j){

    i++;
    if(i>4){
        i=0;
    }
    c.city(i);

    for (int k = 0; k < trafficLightList.size(); k++) {
        trafficLightList.at(k).changeLight();
    }
    tlTick++;



    j++;
    usleep(10000);
    totalTime++;

    ticker(i,c,j);
}






