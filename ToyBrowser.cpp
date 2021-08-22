#include "ToyBrowser.h"

ToyBrowser::ToyBrowser(QWidget *parent)
    : QMainWindow(parent), m_tabs(new QTabWidget()), m_urlbar(new QLineEdit()) {
    //ui.setupUi(this);
    this->setWindowTitle("ToyBrowser");
    this->setWindowIcon(QIcon(":/ToyBrowser/penguin"));

    resize(800, 600);
    //show();

    // 创建导航栏(左侧工具栏按钮组+右侧地址栏)
    QToolBar *navBar = addToolBar("Navigation");
    navBar->setIconSize(QSize(16, 16));

    // 创建工具栏事件按钮
    QAction *backBtn = new QAction(QIcon(":/ToyBrowser/back"), "Back", this);
    QAction *nextBtn = new QAction(QIcon(":/ToyBrowser/next"), "Forward", this);
    QAction *stopBtn = new QAction(QIcon(":/ToyBrowser/cross"), "Stop", this);
    QAction *reloadBtn = new QAction(QIcon(":/ToyBrowser/renew"), "Reload", this);
    QAction *newtabBtn = new QAction(QIcon(":/ToyBrowser/add_page"), "New Page", this);

    // 将事件按钮添加到导航栏
    navBar->addAction(backBtn);
    navBar->addAction(nextBtn);
    navBar->addAction(stopBtn);
    navBar->addAction(reloadBtn);
    navBar->addAction(newtabBtn);

    // 添加标签页
    //m_tabs = new QTabWidget();
    m_tabs->setDocumentMode(true);
    m_tabs->setTabsClosable(true);
    m_tabs->setTabShape(QTabWidget::Triangular);
    m_tabs->setMovable(true);

    // 添加地址栏
    //m_urlbar = new QLineEdit();
    navBar->addSeparator();
    navBar->addWidget(m_urlbar);

    // 设置标签页为中心
    setCentralWidget(m_tabs);

    // 关联事件按钮回调
    connect(backBtn, &QAction::triggered, this, [this] {((QWebEngineView *)(m_tabs->currentWidget()))->back(); });
    connect(nextBtn, &QAction::triggered, this, [this] {((QWebEngineView *)(m_tabs->currentWidget()))->forward(); });
    connect(stopBtn, &QAction::triggered, this, [this] {((QWebEngineView *)(m_tabs->currentWidget()))->stop(); });
    connect(reloadBtn, &QAction::triggered, this, [this] {((QWebEngineView *)(m_tabs->currentWidget()))->reload(); });
    connect(newtabBtn, &QAction::triggered, this, [this] {addNewTab(QUrl(""), "New tab"); });

    // 标签页空白处双击->新建标签页
    connect(m_tabs, &QTabWidget::tabBarDoubleClicked, this, [this](int index) {tabOpenDoubleclick(index); });

    // 标签页关闭按钮->关闭标签页
    connect(m_tabs, &QTabWidget::tabCloseRequested, this, [this](int index) {closeCurrentTab(index); });

    // 地址栏回车键按下->跳转到地址栏 URL
    connect(m_urlbar, &QLineEdit::returnPressed, this, [this] {jumpToUrl(); });

    // 标签页改变->修改窗口标题
    connect(m_tabs, &QTabWidget::currentChanged, this, [this](int index) {currentTabChanged(index); });

    // 打开主页
    addNewTab(QUrl("https://www.google.com"), "Homepage");
}

void ToyBrowser::closeCurrentTab(int index) {
    if (m_tabs->count() < 2) {
        return;
    }

    QWebEngineView *webview = (QWebEngineView *)m_tabs->widget(index);
    webview->close();

    m_tabs->removeTab(index);
}

void ToyBrowser::tabOpenDoubleclick(int index) {
    if (index == -1) {
        addNewTab(QUrl(""), "New tab");
    }
}

void ToyBrowser::renewUrlbar(const QUrl &url, QWebEngineView *webview) {
    if (webview != m_tabs->currentWidget()) {
        return;
    }
    m_urlbar->setText(url.toString());
    m_urlbar->setCursorPosition(0);
}

void ToyBrowser::currentTabChanged(int index) {
    QWebEngineView *webview = (QWebEngineView *)m_tabs->currentWidget();
    renewUrlbar(webview->url(), webview);
}

void ToyBrowser::jumpToUrl() {
    QUrl url(m_urlbar->text());
    if (url.scheme() == QStringLiteral("")) {
        url.setScheme("http");
    }
    QWebEngineView *webview = (QWebEngineView *)m_tabs->currentWidget();
    webview->setUrl(url);
}

void ToyBrowser::addNewTab(const QUrl &url, const QString &label) {
    QWebEngineView *webview = new QWebEngineView();
    webview->setAttribute(Qt::WA_DeleteOnClose);
    webview->setUrl(url);

    const int index = m_tabs->addTab(webview, label);
    m_tabs->setCurrentIndex(index);

    connect(webview, &QWebEngineView::urlChanged, this, [this, webview](const QUrl &text) {renewUrlbar(text, webview); });
    connect(webview, &QWebEngineView::loadFinished, this,
        [this, index, webview](int progress) {
            if (webview == m_tabs->widget(index)) {
                m_tabs->setTabText(index, webview->page()->title());
            } else {
                // 设置标签可移动的话 index 可能被打乱：m_tabs->setMovable(true);
                for (int i = 0; i < m_tabs->count(); ++i) {
                    if (i == index) {
                        continue;
                    }
                    if (webview == m_tabs->widget(i)) {
                        m_tabs->setTabText(i, webview->page()->title());
                    }
                }
            }
        });
}
