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
#include "search_exp_route.h"

/* 定数宣言 */
#define TRUE            EXP_TRUE
#define FALSE           EXP_FALSE
#define DEBUG_SIMPLE    FALSE       // 経路探索の切り替えフラグ 0:ダイヤ探索, 1:平均探索

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
        check_exp_initialize(&h_knb_data);
    }
    else {
        stdout_message("ERROR", "failed to initialize exp databases.");
        return EXP_FAILURE;
    }

    stdout_message("INFO", "database initialized finished.");

    stdout_message("INFO", "==================== start to search average route.");
    // 平均経路探索テスト
    /* ExpNaviHandler を生成 */
    ExpNaviHandler  *h_exp_navi = NULL;
    ExpErr          exp_error;
    h_exp_navi = ExpNavi_NewHandler(h_knb_data, &exp_error);
    if(h_exp_navi != NULL) {
        // 平均経路探索を実行
        ExpRouteResHandler h_result_route = NULL;

#ifdef DEBUG_SIMPLE     // 平均経路探索
        /* 経路検索に利用する交通手段を設定 */
        // 利用交通機関を初期化
        ExpDiaVehicles exp_dia_vehicles;
        ExpDiaVehicles_Clear(&exp_dia_vehicles, EXP_TRUE);
        ExpDiaVehicles_SetTraffic(EXP_TRAFFIC_AIR, EXP_FALSE, &exp_dia_vehicles);       // 航空機を除く
        ExpDiaVehicles_SetTraffic(EXP_TRAFFIC_ROUTEBUS, EXP_FALSE, &exp_dia_vehicles);  // 路線バスを除く

        // 品川：22709, 高円寺:22671
        h_result_route = search_simple_average_route(
                h_exp_navi,
                22709,
                22671,
                20250411,
                &exp_dia_vehicles
                );
#else   // ダイヤ経路探索
        /* 経路検索に利用する交通手段を設定 */
        // 利用交通機関を初期化
        ExpDiaVehicles exp_dia_vehicles;
        ExpDiaVehicles_Clear(&exp_dia_vehicles, EXP_TRUE);
        ExpDiaVehicles_SetTraffic(EXP_TRAFFIC_AIR, EXP_FALSE, &exp_dia_vehicles);       // 航空機を除く

        h_result_route = search_simple_time_search(
                h_exp_navi,
                874407, // 探索の出発駅
                29594, // 探索の到着駅
                0, // 探索モード  0:出発時刻探索　1:到着時刻探索
                20250603, // 探索日付
                585, // 探索時刻(分) 例：08時15分 > 8*60+15
                &exp_dia_vehicles
                );
#endif

        if(h_result_route != NULL) {
            // 平均経路検索の結果を表示
            ExpDebug_PrintRoute(
                    NULL,
                    ExpNavi_GetDataHandler(h_exp_navi),
                    h_result_route,
                    0,
                    NULL);
            ExpRoute_Delete(h_result_route);
        }
        else {
            stdout_message("INFO", "not a route result.");
        }
    }
    else {
        stdout_message("ERROR", "failed to create new ExpNaviHandler.");
        return EXP_FAILURE;
    }

    stdout_message("INFO", "==================== finished.");

    // 後始末
    // データ操作用のハンドラを破棄
    terminate_databases_handler(h_dia_db, h_knb_data);

    return EXP_SUCCESS;
}