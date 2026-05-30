#ifndef CELLBUTTON_H
#define CELLBUTTON_H

#include <QPushButton>

class CellButton : public QPushButton {
    Q_OBJECT
public:
    explicit CellButton(int r, int c, QWidget* parent = nullptr);

    int row() const { return m_row; }
    int col() const { return m_col; }

    bool isMine() const { return m_isMine; }
    void setMine(bool v) { m_isMine = v; }

    bool isRevealed() const { return m_isRevealed; }
    void setRevealed(bool v);

    bool isFlagged() const { return m_isFlagged; }
    void setFlagged(bool v);

    int adjacentMines() const { return m_adjacentMines; }
    void setAdjacentMines(int n) { m_adjacentMines = n; }

signals:
    void leftClicked(int r, int c);
    void rightClicked(int r, int c);

protected:
    void mousePressEvent(QMouseEvent* e) override;

private:
    void updateAppearance();

    int  m_row;
    int  m_col;
    bool m_isMine = false;
    bool m_isRevealed = false;
    bool m_isFlagged = false;
    int  m_adjacentMines = 0;
};

#endif
