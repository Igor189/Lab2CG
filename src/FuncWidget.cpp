#include "FuncWidget.h"
#include <QPainter>


FuncWidget::FuncWidget(QWidget* parent) : QWidget(parent) {}

void FuncWidget::Create(const QSize& size, const int getElem)
{
    m_image = QImage(size, QImage::Format_ARGB32);
    m_image.fill(Qt::cyan);

    m_firstColor = Qt::black;
    m_secondColor = Qt::white;

    m_nNormalComponentIndex = getElem;
}

void FuncWidget::DrawFunc(const std::function<float(const QPointF&)>& rfunc, const QRectF& subSpace)
{
    QPointF spaceStep = { subSpace.width() / static_cast<float>(m_image.width()),
                         subSpace.height() / static_cast<float>(m_image.height()) };

    for (int x = 0; x < m_image.width() - 1; ++x)
    {
        for (int y = 0; y < m_image.height() - 1; ++y)
        {
            QPointF spacePoint1 = { subSpace.left() + static_cast<float>(x) * spaceStep.x(),
                                   subSpace.top() + static_cast<float>(y) * spaceStep.y() };

            const float z1 = rfunc(spacePoint1);

            QPointF spacePoint2 = { subSpace.left() + static_cast<float>(x + 1) * spaceStep.x(),
                                   subSpace.top() + static_cast<float>(y) * spaceStep.y() };

            const float z2 = rfunc(spacePoint2);

            QPointF spacePoint3 = { subSpace.left() + static_cast<float>(x) * spaceStep.x(),
                                   subSpace.top() + static_cast<float>(y + 1) * spaceStep.y() };

            const float z3 = rfunc(spacePoint3);

            const float A = CalculateDeterminant3x3({
                {(float)spacePoint1.y(), z1, 1},
                {(float)spacePoint2.y(), z2, 1},
                {(float)spacePoint3.y(), z3, 1},
                });

            const float B = CalculateDeterminant3x3({
                {(float)spacePoint1.x(), z1, 1},
                {(float)spacePoint2.x(), z2, 1},
                {(float)spacePoint3.x(), z3, 1},
                });

            const float C = CalculateDeterminant3x3({
                {(float)spacePoint1.x(), (float)spacePoint1.y(), 1},
                {(float)spacePoint2.x(), (float)spacePoint2.y(), 1},
                {(float)spacePoint3.x(), (float)spacePoint3.y(), 1},
                });

            const float D = CalculateDeterminant3x3({
                {(float)spacePoint1.x(), (float)spacePoint1.y(), z1},
                {(float)spacePoint2.x(), (float)spacePoint2.y(), z2},
                {(float)spacePoint3.x(), (float)spacePoint3.y(), z3},
                });


            float det = std::sqrt(A * A + B * B + C * C + D * D);

            float nx = A / det;
            float ny = B / det;
            float nz = C / det;
            float nw = D / det;


            float norm = nx;

            switch (m_nNormalComponentIndex)
            {
            case 0:
                break;
            case 1:
                norm = ny;
                break;
            case 2:
                norm = nz;
                break;
            case 3:
                norm = nw;
                break;
            }

            auto pixelColor = InterpolateColors(m_firstColor, m_secondColor, (1.f + norm) / 2);
            m_image.setPixelColor(x, y, pixelColor);
        }
    }
}

float FuncWidget::CalculateDeterminant3x3(const std::vector<std::vector<float>>& matrix)
{
    if (matrix.size() != 3 || matrix[0].size() != 3 || matrix[1].size() != 3 || matrix[2].size() != 3)
    {
        throw std::runtime_error("Wrong");
    }

    return matrix[0][0] * (matrix[1][1] * matrix[2][2] - matrix[1][2] * matrix[2][1]) -
        matrix[0][1] * (matrix[1][0] * matrix[2][2] - matrix[1][2] * matrix[2][0]) +
        matrix[0][2] * (matrix[1][0] * matrix[2][1] - matrix[1][1] * matrix[2][0]);
}

void FuncWidget::UpdateImage(const QColor& firstColor, const QColor& secondColor)
{
    for (int x = 0; x < m_image.width() - 1; ++x)
    {
        for (int y = 0; y < m_image.height() - 1; ++y)
        {
            float t =
                (static_cast<float>(m_image.pixelColor(x, y).red()) - m_firstColor.red()) / (m_secondColor.red() - m_firstColor.red());
            auto pixelColor = InterpolateColors(firstColor, secondColor, t);
            m_image.setPixelColor(x, y, pixelColor);
        }
    }
    m_image = m_image.mirrored(false, true);
    m_firstColor = firstColor;
    m_secondColor = secondColor;
}

void FuncWidget::SaveImage(const QString& filename)
{
    m_image.save(filename);
}

QColor FuncWidget::InterpolateColors(const QColor& colorFirst, const QColor& colorSec, float k)
{
    QColor val;
    val.setRed(static_cast<int>(colorFirst.red() + (colorSec.red() - colorFirst.red()) * k));
    val.setGreen(static_cast<int>(colorFirst.green() + (colorSec.green() - colorFirst.green()) * k));
    val.setBlue(static_cast<int>(colorFirst.blue() + (colorSec.blue() - colorFirst.blue()) * k));
    val.setAlpha(static_cast<int>(colorFirst.alpha() + (colorSec.alpha() - colorFirst.alpha()) * k));

    return val;
}

void FuncWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.drawImage(0, 0, m_image);
}