#ifndef BOARDWIDGET_H
#define BOARDWIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QVector>

class Game;

class BoardWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BoardWidget(Game* game, QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    Game* m_game;
    QVector<QPoint> m_tilePositions;
    QPixmap m_boardImage;
    bool loadBoardImage();
    void setupTilePositions();
};

#endif // BOARDWIDGET_H
