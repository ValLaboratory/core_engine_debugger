/*
 * @file    main.c
 * @brief   core_engine デバッグ用プログラム
 * @author  shiohata@val.co.jp
 * @date    2025-03-21:新規作成
 * */
/* 標準ヘッダー */
#include <stdio.h>

/* core_engine 定義ヘッダー */
#include "ExpType.h"
#include "ExpPublic.h"

/* Debugger 用のヘッダーファイル */
#include "common.h"
#include "initialize_exp_database.h"

/* 定数宣言 */
#define TRUE        EXP_TRUE
#define FALSE       EXP_FALSE

/* グローバル変数 */
ExpDataHandler      h_knb_data;
ExpDiaDBHandler     h_dia_db;

int main(void) {
    printf("Welcome to 'Core Engine'.\n");
    /* プリプロ確認 */
#if defined(DEBUG_XCODE__)
    stdout_message("INFO", "DEBUG_XCODE_ option is enabled.");
#else
    stdout_message("WARN", "You need to setting DEBUG_XCODE_ option.");
#endif
#if defined(EXP_C99_FIXED_WIDTH_INTEGER_TYPES)
    stdout_message("INFO", "EXP_C99_FIXED_WIDTH_INTEGER_TYPES option is enabled.");
#else
    stdout_message("WARN", "You need to setting EXP_C99_FIXED_WIDTH_INTEGER_TYPES option.");
#endif

    /* コアエンジンのDB を初期化 */
    stdout_message("INFO", "start to initialize exp databases.");

    if(initialize_exp_database(&h_knb_data, &h_dia_db, EXP_TRUE) == EXP_SUCCESS) {
        /* 確認 */
        check_exp_initialize(&h_knb_data, &h_dia_db);
    }
    else {
        return EXP_FAILURE;
    }

    stdout_message("INFO", "database initialized finished.");

    // 平均経路探索デモ

    // 後始末
    // データ操作用のハンドラを破棄
    terminate_databases_handler(h_dia_db, h_knb_data);

    return EXP_SUCCESS;
}