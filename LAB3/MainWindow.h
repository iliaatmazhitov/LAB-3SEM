//
// Created by Илья Атмажитов on 17.01.2025.
//

#ifndef LABORATORY3_MAINWINDOW_H
#define LABORATORY3_MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QSpinBox>
#include <QTableWidget>
#include <QLabel>
#include <QMessageBox>
#include "backpack.h"
#include "MemoryManager.h"


struct MemoryBlock {
    void* ptr;
    size_t size;
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr) : QMainWindow(parent) {
        setWindowTitle("Algorithm Visualizer");
        resize(800, 600);

        QTabWidget* tabWidget = new QTabWidget(this);
        setCentralWidget(tabWidget);

        QWidget* backpackTab = createBackpackTab();
        QWidget* memoryTab = createMemoryTab();

        tabWidget->addTab(backpackTab, "Backpack Problem");
        tabWidget->addTab(memoryTab, "Memory Manager");
    }

private:
    QTableWidget* itemsTable;
    QTableWidget* resultTable;
    QSpinBox* capacityInput;
    ArraySequence<Item> items;

    QTableWidget* memoryTable;
    MemoryManager* manager = nullptr;
    QVector<MemoryBlock> allocatedBlocks;

    QWidget* createBackpackTab() {
        QWidget* tab = new QWidget();
        QVBoxLayout* layout = new QVBoxLayout(tab);

        QHBoxLayout* capacityLayout = new QHBoxLayout();
        QLabel* capacityLabel = new QLabel("Backpack Capacity:");
        capacityInput = new QSpinBox();
        capacityInput->setRange(1, 100000);
        capacityInput->setValue(2000);
        capacityLayout->addWidget(capacityLabel);
        capacityLayout->addWidget(capacityInput);
        capacityLayout->addStretch();
        layout->addLayout(capacityLayout);

        itemsTable = new QTableWidget();
        itemsTable->setColumnCount(2);
        itemsTable->setHorizontalHeaderLabels({"Price", "Weight"});
        layout->addWidget(itemsTable);

        QHBoxLayout* buttonLayout = new QHBoxLayout();
        QPushButton* addItemBtn = new QPushButton("Add Item");
        QPushButton* removeItemBtn = new QPushButton("Remove Selected");
        QPushButton* solveBtn = new QPushButton("Solve Backpack");
        buttonLayout->addWidget(addItemBtn);
        buttonLayout->addWidget(removeItemBtn);
        buttonLayout->addWidget(solveBtn);
        layout->addLayout(buttonLayout);

        QLabel* resultLabel = new QLabel("Optimal Solution:");
        layout->addWidget(resultLabel);
        resultTable = new QTableWidget();
        resultTable->setColumnCount(2);
        resultTable->setHorizontalHeaderLabels({"Price", "Weight"});
        layout->addWidget(resultTable);

        connect(addItemBtn, &QPushButton::clicked, this, &MainWindow::addBackpackItem);
        connect(removeItemBtn, &QPushButton::clicked, this, &MainWindow::removeBackpackItem);
        connect(solveBtn, &QPushButton::clicked, this, &MainWindow::solveBackpack);

        addSampleItems();

        return tab;
    }

    QWidget* createMemoryTab() {
        QWidget* tab = new QWidget();
        QVBoxLayout* layout = new QVBoxLayout(tab);

        QHBoxLayout* sizeLayout = new QHBoxLayout();
        QLabel* sizeLabel = new QLabel("Memory Size:");
        QSpinBox* sizeInput = new QSpinBox();
        sizeInput->setRange(1, 1000);
        sizeInput->setValue(100);
        QPushButton* initBtn = new QPushButton("Initialize Memory");
        sizeLayout->addWidget(sizeLabel);
        sizeLayout->addWidget(sizeInput);
        sizeLayout->addWidget(initBtn);
        sizeLayout->addStretch();
        layout->addLayout(sizeLayout);

        QHBoxLayout* allocLayout = new QHBoxLayout();
        QLabel* allocLabel = new QLabel("Allocation Size:");
        QSpinBox* allocSize = new QSpinBox();
        allocSize->setRange(1, 1000);
        QPushButton* allocBtn = new QPushButton("Allocate");
        allocLayout->addWidget(allocLabel);
        allocLayout->addWidget(allocSize);
        allocLayout->addWidget(allocBtn);
        allocLayout->addStretch();
        layout->addLayout(allocLayout);

        memoryTable = new QTableWidget();
        memoryTable->setColumnCount(3);
        memoryTable->setHorizontalHeaderLabels({"Block ID", "Size", "Actions"});
        layout->addWidget(memoryTable);

        connect(initBtn, &QPushButton::clicked, [=]() {
            if (manager) delete manager;
            manager = new MemoryManager(static_cast<size_t>(sizeInput->value()));
            allocatedBlocks.clear();
            updateMemoryView();
        });

        connect(allocBtn, &QPushButton::clicked, [=]() {
            if (!manager) {
                QMessageBox::warning(this, "Error", "Initialize memory first!");
                return;
            }

            size_t allocationSize = static_cast<size_t>(allocSize->value());
            void* ptr = manager->allocate(allocationSize);
            if (ptr) {
                allocatedBlocks.append({ptr, allocationSize});
                updateMemoryView();
            } else {
                QMessageBox::warning(this, "Allocation Failed",
                                     "Not enough continuous memory for allocation.");
            }
        });

        return tab;
    }

    void addSampleItems() {
        items.Append(Item{100, 200});
        items.Append(Item{500, 2000});
        items.Append(Item{150, 2001});
        items.Append(Item{1060, 2050});
        items.Append(Item{1100, 2002});
        items.Append(Item{1030, 2005});
        updateItemsTable();
    }

    void updateItemsTable() {
        itemsTable->setRowCount(items.GetLength());
        for (int i = 0; i < items.GetLength(); i++) {
            itemsTable->setItem(i, 0, new QTableWidgetItem(QString::number(items.Get(i).price)));
            itemsTable->setItem(i, 1, new QTableWidgetItem(QString::number(items.Get(i).weight)));
        }
    }

    void updateMemoryView() {
        memoryTable->setRowCount(0);

        for (int i = 0; i < allocatedBlocks.size(); ++i) {
            const auto& block = allocatedBlocks[i];
            int row = memoryTable->rowCount();
            memoryTable->insertRow(row);

            memoryTable->setItem(row, 0,
                                 new QTableWidgetItem(QString("Block %1").arg(i + 1)));

            memoryTable->setItem(row, 1,
                                 new QTableWidgetItem(QString::number(block.size)));

            QPushButton* deallocBtn = new QPushButton("Deallocate");
            memoryTable->setCellWidget(row, 2, deallocBtn);

            connect(deallocBtn, &QPushButton::clicked, [=]() {
                manager->deallocate(block.ptr);
                allocatedBlocks.remove(i);
                updateMemoryView();
            });
        }
    }

