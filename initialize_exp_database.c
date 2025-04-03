#include "initialize_exp_database.h"

/**
 * @brief   exp database の初期化
 * @param   ExpDataHandler* : knb データハンドラのポインタ
 * @param   ExpDiaDBHandler* : dia DB ハンドラのポインタ
 * @return  ExpBool : 成否判定
 *
 * @copyright   val lab corp.
 * @author      shiohata
 * @data        2025-03-27  新規作成
 */
ExpBool initialize_exp_database(ExpDataHandler* h_knb_data, ExpDiaDBHandler* h_dia_db, ExpBool b_on_memory) {
    struct ExpEntryList st_entry_list;
    char                sz_message[1024];

    // .expcache ディレクトリのパスを生成
    char sz_exp_cache_path[_MAX_FILE_PATH];
    memset(sz_exp_cache_path, '\0', sizeof(sz_exp_cache_path));
    sprintf(sz_exp_cache_path, "%s/%s%s_%s/",
            EXPCACHE_DIR, DATA_YEAR, DATA_MONTH, DATA_EDITION);
#ifdef _DEBUG
    memset(sz_message, '\0', sizeof(sz_message));
    sprintf(sz_message, ".expcach path -> %s", sz_exp_cache_path);
    stdout_message("DEBUG", sz_message);
#endif

    // エントリーリストを生成
    st_entry_list = generate_entry_list(sz_exp_cache_path, b_on_memory);

    // dia DB を初期化し、操作ハンドラを取得
    ExpErr exp_result = EXP_SUCCESS;
    if((*h_dia_db = ExpDiaDB_Initiate(st_entry_list.exp_dia_db, 0, &exp_result)) == NULL) {
        // 確保したエントリーリストを破棄
        delete_entry_list(st_entry_list);

        memset(sz_message, '\0', sizeof(sz_message));
        sprintf(sz_message, "ExpDiaDB_initiate() Failure: code -> %d", exp_result);
        stdout_message("CRITICAL", sz_message);

        return exp_result;
    }

    // knb データを初期化し、操作ハンドラを取得
    if((*h_knb_data = ExpDB_Initiate(st_entry_list.exp_knb, *h_dia_db, EXP_FALSE, &exp_result)) == NULL) {
        // 確保したエントリーリストを破棄
        delete_entry_list(st_entry_list);

        memset(sz_message, '\0', sizeof(sz_message));
        sprintf(sz_message, "ExpDB_Initiate() Failure: code -> %d", exp_result);
        stdout_message("CRITICAL", sz_message);

        return exp_result;
    }

    // 後始末
    delete_entry_list(st_entry_list);

    return exp_result;
}

/**
 * @brief   データのエントリーリストを生成
 * @param   const char* : .expcache ディレクトリパスのポインタ
 * @param   ExpBool : メモリ上へ展開可否フラグ
 * @return  struct ExpEntryList : エントリーリストの構造体
 *
 * @copyright   val lab corp.
 * @author      shiohata
 * @data        2025-03-27  新規作成
 */
struct ExpEntryList generate_entry_list(const char* sz_expcache_path, ExpBool b_on_memory) {
    struct ExpEntryList st_entry_list;
    // dia ファイルをコアエンジンに読み込ませ、エンチリーリストを生成
    st_entry_list.exp_dia_db = (ExpDiaDataFileList)generate_list(sz_expcache_path, _DIA_EXT, b_on_memory);
    // knb ファイルをコアエンジンに読み込ませ、エントリーリストを生成
    st_entry_list.exp_knb = (ExpDataFileList)generate_list(sz_expcache_path, _KNB_EXT, b_on_memory);

    return st_entry_list;
}

/**
 * @brief   .expcache 内にある関連ファイルを読み込み、エントリーリストを生成
 * @param   const char* : .expcache ディレクトリパスのポインタ
 * @param   int* : ファイル数を格納するポインタ
 * @return  ExpVoid* : ExpDataFileList or ExpDiaDataFileList
 *
 * @copyright   val lab corp.
 * @author      shiohata
 * @data        2025-04-01  新規作成
 */
