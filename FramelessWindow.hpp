/*******************************************************************************
 * Copyright (C) 2020 - 2023, winsoft666, <winsoft666@outlook.com>.
 *
 * THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
 * EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Expect bugs
 *
 * Please use and enjoy. Please let me know of any bugs/improvements
 * that you have found/implemented and I will fix/incorporate them into this
 * file.
 *******************************************************************************/

#ifndef FRAMELESS_WINDOW_HPP_
#define FRAMELESS_WINDOW_HPP_
#pragma once

#include <QString>
#include <QFile>
#include <QWidget>
#include <QScreen>
#include <QMouseEvent>
#include <QApplication>
#include <QWindow>
#include <QDebug>

template <typename T>
class FramelessWindow : public T {
public:
    enum class Direction {
        UP = 0,
        DOWN = 1,
        LEFT,
        RIGHT,
        LEFTTOP,
        LEFTBOTTOM,
        RIGHTBOTTOM,
        RIGHTTOP,
        NONE
    };

    FramelessWindow(bool translucentBackground, QWidget* parent = Q_NULLPTR, bool ignoreReturn = false) :
        T(parent),
        m_ignoreReturn(ignoreReturn),
        m_bLeftPressed(false),
        m_bResizeable(false),
        m_bUseSystemMove(true),
        m_bCurUseSystemMove(false),
        m_bNearToScreenEdge(false),
        m_bDisableTitlebarDoubleClick(false),
        m_Direction(Direction::NONE),
        m_iResizeRegionPadding(4) {
        m_iResizeRegionPadding = m_iResizeRegionPadding * this->devicePixelRatioF();
        this->installEventFilter(this);

        Qt::WindowFlags flags = this->windowFlags();
        this->setWindowFlags(flags | Qt::FramelessWindowHint);

        if (translucentBackground) {
            this->setAttribute(Qt::WA_TranslucentBackground);
        }
    }

    ~FramelessWindow() = default;

public:
    void setTitlebar(QVector<QWidget*> titleBar) {
        m_titlebarWidget = titleBar;
    }

    void setResizeable(bool b, const QMargins& transparentMargins) {
        m_bResizeable = b;
        m_transparentMargsins = transparentMargins;
    }

    bool resizeable() const {
        return m_bResizeable;
    }

    void setResizeRegionPadding(int padding) {
        m_iResizeRegionPadding = padding * this->devicePixelRatioF();
    }

    int resizeRegionPadding() const {
        return m_iResizeRegionPadding / this->devicePixelRatioF();
    }

    void setNearToScreenEdge(bool yes) {
        m_bNearToScreenEdge = yes;
    }

    bool nearToScreenEdge() const {
        return m_bNearToScreenEdge;
    }

    QMargins transparentMargins() const {
        return m_transparentMargsins;
    }

    bool useSystemMove() const {
        return m_bUseSystemMove;
    }

    void setUseSystemMove(bool b) {
        m_bUseSystemMove = b;
    }

    bool titlebarDoubleClickDisabled() const {
        return m_bDisableTitlebarDoubleClick;
    }

    void setDisableTitlebarDoubleClicked(bool b) {
        m_bDisableTitlebarDoubleClick = b;
    }

    void setAllWidgetMouseTracking(QWidget* widget) {
        if (!widget)
            return;

        widget->setMouseTracking(true);
        QObjectList list = widget->children();
        foreach (QObject* obj, list) {
            QWidget* w = dynamic_cast<QWidget*>(obj);
            if (w) {
                setAllWidgetMouseTracking(w);
            }
        }
    }

protected:
    virtual bool eventFilter(QObject* target, QEvent* event) {
        if (event->type() == QEvent::Paint) {
            static bool first = false;
            if (!first) {
                first = true;
                setAllWidgetMouseTracking(this);
            }
        }
        else if (event->type() == QEvent::KeyPress) {
            if (m_ignoreReturn) {
                QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
                if (keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return)
                    return true;  // mark the event as handled
            }
        }
        else if (event->type() == QEvent::Move) {
            if (m_bLeftPressed) {
                onManualMoving(this->geometry().topLeft());
            }

            if (m_bNearToScreenEdge) {
                const int NEAR_SPACE = 20;
                QRect curGeo = geometry();

                bool isNearEdge = false;
                QPoint nearToPoint;
                QList<QScreen*> screens = QGuiApplication::screens();
                for (int i = 0; i < screens.size(); i++) {
                    QRect geo = screens[i]->geometry();
                    if (qAbs(geo.left() - curGeo.left()) <= NEAR_SPACE) {
                        isNearEdge = true;
                        nearToPoint = QPoint(geo.left(), curGeo.top());
                        break;
                    }
                    else if (qAbs(geo.top() - curGeo.top()) <= NEAR_SPACE) {
                        isNearEdge = true;
                        nearToPoint = QPoint(curGeo.left(), geo.top());
                        break;
                    }
                    else if (qAbs(geo.right() - curGeo.right()) <= NEAR_SPACE) {
                        isNearEdge = true;
                        nearToPoint = QPoint(geo.right() - curGeo.width(), curGeo.top());
                        break;
                    }
                    else if (qAbs(geo.bottom() - curGeo.bottom()) <= NEAR_SPACE) {
                        isNearEdge = true;
                        nearToPoint = QPoint(curGeo.left(), geo.bottom() - curGeo.height());
                        break;
                    }
                }

                if (isNearEdge) {
                    move(nearToPoint);
                    return true;
                }
            }
        }
        else if (event->type() == QEvent::MouseMove) {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            handleMouseMoveEvent(mouseEvent);
        }
        else if (event->type() == QEvent::MouseButtonPress) {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            handleMousePressEvent(mouseEvent);
        }
        else if (event->type() == QEvent::MouseButtonRelease) {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            handleMouseReleaseEvent(mouseEvent);
        }
        else if (event->type() == QEvent::MouseButtonDblClick) {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            handleMouseDoubleClickEvent(mouseEvent);
        }
        return T::eventFilter(target, event);
    }

