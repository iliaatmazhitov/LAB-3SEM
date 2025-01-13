//
// Created by Илья Атмажитов on 19.10.2024.
//

#include "../include/UI.h"


using namespace std;

void Initialize() {
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
}

void RunTimingTests() {
    endwin();
    TimingResults results = runTimingTests();

    std::cout << "Результаты замеров времени:\n";
    for (size_t i = 0; i < results.smartDynamic.size(); ++i) {
        std::cout << "Размер: " << results.smartDynamic[i].first << "\n";
        std::cout << "Smart Dynamic: " << results.smartDynamic[i].second << "с\n";
        std::cout << "Smart Linked: " << results.smartLinked[i].second << "с\n";
        std::cout << "Weak Dynamic: " << results.weakDynamic[i].second << "с\n";
        std::cout << "Weak Linked: " << results.weakLinked[i].second << "с\n\n";
    }

    plotTimingResults(results);
    Initialize();
}




void RunTests() {
    TestNullPointer();
    TestCopyConstruction();
    TestMoveConstruction();
    TestAssignmentCopy();
    TestAssignmentMove();
    TestWeakPointerBasics();
    TestWeakPointerLock();
    TestWeakPointerExpired();
    TestWeakPointerSelfAssignment();
    TestMultipleWeakPointers();
    TestMultipleReferences();
    TestAssignmentOfNull();
    TestResetFunctionality();
    TestDereferenceOperator();
    TestComparisonOperators();
    TestSelfAssignmentCopy();
    mvprintw(22, 0, "All tests passed! Press any key to continue.");
    getch();
}


