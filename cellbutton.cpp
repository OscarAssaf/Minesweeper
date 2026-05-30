#include "cellbutton.h"
#include <QMouseEvent>
#include <QFont>

CellButton::CellButton(int r, int c, QWidget* parent)
    : QPushButton(parent), m_row(r), m_col(c)
{
    setFixedSize(28, 28);
    QFont f = font();
    f.setBold(true);
    setFont(f);
    setFocusPolicy(Qt::NoFocus);
    updateAppearance();
}

void CellButton::setRevealed(bool v) {
    m_isRevealed = v;
    if (m_isRevealed) setEnabled(false);
    updateAppearance();
}

void CellButton::setFlagged(bool v) {
    m_isFlagged = v;
    updateAppearance();
}

void CellButton::mousePressEvent(QMouseEvent* e) {
    if (e->button() == Qt::RightButton) {
        emit rightClicked(m_row, m_col);
    } else if (e->button() == Qt::LeftButton) {
        emit leftClicked(m_row, m_col);
    }
}

static QString numberColor(int n) {
    switch (n) {
    case 1: return "blue";
    case 2: return "green";
    case 3: return "red";
    case 4: return "navy";
    case 5: return "maroon";
    case 6: return "teal";
    case 7: return "black";
    case 8: return "gray";
    default: return "black";
    }
}

void CellButton::updateAppearance() {
    if (!m_isRevealed) {
        if (m_isFlagged) {
            setText(QString::fromUtf8("⚑"));
            setStyleSheet("QPushButton { background: #d0d0d0; }");
        } else {
            setText("");
            setStyleSheet("QPushButton { background: #d0d0d0; }");
        }
        return;
    }

    //revealed
    if (m_isMine) {
        setText(QString::fromUtf8("✹"));
        setStyleSheet("QPushButton { background: #ffb3b3; }");
    } else {
        if (m_adjacentMines == 0) {
            setText("");
            setStyleSheet("QPushButton { background: #eaeaea; }");
        } else {
            setText(QString::number(m_adjacentMines));
            setStyleSheet(QString("QPushButton { background: #eaeaea; color: %1; }")
                              .arg(numberColor(m_adjacentMines)));
        }
    }
}
