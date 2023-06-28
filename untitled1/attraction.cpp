#include "hw03.h"
Attraction* Attraction::Start = nullptr;
Attraction* Attraction::End = nullptr;
Attraction* Attraction::Selected = nullptr;
Attraction* Attraction::Hover = nullptr;

double Attraction::Distance(const Attraction& other) {
    return sqrt(pow(X - other.X, 2) + pow(Y - other.Y, 2));
}
bool Attraction::IsOpen(Time now) {
    return now >= OpenTime && now <= CloseTime;
}

Time Attraction::getOpenTime() const {
    return OpenTime;
}

void Attraction::setOpenTime(Time openTime) {
    OpenTime = openTime;
}

Time Attraction::getCloseTime() const {
    return CloseTime;
}

void Attraction::setCloseTime(Time closeTime) {
    CloseTime = closeTime;
}

Time Attraction::getPlayTime() const {
    return PlayTime;
}

void Attraction::setPlayTime(Time playTime) {
    PlayTime = playTime;
}

const string &Attraction::getName() const {
    return Name;
}

void Attraction::setName(const string &name) {
    Name = name;
}

double Attraction::getScore() const {
    return Score;
}

void Attraction::setScore(double score) {
    Score = score;
}

double Attraction::getPrice() const {
    return Price;
}

void Attraction::setPrice(double price) {
    Price = price;
}

double Attraction::getX() const {
    return X;
}

void Attraction::setX(double x) {
    X = x;
}

double Attraction::getY() const {
    return Y;
}

void Attraction::setY(double y) {
    Y = y;
}
