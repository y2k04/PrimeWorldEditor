#ifndef WCOLORPICKER_H
#define WCOLORPICKER_H

#include <QWidget>
#include <QColor>

class WColorPicker : public QWidget
{
    Q_OBJECT
    QColor mColor{Qt::transparent};
    QColor mOldColor;

public:
    explicit WColorPicker(QWidget* pParent = nullptr);

    void paintEvent(QPaintEvent*) override;
    void keyPressEvent(QKeyEvent* pEvent) override;
    void mousePressEvent(QMouseEvent*) override;
    void mouseReleaseEvent(QMouseEvent* pEvent) override;
    QColor Color() const;
    void SetColor(const QColor& Color);

signals:
    void ColorChanged(const QColor& NewColor);
    void ColorEditComplete(const QColor& NewColor);

private slots:
    void DialogColorChanged(const QColor& NewColor);
    void DialogRejected();
};

#endif // WCOLORPICKER_H
