#ifndef FRAMELESS_WINDOW_HPP_
#define FRAMELESS_WINDOW_HPP_
#pragma once

#include <QString>
#include <QFile>
#include <QWidget>
#include <QDebug>
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <windowsx.h>
#include <dwmapi.h>

#pragma comment(lib, "Dwmapi.lib")
#endif

template <typename T>
class FramelessWindow : public T {
 public:
  FramelessWindow::FramelessWindow(bool bTranslucentBackground,
                                   bool bSystemShadow,
                                   bool bResizeable,
                                   QWidget* parent = Q_NULLPTR)
      : T(parent)
      , m_bTranslucentBackground(bTranslucentBackground)
      , m_bSystemShadow(bSystemShadow)
      , m_bResizeable(bResizeable) {

    if (m_bTranslucentBackground) {
      setAttribute(Qt::WA_TranslucentBackground);
    }

    Qt::WindowFlags flags = windowFlags();
    setWindowFlags(flags | Qt::FramelessWindowHint | Qt::Window | Qt::WindowSystemMenuHint);

    setResizeable(m_bResizeable);

    HWND hWnd = (HWND)winId();
    DWORD dwOldStyle = ::GetWindowLong(hWnd, GWL_STYLE);
    DWORD dwNewStyle = dwOldStyle | selectBorderlessStyle();
    
    ::SetWindowLongPtrW(hWnd, GWL_STYLE, static_cast<LONG>(dwNewStyle));

    SetShadow(m_bSystemShadow);
  }

  FramelessWindow::~FramelessWindow() {}

  bool isCompositionEnabled() {
    BOOL composition_enabled = FALSE;
    bool success = ::DwmIsCompositionEnabled(&composition_enabled) == S_OK;
    return composition_enabled && success;
  }

  DWORD selectBorderlessStyle() {
    if (isCompositionEnabled()) {
      return WS_POPUP | WS_THICKFRAME | WS_CAPTION | WS_SYSMENU;
    }
    return WS_POPUP | WS_THICKFRAME | WS_SYSMENU;
  }

  void SetShadow(bool bEnabled) {
    if (isCompositionEnabled()) {
      static const MARGINS shadow_state[2]{{0, 0, 0, 0}, {1, 1, 1, 1}};
      ::DwmExtendFrameIntoClientArea((HWND)winId(), &shadow_state[bEnabled ? 1 : 0]);
    }
  }

  void setTitlebar(QVector<QWidget*> titleBarWidget,
                   QVector<QWidget*> exceptWidgetPointer,
                   QVector<QString> exceptWidgetType) {
    m_titlebarWidget = titleBarWidget;
    m_titlebarExceptWidgetPointer = exceptWidgetPointer;
    m_titleBarExceptWidgetType = exceptWidgetType;
  }

