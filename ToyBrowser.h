#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QLineEdit>
#include <QtWebEngine>
#include <QtWebEngineWidgets>

#include "ui_ToyBrowser.h"

class ToyBrowser : public QMainWindow
{
    Q_OBJECT

public:
    ToyBrowser(QWidget* parent = Q_NULLPTR);

private:
    //Ui::ToyBrowserClass ui;
    QTabWidget* m_tabs;
    QLineEdit* m_urlbar;

    void closeCurrentTab(int index);
    void tabOpenDoubleclick(int index);
    void renewUrlbar(const QUrl& url, QWebEngineView* webview);
    void currentTabChanged(int index);
    void jumpToUrl();
    void addNewTab(const QUrl& url, const QString& label);
};
