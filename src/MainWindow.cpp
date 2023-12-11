#include "MainWindow.h"
#include"LogicalOperations.h"
#include<QGridLayout>

MainWindow::MainWindow(QWidget* parent) : QWidget(parent)
{
	QGridLayout* layout = new QGridLayout(this);

	m_pToolBar = new QToolBar(this);
	m_pSaveAction = m_pToolBar->addAction("Save");
	connect(m_pSaveAction, &QAction::triggered, this, &MainWindow::SaveImages);
	layout->addWidget(m_pToolBar, 0, 0, 1, 2);

	m_pWidgetNx = new FuncWidget(this);
	m_pWidgetNx->Create(QSize(200, 200), 0);
	layout->addWidget(m_pWidgetNx, 1, 0);

	m_pWidgetNy = new FuncWidget(this);
	m_pWidgetNy->Create(QSize(200, 200), 1);
	layout->addWidget(m_pWidgetNy, 1, 1);

	m_pWidgetNz = new FuncWidget(this);
	m_pWidgetNz->Create(QSize(200, 200), 2);
	layout->addWidget(m_pWidgetNz, 2, 0);

	m_pWidgetNw = new FuncWidget(this);
	m_pWidgetNw->Create(QSize(200, 200), 3);
	layout->addWidget(m_pWidgetNw, 2, 1);


	this->setFixedSize(430, 430);
}

void MainWindow::Draw()
{
	std::function<float(const QPointF&)> functions[9];
	functions[0] = [](const QPointF& point) -> float { return 1 - std::pow((point.x() - 1), 2) - std::pow((point.y() + 3.5), 2) / std::pow(0.7, 2); };
	functions[1] = [](const QPointF& point) -> float { return 1 - std::pow((point.x() + 1), 2) / std::pow(4, 2) + 1 - std::pow((point.y()), 2) / std::pow(2.5, 2); };
	functions[2] = [](const QPointF& point) -> float { return 1 - std::pow(((point.x() + 3.5)), 2) / std::pow(1.5, 2) + 1 - std::pow(((point.y() - 4)), 2) / std::pow(1.5, 2); };
	functions[3] = [](const QPointF& point) -> float { return 10 + point.x() - point.y(); };
	functions[4] = [](const QPointF& point) -> float { return 2 - point.x() - point.y(); };
	functions[5] = [](const QPointF& point) -> float { return point.y() - 3; };
	functions[6] = [](const QPointF& point) -> float { return 3 + 2.5 * point.x() - 2.5 * point.y(); };
	functions[7] = [](const QPointF& point) -> float { return 12 - 2.5 * point.x() - 2.5 * point.y(); };
	functions[8] = [](const QPointF& point) -> float { return 2.5 * point.y() + 3; };
	std::function<float(const QPointF&)> complexFunction = [&functions](const QPointF& point) -> float
		{
			auto w1 = LogicalOperations::Or(LogicalOperations::Or(functions[3](point), functions[4](point)), functions[5](point));
			auto w2 = LogicalOperations::Or(LogicalOperations::Or(functions[6](point), functions[7](point)), functions[8](point));
			auto w3 = LogicalOperations::And(LogicalOperations::And(LogicalOperations::And(LogicalOperations::And(functions[1](point), functions[2](point)), w1), functions[0](point)), w2);
			return w3;
		};
	QRectF subSpace(-10.f, -10.f, 20.f, 20.f);

	m_pWidgetNx->DrawFunc(complexFunction, subSpace);
	m_pWidgetNy->DrawFunc(complexFunction, subSpace);
	m_pWidgetNz->DrawFunc(complexFunction, subSpace);
	m_pWidgetNw->DrawFunc(complexFunction, subSpace);

}

void MainWindow::Update(const QColor& firstColor, const QColor& secondColor)
{
	m_pWidgetNw->UpdateImage(firstColor, secondColor);
	m_pWidgetNz->UpdateImage(firstColor, secondColor);
	m_pWidgetNy->UpdateImage(firstColor, secondColor);
	m_pWidgetNx->UpdateImage(firstColor, secondColor);
}

void MainWindow::SaveImages()
{
	m_pWidgetNx->SaveImage("Nx.png");
	m_pWidgetNy->SaveImage("Ny.png");
	m_pWidgetNz->SaveImage("Nz.png");
	m_pWidgetNw->SaveImage("Nw.png");
}