private slots:
    void addBackpackItem() {
        int row = itemsTable->rowCount();
        items.Append(Item{0, 0});
        updateItemsTable();
    }

    void removeBackpackItem() {
        int row = itemsTable->currentRow();
        if (row >= 0) {
            ArraySequence<Item> newItems;
            for (int i = 0; i < items.GetLength(); i++) {
                if (i != row) {
                    newItems.Append(items.Get(i));
                }
            }
            items = newItems;
            updateItemsTable();
        }
    }

    void solveBackpack() {
        ArraySequence<Item> currentItems;
        for (int i = 0; i < itemsTable->rowCount(); i++) {
            Item item;
            item.price = itemsTable->item(i, 0)->text().toUInt();
            item.weight = itemsTable->item(i, 1)->text().toUInt();
            currentItems.Append(item);
        }

        Backpack backpack(currentItems, static_cast<size_t>(capacityInput->value()));
        ArraySequence<Item> result = backpack.dynamic_backpack();

        resultTable->setRowCount(result.GetLength());
        size_t totalPrice = 0, totalWeight = 0;
        for (int i = 0; i < result.GetLength(); i++) {
            resultTable->setItem(i, 0, new QTableWidgetItem(QString::number(result.Get(i).price)));
            resultTable->setItem(i, 1, new QTableWidgetItem(QString::number(result.Get(i).weight)));
            totalPrice += result.Get(i).price;
            totalWeight += result.Get(i).weight;
        }

        int totalRow = resultTable->rowCount();
        resultTable->insertRow(totalRow);
        resultTable->setItem(totalRow, 0, new QTableWidgetItem(QString("Total: %1").arg(totalPrice)));
        resultTable->setItem(totalRow, 1, new QTableWidgetItem(QString("Total: %1").arg(totalWeight)));
    }
};


#endif //LABORATORY3_MAINWINDOW_H