    void handleMouseDoubleClickEvent(QMouseEvent* event) {
        if (m_bResizeable && !m_bDisableTitlebarDoubleClick) {
            QWidget* actionWidget = QApplication::widgetAt(event->globalPos());
            if (actionWidget) {
                bool inTitlebar = false;
                for (auto& item : m_titlebarWidget) {
                    if (actionWidget == item) {
                        inTitlebar = true;
                        break;
                    }
                }
                if (inTitlebar) {
                    if (this->isMaximized()) {
                        this->showNormal();
                    }
                    else {
                        this->showMaximized();
                    }
                }
            }
        }
    }

    void handleMousePressEvent(QMouseEvent* event) {
        m_MousePressedGeo = this->geometry();
        if (event->button() == Qt::LeftButton) {
            if (m_Direction != Direction::NONE) {
                m_bLeftPressed = true;
                this->mouseGrabber();
            }
            else {
                QWidget* action = QApplication::widgetAt(event->globalPos());
                if (action) {
                    bool inTitlebar = false;
                    for (auto& item : m_titlebarWidget) {
                        if (action == item) {
                            inTitlebar = true;
                            break;
                        }
                    }
                    if (inTitlebar) {
                        m_bLeftPressed = true;
                        m_DragPos = event->globalPos() - this->frameGeometry().topLeft();
                        if (m_bUseSystemMove) {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 15, 0))
                            m_bCurUseSystemMove = this->windowHandle()->startSystemMove();
                            Q_ASSERT_X(m_bCurUseSystemMove, "mousePressEvent()",
                                       "this->windowHandle()->startSystemMove() failed");
#endif
                        }
                    }
                }
            }
        }
    }

    void handleMouseMoveEvent(QMouseEvent* event) {
        QPoint globalPoint = event->globalPos();
        if (m_bLeftPressed) {
            bool bIgnore = true;
            QList<QScreen*> screens = QGuiApplication::screens();
            for (int i = 0; i < screens.size(); i++) {
                QScreen* pScreen = screens[i];
                QRect geometryRect = pScreen->availableGeometry();
                if (geometryRect.contains(globalPoint)) {
                    bIgnore = false;
                    break;
                }
            }

            if (bIgnore) {
                event->ignore();
                return;
            }

            if (m_Direction != Direction::NONE) {
                QRect rect = this->rect();
                QPoint tl = this->mapToGlobal(rect.topLeft());
                QPoint rb = this->mapToGlobal(rect.bottomRight());

                QRect rMove(tl, rb);

                switch (m_Direction) {
                case Direction::LEFT:
                    if (rb.x() - globalPoint.x() <= this->minimumWidth())
                        rMove.setX(tl.x());
                    else
                        rMove.setX(globalPoint.x());
                    break;
                case Direction::RIGHT:
                    rMove.setWidth(globalPoint.x() - tl.x());
                    break;
                case Direction::UP:
                    if (rb.y() - globalPoint.y() <= this->minimumHeight())
                        rMove.setY(tl.y());
                    else
                        rMove.setY(globalPoint.y());
                    break;
                case Direction::DOWN:
                    rMove.setHeight(globalPoint.y() - tl.y());
                    break;
                case Direction::LEFTTOP:
                    if (rb.x() - globalPoint.x() <= this->minimumWidth())
                        rMove.setX(tl.x());
                    else
                        rMove.setX(globalPoint.x());
                    if (rb.y() - globalPoint.y() <= this->minimumHeight())
                        rMove.setY(tl.y());
                    else
                        rMove.setY(globalPoint.y());
                    break;
                case Direction::RIGHTTOP:
                    rMove.setWidth(globalPoint.x() - tl.x());
                    rMove.setY(globalPoint.y());
                    break;
                case Direction::LEFTBOTTOM:
                    rMove.setX(globalPoint.x());
                    rMove.setHeight(globalPoint.y() - tl.y());
                    break;
                case Direction::RIGHTBOTTOM:
                    rMove.setWidth(globalPoint.x() - tl.x());
                    rMove.setHeight(globalPoint.y() - tl.y());
                    break;
                default:
                    break;
                }
                this->setGeometry(rMove);
                onManualResizing(rMove);
            }
            else {
                QPoint newPoint = event->globalPos() - m_DragPos;
                if (!m_bCurUseSystemMove) {
                    this->move(newPoint);
                }
                event->accept();
            }
        }
        else {
            region(globalPoint);
        }
    }

    void region(const QPoint& cursorGlobalPoint) {
        if (!m_bResizeable)
            return;

        QRect rect = this->contentsRect();
        rect.setLeft(rect.left() + m_transparentMargsins.left());
        rect.setTop(rect.top() + m_transparentMargsins.top());
        rect.setRight(rect.right() - m_transparentMargsins.right());
        rect.setBottom(rect.bottom() - m_transparentMargsins.bottom());

        QPoint tl = this->mapToGlobal(rect.topLeft());
        QPoint rb = this->mapToGlobal(rect.bottomRight());
        int x = cursorGlobalPoint.x();
        int y = cursorGlobalPoint.y();

        if (tl.x() + m_iResizeRegionPadding >= x && tl.x() <= x &&
            tl.y() + m_iResizeRegionPadding >= y && tl.y() <= y) {
            m_Direction = Direction::LEFTTOP;
            this->setCursor(QCursor(Qt::SizeFDiagCursor));
        }
        else if (x >= rb.x() - m_iResizeRegionPadding && x <= rb.x() &&
                 y >= rb.y() - m_iResizeRegionPadding && y <= rb.y()) {
            m_Direction = Direction::RIGHTBOTTOM;
            this->setCursor(QCursor(Qt::SizeFDiagCursor));
        }
        else if (x <= tl.x() + m_iResizeRegionPadding && x >= tl.x() &&
                 y >= rb.y() - m_iResizeRegionPadding && y <= rb.y()) {
            m_Direction = Direction::LEFTBOTTOM;
            this->setCursor(QCursor(Qt::SizeBDiagCursor));
        }
        else if (x <= rb.x() && x >= rb.x() - m_iResizeRegionPadding && y >= tl.y() &&
                 y <= tl.y() + m_iResizeRegionPadding) {
            m_Direction = Direction::RIGHTTOP;
            this->setCursor(QCursor(Qt::SizeBDiagCursor));
        }
        else if (x <= tl.x() + m_iResizeRegionPadding && x >= tl.x()) {
            m_Direction = Direction::LEFT;
            this->setCursor(QCursor(Qt::SizeHorCursor));
        }
        else if (x <= rb.x() && x >= rb.x() - m_iResizeRegionPadding) {
            m_Direction = Direction::RIGHT;
            this->setCursor(QCursor(Qt::SizeHorCursor));
        }
        else if (y >= tl.y() && y <= tl.y() + m_iResizeRegionPadding) {
            m_Direction = Direction::UP;
            this->setCursor(QCursor(Qt::SizeVerCursor));
        }
        else if (y <= rb.y() && y >= rb.y() - m_iResizeRegionPadding) {
            m_Direction = Direction::DOWN;
            this->setCursor(QCursor(Qt::SizeVerCursor));
        }
        else {
            m_Direction = Direction::NONE;
            this->setCursor(QCursor(Qt::ArrowCursor));
        }
    }

    void handleMouseReleaseEvent(QMouseEvent* event) {
        m_bLeftPressed = false;
        if (m_Direction != Direction::NONE) {
            m_Direction = Direction::NONE;
            this->releaseMouse();
            this->setCursor(QCursor(Qt::ArrowCursor));
        }

        QRect curGeo = this->geometry();
        if (m_MousePressedGeo != curGeo) {
            onManualMoved(curGeo.topLeft());
        }
        if (m_MousePressedGeo.size() != curGeo.size()) {
            onManualResized(this->rect());
        }
    }

    virtual void onManualResizing(const QRect& rc) {}
    virtual void onManualResized(const QRect& rc) {}
    virtual void onManualMoving(const QPoint& pt) {}
    virtual void onManualMoved(const QPoint& pt) {}

protected:
    const bool m_ignoreReturn;
    bool m_bLeftPressed;
    bool m_bResizeable;
    bool m_bUseSystemMove;
    bool m_bCurUseSystemMove;
    bool m_bNearToScreenEdge;
    bool m_bDisableTitlebarDoubleClick;
    Direction m_Direction;
    int m_iResizeRegionPadding;
    QPoint m_DragPos;
    QRect m_MousePressedGeo;
    QMargins m_transparentMargsins;
    QVector<QWidget*> m_titlebarWidget;
};
#endif
