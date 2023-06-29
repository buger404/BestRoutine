
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "hw03.h"
#include "time.h"

Time endTime(19, 0);
int LoopCnt = 100;
double* pheromone;
double* base_pheromone;
bool* ban;
void UpdatePheromone(Routine* ants, int& s, Attraction* des){
    for(int i = 0;i < s * 2;i++){
        if (ants[i].Data.empty())
            continue;
        if (&Routine::Attractions[ants[i].Data[ants[i].Data.size() - 1]] != des)
            continue;
        double f = ants[i].QualityCoefficient() / (s * 2.0) * 100.0;
        for(int j = 0;j < ants[i].Data.size() - 1;j++){
            pheromone[ants[i].Data[j] * s + ants[i].Data[j + 1]] += f;
        }
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

        if (earliest != -1 && start == -1)
            goto skipDrawing;

        if (sum == 0)
        {
            if (start != -1){
                stick = start;
                ant.Time = Routine::Attractions[stick].getOpenTime();
            }
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
Routine BestRoutine(double money, double energy, Attraction* start, Attraction* des){
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
    int first = -1;
    for(int i = 0;i < s;i++)
        if (&Routine::Attractions[i] == start)
            first = i;

    srand(time(nullptr));
    for(int i = 1;i <= LoopCnt;i++){
        for(int j = 0;j < s * 2;j++){
            GoAnt(ants[j], s, money, energy, first);
        }
        UpdatePheromone(ants, s, des);
    }
    Routine ret;
    int fail = 0;
    while((ret.Data.empty() || &Routine::Attractions[ret.Data[ret.Data.size() - 1]] != des) && fail < 5){
        GoAnt(ret, s, money, energy, first);
        fail++;
    }
    if (&Routine::Attractions[ret.Data[ret.Data.size() - 1]] != des)
        ret.Data.clear();
    delete[] pheromone;
    delete[] ants;
    delete[] ban;
    delete[] base_pheromone;
    return ret;
}

Ui::MainWindow* MainWindow::UI = nullptr;

bool setMode = false;

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
    if (!ok || money <= 0){
        warning = " *请输入正确的预算金额";
        goto last;
    }
    energy = ui->energy->toPlainText().toDouble(&ok);
    if (!ok || energy <= 0){
        warning = " *请输入正确的最长路程";
        goto last;
    }
    LoopCnt = ui->loopCnt->toPlainText().toInt(&ok);
    if (!ok || LoopCnt <= 0 || LoopCnt >= 10000){
        warning = " *请输入正确的迭代次数";
        goto last;
    }
    if (Attraction::Start == nullptr){
        warning = " *未设置起点";
        goto last;
    }
    if (Attraction::End == nullptr){
        warning = " *未设置终点";
        goto last;
    }
    ret = BestRoutine(money, energy, Attraction::Start, Attraction::End);
    Routine::CurrentResult = ret;
    if (ret.Data.empty())
    {
        ui->summary->setText("无法规划路线!");
    }
    else
    {
        ui->summary->setText(QString::asprintf("已规划路线，质量: %g, 得分: %g, 消耗金钱: %g, 共计路程: %g",
                                               ret.QualityCoefficient(), ret.TotalScore(),
                                               ret.MaxMoney - ret.RemainingMoney,
                                               ret.MaxEnergy - ret.RemainingEnergy));
    }
    ui->displayer->repaint();
last:
    ui->warning->setText(warning);
}


void MainWindow::on_startSetBtn_clicked()
{
    auto ptr = Attraction::Selected;
    if (Attraction::Start == ptr)
        Attraction::Start = nullptr;
    if (Attraction::End == ptr)
        Attraction::End = nullptr;
    Attraction::Start = ptr;
    ui->displayer->repaint();
}

void MainWindow::on_endSetBtn_clicked()
{
    auto ptr = Attraction::Selected;
    if (Attraction::Start == ptr)
        Attraction::Start = nullptr;
    if (Attraction::End == ptr)
        Attraction::End = nullptr;
    Attraction::End = ptr;
    ui->displayer->repaint();
}

struct UIFactors{
    double x, y, w, h, f;
};
int orWidth, orHeight;
map<QWidget*, UIFactors> uiFactors;

void SetUIFactor(QWidget* w){
    UIFactors factor;
    factor.x = w->pos().x() * 1.0 / orWidth;
    factor.y = w->pos().y() * 1.0 / orHeight;
    factor.w = w->size().width() * 1.0 / orWidth;
    factor.h = w->size().height() * 1.0 / orHeight;
    factor.f = w->font().pointSizeF() * 1.0 / orHeight;
    uiFactors[w] = factor;
    for(auto &u:w->children()){
        if (!u->isWidgetType())
            continue;
        SetUIFactor((QWidget*)u);
    }
}

void ScaleByFactor(QWidget* w, double wFactor, double hFactor){
    UIFactors factor = uiFactors[w];
    w->move(QPoint(factor.x * wFactor, factor.y * hFactor));
    w->resize(QSize(factor.w * wFactor, factor.h * hFactor));
    QFont f = w->font();
    f.setPointSizeF(factor.f * hFactor);
    w->setFont(f);
    for(auto &u:w->children()){
        if (!u->isWidgetType())
            continue;
        ScaleByFactor((QWidget*)u, wFactor, hFactor);
    }
}

bool MainWindow::event(QEvent *e){
    if(e->type() == QEvent::Resize)
    {
        QResizeEvent *args = static_cast<QResizeEvent*>(e);
        if (args->oldSize().isEmpty()){
            orWidth = args->size().width();
            orHeight = args->size().height();
            SetUIFactor(ui->centralwidget);
            goto last;
        }

        double wFactor = args->size().width() * 1.0;
        double hFactor = args->size().height() * 1.0;
        ScaleByFactor(ui->centralwidget, wFactor, hFactor);
    }
    last:
    return QMainWindow::event(e);
}
