//
// Created by Илья Атмажитов on 16.01.2025.
//

#ifndef LABORATORY2_MAINWINDOW_H
#define LABORATORY2_MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QBarCategoryAxis>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QTabWidget>
#include <QTableWidget>
#include <QGroupBox>
#include <QTextEdit>
#include <QRadioButton>
#include <memory>
#include "sequences/ArraySequence.h"
#include "algorithms/Sorts.h"
#include "Person.h"

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;

private slots:
    void runSort();
    void updateCharts();
    void addManualDataPoint();
    void clearData();
    void switchDataSource(bool checked);
    void updateTheoreticalComplexity();

private:
    bool hasDataSize(size_t size) const {
        return std::any_of(historicalData.begin(), historicalData.end(),
                           [size](const PerformanceResult& result) {
                               return result.dataSize == size;
                           });
    }
    QVBoxLayout* mainLayout;

    QTextEdit* resultPreview;
    struct PerformanceResult {
        double mergeSortTime{0.0};
        double heapSortTime{0.0};
        size_t dataSize{0};
    };


    QWidget* centralWidget{nullptr};
    QTabWidget* tabWidget{nullptr};

    QWidget* performanceTab{nullptr};
    QVBoxLayout* performanceLayout{nullptr};
    QChartView* currentPerformanceView{nullptr};
    QChartView* historicalPerformanceView{nullptr};
    QChart* currentPerformanceChart{nullptr};
    QChart* historicalPerformanceChart{nullptr};

    QWidget* complexityTab{nullptr};
    QVBoxLayout* complexityLayout{nullptr};
    QChartView* complexityView{nullptr};
    QChart* complexityChart{nullptr};

    QWidget* manualInputTab{nullptr};
    QVBoxLayout* manualInputLayout{nullptr};
    QTableWidget* dataTable{nullptr};

    QGroupBox* dataSourceGroup{nullptr};
    QRadioButton* csvRadio{nullptr};
    QRadioButton* manualRadio{nullptr};
    QComboBox* sizeSelector{nullptr};
    QSpinBox* customSizeInput{nullptr};
    QPushButton* sortButton{nullptr};
    QPushButton* addDataButton{nullptr};
    QPushButton* clearDataButton{nullptr};

    MergeSort<Person> mergeSort;
    HeapSort<Person> heapSort;
    Less<Person> lessComp;
    QVector<PerformanceResult> historicalData;

    void setupUI();
    void setupCharts();
    void setupComplexityTab();
    void setupManualInputTab();
    void setupControls();

    ArraySequence<Person>* readCSV(const QString& filename, int limit);
    PerformanceResult measurePerformance(size_t size);

    void updateCurrentPerformanceChart(const PerformanceResult& result);
    void updateHistoricalPerformanceChart();
    void updateComplexityChart();

    double calculateTheoretical(size_t n, const QString& complexity);
};


#endif //LABORATORY2_MAINWINDOW_H
