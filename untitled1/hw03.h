#ifndef HW03_HW03_H
#define HW03_HW03_H
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include "time.h"
using namespace std;
class Attraction{
private:
    Time OpenTime, CloseTime, PlayTime;
    string Name;
    double Score,Price,X,Y;
public:
    Time getOpenTime() const;
    void setOpenTime(Time openTime);
    Time getCloseTime() const;
    void setCloseTime(Time closeTime);
    Time getPlayTime() const;
    void setPlayTime(Time playTime);
    const string &getName() const;
    void setName(const string &name);
    double getScore() const;
    void setScore(double score);
    double getPrice() const;
    void setPrice(double price);
    double getX() const;
    void setX(double x);
    double getY() const;
    void setY(double y);
    double Distance(const Attraction&);
    bool IsOpen(Time);
    static Attraction *Start, *End, *Selected, *Hover;
};
class Routine{
public:
    vector<int> Data;
    vector<int> Choices;
    int Position;
    double RemainingMoney, RemainingEnergy, MaxMoney, MaxEnergy;
    Time Time;
    double TotalScore();
    double QualityCoefficient();

    static vector<Attraction> Attractions;
    static Routine CurrentResult;
};
#endif //HW03_HW03_H
