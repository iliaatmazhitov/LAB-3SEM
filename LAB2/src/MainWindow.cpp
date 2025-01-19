//
// Created by Илья Атмажитов on 16.01.2025.
//
#include "MainWindow.h"
#include <QApplication>
#include <chrono>
#include <fstream>
#include <sstream>
#include <QFileInfo>
#include <QDir>
#include <QMessageBox>
#include <QCoreApplication>
#include <QtMath>
#include <QGridLayout>
#include <QHeaderView>
#include "CSVParser.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    centralWidget = nullptr;
    tabWidget = nullptr;
    performanceTab = nullptr;
    complexityTab = nullptr;
    manualInputTab = nullptr;
    currentPerformanceView = nullptr;
    historicalPerformanceView = nullptr;
    currentPerformanceChart = nullptr;
    historicalPerformanceChart = nullptr;
    complexityView = nullptr;
    complexityChart = nullptr;
    dataTable = nullptr;
    dataSourceGroup = nullptr;
    csvRadio = nullptr;
    manualRadio = nullptr;
    sizeSelector = nullptr;
    customSizeInput = nullptr;
    sortButton = nullptr;
    addDataButton = nullptr;
    clearDataButton = nullptr;

    try {
        setupUI();
        setupCharts();
        setupComplexityTab();
        setupManualInputTab();
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Initialization Error",
                              QString("Failed to initialize application: %1").arg(e.what()));
        throw;
    }
}

void MainWindow::setupUI() {
    centralWidget = new QWidget(this);
    if (!centralWidget) throw std::runtime_error("Failed to create central widget");
    setCentralWidget(centralWidget);

    mainLayout = new QVBoxLayout(centralWidget);
    if (!mainLayout) throw std::runtime_error("Failed to create main layout");

    tabWidget = new QTabWidget();
    if (!tabWidget) throw std::runtime_error("Failed to create tab widget");

    performanceTab = new QWidget();
    complexityTab = new QWidget();
    manualInputTab = new QWidget();

    if (!performanceTab || !complexityTab || !manualInputTab) {
        throw std::runtime_error("Failed to create tab widgets");
    }

    performanceLayout = new QVBoxLayout(performanceTab);
    complexityLayout = new QVBoxLayout(complexityTab);
    manualInputLayout = new QVBoxLayout(manualInputTab);

    if (!performanceLayout || !complexityLayout || !manualInputLayout) {
        throw std::runtime_error("Failed to create layout widgets");
    }

    auto* controlsLayout = new QHBoxLayout();
    if (!controlsLayout) throw std::runtime_error("Failed to create controls layout");

    auto* sizeGroup = new QGroupBox("Data Size");
    if (!sizeGroup) throw std::runtime_error("Failed to create size group");

    auto* sizeLayout = new QHBoxLayout();
    if (!sizeLayout) throw std::runtime_error("Failed to create size layout");

    sizeSelector = new QComboBox();
    if (!sizeSelector) throw std::runtime_error("Failed to create size selector");
    sizeSelector->addItems({"10000", "20000", "25000", "30000", "40000", "50000"});

    customSizeInput = new QSpinBox();
    if (!customSizeInput) throw std::runtime_error("Failed to create size input");
    customSizeInput->setRange(10000, 50000);
    customSizeInput->setValue(2000);
    customSizeInput->setSingleStep(100);

    sizeLayout->addWidget(new QLabel("Preset:"));
    sizeLayout->addWidget(sizeSelector);
    sizeLayout->addWidget(new QLabel("Custom:"));
    sizeLayout->addWidget(customSizeInput);
    sizeGroup->setLayout(sizeLayout);

    dataSourceGroup = new QGroupBox("Data Source");
    if (!dataSourceGroup) throw std::runtime_error("Failed to create data source group");

    auto* sourceLayout = new QHBoxLayout();
    if (!sourceLayout) throw std::runtime_error("Failed to create source layout");

    csvRadio = new QRadioButton("CSV File");
    manualRadio = new QRadioButton("Manual Input");

    if (!csvRadio || !manualRadio) {
        throw std::runtime_error("Failed to create radio buttons");
    }

    csvRadio->setChecked(true);
    sourceLayout->addWidget(csvRadio);
    sourceLayout->addWidget(manualRadio);
    dataSourceGroup->setLayout(sourceLayout);

    sortButton = new QPushButton("Run Analysis");
    addDataButton = new QPushButton("Add Data Point");
    clearDataButton = new QPushButton("Clear Data");

    if (!sortButton || !addDataButton || !clearDataButton) {
        throw std::runtime_error("Failed to create buttons");
    }

    controlsLayout->addWidget(dataSourceGroup);
    controlsLayout->addWidget(sizeGroup);
    controlsLayout->addWidget(sortButton);
    controlsLayout->addWidget(addDataButton);
    controlsLayout->addWidget(clearDataButton);
    controlsLayout->addStretch();

    performanceLayout->addLayout(controlsLayout);

    tabWidget->addTab(performanceTab, "Performance Analysis");
    tabWidget->addTab(complexityTab, "Complexity Analysis");
    tabWidget->addTab(manualInputTab, "Manual Data");

    mainLayout->addWidget(tabWidget);

    connect(sortButton, &QPushButton::clicked, this, &MainWindow::runSort);
    connect(addDataButton, &QPushButton::clicked, this, &MainWindow::addManualDataPoint);
    connect(clearDataButton, &QPushButton::clicked, this, &MainWindow::clearData);
    connect(csvRadio, &QRadioButton::toggled, this, &MainWindow::switchDataSource);
    connect(sizeSelector, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [this](int index) {
                if (customSizeInput) {
                    customSizeInput->setValue(sizeSelector->currentText().toInt());
                }
            });

    setWindowTitle("Sorting Algorithm Analysis");
    resize(1200, 800);

    addDataButton->setEnabled(false);
}

