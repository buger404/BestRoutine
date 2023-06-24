#include "time.h"
#include <stdexcept>
Time::Time(int hour, int minute){
    if (minute > 60 || minute < 0)
        throw std::runtime_error("分钟数不正确。");
    Hour = hour;
    Minute = minute;
}

bool Time::operator<(const Time & other) const{
    if (other.Hour > Hour){
        return true;
    }else if (other.Hour == Hour){
        return other.Minute > Minute;
    }else{
        return false;
    }
}

bool Time::operator>(const Time & other) const {
    if (other.Hour < Hour){
        return true;
    }else if (other.Hour == Hour){
        return other.Minute < Minute;
    }else{
        return false;
    }
}

bool Time::operator<=(const Time & other) const{
    if (other.Hour > Hour){
        return true;
    }else if (other.Hour == Hour){
        return other.Minute >= Minute;
    }else{
        return false;
    }
}

bool Time::operator>=(const Time & other) const {
    if (other.Hour < Hour){
        return true;
    }else if (other.Hour == Hour){
        return other.Minute <= Minute;
    }else{
        return false;
    }
}

void Time::operator+=(Time & other) {
    Minute += other.Minute;
    if (Minute > 60){
        Hour++;
        Minute -= 60;
    }
    Hour += other.Hour;
}

int Time::getHour() const {
    return Hour;
}

void Time::setHour(int hour) {
    Hour = hour;
}

int Time::getMinute() const {
    return Minute;
}

void Time::setMinute(int minute) {
    if (minute > 60 || minute < 0)
        throw std::runtime_error("分钟数不正确。");
    Minute = minute;
}

Time Time::operator+(const Time & other) {
    Time ret(other.Hour, other.Minute);
    ret += *this;
    return ret;
}

void Time::operator+=(const Time & other) {
    Minute += other.Minute;
    if (Minute > 60){
        Hour++;
        Minute -= 60;
    }
    Hour += other.Hour;
}