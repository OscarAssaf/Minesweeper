#include "mainwindow.h"
#include "gameboard.h"
#include "cellbutton.h"
#include <QComboBox>
#include <QAction>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QLabel>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QTimer>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    buildUi();
    buildMenu();
    createBoard(R, C, M);
}

MainWindow::~MainWindow() {}

void MainWindow::buildUi() {
    central = new QWidget(this);
    setCentralWidget(central);

    auto* v = new QVBoxLayout;
    central->setLayout(v);

    // top bar UI
    auto* top = new QWidget(this);
    auto* topV = new QVBoxLayout;
    topV->setAlignment(Qt::AlignCenter);
    top->setLayout(topV);

    // difficulty
    difficultyBox = new QComboBox(this); //dropdown
    difficultyBox->addItem("Easy");
    difficultyBox->addItem("Medium");
    difficultyBox->addItem("Hard");
    difficultyBox->addItem("Custom");
    connect(difficultyBox, &QComboBox::currentTextChanged, this, &MainWindow::onDifficultyChanged);

    auto* diffRow = new QHBoxLayout;
    diffRow->addStretch();
    diffRow->addWidget(difficultyBox);
    diffRow->addStretch();
    topV->addLayout(diffRow);

    // flags, restart, time taken
    minesLabel = new QLabel("Flags: 0", this);
    timeLabel = new QLabel("Time: 0", this);
    resetButton = new QPushButton(QString::fromUtf8("🙂"), this);
    resetButton->setFixedWidth(48);
    resetButton->setToolTip("New game");
    connect(resetButton, &QPushButton::clicked, this, &MainWindow::resetGame);

    auto* statusRow = new QHBoxLayout;
    statusRow->setAlignment(Qt::AlignCenter);
    statusRow->addWidget(minesLabel);
    statusRow->addSpacing(20);
    statusRow->addWidget(resetButton);
    statusRow->addSpacing(20);
    statusRow->addWidget(timeLabel);
    topV->addLayout(statusRow);

    v->addWidget(top);

    // grid
    gridHolder = new QWidget(this);
    gridLayout = new QGridLayout;
    gridLayout->setSpacing(1);
    gridHolder->setLayout(gridLayout);
    v->addWidget(gridHolder);

    // timer
    timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer, &QTimer::timeout, [this](){
        elapsed++;
        timeLabel->setText(QString("Time: %1").arg(elapsed));
    });

    setWindowTitle("MinesweeperQt");
    resize(420, 520);
}

void MainWindow::buildMenu() {
    auto* game = menuBar()->addMenu("&Game");

    auto* easy = new QAction("New (Beginner 9x9, 10 mines)", this);
    auto* med  = new QAction("New (Intermediate 16x16, 40 mines)", this);
    auto* hard = new QAction("New (Expert 16x30, 99 mines)", this);
    auto* custom = new QAction("New (Custom…)", this);
    auto* reset = new QAction("Reset", this);

    connect(easy,   &QAction::triggered, this, &MainWindow::newGameEasy);
    connect(med,    &QAction::triggered, this, &MainWindow::newGameMedium);
    connect(hard,   &QAction::triggered, this, &MainWindow::newGameHard);
    connect(custom, &QAction::triggered, this, &MainWindow::newGameCustom);
    connect(reset,  &QAction::triggered, this, &MainWindow::resetGame);

    game->addAction(easy);
    game->addAction(med);
    game->addAction(hard);
    game->addSeparator();
    game->addAction(custom);
    game->addSeparator();
    game->addAction(reset);
}

