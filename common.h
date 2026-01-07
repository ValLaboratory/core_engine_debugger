/* 標準ヘッダー */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/* ログを出力 */
void stdout_message(const char* sz_category, const char* sz_message);

/* Shift-JIS(CP932) からUTF-8 へ文字列を変換*/
void convert_sjis2utf8(char *sz_str, char *sz_dst);