ExpVoid* generate_list(const char* sz_expcache_path, const char* sz_ext, ExpBool b_on_memory) {
    char sz_message[1024];
    memset(sz_message, '\0', sizeof(sz_message));
    sprintf(sz_message, "start to generating %s entry list.", sz_ext);
    stdout_message("INFO", sz_message);

    // ディレクトリ内を検索する為の値を生成
    char sz_path[_MAX_FILE_PATH];
    char sz_wildcard[8];
    sprintf(sz_path, "%s%s/", sz_expcache_path, sz_ext);
    if(strcmp(sz_ext, "knb") == 0) {
        //strcpy(sz_wildcard, "*.knb");
        sprintf(sz_wildcard, "*.%s", sz_ext);
    }
    else if(strcmp(sz_ext, "dia") == 0) {
        strcpy(sz_wildcard, "*.dat");
    }
    else {
        stdout_message("ERROR", "undefined ext.");
        return NULL;
    }

#ifdef _DEBUG
    memset(sz_message, '\0', sizeof(sz_message));
    sprintf(sz_message, "check param -> path: %s, wildcard: %s", sz_path, sz_wildcard);
    stdout_message("DEBUG", sz_message);
#endif

    // 拡張子に合わせたエントリーリストを作成
    ExpVoid *entry_list;
    if(strcmp(sz_ext, "knb") == 0) {
        entry_list = ExpDB_NewFileList();
    }
    else if(strcmp(sz_ext, "dia") == 0) {
        entry_list = ExpDiaDB_NewFileList();
    }
    else {
        // none
    }

    // 指定のディレクトリを開く
    ExpErr  exp_result  = EXP_SUCCESS;
    int     n_counter   = 1;

    DIR*            dp;
    struct dirent*  st_dir;

    dp = opendir(sz_path);
    if(dp) {
        // ディレクトリ内のファイル一覧を読込む
        while((st_dir = readdir(dp)) != NULL) {
            int n_result = 0;
            if(st_dir != NULL) {
                // ワイルドカードにマッチしたファイル名のものを判定
                n_result = fnmatch(sz_wildcard, st_dir->d_name, FNM_PATHNAME);
                if(n_result == 0) {
                    char sz_full_path[_MAX_FILE_PATH];
                    memset(sz_full_path, '\0', sizeof(sz_full_path));
                    sprintf(sz_full_path, "%s%s", sz_path, st_dir->d_name);
#ifdef _DEBUG
                    memset(sz_message, '\0', sizeof(sz_message));
                    sprintf(sz_message, "(%s: %03d) add a file. -> %s", sz_ext, n_counter, sz_full_path);
                    stdout_message("DEBUG", sz_message);
#endif
                    // knb ファイルをリストへ追加
                    if(strcmp(sz_ext, "knb") == 0) {
                        ExpFileID   exp_file_id;
                        ExpInt      n_io_level = EXP_IO_LEVEL_HIGH;
                        exp_result = ExpDB_AddFileList3(sz_full_path, n_io_level, b_on_memory, entry_list, exp_file_id);
                    }
                    // dia DB ファイルをリストへ追加
                    else if(strcmp(sz_ext, "dia") == 0) {
                        exp_result = ExpDiaDB_AddFileList2(sz_full_path, b_on_memory, entry_list);
                    }
                    else {
                        // none
                    }
                    if(exp_result != EXP_SUCCESS) {
                        memset(sz_message, '\0', sizeof(sz_message));
                        char sz_char[8] = "";
                        if(strcmp(sz_ext, "knb") == 0) {
                            strcpy(sz_char, "DB");
                        }
                        else if(strcmp(sz_ext, "dia") == 0) {
                            strcpy(sz_char, "diaDB");
                        }
                        sprintf(sz_message, "code: %d -> function Exp%s_AddFileList() is failed. file: %s",
                                (int)exp_result, sz_char, st_dir->d_name);
                        stdout_message("ERROR", sz_message);

                        return NULL;
                    }

                    n_counter++;
                }
            }
        }
    }
    else {
        memset(sz_message, '\0', sizeof(sz_message));
        sprintf(sz_message, "failed to open the directory. -> %s", sz_expcache_path);
        stdout_message("ERROR", sz_message);

        return NULL;
    }

    // ディレクトリを閉じる
    closedir(dp);
    dp = NULL;

#ifdef _DEBUG
    memset(sz_message, '\0', sizeof(sz_message));
    sprintf(sz_message, "number of %s file. -> %d", sz_ext, (n_counter - 1));
    stdout_message("DEBUG", sz_message);
#endif
    memset(sz_message, '\0', sizeof(sz_message));
    sprintf(sz_message, "finished to generating %s entry list.", sz_ext);
    stdout_message("INFO", sz_message);

    return entry_list;
}

/**
 * @brief   各種エントリーリストを破棄
 * @param   struct ExpEntryList : knb, dia DB のエントリーリスト
 * @return  none
 *
 * @copyright   val lab corp.
 * @author      shiohata
 * @data        2025-03-28  新規作成
 */
void delete_entry_list(struct ExpEntryList st_entry_list) {
    ExpDiaDB_DeleteFileList(st_entry_list.exp_dia_db);
    ExpDB_DeleteFileList(st_entry_list.exp_knb);
}

/**
 * @brief   diaDB とknb data の操作ハンドラを破棄
 * @param   ExpDiaDBHandler
 * @param   ExpDataHandler
 * @return  none
 *
 * @copyright   val lab corp.
 * @author      shiohata
 * @data        2025-04-01  新規作成
 */
void terminate_databases_handler(ExpDiaDBHandler h_dia_db, ExpDataHandler h_knb_data) {
    // DiaDB のハンドラを破棄
    ExpDiaDB_Terminate(h_dia_db);
    // knb データのハンドラを破棄
    ExpDB_Terminate(h_knb_data);
}

/**
 * @brief   取得した各種ハンドラの動作確認
 * @param   ExpDataHandler* : KNB データハンドラのポインタ
 * @param   ExpDiaDBHandler* : DIA DB ハンドラのポインタ
 * @return  none
 *
 * @copyright   val lab corp.
 * @author      shiohata
 * @data        2025-03-28  新規作成
 */
void check_exp_initialize(ExpDataHandler* h_knb_data, ExpDiaDBHandler* h_dia_db) {
    char sz_message[1024];
    memset(sz_message, '\0', sizeof(sz_message));
    sprintf(sz_message, "Data Version is ...\nKNB Version\t\t\t\t: %d\nDia DB Version\t\t\t: %d\nJR Timetable Version\t: %d",
            (int) ExpDB_GetBaseDateVersion(*h_knb_data),
            (int) ExpDB_GetDiaEditionDateVersion(*h_knb_data),
            (int) ExpDB_GetJrTrainDiaDateVersion(*h_knb_data));
    stdout_message("INFO", sz_message);
}
