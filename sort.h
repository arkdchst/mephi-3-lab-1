#pragma once

#include "sequence.h"

template <typename T>
Sequence<T>* bubbleSort(Sequence<T> *seq, int (*cmp)(T, T));

template <typename T>
Sequence<T>* quickSort(Sequence<T> *seq, int (*cmp)(T, T));

template <typename T>
Sequence<T>* shakerSort(Sequence<T> *seq, int (*cmp)(T, T));