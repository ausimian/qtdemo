#include <qapplication.h>
#include <qabstractnativeeventfilter.h>
#include <qdockwidget.h>
#include <qmainwindow.h>
#include <qstring.h>
#include <qtableview.h>
#include <qtimer.h>
#include <qheaderview.h>
#include <qstyleditemdelegate.h>
#include <qstyleoption.h>
#include <qpainter.h>
#include <qrawfont.h>
#include <qglyphrun.h>
#include <vector>

#include "my_model.h"
#include "main_widget.h"


class MyWindowSnapEventFilter : public QAbstractNativeEventFilter
{
public:
    bool nativeEventFilter(const QByteArray &eventType, void *message, long*) override
    {
        return false;
    }
};

class MyFlagsItemDelegate : public QStyledItemDelegate
{
public:
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override
    {
        QStyledItemDelegate::paint(painter, option, QModelIndex());

        QRect box(option.rect.left() + 2, option.rect.top() + 2, option.rect.width() - 4, option.rect.height() - 4);
        painter->drawRect(box);
        if (index.data(Qt::CheckStateRole) == QVariant(true))
        {
            QRect inner(box.left() + 1, box.top() + 1, box.width() - 1, box.height() - 1);
            painter->fillRect(inner, Qt::GlobalColor::darkMagenta);
        }
    }
};

class MyTextItemDelegate : public QStyledItemDelegate
{
public:
    MyTextItemDelegate(QRawFont& rawFont)
    {
        QChar chars[128];
        for (int i = 0; i < 127; ++i)
            chars[i] = i;

        mGlyphs.resize(128);
        int nGlyphs = static_cast<int>(mGlyphs.size());
        while (!rawFont.glyphIndexesForChars(chars, 128, mGlyphs.data(), &nGlyphs))
            mGlyphs.resize(nGlyphs);

        run.setRawFont(rawFont);

        for (auto& pos : glyphPositions)
            pos.setY(run.rawFont().pixelSize());
    }

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override
    {
        const auto cellWidth = option.rect.width();
        const auto charWidth = run.rawFont().averageCharWidth();

        auto data = index.data().value<QString>();
        auto size = 0;
        while (size < 256 && data[size] != 0 && (size + 1) * charWidth < cellWidth)
        {
            glyphPositions[size].setX(size * charWidth);

            glyphIndices[size] = mGlyphs.data()[static_cast<unsigned>(data[size].cell())];
            size++;
        }

        run.setRawData(glyphIndices, glyphPositions, size);
        QPointF pos(option.rect.topLeft());

        QStyledItemDelegate::paint(painter, option, QModelIndex());
        painter->drawGlyphRun(pos, run);
    }

private:
    std::vector<quint32> mGlyphs;
    mutable QGlyphRun run;
    mutable quint32 glyphIndices[256];
    mutable QPointF glyphPositions[256];
};

int main(int argc, char**argv)
{
    MyWindowSnapEventFilter snapper;
    QApplication app(argc, argv);

    QMainWindow window;

    app.installNativeEventFilter(&snapper);

    QDockWidget dockWidget1("ABC", &window);
    QTableView  tableView(&dockWidget1);
    dockWidget1.setWidget(&tableView);

    auto font = tableView.font();
    font.setFamily("DejaVu Sans Mono");
    font.setPointSize(6);
    tableView.setFont(font);

    auto vhdr = tableView.verticalHeader();
    vhdr->setSectionResizeMode(QHeaderView::Fixed);
    vhdr->setDefaultSectionSize(12);
    vhdr->setVisible(false);

    auto hhdr = tableView.horizontalHeader();
    hhdr->setMinimumSectionSize(0);

    MyModel model;
    tableView.setModel(&model);

    auto rawFont = QRawFont::fromFont(font);


    MyTextItemDelegate textDel(rawFont);
    MyFlagsItemDelegate flagsDel;
    for (int i = 0; i <= model.columnCount(QModelIndex()); ++i)
    {
        if (i >= 15 && i <= 20)
        {
            tableView.setItemDelegateForColumn(i, &flagsDel);
            tableView.setColumnWidth(i, 12);
        }
        else
        {
            tableView.setItemDelegateForColumn(i, &textDel);
            tableView.setColumnWidth(i, tableView.columnWidth(i) / 2);
        }
    }

    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, [&](){ tableView.viewport()->update(); });
    timer.start(500);

    window.addDockWidget(Qt::LeftDockWidgetArea, &dockWidget1);
    window.setTabPosition(Qt::LeftDockWidgetArea, QTabWidget::TabPosition::North);
    window.show();

    /*
    MainWidget widget;
    widget.resize(100, 30);
    widget.show();
    */

    return app.exec();
}