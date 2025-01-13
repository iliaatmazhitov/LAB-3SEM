#include <iostream>
#include "headers/data_structures/Graph.h"
#include "headers/algorithms.h"
#include "headers/UI.h"
#include <QApplication>
#include <QMainWindow>
#include <QGroupBox>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsPolygonItem>
#include <QGraphicsTextItem>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QInputDialog>
#include <QMessageBox>
#include <QGraphicsDropShadowEffect>
#include <cmath>

class GraphScene;
class GraphWindow;

class GraphScene : public QGraphicsScene {
    Q_OBJECT

public:
    GraphScene(OrientedGraph<int>* graph, QObject* parent = nullptr)
            : QGraphicsScene(parent), graph(graph), startVertex(nullptr) {
        setSceneRect(-400, -300, 800, 600);
        setBackgroundBrush(QColor(240, 242, 245));
    }



    void setGraph(OrientedGraph<int>* newGraph) {
        graph = newGraph;
        clear();
        layoutGraph();
    }

    void updateVertexLabel(QGraphicsEllipseItem* vertexItem, size_t id, int data) {
        for (QGraphicsItem* child : vertexItem->childItems()) {
            delete child;
        }

        QRectF rect = vertexItem->boundingRect();
        QGraphicsRectItem* textBg = new QGraphicsRectItem(rect.center().x() - 20, rect.center().y() - 20, 40, 40, vertexItem);
        textBg->setBrush(QColor(255, 255, 255, 50));
        textBg->setPen(Qt::NoPen);

        QGraphicsTextItem* idLabel = new QGraphicsTextItem(vertexItem);
        idLabel->setPlainText(QString("%1").arg(id));
        QFont idFont("Segoe UI", 10, QFont::Bold);
        idLabel->setFont(idFont);
        idLabel->setDefaultTextColor(Qt::white);

        QGraphicsTextItem* valueLabel = new QGraphicsTextItem(vertexItem);
        valueLabel->setPlainText(QString("{%1}").arg(data));
        QFont valueFont("Segoe UI", 9);
        valueLabel->setFont(valueFont);
        valueLabel->setDefaultTextColor(Qt::white);

        QRectF idRect = idLabel->boundingRect();
        QRectF valueRect = valueLabel->boundingRect();

        idLabel->setPos(rect.center().x() - idRect.width() / 2,
                        rect.center().y() - idRect.height() - 2);
        valueLabel->setPos(rect.center().x() - valueRect.width() / 2,
                           rect.center().y() + 2);

        QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect;
        shadowEffect->setBlurRadius(8);
        shadowEffect->setColor(QColor(0, 0, 0, 150));
        shadowEffect->setOffset(4, 4);
        vertexItem->setGraphicsEffect(shadowEffect);
    }


