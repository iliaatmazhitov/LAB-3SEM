//
// Created by Илья Атмажитов on 15.12.2024.
//


#ifndef LABORATORY4_TESTS_H
#define LABORATORY4_TESTS_H

#include "data_structures/Graph.h"
#include "algorithms.h"
#include <cassert>
#include <chrono>
#include <random>

class GraphTester {
private:
    static std::string generateRandomString(size_t length) {
        static const char alphanum[] =
                "0123456789"
                "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                "abcdefghijklmnopqrstuvwxyz";
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, sizeof(alphanum) - 2);

        std::string result;
        result.reserve(length);
        for (size_t i = 0; i < length; ++i) {
            result += alphanum[dis(gen)];
        }
        return result;
    }

public:
    static void RunAllTests() {
        TestBasicOperations();
        TestBFS();
        TestLoadGraph();
        std::cout << "All tests passed successfully!" << std::endl;
    }

    static void TestBasicOperations() {
        OrientedGraph<std::string> graph;

        graph.AddVertex(1, "A");
        graph.AddVertex(2, "B");
        graph.AddVertex(3, "C");

        graph.AddEdge(1, 2, 5);
        graph.AddEdge(2, 3, 3);
        graph.AddEdge(1, 3, 7);

        auto neighbors = graph.GetNeighbours(1);
        assert(neighbors.GetLength() == 2);
        assert(neighbors.Get(0).to == 2);
        assert(neighbors.Get(0).weight == 5);
        assert(neighbors.Get(1).to == 3);
        assert(neighbors.Get(1).weight == 7);

        std::cout << "Basic operations test passed!" << std::endl;
    }

    static void TestBFS() {
        OrientedGraph<std::string> graph;

        graph.AddVertex(1, "A");
        graph.AddVertex(2, "B");
        graph.AddVertex(3, "C");
        graph.AddVertex(4, "D");

        graph.AddEdge(1, 2);
        graph.AddEdge(1, 3);
        graph.AddEdge(2, 4);
        graph.AddEdge(3, 4);

        auto bfsResult = BFS(graph, 1);
        assert(bfsResult.GetLength() == 4);
        assert(bfsResult.Get(0) == 1);  // Starting vertex
        assert(bfsResult.Get(1) == 2 || bfsResult.Get(1) == 3);  // Second level
        assert(bfsResult.Get(2) == 2 || bfsResult.Get(2) == 3);  // Second level
        assert(bfsResult.Get(3) == 4);  // Third level

        std::cout << "BFS test passed!" << std::endl;
    }

    static void TestLoadGraph() {
        OrientedGraph<std::string> graph;
        const int NUM_VERTICES = 1000;
        const int NUM_EDGES = 5000;

        auto start = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < NUM_VERTICES; i++) {
            graph.AddVertex(i, generateRandomString(5));
        }

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> vertex_dis(0, NUM_VERTICES - 1);
        std::uniform_int_distribution<> weight_dis(1, 100);

        for (int i = 0; i < NUM_EDGES; i++) {
            int from = vertex_dis(gen);
            int to = vertex_dis(gen);
            graph.AddEdge(from, to, weight_dis(gen));
        }

        auto bfsResult = BFS(graph, 0);

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        std::cout << "Load test completed in " << duration.count() << "ms" << std::endl;
        std::cout << "Vertices: " << NUM_VERTICES << ", Edges: " << NUM_EDGES << std::endl;
        std::cout << "BFS visited " << bfsResult.GetLength() << " vertices" << std::endl;
    }
};

#endif //LABORATORY4_TESTS_H
