#include "common.h"

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
