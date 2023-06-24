#ifndef HW02_TIME_H
#define HW02_TIME_H
class Time{
private:
    int Hour, Minute;
public:
    int getHour() const;
    void setHour(int hour);
    int getMinute() const;
    void setMinute(int minute);
    Time(int, int);
    bool operator<(const Time&) const;
    bool operator>(const Time&) const;
    bool operator<=(const Time&) const;
    bool operator>=(const Time&) const;
    void operator+=(Time&);
    void operator+=(const Time&);
    Time operator+(const Time&);
    Time(){
        Hour = 0; Minute = 0;
    }
};
#endif //HW02_TIME_H
