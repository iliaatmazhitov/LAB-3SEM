//
//  tests.cpp
//  laboratory1
//
//  Created by Илья Атмажитов on 11.10.2024.
//

#include "../include/SmartPointer.h"
#include <cassert>
#include <iostream>



void TestNullPointer() {
    SmrtPtr<int> ptr;
    assert(!ptr);
    assert(ptr.GetRefCount() == 0);
    assert(ptr.GetWeakCount() == 0);
}

void TestCopyConstruction() {
    SmrtPtr<int> ptr1(new int(42));
    SmrtPtr<int> ptr2(ptr1);
    assert(ptr1.GetRefCount() == 2);
    assert(ptr2.GetRefCount() == 2);
    assert(*ptr1 == 42);
    assert(ptr1 == ptr2);
}

void TestMoveConstruction() {
    SmrtPtr<int> ptr1(new int(100));
    SmrtPtr<int> ptr2(std::move(ptr1));
    assert(!ptr1);
    assert(ptr2.GetRefCount() == 1);
    assert(*ptr2 == 100);
}

void TestAssignmentCopy() {
    SmrtPtr<int> ptr1(new int(20));
    SmrtPtr<int> ptr2;
    ptr2 = ptr1;
    assert(ptr1.GetRefCount() == 2);
    assert(ptr2.GetRefCount() == 2);
    assert(*ptr1 == 20);
    assert(ptr1 == ptr2);
}

void TestAssignmentMove() {
    SmrtPtr<int> ptr1(new int(30));
    SmrtPtr<int> ptr2(new int(40));
    ptr2 = std::move(ptr1);
    assert(!ptr1);
    assert(ptr2.GetRefCount() == 1);
    assert(*ptr2 == 30);
}

void TestWeakPointerBasics() {
    SmrtPtr<int> ptr(new int(5));
    WeakPtr<int> weak(ptr);
    assert(weak.GetRefCount() == 1);
    assert(weak.GetWeakCount() == 1);
    assert(!weak.Expired());
}

void TestWeakPointerLock() {
    SmrtPtr<int> ptr(new int(15));
    WeakPtr<int> weak(ptr);
    SmrtPtr<int> locked = weak.Lock();
    assert(locked);
    assert(locked.GetRefCount() == 2);
    assert(*locked == 15);
}

void TestWeakPointerExpired() {
    WeakPtr<int> weak;
    {
        SmrtPtr<int> ptr(new int(50));
        weak = ptr;
    }
    assert(weak.Expired());
    SmrtPtr<int> locked = weak.Lock();
    assert(!locked);
}

void TestWeakPointerSelfAssignment() {
    SmrtPtr<int> ptr(new int(60));
    WeakPtr<int> weak(ptr);
    weak = weak; // Self-assignment
    assert(weak.GetWeakCount() == 1);
    assert(!weak.Expired());
}

void TestMultipleWeakPointers() {
    SmrtPtr<int> ptr(new int(70));
    WeakPtr<int> weak1(ptr);
    WeakPtr<int> weak2(ptr);
    assert(ptr.GetWeakCount() == 2);
    assert(!weak1.Expired());
    assert(!weak2.Expired());
}

void TestMultipleReferences() {
    SmrtPtr<int> ptr1(new int(80));
    SmrtPtr<int> ptr2 = ptr1;
    SmrtPtr<int> ptr3 = ptr2;
    assert(ptr1.GetRefCount() == 3);
    assert(ptr2.GetRefCount() == 3);
    assert(ptr3.GetRefCount() == 3);
}

void TestAssignmentOfNull() {
    SmrtPtr<int> ptr1(new int(90));
    SmrtPtr<int> ptr2;
    ptr1 = ptr2; // Assigning null
    assert(!ptr1);
    assert(ptr2.GetRefCount() == 0);
}

void TestResetFunctionality() {
    SmrtPtr<int> ptr(new int(100));
    ptr = nullptr;
    assert(!ptr);
    assert(ptr.GetRefCount() == 0);
}

void TestDereferenceOperator() {
    SmrtPtr<int> ptr(new int(110));
    assert(*ptr == 110);
    *ptr = 120;
    assert(*ptr == 120);
}

void TestComparisonOperators() {
    SmrtPtr<int> ptr1(new int(130));
    SmrtPtr<int> ptr2(ptr1);
    SmrtPtr<int> ptr3(new int(130));
    assert(ptr1 == ptr2);
    assert(ptr1 != ptr3);
}

void TestSelfAssignmentCopy() {
    SmrtPtr<int> ptr(new int(140));
    ptr = ptr; // Self-assignment
    assert(ptr.GetRefCount() == 1);
    assert(*ptr == 140);
}



