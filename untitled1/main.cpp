#include "hw03.h"
#include "mainwindow.h"
#include "displayer.h"

#include <QApplication>

Time ParseTime(const string& str){
    auto separator = str.find(':');
    int hour = stoi(str.substr(0, separator)),
        min = stoi(str.substr(separator + 1));
    return {hour, min};
}
void Initialize(const string& path){
    ifstream stream(path, ios::in);
    if (stream.fail()){
        throw runtime_error("无效的数据文件路径");
    }
    string data;
    Attraction* attraction = nullptr;
    while(getline(stream,data)){
        if (data.find("---") == 0){
            if (attraction != nullptr)
                Routine::Attractions.push_back(*attraction);
            attraction = new Attraction();
            attraction->setName(data.substr(4));
        }
        else if (attraction != nullptr){
            auto separator = data.find(':');
            string property = data.substr(0, separator);
            string value = data.substr(separator + 1);
            if (property == "OpenTime"){
                attraction->setOpenTime(ParseTime(value));
            }
            else if (property == "CloseTime"){
                attraction->setCloseTime(ParseTime(value));
            }
            else if (property == "PlayTime"){
                attraction->setPlayTime(ParseTime(value));
            }
            else if (property == "Score"){
                attraction->setScore(stod(value));
            }
            else if (property == "Price"){
                attraction->setPrice(stod(value));
            }
            else if (property == "Position"){
                separator = value.find(',');
                attraction->setX(stod(value.substr(0, separator)));
                attraction->setY(stod(value.substr(separator + 1)));
            }
        }
    }
    if (attraction != nullptr)
        Routine::Attractions.push_back(*attraction);
    stream.close();
    qDebug() << "Loaded " << Routine::Attractions.size() << " attractions.";
    Attraction::Selected = &Routine::Attractions[0];
}

int main(int argc, char *argv[])
{
    Initialize("attractions.txt");
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
