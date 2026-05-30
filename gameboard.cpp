#include "gameboard.h"
#include "cellbutton.h"
#include <QRandomGenerator>
#include <QQueue>

GameBoard::GameBoard(QObject* parent) : QObject(parent) {}

void GameBoard::setup(int rows, int cols, int mines) {
    m_rows = rows;
    m_cols = cols;
    m_mines = qMin(mines, rows * cols - 1);
    m_flagsPlaced = 0;
    m_hasLaidMines = false;
}

void GameBoard::bindCells(const QVector<QVector<CellButton*>>& cells) {
    m_cells = cells;
    for (int r = 0; r < m_rows; ++r) {
        for (int c = 0; c < m_cols; ++c) {
            auto* cell = m_cells[r][c];
            connect(cell, &CellButton::leftClicked, this, &GameBoard::onLeftClick);
            connect(cell, &CellButton::rightClicked, this, &GameBoard::onRightClick);
        }
    }
}

void GameBoard::resetRevealsAndFlags() {
    m_flagsPlaced = 0;
    emit flagsChanged(m_flagsPlaced);
    for (int r = 0; r < m_rows; ++r) {
        for (int c = 0; c < m_cols; ++c) {
            auto* cell = m_cells[r][c];
            cell->setMine(false);
            cell->setFlagged(false);
            cell->setAdjacentMines(0);
            cell->setRevealed(false);
            cell->setEnabled(true);
        }
    }
    m_hasLaidMines = false;
}

bool GameBoard::inBounds(int r, int c) const {
    return r >= 0 && r < m_rows && c >= 0 && c < m_cols;
}

void GameBoard::randomizeMines(int firstClickRow, int firstClickCol) {
    QVector<int> indices;
    indices.reserve(m_rows * m_cols);
    for (int i = 0; i < m_rows * m_cols; ++i) indices.push_back(i);

    auto idx = [this](int r, int c) { return r * m_cols + c; };

    QVector<bool> excluded(m_rows * m_cols, false);
    for (int dr = -1; dr <= 1; ++dr) {
        for (int dc = -1; dc <= 1; ++dc) {
            int rr = firstClickRow + dr, cc = firstClickCol + dc;
            if (inBounds(rr, cc)) excluded[idx(rr, cc)] = true;
        }
    }

    int placed = 0;
    while (placed < m_mines) {
        int r = QRandomGenerator::global()->bounded(m_rows);
        int c = QRandomGenerator::global()->bounded(m_cols);
        auto* cell = m_cells[r][c];
        if (cell->isMine()) continue;
        if (excluded[idx(r,c)]) continue;
        cell->setMine(true);
        placed++;
    }

    for (int r = 0; r < m_rows; ++r) {
        for (int c = 0; c < m_cols; ++c) {
            if (m_cells[r][c]->isMine()) {
                m_cells[r][c]->setAdjacentMines(0);
                continue;
            }
            int count = countAdjacentMines(r, c);
            m_cells[r][c]->setAdjacentMines(count);
        }
    }

    m_hasLaidMines = true;
}

int GameBoard::countAdjacentMines(int r, int c) const {
    int cnt = 0;
    for (int dr = -1; dr <= 1; ++dr) {
        for (int dc = -1; dc <= 1; ++dc) {
            if (dr == 0 && dc == 0) continue;
            int rr = r + dr, cc = c + dc;
            if (inBounds(rr, cc) && m_cells[rr][cc]->isMine()) cnt++;
        }
    }
    return cnt;
}

void GameBoard::onLeftClick(int r, int c) {
    auto* cell = m_cells[r][c];
    if (cell->isRevealed() || cell->isFlagged()) return;

    if (!m_hasLaidMines) {
        randomizeMines(r, c);
        emit firstRevealHappened();
    }

    if (cell->isMine()) {
        for (int rr = 0; rr < m_rows; ++rr)
            for (int cc = 0; cc < m_cols; ++cc)
                if (m_cells[rr][cc]->isMine())
                    m_cells[rr][cc]->setRevealed(true);
        emit mineTriggered();
        return;
    }

    revealCell(r, c);

    if (checkWin()) {
        emit allSafeRevealed();
    }
}

void GameBoard::onRightClick(int r, int c) {
    auto* cell = m_cells[r][c];
    if (cell->isRevealed()) return;

    cell->setFlagged(!cell->isFlagged());
    m_flagsPlaced += cell->isFlagged() ? 1 : -1;
    emit flagsChanged(m_flagsPlaced);
}

void GameBoard::revealCell(int r, int c) {
    auto* cell = m_cells[r][c];
    if (cell->isRevealed() || cell->isFlagged()) return;
    cell->setRevealed(true);
    emit revealedSafeCell();

    if (cell->adjacentMines() == 0) {
        floodReveal(r, c);
    }
}

void GameBoard::floodReveal(int r, int c) {
    QQueue<QPair<int,int>> q;
    q.enqueue({r,c});

    while (!q.isEmpty()) {
        auto [rr, cc] = q.dequeue();
        for (int dr = -1; dr <= 1; ++dr) {
            for (int dc = -1; dc <= 1; ++dc) {
                int nr = rr + dr, nc = cc + dc;
                if (!inBounds(nr, nc)) continue;
                auto* ncell = m_cells[nr][nc];
                if (ncell->isRevealed() || ncell->isFlagged()) continue;
                if (ncell->isMine()) continue;

                ncell->setRevealed(true);
                emit revealedSafeCell();

                if (ncell->adjacentMines() == 0) {
                    q.enqueue({nr, nc});
                }
            }
        }
    }
}

bool GameBoard::checkWin() const {
    int revealed = 0;
    for (int r = 0; r < m_rows; ++r)
        for (int c = 0; c < m_cols; ++c)
            if (m_cells[r][c]->isRevealed() && !m_cells[r][c]->isMine())
                ++revealed;

    int totalSafe = m_rows * m_cols - m_mines;
    return revealed == totalSafe;
}
