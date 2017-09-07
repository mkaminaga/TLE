﻿  // @file tle.cc
  // @brief TLE elements resolver.
  // @author Mamoru Kaminaga
  // @date 2017-09-07 12:52:18
  // Copyright 2017 Mamoru Kaminaga
#include <assert.h>
#include <wchar.h>
#include <string>
#include "./tle.h"
#define S_MAX  (16)
#if 0
struct InputTLE {
  std::wstring line_0;
  std::wstring line_1;
  std::wstring line_2;
};
struct OutputTLE {
  int num;  // line 1, column 1-2.
  int catalog_num;  // line 1, column 3-7.
  int id_1;  // line 1, column 10-11.
  int id_2;  // line 1, column 12-14.
  int id_3;  // line 1, column 15-18.
  int model;  // line 1, column 63.
  int s_num;  // line 1, column 65-68.
  int epoch_1;  // line 1, column 19-21.
  int check_sum_1;  // line 1, column 69.
  int check_sum_2;  // line 2, column 69.
  char mil_level;  // line 1, column 8-9.
  double epoch_2;  // line 1, column 21-33.
  double mm_1;  // line 1, column 34-44.
  double mm_2;  // line 1, column 45-53.
  double b_star;  // line 1, column 54-62.
  double incl;  // line 2, column 9-17.
  double raan;  // line 2, column 9-26.
  double eccn;  // line 2, column 27-34.
  double argp;  // line 2, column 35-43.
  double ma;  // line 2, column 44-52.
  double mm;  // line 2, column 53-63.
  double rev;  // line 2, column 64-68.
  std::wstring name;  // line 0, column 1-24.
};
#endif
namespace {
  // The wtoi with range.
int wtoi2(const wchar_t* s, int curs, int len) {
  if ((curs < 0) || (len < 1) || (curs + len) > static_cast<int>(wcslen(s))) {
    return -1;
  }
  wchar_t buf[S_MAX] = {0};
  for (int i = 0; i < len; ++i) buf[i] = s[curs + i];
  buf[len + 1] = L'\0';
  return _wtoi(buf);
}
  // The wtof with range.
double wtof2(const wchar_t* s, int curs, int len) {
  if ((curs < 0) || (len < 1) || (curs + len) > static_cast<int>(wcslen(s))) {
    return -1;
  }
  wchar_t buf[S_MAX] = {0};
  for (int i = 0; i < len; ++i) buf[i] = s[curs + i];
  buf[len + 1] = L'\0';
  return _wtof(buf);
}
};
bool ReadTLE(const InputTLE& input, OutputTLE* output) {
  assert(output);
  wchar_t buf_1[9][S_MAX] = {0};
  wchar_t buf_2[10][S_MAX] = {0};
  wchar_t tmp[S_MAX] = {0};
  // The line 0 is processed.
  output->name = input.line_0;
#if 1
  wprintf(L"Debug:\n");
  wprintf(L"line_0:%s\n", input.line_0.c_str());
#endif
  // The line 1 is processed.
  swscanf_s(input.line_1.c_str(), L"%s%s%s%s%s%s%s%s%s",
            buf_1[0], S_MAX, buf_1[1], S_MAX, buf_1[2], S_MAX, buf_1[3], S_MAX,
            buf_1[4], S_MAX, buf_1[5], S_MAX, buf_1[6], S_MAX, buf_1[7], S_MAX,
            buf_1[8], S_MAX);
#if 1
  wprintf(L"line_1:%s\n", input.line_1.c_str());
  wprintf(L"buf_1[0]:%s\n", buf_1[0]);
  wprintf(L"buf_1[1]:%s\n", buf_1[1]);
  wprintf(L"buf_1[2]:%s\n", buf_1[2]);
  wprintf(L"buf_1[3]:%s\n", buf_1[3]);
  wprintf(L"buf_1[4]:%s\n", buf_1[4]);
  wprintf(L"buf_1[5]:%s\n", buf_1[5]);
  wprintf(L"buf_1[6]:%s\n", buf_1[6]);
  wprintf(L"buf_1[7]:%s\n", buf_1[7]);
  wprintf(L"buf_1[8]:%s\n", buf_1[8]);
#endif
  // The catalog number.
  output->catalog_num = wtoi2(buf_1[1], 0, 5);
  // The military secret level.
  output->mil_level = buf_1[1][5];
  // Ids.
  output->id_1 = wtoi2(buf_1[2], 0, 2);
  output->id_2 = wtoi2(buf_1[2], 2, 3);
  output->id_3[0] = buf_1[2][5];
  output->id_3[1] = buf_1[2][6];
  output->id_3[2] = buf_1[2][7];
  // The epoch.
  output->epoch_1 = wtoi2(buf_1[3], 0, 2);
  output->epoch_2 = wtof2(buf_1[3], 2, 12);
  // The mean motion differential level 1.
  if (buf_1[4][0] == L'-') {
    tmp[0] = L'-';
    tmp[1] = L'0';
    tmp[2] = L'.';
    for (int i = 0; i < 8; ++i) tmp[3 + i] = buf_1[4][2 + i];
  } else {
    tmp[0] = L'0';
    tmp[1] = L'.';
    for (int i = 0; i < 8; ++i) tmp[2 + i] = buf_1[4][1 + i];
  }
#if 1
  wprintf(L"mm_1 tmp:%s\n", tmp);
#endif
  output->mm_1 = _wtof(tmp);
  // The mean motion differential level 2.
  if (buf_1[5][0] == L'-') {
    tmp[0] = L'-';
    tmp[1] = L'0';
    tmp[2] = L'.';
    for (int i = 0; i < 5; ++i) tmp[3 + i] = buf_1[5][1 + i];
    tmp[8] = L'E';
    tmp[9] = buf_1[5][6];  // L'+' or L'-'.
    tmp[10] = buf_1[5][7];
  } else {
    tmp[0] = L'0';
    tmp[1] = L'.';
    for (int i = 0; i < 5; ++i) tmp[2 + i] = buf_1[5][i];
    tmp[7] = L'E';
    tmp[8] = buf_1[5][6];  // L'+' or L'-'.
    tmp[9] = buf_1[5][7];
  }
#if 1
  wprintf(L"mm_2 tmp:%s\n", tmp);
#endif
  output->mm_2 = _wtof(tmp);
  // The line 2 is processed.
  return true;
}