    void highlightPath(const ArraySequence<size_t>& path) {
        for (auto item : items()) {
            if (auto line = dynamic_cast<QGraphicsLineItem*>(item)) {
                line->setPen(QPen(QColor(200, 200, 200), 2));
            }
            if (auto ellipse = dynamic_cast<QGraphicsEllipseItem*>(item)) {
                QLinearGradient gradient(ellipse->rect().topLeft(), ellipse->rect().bottomRight());
                gradient.setColorAt(0, QColor(65, 105, 225));
                gradient.setColorAt(1, QColor(30, 144, 255));
                ellipse->setBrush(gradient);
            }
        }

        for (size_t i = 0; i < path.GetLength() - 1; i++) {
            size_t current = path.Get(i);
            size_t next = path.Get(i + 1);

            for (auto item : items()) {
                if (auto vertex = dynamic_cast<QGraphicsEllipseItem*>(item)) {
                    if (vertex->data(0).toULongLong() == current ||
                        vertex->data(0).toULongLong() == next) {
                        QLinearGradient gradient(vertex->rect().topLeft(), vertex->rect().bottomRight());
                        gradient.setColorAt(0, QColor(50, 205, 50));
                        gradient.setColorAt(1, QColor(34, 139, 34));
                        vertex->setBrush(gradient);
                    }
                }
                if (auto edge = dynamic_cast<QGraphicsLineItem*>(item)) {
                    if (edge->data(0).toULongLong() == current &&
                        edge->data(1).toULongLong() == next) {
                        QPen pen(QColor(255, 69, 0), 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
                        edge->setPen(pen);
                    }
                }
            }
        }
    }

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override {
        QPointF pos = event->scenePos();
        QGraphicsItem* clickedItem = itemAt(pos, QTransform());

        if (event->button() == Qt::LeftButton) {
            if (!clickedItem) {
                bool ok;
                int id = QInputDialog::getInt(nullptr, "New Vertex", "Enter vertex ID:",
                                              0, 0, 1000000, 1, &ok);
                if (ok) {
                    int data = QInputDialog::getInt(nullptr, "Vertex Data", "Enter vertex ",
                                                    0, INT_MIN, INT_MAX, 1, &ok);
                    if (ok) {
                        try {
                            graph->AddVertex(id, data);
                            addVertexToScene(id, data, pos);
                        } catch (std::exception& e) {
                            QMessageBox::warning(nullptr, "Error", e.what());
                        }
                    }
                }
            } else if (QGraphicsEllipseItem* vertexItem = dynamic_cast<QGraphicsEllipseItem*>(clickedItem)) {
                if (event->modifiers() & Qt::ControlModifier) {
                    bool ok;
                    size_t id = vertexItem->data(0).toULongLong();
                    int newData = QInputDialog::getInt(nullptr, "Edit Data", "Enter new vertex ",
                                                       vertexItem->data(1).toInt(), INT_MIN, INT_MAX, 1, &ok);
                    if (ok) {
                        graph->SetVertexData(id, newData);
                        updateVertexLabel(vertexItem, id, newData);
                    }
                }
            }
        }
    }

private:


    void addVertexToScene(size_t id, int data, const QPointF& pos) {
        QGraphicsEllipseItem* vertex = addEllipse(pos.x() - 25, pos.y() - 25, 50, 50);

        QLinearGradient gradient(vertex->rect().topLeft(), vertex->rect().bottomRight());
        gradient.setColorAt(0, QColor(100, 149, 237));
        gradient.setColorAt(1, QColor(30, 144, 255));

        vertex->setBrush(gradient);
        vertex->setPen(QPen(Qt::white, 2));
        vertex->setData(0, QVariant(qulonglong(id)));
        vertex->setData(1, QVariant(data));
        updateVertexLabel(vertex, id, data);

        QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect;
        shadowEffect->setBlurRadius(10);
        shadowEffect->setColor(QColor(0, 0, 0, 80));
        shadowEffect->setOffset(4, 4);
        vertex->setGraphicsEffect(shadowEffect);
    }

    void addEdgeToScene(size_t from, size_t to, int64_t weight) {
        QPointF fromPos, toPos;

        for (QGraphicsItem* item : items()) {
            if (QGraphicsEllipseItem* vertex = dynamic_cast<QGraphicsEllipseItem*>(item)) {
                size_t id = vertex->data(0).toULongLong();
                if (id == from) {
                    fromPos = vertex->sceneBoundingRect().center();
                } else if (id == to) {
                    toPos = vertex->sceneBoundingRect().center();
                }
            }
        }

        if (fromPos.isNull() || toPos.isNull()) return;

        QLineF line(fromPos, toPos);
        double angle = std::atan2(-line.dy(), line.dx());

        QPen edgePen(QColor(200, 200, 200), 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        QGraphicsLineItem* edgeLine = addLine(QLineF(fromPos, toPos), edgePen);

        QPointF fromPoint = fromPos + QPointF(25 * std::cos(angle), 25 * std::sin(angle));
        QPointF toPoint = toPos + QPointF(-25 * std::cos(angle), -25 * std::sin(angle));
        edgeLine->setLine(QLineF(fromPoint, toPoint));

        QPolygonF arrowHead;
        arrowHead << QPointF(0, 0)
                  << QPointF(-10, -5)
                  << QPointF(-10, 5);
        QGraphicsPolygonItem* arrow = new QGraphicsPolygonItem(arrowHead);
        arrow->setBrush(QColor(100, 149, 237));
        arrow->setPen(Qt::NoPen);
        arrow->setPos(toPoint);
        arrow->setRotation(-angle * 180 / M_PI);
        addItem(arrow);

        QGraphicsTextItem* weightLabel = addText(QString::number(weight));
        weightLabel->setDefaultTextColor(QColor(100, 100, 100));
        QFont font("Segoe  UI", 10);
        weightLabel->setFont(font);
        weightLabel->setPos((fromPoint + toPoint) / 2);
    }

    void layoutGraph() {
        clear();

        auto vertices = graph->GetAllVertices();
        int vertexCount = vertices.GetLength();

        if (vertexCount == 0) return;

        double radius = 200;
        double angleStep = 2 * M_PI / vertexCount;

        QMap<size_t, QGraphicsEllipseItem*> vertexItems;
        for (int i = 0; i < vertexCount; i++) {
            auto vertex = vertices.Get(i);
            QPointF pos(radius * std::cos(i * angleStep),
                        radius * std::sin(i * angleStep));

            QGraphicsEllipseItem* vertexItem = addEllipse(
                    pos.x() - 25, pos.y() - 25, 50, 50);

            QLinearGradient gradient(vertexItem->rect().topLeft(), vertexItem->rect().bottomRight());
            gradient.setColorAt(0, QColor(65, 105, 225));
            gradient.setColorAt(1, QColor(30, 144, 255));

            vertexItem->setBrush(gradient);
            vertexItem->setPen(QPen(Qt::white, 2));
            vertexItem->setData(0, QVariant(qulonglong(vertex.id)));
            vertexItem->setData(1, QVariant(vertex.value));
            updateVertexLabel(vertexItem, vertex.id, vertex.value);

            vertexItems[vertex.id] = vertexItem;
        }

        for (int i = 0; i < vertexCount; i++) {
            auto vertex = vertices.Get(i);
            auto edges = vertex.edges;

            for (int j = 0; j < edges.GetLength(); j++) {
                auto edge = edges.Get(j);
                if (vertexItems.contains(edge.from) && vertexItems.contains(edge.to)) {
                    addEdgeToScene(edge.from, edge.to, edge.weight);
                }
            }
        }
    }

    OrientedGraph<int>* graph;
    QGraphicsEllipseItem* startVertex;
};

class GraphWindow : public QMainWindow {
    Q_OBJECT

public:
    GraphWindow(QWidget *parent = nullptr)
            : QMainWindow(parent) {
        graph = new OrientedGraph<int>();

        QWidget *centralWidget = new QWidget();
        QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);

        QVBoxLayout *controlsLayout = new QVBoxLayout();
        controlsLayout->setSpacing(10);

        QGroupBox *graphOpsGroup = new QGroupBox("Graph Operations");
        QVBoxLayout *graphOpsLayout = new QVBoxLayout();
        graphOpsLayout->setSpacing(15);

        QPushButton *clearButton = new QPushButton("Clear Graph");
        clearButton->setStyleSheet("background-color: #4CAF50; color: white; border-radius: 5px; padding: 10px;");
        QPushButton *deleteVertexButton = new QPushButton("Delete Vertex");
        deleteVertexButton->setStyleSheet("background-color: #f44336; color: white; border-radius: 5px; padding: 10px;");
        QPushButton *addEdgeButton = new QPushButton("Add Edge");
        addEdgeButton->setStyleSheet("background-color: #2196F3; color: white; border-radius: 5px; padding: 10px;");
        presetCombo = new QComboBox();
        presetCombo->addItems({"Empty", "Simple Path", "Circle", "Binary Tree", "Complete Graph", "Star Graph"});

        graphOpsLayout->addWidget(clearButton);
        graphOpsLayout->addWidget(deleteVertexButton);
        graphOpsLayout->addWidget(addEdgeButton);
        graphOpsLayout->addWidget(presetCombo);
        graphOpsGroup->setLayout(graphOpsLayout);

        QGroupBox *algoGroup = new QGroupBox("Algorithms");
        QVBoxLayout *algoLayout = new QVBoxLayout();

        algoCombo = new QComboBox();
        algoCombo->addItems({"BFS", "DFS"});
        runAlgoButton = new QPushButton("Run Algorithm");

        algoLayout->addWidget(algoCombo);
        algoLayout->addWidget(runAlgoButton);
        algoGroup->setLayout(algoLayout);

        controlsLayout->addWidget(graphOpsGroup);
        controlsLayout->addWidget(algoGroup);
        controlsLayout->addStretch();

        scene = new GraphScene(graph);
        view = new QGraphicsView(scene);
        view->setRenderHint(QPainter::Antialiasing);
        view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
        view->setRenderHint(QPainter::SmoothPixmapTransform);
        view->setStyleSheet("background-color: #f1f1f1;");

        mainLayout->addWidget(view, 3);
        mainLayout->addLayout(controlsLayout, 1);

        setCentralWidget(centralWidget);

        connect(clearButton, &QPushButton::clicked, this, &GraphWindow::clearGraph);
        connect(deleteVertexButton, &QPushButton::clicked, this, &GraphWindow::deleteVertex);
        connect(addEdgeButton, &QPushButton::clicked, this, &GraphWindow::addEdge);
        connect(runAlgoButton, &QPushButton::clicked, this, &GraphWindow::runAlgorithm);
        connect(presetCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
                this, &GraphWindow::loadPreset);
    }

private slots:
    void clearGraph() {
        delete graph;
        graph = new OrientedGraph<int>();
        scene->setGraph(graph);
    }

    void deleteVertex() {
        bool ok;
        int id = QInputDialog::getInt(this, "Delete Vertex",
                                      "Enter vertex ID to delete:", 0, 0, 1000000, 1, &ok);
        if (ok) {
            try {
                graph->DeleteVertex(id);
                scene->setGraph(graph);
            } catch (std::exception &e) {
                QMessageBox::warning(this, "Error", e.what());
            }
        }
    }

    void addEdge() {
        bool ok;
        size_t fromId = QInputDialog::getInt(this, "Add Edge",
                                             "Enter source vertex ID:", 0, 0, 1000000, 1, &ok);
        if (!ok) return;

        size_t toId = QInputDialog::getInt(this, "Add Edge",
                                           "Enter destination vertex ID:", 0, 0, 1000000, 1, &ok);
        if (!ok) return;

        int weight = QInputDialog::getInt(this, "Edge Weight",
                                          "Enter edge weight:", 1, -1000000, 1000000, 1, &ok);
        if (!ok) return;

        try {
            graph->AddEdge(fromId, toId, weight);
            scene->setGraph(graph);
        } catch (std::exception &e) {
            QMessageBox::warning(this, "Error", e.what());
        }
    }

    void runAlgorithm() {
        bool ok;
        size_t startId = QInputDialog::getInt(this, "Start Vertex",
                                              "Enter start vertex ID:", 0, 0, 1000000, 1, &ok);
        if (!ok) return;

        try {
            ArraySequence<size_t> result;
            switch (algoCombo->currentIndex()) {
                case 0: // BFS
                    result = BFS(*graph, startId);
                    break;
                case 1: // DFS
                    result = DFS(*graph, startId);
                    break;
            }
            scene->highlightPath(result);
        } catch (const std::exception &e) {
            QMessageBox::warning(this, "Error", e.what());
        }
    }

    void loadPreset(int index) {
        clearGraph();
        switch (index) {
            case 1: createSimplePath(); break;
            case 2: createCircle(); break;
            case 3: createBinaryTree(); break;
            case 4: createCompleteGraph(); break;
            case 5: createStarGraph(); break;
        }
        scene->setGraph(graph);
    }

private:
    void createSimplePath() {
        for (int i = 0; i < 5; i++) {
            graph->AddVertex(i, i * 10);
            if (i > 0) {
                graph->AddEdge(i - 1, i, 1);
            }
        }
    }

    void createCircle() {
        const int vertexCount = 6;

        for (int i = 0; i < vertexCount; i++) {
            graph->AddVertex(i, i * 10);
        }

        for (int i = 0; i < vertexCount; i++) {
            graph->AddEdge(i, (i + 1) % vertexCount, 1);
        }
    }


    void createBinaryTree() {
        for (int i = 0; i < 7; i++) {
            graph->AddVertex(i, i * 10);
        }

        graph->AddEdge(0, 1, 1);
        graph->AddEdge(0, 2, 1);
        graph->AddEdge(1, 3, 1);
        graph->AddEdge(1, 4, 1);
        graph->AddEdge(2, 5, 1);
        graph->AddEdge(2, 6, 1);
    }

    void createCompleteGraph() {
        const int vertexCount = 5;
        for (int i = 0; i < vertexCount; i++) {
            graph->AddVertex(i, i * 10);
        }

        for (int i = 0; i < vertexCount; i++) {
            for (int j = 0; j < vertexCount; j++) {
                if (i != j) {
                    graph->AddEdge(i, j, 1);
                }
            }
        }
    }

    void createStarGraph() {
        const int outerVertices = 6;

        graph->AddVertex(0, 0);

        for (int i = 1; i <= outerVertices; i++) {
            graph->AddVertex(i, i * 10);
            graph->AddEdge(0, i, 1);
        }
    }


private:
    OrientedGraph<int>* graph;
    GraphScene* scene;
    QGraphicsView* view;
    QComboBox* presetCombo;
    QComboBox* algoCombo;
    QPushButton* runAlgoButton;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    GraphWindow window;
    window.show();
    return app.exec();
}

#include "main.moc"