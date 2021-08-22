#include "ToyBrowser.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[]) {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
    // Qt 5.6 以上适配高分辨率屏幕，开启缩放
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    //qputenv("QT_SCALE_FACTOR", "1");
#endif
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
    // Qt 5.14 以上可以通过下面这行保证分辨率自动跟随系统变化
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
#endif
    QApplication a(argc, argv);
    ToyBrowser w;
    w.show();
    return a.exec();
}
