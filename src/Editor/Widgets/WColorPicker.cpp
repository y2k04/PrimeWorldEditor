#include "Editor/Widgets/WColorPicker.h"

#include <QColorDialog>
#include <QMouseEvent>
#include <QPainter>
#include <QRectF>

WColorPicker::WColorPicker(QWidget *parent)
    : QWidget(parent)
{
}

void WColorPicker::paintEvent(QPaintEvent *)
{
    QRect Area(QPoint(2,2), size() - QSize(5,5)); // Subtraction makes room for the stroke
    QColor FillColor = mColor;
    FillColor.setAlpha(255);

    QBrush Fill(FillColor);
    QPen Outline(Qt::black, 1);

    QPainter Painter(this);
    Painter.setBrush(Fill);
    Painter.setPen(Outline);
    Painter.drawRect(Area);

    if (hasFocus())
    {
        QRect DottedLine(QPoint(0,0), size() - QSize(1,1));
        Fill.setColor(Qt::transparent);
        Outline.setStyle(Qt::DotLine);

        Painter.setBrush(Fill);
        Painter.setPen(Outline);
        Painter.drawRect(DottedLine);
    }
}

void WColorPicker::keyPressEvent(QKeyEvent *pEvent)
{
    if (pEvent->key() != Qt::Key_Return)
        return;

    QColorDialog ColorPick;
    ColorPick.setOptions(QColorDialog::ShowAlphaChannel);
    ColorPick.setCurrentColor(mColor);
    const int result = ColorPick.exec();

    if (result)
    {
        mColor = ColorPick.currentColor();
        emit ColorChanged(mColor);
    }
}

void WColorPicker::mousePressEvent(QMouseEvent *)
{
    setFocus();
}

void WColorPicker::mouseReleaseEvent(QMouseEvent *pEvent)
{
    const auto pos = pEvent->position().toPoint();
    if (pos.x() >= width() || pos.y() >= height())
        return;

    mOldColor = mColor;

    QColorDialog ColorPick(this);
    ColorPick.setOptions(QColorDialog::ShowAlphaChannel);
    ColorPick.setCurrentColor(mColor);
    connect(&ColorPick, &QColorDialog::currentColorChanged, this, &WColorPicker::DialogColorChanged);
    connect(&ColorPick, &QColorDialog::rejected, this, &WColorPicker::DialogRejected);
    const int Result = ColorPick.exec();

    if (Result)
    {
        mColor = ColorPick.currentColor();
        emit ColorEditComplete(mColor);
    }
}

void WColorPicker::SetColor(const QColor& Color)
{
    if (mColor == Color)
        return;

    mColor = Color;
    emit ColorEditComplete(mColor);
    update();
}

void WColorPicker::DialogColorChanged(const QColor& NewColor)
{
    mColor = NewColor;
    emit ColorChanged(mColor);
    update();
}

void WColorPicker::DialogRejected()
{
    SetColor(mOldColor);
}
