/* 標準ヘッダー */
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <fnmatch.h>
#include <err.h>

/* core_engine 定義ヘッダー */
#include "ExpType.h"
#include "ExpPublic.h"

/* 設定ファイル */
#include "exp_configure.h"
#include "common.h"

/* 定数宣言 */
#define _DEBUG          1
#define _MAX_FILE_PATH  256
#define _KNB_EXT        "knb"
#define _DIA_EXT        "dia"

/* 構造体 */
struct ExpEntryList {
    ExpDataFileList     exp_knb;
    ExpDiaDataFileList  exp_dia_db;
};

/* knb, dia の初期化 */
ExpBool initialize_exp_database(ExpDataHandler* h_knb_data, ExpDiaDBHandler* h_dia_db, ExpBool b_on_memory);

/* エントリーリストの生成 */
struct ExpEntryList generate_entry_list(const char* sz_expcache_path, ExpBool b_on_memory);
ExpVoid* generate_list(const char* sz_expcache_path, const char* sz_ext, ExpBool b_on_memory);

/* 各種エントリーリストを破棄 */
void delete_entry_list(struct ExpEntryList st_entry_list);

/* データの操作ハンドラを破棄 */
void terminate_databases_handler(ExpDiaDBHandler h_dia_db, ExpDataHandler h_knb_data);

/* 動作確認 */
void check_exp_initialize(ExpDataHandler* h_knb_data, ExpDiaDBHandler* h_dia_db);
