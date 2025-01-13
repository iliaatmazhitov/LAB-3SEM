//
// Created by Илья Атмажитов on 15.12.2024.
//

#ifndef LABORATORY4_UI_H
#define LABORATORY4_UI_H

#include <iostream>
#include "data_structures/Graph.h"
#include "algorithms.h"
#include "tests.h"

void printMenu() {
    std::cout << "\nOriented Graph Operations:\n";
    std::cout << "1. Add vertex\n";
    std::cout << "2. Add edge\n";
    std::cout << "3. Perform BFS\n";
    std::cout << "4. Save graph visualization\n";
    std::cout << "5. Run tests\n";
    std::cout << "0. Exit\n";
    std::cout << "Enter your choice: ";
}

void handleAddVertex(OrientedGraph<std::string>& graph) {
    size_t id;
    std::string value;

    std::cout << "Enter vertex ID: ";
    std::cin >> id;
    std::cout << "Enter vertex value: ";
    std::cin >> value;

    try {
        graph.AddVertex(id, value);
        std::cout << "Vertex added successfully!\n";
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
}

void handleAddEdge(OrientedGraph<std::string>& graph) {
    size_t from, to;
    int64_t weight;

    std::cout << "Enter source vertex ID: ";
    std::cin >> from;
    std::cout << "Enter destination vertex ID: ";
    std::cin >> to;
    std::cout << "Enter edge weight: ";
    std::cin >> weight;

    try {
        graph.AddEdge(from, to, weight);
        std::cout << "Edge added successfully!\n";
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
}

void handleBFS(const OrientedGraph<std::string>& graph) {
    size_t startId;
    std::cout << "Enter starting vertex ID for BFS: ";
    std::cin >> startId;

    try {
        ArraySequence<size_t> result = BFS(graph, startId);
        std::cout << "BFS traversal order: ";
        for (size_t i = 0; i < result.GetLength(); i++) {
            std::cout << result.Get(i) << " ";
        }
        std::cout << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
}

void handleSaveVisualization(const OrientedGraph<std::string>& graph) {
    std::string filename = "graph";

    try {
        graph.GenerateGraphvizDot();
        graph.SaveGraphvizFile(filename + ".dot");
        graph.RenderGraph(filename);
        std::cout << "Graph visualization saved as " << filename << ".png\n";
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
}




#endif //LABORATORY4_UI_H