  void setResizeable(bool b) { 
    m_bResizeable = b; 

    if (m_bResizeable) {
      setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint);
    }
    else {
      setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
    }
  }

  bool resizeable() const { return m_bResizeable; }


 protected:
  bool nativeEvent(const QByteArray& eventType, void* message, long* result) {
    MSG* msg = reinterpret_cast<MSG*>(message);

    switch (msg->message) {
      case WM_NCCALCSIZE: {
        //NCCALCSIZE_PARAMS& params = *reinterpret_cast<NCCALCSIZE_PARAMS*>(msg->lParam);
        //if (params.rgrc[0].top != 0)
        //  params.rgrc[0].top -= 1;

        //this kills the window frame and title bar we added with WS_THICKFRAME and WS_CAPTION
        //*result = WVR_REDRAW;
        //if (msg->wParam == TRUE) {
        //  return true;
        //}
        //return false;

        if (msg->wParam == TRUE) {
          *result = 0;
          return true;
        }
        return false;
      }
      case WM_NCACTIVATE: {
        if (!isCompositionEnabled())
          return true;
        return false;
      }
      case WM_NCHITTEST: {
        *result = 0;
        double dpr = this->devicePixelRatioF();
        long iPadding = 5.0f * dpr;
        RECT winRect;
        HWND hWnd = (HWND)winId();
        ::GetClientRect(hWnd, &winRect);

        POINT mousePT;
        mousePT.x = GET_X_LPARAM(msg->lParam);
        mousePT.y = GET_Y_LPARAM(msg->lParam);
        //qInfo() << "Screen Mouse Pos: " << mousePT.x << ", " << mousePT.y;
        ::ScreenToClient(hWnd, &mousePT);
        //qInfo() << "Client Mouse Pos: " << mousePT.x << ", " << mousePT.y;

        if (m_bResizeable) {
          bool bResizeWidth = minimumWidth() != maximumWidth();
          bool bResizeHeight = minimumHeight() != maximumHeight();

          if (m_bTranslucentBackground) {
            QMargins margins = contentsMargins();

            winRect.left += margins.left();
            winRect.top += margins.top();
            winRect.right -= margins.right();
            winRect.bottom -= margins.bottom();
          }

          if (bResizeWidth) {
            //left border
            if (mousePT.x >= winRect.left && mousePT.x < winRect.left + iPadding) {
              *result = HTLEFT;
            }
            //right border
            if (mousePT.x < winRect.right && mousePT.x >= winRect.right - iPadding) {
              *result = HTRIGHT;
            }
          }

          if (bResizeHeight) {
            //bottom border
            if (mousePT.y < winRect.bottom && mousePT.y >= winRect.bottom - iPadding) {
              *result = HTBOTTOM;
            }
            //top border
            if (mousePT.y >= winRect.top && mousePT.y < winRect.top + iPadding) {
              *result = HTTOP;
            }
          }

          if (bResizeWidth && bResizeHeight) {
            //bottom left corner
            if (mousePT.x >= winRect.left && mousePT.x < winRect.left + iPadding &&
                mousePT.y < winRect.bottom && mousePT.y >= winRect.bottom - iPadding) {
              *result = HTBOTTOMLEFT;
            }
            //bottom right corner
            if (mousePT.x < winRect.right && mousePT.x >= winRect.right - iPadding &&
                mousePT.y < winRect.bottom && mousePT.y >= winRect.bottom - iPadding) {
              *result = HTBOTTOMRIGHT;
            }
            //top left corner
            if (mousePT.x >= winRect.left && mousePT.x < winRect.left + iPadding &&
                mousePT.y >= winRect.top && mousePT.y < winRect.top + iPadding) {
              *result = HTTOPLEFT;
            }
            //top right corner
            if (mousePT.x < winRect.right && mousePT.x >= winRect.right - iPadding &&
                mousePT.y >= winRect.top && mousePT.y < winRect.top + iPadding) {
              *result = HTTOPRIGHT;
            }
          }
        }
        if (*result)
          return true;

        QMargins margins = contentsMargins();
        //qInfo() << "margins:" << margins;
        //qInfo() << "x: " << mousePT.x << ", y: " << mousePT.y;
        QPoint pos(mousePT.x / dpr - margins.left(), mousePT.y  / dpr - margins.top() );

        bool bInTitleBar = false;
        bool bExcept = false;
        for (auto& w : m_titlebarWidget) {
          if (w->rect().contains(pos)) {
            QWidget* child = w->childAt(pos);
            if (child) {
              //qInfo() << child;
              bExcept = false;
              for (auto& e : m_titlebarExceptWidgetPointer) {
                //qInfo() << child->objectName() << ", " << e->objectName();
                if (child->objectName() == e->objectName()) {
                  bExcept = true;
                  break;
                }
              }

              if (!bExcept) {
                for (auto& e : m_titleBarExceptWidgetType) {
                  if (child->inherits(e.toLocal8Bit())) {
                    bExcept = true;
                    break;
                  }
                }
              }
              if (!bExcept)
                bInTitleBar = true;
            }
            else {
              bInTitleBar = true;
            }
            break;
          }
        }

        if (!bInTitleBar)
          return false;

        *result = HTCAPTION;
        return true;
      }

      default:
        return T::nativeEvent(eventType, message, result);
    }
  }

 protected:
  bool m_bResizeable;
  bool m_bSystemShadow;
  bool m_bTranslucentBackground;
  QVector<QWidget*> m_titlebarWidget;
  QVector<QWidget*> m_titlebarExceptWidgetPointer;
  QVector<QString> m_titleBarExceptWidgetType;

  QMargins m_margins;
  QMargins m_frames;
};

static void loadStyleSheetFile(const QString& sheetName, QWidget* widget) {
  if (widget) {
    QString qss;
    QFile qssFile(sheetName);
    qssFile.open(QFile::ReadOnly);
    if (qssFile.isOpen()) {
      qss = QLatin1String(qssFile.readAll());
      widget->setStyleSheet(qss);
      qssFile.close();
    }
  }
}

#endif