ArraySequence<Person>* MainWindow::readCSV(const QString& filename, int limit) {
    try {
        QFileInfo fileInfo(filename);
        if (!fileInfo.exists()) {
            QMessageBox::critical(this, "Error",
                                  QString("File not found: %1").arg(filename));
            return nullptr;
        }

        return CSVParser::readCSV(filename.toStdString(), limit);
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error",
                              QString("Failed to read CSV: %1").arg(e.what()));
        return nullptr;
    }
}



void MainWindow::setupCharts() {
    QWidget* currentChartWidget = new QWidget();
    QWidget* historicalChartWidget = new QWidget();

    QVBoxLayout* currentChartLayout = new QVBoxLayout(currentChartWidget);
    QVBoxLayout* historicalChartLayout = new QVBoxLayout(historicalChartWidget);

    currentPerformanceChart = new QChart();
    currentPerformanceChart->setTitle("Current Run Performance");
    currentPerformanceChart->setAnimationOptions(QChart::SeriesAnimations);


    QBarCategoryAxis *currentXAxis = new QBarCategoryAxis();
    QValueAxis *currentYAxis = new QValueAxis();
    currentYAxis->setTitleText("Time (ms)");
    currentYAxis->setMin(0);
    currentPerformanceChart->addAxis(currentXAxis, Qt::AlignBottom);
    currentPerformanceChart->addAxis(currentYAxis, Qt::AlignLeft);

    currentPerformanceView = new QChartView(currentPerformanceChart);
    currentPerformanceView->setRenderHint(QPainter::Antialiasing);
    currentPerformanceView->setMinimumHeight(300);


    historicalPerformanceChart = new QChart();
    historicalPerformanceChart->setTitle("Historical Performance");
    historicalPerformanceChart->setAnimationOptions(QChart::SeriesAnimations);

    QValueAxis *histXAxis = new QValueAxis();
    QValueAxis *histYAxis = new QValueAxis();
    histXAxis->setTitleText("Data Size");
    histYAxis->setTitleText("Time (ms)");
    histXAxis->setMin(0);
    histYAxis->setMin(0);
    historicalPerformanceChart->addAxis(histXAxis, Qt::AlignBottom);
    historicalPerformanceChart->addAxis(histYAxis, Qt::AlignLeft);

    historicalPerformanceView = new QChartView(historicalPerformanceChart);
    historicalPerformanceView->setRenderHint(QPainter::Antialiasing);
    historicalPerformanceView->setMinimumHeight(300);

    currentChartLayout->addWidget(currentPerformanceView);
    historicalChartLayout->addWidget(historicalPerformanceView);

    performanceLayout->addWidget(currentChartWidget);
    performanceLayout->addWidget(historicalChartWidget);
}


