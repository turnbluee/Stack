#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "stackUserHeader.h"
#include "stackFuncsHeader.h"

static void test_int_stack(void) {
    printf("=== Testing int stack ===\n");
    Stack *s = malloc(sizeof *s);
    assert(s);

    // 1. Конструктор и начальный размер
    Construct(s, sizeof(int));
    assert(Size(s) == 0);
    printf("Construct: OK (size=0)\n");

    // 2. Push & automatic Extend
    const int initial = INITIAL_ELEM_COUNT;
    const int N = initial * 2 + 5;  // чуть больше, чтобы проверить Extend
    for (int i = 0; i < N; i++) {
        Push(s, &i);
        assert(Size(s) == i + 1);
    }
    printf("Push %d elements: OK (size=%d)\n", N, Size(s));

    // 3. Попробуем вывести весь стек (снизу‑вверх)
    printf("Current stack (top→bottom): ");
    PrintStack(s);  // должно распечатать N−1, N−2, …, 0

    // 4. Pop & проверка LIFO
    for (int i = N - 1; i >= 0; i--) {
        int x = 0;
        Pop(s, &x);
        assert(x == i);
    }
    printf("Pop all elements: OK (size=%d)\n", Size(s));
    assert(Size(s) == 0);

    // 5. Проверка Pop из пустого стека — должна фатально завершиться
    int dummy;
    Pop(s, &dummy);

    Destruct(s);
    printf("Destruct: OK\n\n");
}

static void test_char_stack(void) {
    printf("=== Testing char stack ===\n");
    Stack *s = malloc(sizeof *s);
    Construct(s, sizeof(char));
    assert(Size(s) == 0);

    const char data[] = "HELLO";
    for (size_t i = 0; i < sizeof data - 1; i++) {
        Push(s, &data[i]);
    }
    printf("Pushed string: ");
    PrintStack(s);  // prints O L L E H

    for (size_t i = 0; i < sizeof data - 1; i++) {
        char c = 0;
        Pop(s, &c);
        assert(c == data[sizeof data - 2 - i]);
    }
    Destruct(s);
    printf("Char stack OK\n\n");
}

static void test_double_stack(void) {
    printf("=== Testing double stack ===\n");
    Stack *s = malloc(sizeof *s);
    Construct(s, sizeof(double));
    assert(Size(s) == 0);

    double vals[] = {3.14, 2.718, 1.414, 0.577};
    for (size_t i = 0; i < sizeof vals/sizeof *vals; i++) {
        Push(s, &vals[i]);
    }
    printf("Pushed doubles: ");
    PrintStack(s);

    for (int i = (int)(sizeof vals/sizeof *vals) - 1; i >= 0; i--) {
        double d = 0;
        Pop(s, &d);
        assert(d == vals[i]);
    }
    Destruct(s);
    printf("Double stack OK\n\n");
}

int main(void) {
    test_int_stack();
    test_char_stack();
    test_double_stack();
    printf("All stack tests passed!\n");
    return 0;
}
