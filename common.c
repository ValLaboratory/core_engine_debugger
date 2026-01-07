#include <iconv.h>

#include "common.h"

#define MAX_BUFFER_SIZE 1024

/**
 * @brief   ログメッセージをコンソールへ表示
 * @param   const char* : メッセージのカテゴリ
 * @param   const char* : ログメッセージ
 * @return  none
 *
 * @copyright   val lab corp.
 * @author      shiohata
 * @data        2025-03-28  新規作成
 */
void stdout_message(const char* sz_category, const char* sz_message) {
    time_t      t           = time(NULL);
    struct tm*  st_local    = localtime(&t);
    char        sz_timestamp[32];

    memset(sz_timestamp, '\0', sizeof(sz_timestamp));
    strftime(sz_timestamp, sizeof(sz_timestamp), "%Y-%m-%d %H:%M:%S", st_local);

    printf("%s: [%s] %s\n", sz_timestamp, sz_category, sz_message);
}

/**
 * @brief   Shift-JIS(CP932) からUTF-8 へ文字コードを変換
 * @param   const char* : 変換対象の文字列
 * @return  char*
 *
 * @copyright   val lab corp.
 * @author      shiohata
 * @data        2026-01-07  新規作成
*/
void convert_sjis2utf8(char* sz_src, char* sz_dst) {
    iconv_t ic = iconv_open("utf8", "cp932");
    if(ic == (iconv_t)-1) {
        stdout_message("ERROR", "failed to initialize iconv.");
        return;
    }

    char*   sz_src_buf  = sz_src;
    char*   sz_dst_buf  = sz_dst;
    size_t  src_size    = (size_t)MAX_BUFFER_SIZE;
    size_t  dst_size    = (size_t)MAX_BUFFER_SIZE;
    if(iconv(ic, &sz_src_buf, &src_size, &sz_dst_buf, &dst_size) == -1) {
        stdout_message("ERROR", "failed to convert strings.");
        return;
    }
    iconv_close(ic);
}