void MainWindow::setupComplexityTab() {
    complexityChart = new QChart();
    complexityChart->setTitle("Algorithmic Complexity Analysis");

    complexityView = new QChartView(complexityChart);
    complexityView->setRenderHint(QPainter::Antialiasing);

    complexityLayout->addWidget(complexityView);
}

void MainWindow::setupManualInputTab() {
    QVBoxLayout* manualLayout = new QVBoxLayout(manualInputTab);

    QGroupBox* inputGroup = new QGroupBox("Ввод данных");
    QVBoxLayout* inputLayout = new QVBoxLayout(inputGroup);

    dataTable = new QTableWidget();
    dataTable->setColumnCount(4);
    dataTable->setHorizontalHeaderLabels({
                                                 "Размер данных",
                                                 "Время MergeSort (мс)",
                                                 "Время HeapSort (мс)",
                                                 "Статус"
                                         });
    dataTable->setAlternatingRowColors(true);
    dataTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    dataTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QGroupBox* previewGroup = new QGroupBox("Предпросмотр данных");
    QVBoxLayout* previewLayout = new QVBoxLayout(previewGroup);

    resultPreview = new QTextEdit();
    resultPreview->setReadOnly(true);
    resultPreview->setMaximumHeight(150);
    previewLayout->addWidget(resultPreview);

    inputLayout->addWidget(dataTable);
    manualLayout->addWidget(inputGroup);
    manualLayout->addWidget(previewGroup);
}



void MainWindow::runSort() {
    sortButton->setEnabled(false);
    QApplication::setOverrideCursor(Qt::WaitCursor);

    try {
        size_t size = customSizeInput->value();

        if (!hasDataSize(size)) {
            auto result = measurePerformance(size);
            historicalData.append(result);
            updateCurrentPerformanceChart(result);
            updateHistoricalPerformanceChart();
            updateComplexityChart();
        } else {
            QMessageBox::warning(this, "Предупреждение",
                                 "Данные для этого размера уже существуют. "
                                 "Выберите другой размер данных.");
        }
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", QString("Analysis failed: ") + e.what());
    }

    sortButton->setEnabled(true);
    QApplication::restoreOverrideCursor();
}

MainWindow::PerformanceResult MainWindow::measurePerformance(size_t size) {
    auto data = readCSV("data/people.csv", size);
    if (!data) {
        throw std::runtime_error("Failed to read data");
    }

    PerformanceResult result;
    result.dataSize = data->GetLength();

    {
        auto dataCopy = *data;
        auto start = std::chrono::high_resolution_clock::now();
        mergeSort.Sort(dataCopy, lessComp);
        auto end = std::chrono::high_resolution_clock::now();
        result.mergeSortTime = std::chrono::duration<double, std::milli>(end - start).count();
    }

    {
        auto dataCopy = *data;
        auto start = std::chrono::high_resolution_clock::now();
        heapSort.Sort(dataCopy, lessComp);
        auto end = std::chrono::high_resolution_clock::now();
        result.heapSortTime = std::chrono::duration<double, std::milli>(end - start).count();
    }

    delete data;
    return result;
}

void MainWindow::updateCurrentPerformanceChart(const PerformanceResult& result) {
    currentPerformanceChart->removeAllSeries();
    while (!currentPerformanceChart->axes().isEmpty()) {
        currentPerformanceChart->removeAxis(currentPerformanceChart->axes().first());
    }

    QBarCategoryAxis *xAxis = new QBarCategoryAxis();
    QValueAxis *yAxis = new QValueAxis();
    yAxis->setTitleText("Time (ms)");
    yAxis->setMin(0);

    currentPerformanceChart->addAxis(xAxis, Qt::AlignBottom);
    currentPerformanceChart->addAxis(yAxis, Qt::AlignLeft);

    QBarSet *mergeSet = new QBarSet("MergeSort");
    QBarSet *heapSet = new QBarSet("HeapSort");

    *mergeSet << result.mergeSortTime;
    *heapSet << result.heapSortTime;

    QBarSeries *series = new QBarSeries();
    series->append(mergeSet);
    series->append(heapSet);

    QStringList categories;
    categories << QString("Size: %1").arg(result.dataSize);
    xAxis->append(categories);

    double maxTime = qMax(result.mergeSortTime, result.heapSortTime);
    yAxis->setRange(0, maxTime * 1.1);

    currentPerformanceChart->addSeries(series);
    series->attachAxis(xAxis);
    series->attachAxis(yAxis);
}

