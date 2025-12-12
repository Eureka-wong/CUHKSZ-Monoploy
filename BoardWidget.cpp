#include "BoardWidget.h"
#include "Game.h"
#include "Player.h"
#include <QPainter>
#include <QDebug>

BoardWidget::BoardWidget(Game* game, QWidget* parent)
    : QWidget(parent), m_game(game)
{
    qDebug() << "=== BoardWidget构造函数开始 ===";
    qDebug() << "传入的game指针:" << m_game;

    // call the method and upload the board
    loadBoardImage();

    // locate the corresponding position for each tile in the picture
    setupTilePositions();

    qDebug() << "=== BoardWidget构造函数完成 ===";
}

bool BoardWidget::loadBoardImage()
{

    if (m_boardImage.load(":/images/board.png")) {
        qDebug() << "从资源文件加载棋盘图片成功";
        qDebug() << "原始图片尺寸:" << m_boardImage.size();


        setFixedSize(m_boardImage.size());
        return true;
    }
    qDebug() << "从资源文件加载失败: :/images/board.png";
    return false;
}

void BoardWidget::paintEvent(QPaintEvent* event)
{
    qDebug() << ">>> paintEvent开始";

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if (!m_boardImage.isNull()) {
        painter.drawPixmap(0, 0, m_boardImage);
    } else {
        painter.fillRect(rect(), QColor(200, 230, 200));
    }


    if (m_game && m_game->getPlayerCount() > 0) {
        QColor playerColors[] = {Qt::red, Qt::blue, Qt::green, Qt::yellow};

        for (int i = 0; i < m_game->getPlayerCount(); ++i) {
            const Player& player = m_game->getPlayer(i);
            int position = player.getPosition();

            if (position < 0 || position >= m_tilePositions.size()) {
                qDebug() << "玩家" << i << "位置无效:" << position;
                continue;
            }

            QPoint center = m_tilePositions[position]; 

            int playersPerRow = 2;
            int row = i / playersPerRow;
            int col = i % playersPerRow;

            QPoint offset(col * 20 - 10, row * 20 - 10);

            painter.setBrush(playerColors[i % 4]);
            painter.setPen(Qt::black); 
            painter.drawEllipse(center + offset, 12, 12); 

            painter.setPen(Qt::white);
            painter.setFont(QFont("Arial", 8, QFont::Bold));
            painter.drawText(center + offset - QPoint(3, -3), QString::number(i + 1));
        }
    }

    qDebug() << "<<< paintEvent完成";
}

void BoardWidget::setupTilePositions()
{
    m_tilePositions.resize(40); 

    const int BOARD_SIZE = 800;
    const int CORNER_SIZE = 130;
    const int SIDE_WIDTH = 60;
    const int SIDE_HEIGHT = 130;


    const int SIDE_LENGTH = BOARD_SIZE - 2 * CORNER_SIZE; // 540
    const int HORIZONTAL_TILES = 9; 
    const int VERTICAL_TILES = 9;

    const int HORIZONTAL_SPACING = SIDE_LENGTH / HORIZONTAL_TILES; // 60
    const int VERTICAL_SPACING = SIDE_LENGTH / VERTICAL_TILES; // 60

    qDebug() << "棋盘设置 - 尺寸:" << BOARD_SIZE << "角落:" << CORNER_SIZE
             << "侧边格子:" << SIDE_WIDTH << "x" << SIDE_HEIGHT;


    m_tilePositions[0] = QPoint(BOARD_SIZE - CORNER_SIZE/2, BOARD_SIZE - CORNER_SIZE/2);

    for (int i = 1; i <= 9; ++i) {
        int x = BOARD_SIZE - CORNER_SIZE - (i * HORIZONTAL_SPACING) + HORIZONTAL_SPACING/2;
        int y = BOARD_SIZE - SIDE_HEIGHT/2;
        m_tilePositions[i] = QPoint(x, y);
    }

    m_tilePositions[10] = QPoint(CORNER_SIZE/2, BOARD_SIZE - CORNER_SIZE/2);

    for (int i = 11; i <= 19; ++i) {
        int x = SIDE_HEIGHT/2;
        int y = BOARD_SIZE - CORNER_SIZE - ((i-10) * VERTICAL_SPACING) + VERTICAL_SPACING/2;
        m_tilePositions[i] = QPoint(x, y);
    }

    m_tilePositions[20] = QPoint(CORNER_SIZE/2, CORNER_SIZE/2);

    for (int i = 21; i <= 29; ++i) {
        int x = CORNER_SIZE + ((i-20) * HORIZONTAL_SPACING) - HORIZONTAL_SPACING/2;
        int y = SIDE_HEIGHT/2;
        m_tilePositions[i] = QPoint(x, y);
    }

    m_tilePositions[30] = QPoint(BOARD_SIZE - CORNER_SIZE/2, CORNER_SIZE/2);

    for (int i = 31; i <= 39; ++i) {
        int x = BOARD_SIZE - SIDE_HEIGHT/2;
        int y = CORNER_SIZE + ((i-30) * VERTICAL_SPACING) - VERTICAL_SPACING/2;
        m_tilePositions[i] = QPoint(x, y);
    }

    qDebug() << "格子位置设置完成:";
    for (int i = 0; i < 5; ++i) {
        qDebug() << "位置" << i << ":" << m_tilePositions[i];
    }
    qDebug() << "位置 10:" << m_tilePositions[10];
    qDebug() << "位置 20:" << m_tilePositions[20];
    qDebug() << "位置 30:" << m_tilePositions[30];
}
