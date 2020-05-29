[>>> Enginsh Version](README.md)

# 1. FramelessWindow介绍
当我开始用Qt作为界面库时，我发现Qt没有提供一个完美的无边框窗体解决方案来支持如下功能，需要自己来实现这些功能：
1. 支持通过使用鼠标拖拽来改变窗体位置和大小；`[Support]`
2. 支持双击标题栏最大化窗体和还原窗体；`[Support]`
3. 支持`Windows Areo Snap`特性；
4. 支持系统阴影；
5. 支持跨不同DPI的屏幕拖拽；`[Support]`
6. 适应分辨率和DPI改变；`[Support]`

# 2. 实现方案比较

在Qt中实现无边框窗体有2种方案：

## 2.1 方案一
通过重载`nativeEvent`函数拦截Windows消息（如`WM_NCHITTEST`）来实现，大致步骤如下：
1. 给窗体设置`WS_THICKFRAME | WS_CAPTION`属性从而还原窗体边框和标题栏，这样窗体就可以接收到`WM_NCHITTEST`消息。
2. 在`WM_NCCALCSIZE`消息处理中再移除边框和标题栏。
3. 在`WM_NCHITTEST`消息处理中通过判断鼠标位置来设置鼠标行为（`HTLEFT`, `HTRIGHT`等）。

这种方案的优点是可以支持`Windows Areo Snap`和`系统阴影`的特性，但是针对消息处理起来很复杂而且需要兼容Qt的各个版本，目前我还没有找到一个通过这种方案来完美实现无边框的解决方案。

据我所知，有如下的开源项目是通过这种方式来实现的，但都有些许问题，如不支持跨不同DPI屏幕拖拽、不能适应分辨率和DPI改变、`WM_NCHITTEST`有时无响应等。另外，在设置了背景透明属性之后（如`Qt::WA_TranslucentBackground`），系统阴影特性也将消失。

- qtdevs/FramelessHelper: [https://github.com/qtdevs/FramelessHelper](https://github.com/qtdevs/FramelessHelper)
- wangwenx190/framelesshelper: [https://github.com/wangwenx190/framelesshelper](https://github.com/wangwenx190/framelesshelper)

## 2.2 方案二

这种方案不Hook windows `WM_NCHITTEST`、`WM_NCCALCSIZE`消息，也不改变窗体样式，通过纯Qt方式实现。通过对每个Widget设置MouseTracking，来使每个Widget都可以响应鼠标事件（mouseMoveEvent、mousePressEvent、mouseReleaseEvent等），然后这些事件中判断鼠标位置来设置鼠标的形状和行为。

这种方式虽然对鼠标位置的判断逻辑比较繁琐，但兼容性较好，较纯粹，不需要处理Windows的各个消息。

**本项目就是通过这种方式来实现的，可以支持上述除`“Windows Areo Snap”和“系统阴影”`特性之外的所有无边框窗体的特性**。

# Screenshot On Windows
![screenshot on windows](frameless.gif)