void MainWindow::updateHistoricalPerformanceChart() {
    historicalPerformanceChart->removeAllSeries();
    while (!historicalPerformanceChart->axes().isEmpty()) {
        historicalPerformanceChart->removeAxis(historicalPerformanceChart->axes().first());
    }

    QLineSeries *mergeSeries = new QLineSeries();
    mergeSeries->setName("MergeSort");
    QPen mergePen(Qt::blue);
    mergePen.setWidth(2);
    mergeSeries->setPen(mergePen);

    QLineSeries *heapSeries = new QLineSeries();
    heapSeries->setName("HeapSort");
    QPen heapPen(Qt::red);
    heapPen.setWidth(2);
    heapSeries->setPen(heapPen);

    double maxTime = 0;
    double maxSize = 0;

    QVector<PerformanceResult> sortedData = historicalData;
    std::sort(sortedData.begin(), sortedData.end(),
              [](const PerformanceResult& a, const PerformanceResult& b) {
                  return a.dataSize < b.dataSize;
              });

    for (const auto& result : sortedData) {
        mergeSeries->append(result.dataSize, result.mergeSortTime);
        heapSeries->append(result.dataSize, result.heapSortTime);

        maxTime = qMax(maxTime, qMax(result.mergeSortTime, result.heapSortTime));
        maxSize = qMax(maxSize, double(result.dataSize));
    }

    auto xAxis = new QValueAxis();
    auto yAxis = new QValueAxis();

    xAxis->setTitleText("Размер данных (элементов)");
    yAxis->setTitleText("Время выполнения (мс)");

    xAxis->setGridLineVisible(true);
    yAxis->setGridLineVisible(true);

    int tickCount = 10;
    xAxis->setTickCount(tickCount);
    yAxis->setTickCount(tickCount);

    xAxis->setRange(0, maxSize * 1.1);
    yAxis->setRange(0, maxTime * 1.1);

    historicalPerformanceChart->addAxis(xAxis, Qt::AlignBottom);
    historicalPerformanceChart->addAxis(yAxis, Qt::AlignLeft);

    historicalPerformanceChart->addSeries(mergeSeries);
    historicalPerformanceChart->addSeries(heapSeries);

    mergeSeries->attachAxis(xAxis);
    mergeSeries->attachAxis(yAxis);
    heapSeries->attachAxis(xAxis);
    heapSeries->attachAxis(yAxis);

    historicalPerformanceChart->legend()->setVisible(true);
    historicalPerformanceChart->legend()->setAlignment(Qt::AlignBottom);
}
void MainWindow::updateComplexityChart() {
    complexityChart->removeAllSeries();
    while (!complexityChart->axes().isEmpty()) {
        complexityChart->removeAxis(complexityChart->axes().first());
    }

    QLineSeries *mergeSeries = new QLineSeries();
    mergeSeries->setName("MergeSort (Фактическое)");
    QPen mergePen(Qt::blue);
    mergePen.setWidth(2);
    mergeSeries->setPen(mergePen);

    QLineSeries *heapSeries = new QLineSeries();
    heapSeries->setName("HeapSort (Фактическое)");
    QPen heapPen(Qt::red);
    heapPen.setWidth(2);
    heapSeries->setPen(heapPen);

    QLineSeries *theoreticalSeries = new QLineSeries();
    theoreticalSeries->setName("O(n log n) Теоретическое");
    QPen theoreticalPen(Qt::green);
    theoreticalPen.setWidth(2);
    theoreticalPen.setStyle(Qt::DashLine);
    theoreticalSeries->setPen(theoreticalPen);

    double maxTime = 0;
    double maxSize = 0;

    QVector<PerformanceResult> sortedData = historicalData;
    std::sort(sortedData.begin(), sortedData.end(),
              [](const PerformanceResult& a, const PerformanceResult& b) {
                  return a.dataSize < b.dataSize;
              });

    for (const auto& result : sortedData) {
        mergeSeries->append(result.dataSize, result.mergeSortTime);
        heapSeries->append(result.dataSize, result.heapSortTime);
        maxTime = qMax(maxTime, qMax(result.mergeSortTime, result.heapSortTime));
        maxSize = qMax(maxSize, double(result.dataSize));
    }

    if (!historicalData.isEmpty()) {
        double scale = maxTime / (maxSize * log2(maxSize));
        for (size_t n = 100; n <= maxSize; n += 100) {
            double theoretical = scale * n * log2(n);
            theoreticalSeries->append(n, theoretical);
        }
    }

    auto xAxis = new QValueAxis();
    auto yAxis = new QValueAxis();

    xAxis->setTitleText("Размер входных данных (n)");
    yAxis->setTitleText("Время выполнения (мс)");

    xAxis->setGridLineVisible(true);
    yAxis->setGridLineVisible(true);

    int tickCount = 10;
    xAxis->setTickCount(tickCount);
    yAxis->setTickCount(tickCount);

    xAxis->setRange(0, maxSize * 1.1);
    yAxis->setRange(0, maxTime * 1.1);

    complexityChart->addAxis(xAxis, Qt::AlignBottom);
    complexityChart->addAxis(yAxis, Qt::AlignLeft);

    complexityChart->addSeries(mergeSeries);
    complexityChart->addSeries(heapSeries);
    complexityChart->addSeries(theoreticalSeries);

    mergeSeries->attachAxis(xAxis);
    mergeSeries->attachAxis(yAxis);
    heapSeries->attachAxis(xAxis);
    heapSeries->attachAxis(yAxis);
    theoreticalSeries->attachAxis(xAxis);
    theoreticalSeries->attachAxis(yAxis);

    complexityChart->legend()->setVisible(true);
    complexityChart->legend()->setAlignment(Qt::AlignBottom);
}


