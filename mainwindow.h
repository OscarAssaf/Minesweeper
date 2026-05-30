#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>

class QLabel;
class QPushButton;
class QTimer;
class QGridLayout;
class QComboBox;
class CellButton;
class GameBoard;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void newGameEasy();
    void newGameMedium();
    void newGameHard();
    void newGameCustom();
    void resetGame();

    void onMineTriggered();
    void onWin();
    void onFlagsChanged(int flags);
    void onFirstReveal();
    void onSafeReveal();

    void onDifficultyChanged(const QString& diff);

private:
    void buildUi();
    void buildMenu();
    void createBoard(int rows, int cols, int mines);
    void rebuildGrid();
    void stopGame(bool won);

    QWidget* central = nullptr;
    QLabel* minesLabel = nullptr;
    QLabel* timeLabel = nullptr;
    QPushButton* resetButton = nullptr;
    QComboBox* difficultyBox = nullptr;

    QTimer* timer = nullptr;
    int elapsed = 0;

    QWidget* gridHolder = nullptr;
    QGridLayout* gridLayout = nullptr;

    int R = 9, C = 9, M = 10;
    GameBoard* board = nullptr;
    QVector<QVector<CellButton*>> cells;
};

#endif