void MainWindow::createBoard(int rows, int cols, int mines) {
    R = rows; C = cols; M = mines;

    if (board) {
        board->deleteLater();
        board = nullptr;
    }
    board = new GameBoard(this);
    board->setup(R, C, M);

    // remove old cells
    if (!cells.isEmpty()) {
        for (auto& row : cells) for (auto* b : row) b->deleteLater();
    }
    cells.clear();
    cells.resize(R);
    for (int r = 0; r < R; ++r) cells[r].resize(C);

    rebuildGrid();

    board->bindCells(cells);
    connect(board, &GameBoard::mineTriggered, this, &MainWindow::onMineTriggered);
    connect(board, &GameBoard::allSafeRevealed, this, &MainWindow::onWin);
    connect(board, &GameBoard::flagsChanged, this, &MainWindow::onFlagsChanged);
    connect(board, &GameBoard::firstRevealHappened, this, &MainWindow::onFirstReveal);
    connect(board, &GameBoard::revealedSafeCell, this, &MainWindow::onSafeReveal);

    minesLabel->setText(QString("Mines: %1  Flags: 0").arg(M));
    timeLabel->setText("Time: 0");
    elapsed = 0;
    timer->stop();
    resetButton->setText(QString::fromUtf8("🙂"));

    // resize windows, works only upwards
    int cellSize = 25;
    int topBar = 100;
    resize(C * cellSize, R * cellSize + topBar);
}


void MainWindow::rebuildGrid() {
    // clear the layout
    QLayoutItem* child;
    while ((child = gridLayout->takeAt(0)) != nullptr) {
        if (child->widget()) child->widget()->setParent(nullptr);
        delete child;
    }

    for (int r = 0; r < R; ++r) {
        for (int c = 0; c < C; ++c) {
            auto* btn = new CellButton(r, c, gridHolder);
            gridLayout->addWidget(btn, r, c);
            cells[r][c] = btn;
        }
    }
}

void MainWindow::resetGame() {
    createBoard(R, C, M);
}

void MainWindow::newGameEasy()   { createBoard(9, 9, 10); }
void MainWindow::newGameMedium() { createBoard(16, 16, 40); }
void MainWindow::newGameHard()   { createBoard(16, 30, 99); }

void MainWindow::newGameCustom() {
    bool ok = false;
    int rows = QInputDialog::getInt(this, "Custom Board", "Rows (5-32):", R, 5, 32, 1, &ok);
    if (!ok) return;
    int cols = QInputDialog::getInt(this, "Custom Board", "Cols (5-48):", C, 5, 48, 1, &ok);
    if (!ok) return;
    int maxMines = rows * cols - 1;
    int mines = QInputDialog::getInt(this, "Custom Board", "Mines:", qMin(M, maxMines), 1, maxMines, 1, &ok);
    if (!ok) return;
    createBoard(rows, cols, mines);
}

void MainWindow::onMineTriggered() {
    timer->stop();
    resetButton->setText(QString::fromUtf8("💥"));
    QMessageBox::information(this, "Boom!", "You hit a mine. Game over!");

    for (int r = 0; r < R; ++r)
        for (int c = 0; c < C; ++c)
            cells[r][c]->setEnabled(false);
}

void MainWindow::onWin() {
    timer->stop();
    resetButton->setText(QString::fromUtf8("😎"));
    QMessageBox::information(this, "You win!", "All safe cells revealed. Nice!");

    for (int r = 0; r < R; ++r)
        for (int c = 0; c < C; ++c)
            cells[r][c]->setEnabled(false);
}

void MainWindow::onFlagsChanged(int flags) {
    minesLabel->setText(QString("Mines: %1  Flags: %2").arg(M).arg(flags));
}

void MainWindow::onFirstReveal() {
    elapsed = 0;
    timeLabel->setText("Time: 0");
    timer->start();
}

void MainWindow::onSafeReveal() {
}


void MainWindow::onDifficultyChanged(const QString& diff) {
    if (diff == "Easy") {
        createBoard(9, 9, 10);
    } else if (diff == "Medium") {
        createBoard(16, 16, 40);
    } else if (diff == "Hard") {
        createBoard(16, 30, 99);
    } else if (diff == "Custom") {
        newGameCustom();
    }
}