void MainWindow::addManualDataPoint() {
    try {
        auto result = measurePerformance(customSizeInput->value());

        int row = dataTable->rowCount();
        dataTable->insertRow(row);

        dataTable->setItem(row, 0, new QTableWidgetItem(QString::number(result.dataSize)));
        dataTable->setItem(row, 1, new QTableWidgetItem(QString::number(result.mergeSortTime, 'f', 2)));
        dataTable->setItem(row, 2, new QTableWidgetItem(QString::number(result.heapSortTime, 'f', 2)));

        QString status = "Успешно ✓";
        dataTable->setItem(row, 3, new QTableWidgetItem(status));

        auto data = readCSV("data/people.csv", result.dataSize);
        if (data) {
            QString preview = QString("Размер данных: %1\nПервые 5 элементов:\n").arg(result.dataSize);

            ArraySequence<Person> sortedData = *data;
            mergeSort.Sort(sortedData, lessComp);



            resultPreview->setText(preview);
            delete data;
        }

        historicalData.append(result);
        updateCharts();

    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error",
                              QString("Failed to add data point: %1").arg(e.what()));
    }
}


void MainWindow::clearData() {
    historicalData.clear();
    dataTable->setRowCount(0);
    updateCharts();
}

void MainWindow::switchDataSource(bool checked) {
    bool isManual = manualRadio->isChecked();
    sortButton->setEnabled(!isManual);
    addDataButton->setEnabled(isManual);
}

void MainWindow::updateCharts() {
    if (!historicalData.isEmpty()) {
        updateCurrentPerformanceChart(historicalData.last());
        updateHistoricalPerformanceChart();
        updateComplexityChart();
    }
}

double MainWindow::calculateTheoretical(size_t n, const QString& complexity) {
    if (complexity == "nlogn") {
        double scale = 0.001;
        return scale * n * qLn(n);
    }
    return 0.0;
}

void MainWindow::updateTheoreticalComplexity() {
    updateComplexityChart();
}