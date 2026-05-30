#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <QObject>
#include <QVector>

class CellButton;

struct BoardSize {
    int rows;
    int cols;
    int mines;
};

class GameBoard : public QObject {
    Q_OBJECT
public:
    explicit GameBoard(QObject* parent = nullptr);

    void setup(int rows, int cols, int mines);
    void bindCells(const QVector<QVector<CellButton*>>& cells);

    int rows() const { return m_rows; }
    int cols() const { return m_cols; }
    int totalMines() const { return m_mines; }
    int flagsPlaced() const { return m_flagsPlaced; }

    void resetRevealsAndFlags();
    void randomizeMines(int firstClickRow, int firstClickCol);

signals:
    void revealedSafeCell();
    void mineTriggered();
    void allSafeRevealed(); // win
    void flagsChanged(int flags);
    void firstRevealHappened();

public slots:
    void onLeftClick(int r, int c);
    void onRightClick(int r, int c);

private:
    bool inBounds(int r, int c) const;
    int countAdjacentMines(int r, int c) const;
    void revealCell(int r, int c);
    void floodReveal(int r, int c);
    bool checkWin() const;

    int m_rows = 0;
    int m_cols = 0;
    int m_mines = 0;
    int m_flagsPlaced = 0;
    bool m_hasLaidMines = false;

    QVector<QVector<CellButton*>> m_cells;
};

#endif
