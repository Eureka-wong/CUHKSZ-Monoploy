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
    // 方式1: 从Qt资源系统加载
    if (m_boardImage.load(":/images/board.png")) {
        qDebug() << "从资源文件加载棋盘图片成功";
        qDebug() << "原始图片尺寸:" << m_boardImage.size();

        // // 缩放图片到合适大小
        // m_boardImage = m_boardImage.scaled(800, 800, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        // qDebug() << "缩放后图片尺寸:" << m_boardImage.size();

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

    // 绘制背景
    if (!m_boardImage.isNull()) {
        painter.drawPixmap(0, 0, m_boardImage);
    } else {
        painter.fillRect(rect(), QColor(200, 230, 200));
    }

    // 绘制玩家位置
    if (m_game && m_game->getPlayerCount() > 0) {
        QColor playerColors[] = {Qt::red, Qt::blue, Qt::green, Qt::yellow};

        for (int i = 0; i < m_game->getPlayerCount(); ++i) {
            const Player& player = m_game->getPlayer(i);
            int position = player.getPosition();  // 获取玩家当前位置

            // 安全检查玩家位置是否有效
            if (position < 0 || position >= m_tilePositions.size()) {
                qDebug() << "玩家" << i << "位置无效:" << position;
                continue;
            }

            QPoint center = m_tilePositions[position];  // 获取格子中心坐标

            // 3.1 计算玩家在格子内的偏移（避免重叠）
            int playersPerRow = 2; // 每行最多2个玩家
            int row = i / playersPerRow;  // 计算行号：0,0,1,1 对应4个玩家
            int col = i % playersPerRow;  // 计算列号：0,1,0,1 对应4个玩家

            // 计算偏移量：让玩家在格子内分散显示
            QPoint offset(col * 20 - 10, row * 20 - 10);

            // 3.2 绘制玩家圆圈
            painter.setBrush(playerColors[i % 4]);  // 设置玩家颜色
            painter.setPen(Qt::black);              // 设置边框颜色
            painter.drawEllipse(center + offset, 12, 12);  // 绘制圆形玩家图标

            // 3.3 显示玩家编号（在圆圈内显示数字）
            painter.setPen(Qt::white);              // 设置文字颜色为白色
            painter.setFont(QFont("Arial", 8, QFont::Bold));  // 设置字体
            painter.drawText(center + offset - QPoint(3, -3), QString::number(i + 1));
        }
    }
    // 显示调试信息
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 12));
    qDebug() << "<<< paintEvent完成";
}

void BoardWidget::setupTilePositions()
{
    m_tilePositions.resize(40);  // 大富翁有40个格子

    // 棋盘尺寸
    const int BOARD_SIZE = 800;
    const int CORNER_SIZE = 130;    // 四个角落格子尺寸
    const int SIDE_WIDTH = 60;      // 侧边格子宽度
    const int SIDE_HEIGHT = 130;    // 侧边格子高度

    // 计算可用空间
    const int SIDE_LENGTH = BOARD_SIZE - 2 * CORNER_SIZE; // 540
    const int HORIZONTAL_TILES = 9;  // 每条水平边的格子数 (不包括角落)
    const int VERTICAL_TILES = 9;    // 每条垂直边的格子数 (不包括角落)

    const int HORIZONTAL_SPACING = SIDE_LENGTH / HORIZONTAL_TILES; // 60
    const int VERTICAL_SPACING = SIDE_LENGTH / VERTICAL_TILES; // 60

    qDebug() << "棋盘设置 - 尺寸:" << BOARD_SIZE << "角落:" << CORNER_SIZE
             << "侧边格子:" << SIDE_WIDTH << "x" << SIDE_HEIGHT;

    // 位置 0: GO (右下角)
    m_tilePositions[0] = QPoint(BOARD_SIZE - CORNER_SIZE/2, BOARD_SIZE - CORNER_SIZE/2);

    // 底部行 (位置 1-9: 从右到左)
    for (int i = 1; i <= 9; ++i) {
        int x = BOARD_SIZE - CORNER_SIZE - (i * HORIZONTAL_SPACING) + HORIZONTAL_SPACING/2;
        int y = BOARD_SIZE - SIDE_HEIGHT/2;
        m_tilePositions[i] = QPoint(x, y);
    }

    // 位置 10: Jail/Just Visiting (左下角)
    m_tilePositions[10] = QPoint(CORNER_SIZE/2, BOARD_SIZE - CORNER_SIZE/2);

    // 左边列 (位置 11-19: 从下到上)
    for (int i = 11; i <= 19; ++i) {
        int x = SIDE_HEIGHT/2;
        int y = BOARD_SIZE - CORNER_SIZE - ((i-10) * VERTICAL_SPACING) + VERTICAL_SPACING/2;
        m_tilePositions[i] = QPoint(x, y);
    }

    // 位置 20: Free Parking (左上角)
    m_tilePositions[20] = QPoint(CORNER_SIZE/2, CORNER_SIZE/2);

    // 顶部行 (位置 21-29: 从左到右)
    for (int i = 21; i <= 29; ++i) {
        int x = CORNER_SIZE + ((i-20) * HORIZONTAL_SPACING) - HORIZONTAL_SPACING/2;
        int y = SIDE_HEIGHT/2;
        m_tilePositions[i] = QPoint(x, y);
    }

    // 位置 30: Go To Jail (右上角)
    m_tilePositions[30] = QPoint(BOARD_SIZE - CORNER_SIZE/2, CORNER_SIZE/2);

    // 右边列 (位置 31-39: 从上到下)
    for (int i = 31; i <= 39; ++i) {
        int x = BOARD_SIZE - SIDE_HEIGHT/2;
        int y = CORNER_SIZE + ((i-30) * VERTICAL_SPACING) - VERTICAL_SPACING/2;
        m_tilePositions[i] = QPoint(x, y);
    }
}
