#include "search_exp_route.h"

/**
 * @brief   平均経路探索
 * @param   ExpNaviHandler
 * @param   ExpInt32*           : 外部駅コードの配列のポインタ
 * @param   ExpInt16            : 駅コードの配列のバッファサイズ
 * @param   ExpDate             : 検索開始日付
 * @param   ExpDiaVehicles*     : 利用交通機関の種別フラグのポインタ
 * @return　ExpRouteResHandler  : 経路探索結果の操作ハンドラー
 */
ExpRouteResHandler search_simple_average_route(ExpNaviHandler h_exp_navi, const ExpInt32* p_eki_code, ExpInt16 n_buf_size, ExpDate n_date, ExpDiaVehicles* p_exp_vehicles) {
    ExpDataHandler  h_exp_data;

    // データ操作ハンドラを取得
    h_exp_data = ExpNavi_GetDataHandler(h_exp_navi);
    if(h_exp_data == NULL) {
        return NULL;
    }

    // naviHandler へのエントリーをクリア
    ExpNavi_ClearEntries(h_exp_navi);

    int n_eki_code_cnt = n_buf_size / sizeof(p_eki_code[0]);
    for(int i = 0; i < n_eki_code_cnt; i++) {
        char    msg[128];
        sprintf(msg, "[%02d]: eki code -> %d", (i + 1), p_eki_code[i]);
        stdout_message("DEBUG", msg);

        // 外部駅コードから内部駅コードへ変換
        ExpStationCode st_internal_eki_code;
        ExpStation_SetFromID(h_exp_data, p_eki_code[i], &st_internal_eki_code);

        // 内部駅コードを出発地から経由地、到着地までを順番に設定
        ExpNavi_SetStationEntry(h_exp_navi, (ExpInt16)(i + 1), &st_internal_eki_code);
    }

    // 経路検索の日付を設定
    ExpNavi_SetDepartureDate(h_exp_navi, n_date);

    return ExpRoute_Search2(h_exp_navi, p_exp_vehicles);
}

/**
 * @brief   ダイヤ経路検索
 * @param   ExpNaviHandler
 * @param   ExpInt16*           : 外部駅コードの配列のポインタ
 * @param   ExpInt16            : 駅コードの配列のバッファサイズ
 * @param   ExpInt16            : 経路探索開始時刻切り替えモード　(0:出発時刻, 1:到着時刻)
 * @param   ExpDate             : 経路検索開始日付
 * @param   ExpUnt16            : 経路検索開始時刻
 * @param   ExpDiaVehicles*     : 利用交通機関の種別フラグのポインタ
 * @return  ExpRouteResHandler  : 経路探索結果の操作ハンドラー
 */
ExpRouteResHandler search_simple_time_search(ExpNaviHandler h_exp_navi, const ExpInt32* p_eki_code, ExpInt16 n_buf_size, ExpInt16 n_mode, ExpDate n_date, ExpInt16 n_time, ExpDiaVehicles* p_exp_vehicles) {
    ExpDataHandler h_exp_data;
    h_exp_data = ExpNavi_GetDataHandler(h_exp_navi);
    if(h_exp_navi == NULL) {
        return NULL;
    }

    // naviHandler へのエントリーをクリア
    ExpNavi_ClearEntries(h_exp_navi);

    ExpInt16 n_eki_code_cnt = n_buf_size / sizeof(ExpInt32);
    for(int i = 0; i < n_eki_code_cnt; i++) {
        char    msg[128];
        sprintf(msg, "[%02d]: eki code -> %d", (i + 1), p_eki_code[i]);
        stdout_message("DEBUG", msg);

        // 外部駅コードから内部駅コードへ変換
        ExpStationCode st_internal_eki_code;
        ExpStation_SetFromID(h_exp_data, p_eki_code[i], &st_internal_eki_code);

        // 内部駅コードを出発地から経由地、到着地までを順番に設定
        ExpNavi_SetStationEntry(h_exp_navi, (ExpInt16)(i + 1), &st_internal_eki_code);
    }

    return ExpRoute_DiaSearch(
                h_exp_navi,
                n_mode,
                n_date,
                n_time,
                NULL,
                0,
                p_exp_vehicles
            );
}

/**
 * @brief   経路探索結果からダイヤのリスト情報を表字
 * @param   ExpRouteResHandler  : 経路探索結果の操作ハンドラー
 * @return  none
 */
 void display_dia_list(ExpRouteResHandler h_result_route, ExpInt16 n_route_no, ExpInt16 n_rail_seq_no) {
    // 対象経路区間の運行日
    ExpDate     n_drive_date    = ExpCRouteRPart_GetDriveDate(h_result_route, n_route_no, n_rail_seq_no);

    // 対象経路区間のダイヤリストを作成
    ExpDiaRailList st_dia_rail_list = ExpCRouteRPart_MakeDiaRailListFromStation(h_result_route, n_route_no, n_rail_seq_no, n_drive_date, 0);
    if(st_dia_rail_list) {
        printf("\t\t列車No.\t\t出発時刻\t到着時刻\t列車名称\n");

        // 取得したダイヤリストの数を取得
        ExpInt32 n_dia_cnt = ExpDiaRailList_GetCount(st_dia_rail_list);
        for(ExpInt32 n_list_no = 1; n_list_no <= n_dia_cnt; n_list_no++) {
            ExpStationCode  st_from_station_code;   // 出発駅コード
            ExpStationCode  st_to_station_code;     // 到着駅コード
            ExpRailCode     st_rail_code;           // 路線コード
            ExpInt16        n_dep_time;             // 出発時刻
            ExpInt16        n_arr_time;             // 到着時刻
            ExpInt16        n_flight_no;            // 列車の号番
            ExpUInt32       n_train_code;           // 列車コード
            ExpChar         sz_name_sjis[256] = {}; // 列車名称（Shift_JIS）

            //*** リストある各種情報を取得
            // 対象経路区間の出発駅・到着駅の情報を取得
            ExpDiaRailList_GetSection(st_dia_rail_list, &st_from_station_code, &st_to_station_code);
            // 出発時刻を取得
            ExpDiaRailList_GetDepartureTime(st_dia_rail_list, n_list_no, &n_dep_time);
            // 到着時刻を取得
            ExpDiaRailList_GetArrivalTime(st_dia_rail_list, n_list_no, &n_arr_time);
            // 路線名称を取得
            ExpDiaRailList_GetRailName(st_dia_rail_list, n_list_no, EXP_LANG_JAPANESE, sz_name_sjis, 256, 0);
            // 路線コードを取得
            ExpDiaRailList_GetRailCode(st_dia_rail_list, n_list_no, &st_rail_code);
            // 列車の号数を取得
            n_flight_no = ExpRail_GetNo(&st_rail_code);
            // 列車コードを取得
            ExpDiaRailList_GetTrainCode(st_dia_rail_list, n_list_no, &n_train_code);

            // 取得した内容を標準出力
            ExpChar sz_name_utf8[256] = {};
            convert_sjis2utf8(sz_name_sjis, sz_name_utf8);
            printf("No.: %02d\t[%d]\t\t%02d:%02d\t->\t%02d:%02d\t\t%s(%03d号)\n",
                   n_list_no,
                   n_train_code,
                   n_dep_time / 60,
                   n_dep_time % 60,
                   n_arr_time / 60,
                   n_arr_time % 60,
                   sz_name_utf8,
                   n_flight_no);
        }

        // 後始末
        ExpDiaRailList_Delete(st_dia_rail_list);
    }
 }