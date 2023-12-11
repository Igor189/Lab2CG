#pragma once
#include <QImage>
#include <QWidget>

class FuncWidget : public QWidget
{
public:
    FuncWidget(QWidget* parent = nullptr);

    void Create(const QSize& size, const int getElem);
    void DrawFunc(const std::function<float(const QPointF&)>& rfunc, const QRectF& subSpace);

    float CalculateDeterminant3x3(const std::vector<std::vector<float>>& matrix);

    void UpdateImage(const QColor& firstColor, const QColor& secondColor);
    void SaveImage(const QString& filename);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QColor m_firstColor;
    QColor m_secondColor;
    QImage m_image;
    int m_nNormalComponentIndex;

    QColor InterpolateColors(const QColor& colorFirst, const QColor& colorSec, float k);
};