#include <vector>
#include <iostream>

using vecI = std::vector<int>;
void printArr(const vecI&);

void merge(vecI& data, int left, int mid, int right) {
  int sizeL = mid - left + 1;
  int sizeR = right - mid;

  vecI lPart(sizeL), rPart(sizeR);
  //  auto begin = data.cbegin() + left;
  //  auto end = data.cbegin() + left + sizeL;
  //  std::copy(begin, end, lPart.begin());
  //
  //  begin = data.cbegin() + mid + 1;
  //  end = data.cbegin() + mid + 1 + sizeR;
  //  std::copy(begin, end, rPart.begin());

  for (int i = 0; i < sizeL; ++i) {
    lPart[i] = data[left + i];
  }
  for (int j = 0; j < sizeR; ++j) {
    rPart[j] = data[mid + 1 + j];
  }

  int lIdx = 0, rIdx = 0, dataIdx = left;

  while (lIdx < sizeL && rIdx < sizeR) {
    if (lPart[lIdx] <= rPart[rIdx]) {
      data[dataIdx] = lPart[lIdx];
      ++lIdx;
    } else {
      data[dataIdx] = rPart[rIdx];
      ++rIdx;
    }
    ++dataIdx;
  }

  while (lIdx < sizeL) {
    data[dataIdx] = lPart[lIdx];
    lIdx++, dataIdx++;
  }

  while (rIdx < sizeR) {
    data[dataIdx] = rPart[rIdx];
    rIdx++, dataIdx++;
  }

//  std::copy(lPart.begin() + lIdx, lPart.end(), data.begin() + dataIdx);
//  std::copy(rPart.begin() + rIdx, rPart.end(), data.begin() + sizeL + dataIdx - 1);
}

void merge_sort(vecI& data, int left, int right) {
  if (left < right) {
    int mid = left + (right - left) / 2;

    merge_sort(data, left, mid);
    merge_sort(data, mid + 1, right);

    merge(data, left, mid, right);
  }
}

void test_merge_sort() {
  std::cout << "test_merge_sort" << std::endl;
  std::vector<int> arr { 9, -3, 5, 2, 6, 8, -6, 1, 3 };
  int right = static_cast<int>(arr.size()) - 1;
  std::cout << "Before: ";
  printArr(arr);
  merge_sort(arr, 0, right);
  std::cout << "After: ";
  printArr(arr);
}