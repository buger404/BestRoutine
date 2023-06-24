
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "hw03.h"
#include "time.h"

Time endTime(19, 0);
int LoopCnt = 100;
double* pheromone;
double* base_pheromone;
bool* ban;
void UpdatePheromone(Routine* ants, int& s){
    for(int i = 0;i < s * 2;i++){
        if (ants[i].Data.empty())
            continue;
        double f = ants[i].QualityCoefficient() / (s * 2);
        for(int j = 0;j < ants[i].Data.size() - 1;j++){
            pheromone[ants[i].Data[j] * s + ants[i].Data[j + 1]] += f;
        }
        base_pheromone[ants[i].Data[0]] += f;
    }
}
void GoAnt(Routine &ant, int& s, double& money, double& energy, int start = -1){
    for (int k = 0;k < s;k++)
        ban[k] = false;
    ant.RemainingEnergy = ant.MaxEnergy = energy;
    ant.RemainingMoney = ant.MaxMoney = money;
    ant.Time = Time(8,0);
    ant.Data.clear();
    ant.Position = -1;
    while(true){
        ant.Choices.clear();
        double sum = 0;
        int earliest = -1;
        Time earliestTime(999,0);
        for(int k = 0;k < s;k++){
            auto &attraction= Routine::Attractions[k];
            double ener = 0;
            if (ant.Position != -1)
                ener = attraction.Distance(Routine::Attractions[ant.Position]);
            if (!ban[k] &&
                ant.RemainingMoney >= attraction.getPrice() &&
                ant.RemainingEnergy >= ener &&
                (ant.Time + attraction.getPlayTime()) < endTime){

                if (attraction.IsOpen(ant.Time)){
                    ant.Choices.push_back(k);
                    if (ant.Position != -1)
                        sum += pheromone[ant.Position * s + k];
                }else{
                    if (attraction.getOpenTime() < earliestTime){
                        earliestTime = attraction.getOpenTime();
                        earliest = k;
                    }
                }

            }
        }
        if (ant.Choices.size() == 0 && earliest == -1)
            break;

        int stick = -1;

        if (earliest != -1)
            goto skipDrawing;

        if (sum == 0)
        {
            if (start != -1)
                stick = start;
            else
                stick = rand() % ant.Choices.size();
        }
        else
        {
            sum = (rand() % 10000) / 10000.0 * sum;
            stick = -1;
            for(auto &a:ant.Choices){
                sum -= pheromone[ant.Position * s + a];
                if (sum <= 0){
                    stick = a;
                    break;
                }
            }
            if (stick == -1)
                stick = ant.Choices[ant.Choices.size() - 1];
        }
    skipDrawing:
        if (stick == -1){
            stick = earliest;
            ant.Time = Routine::Attractions[stick].getOpenTime();
        }
        ban[stick] = true;
        auto &attraction= Routine::Attractions[stick];
        if (ant.Position != -1)
            ant.RemainingEnergy -= attraction.Distance(Routine::Attractions[ant.Position]);
        ant.RemainingMoney -= attraction.getPrice();
        ant.Data.push_back(stick);
        ant.Position = stick;
        ant.Time += attraction.getPlayTime();
    }
}
Routine BestRoutine(double money, double energy){
    int s = (int)Routine::Attractions.size();
    auto* ants = new Routine[s * 2];
    ban = new bool[s];
    base_pheromone = new double[s];
    pheromone = new double[s * s];
    for(int x = 0;x < s;x++){
        for(int y = 0;y < s;y++){
            pheromone[x * s + y] = 1;
            base_pheromone[x] = 1;
        }
    }

    srand(time(nullptr));
    for(int i = 1;i <= LoopCnt;i++){
        for(int j = 0;j < s * 2;j++){
            GoAnt(ants[j], s, money, energy);
        }
        UpdatePheromone(ants, s);
    }
    Routine ret;
    double max = -1;
    int p = 0;
    for(int i = 0;i < s;i++){
        if (base_pheromone[i] > max){
            max = base_pheromone[i];
            p = i;
        }
    }
    GoAnt(ret, s, money, energy, p);
    delete[] pheromone;
    delete[] ants;
    delete[] ban;
    delete[] base_pheromone;
    return ret;
}

Ui::MainWindow* MainWindow::UI = nullptr;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    MainWindow::UI = ui;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_calculateBtn_clicked()
{
    Routine ret;
    QString warning = "";
    double money, energy;
    bool ok;
    money = ui->money->toPlainText().toDouble(&ok);
    if (!ok){
        warning = " *请输入正确的预算金额";
        goto last;
    }
    energy = ui->energy->toPlainText().toDouble(&ok);
    if (!ok){
        warning = " *请输入正确的最长路程";
        goto last;
    }
    LoopCnt = ui->loopCnt->toPlainText().toInt(&ok);
    if (!ok || LoopCnt <= 0 || LoopCnt >= 10000){
        warning = " *请输入正确的迭代次数";
        goto last;
    }
    ret = BestRoutine(money, energy);
    Routine::CurrentResult = ret;
    ui->displayer->repaint();
last:
    ui->warning->setText(warning);
}

