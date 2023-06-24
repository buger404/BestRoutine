#include "hw03.h"
vector<Attraction> Routine::Attractions = vector<Attraction>();
Routine Routine::CurrentResult = Routine();

double Routine::QualityCoefficient() {
    double score, price, distance;
    score = TotalScore() / Data.size() / 5.0;
    price = RemainingMoney / MaxMoney;
    distance = RemainingEnergy / MaxEnergy;
    return 0.4 * score + 0.4 * price + 0.2 * distance;
}

double Routine::TotalScore() {
    double score = 0;
    for(auto &i:Data){
        score += Attractions[i].getScore();
    }
    return score;
}
