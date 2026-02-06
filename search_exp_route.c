#include "search_exp_route.h"

/**
 * @brief   平均経路探索
 * @param   ExpNaviHandler
 * @param   ExpInt32*       : 外部駅コードの配列のポインタ
 * @param   ExpInt16        : 駅コードの配列のバッファサイズ
 * @param   ExpDate         : 検索開始日付
 * @param   ExpDiaVehicles* : 利用交通機関の種別フラグのポインタ
 * @return
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
 * @param   ExpInt16*       : 外部駅コードの配列のポインタ
 * @param   ExpInt16        : 駅コードの配列のバッファサイズ
 * @param   ExpInt16        : 経路探索開始時刻切り替えモード　(0:出発時刻, 1:到着時刻)
 * @param   ExpDate         : 経路検索開始日付
 * @param   ExpUnt16        : 経路検索開始時刻
 * @param   ExpDiaVehicles* : 利用交通機関の種別フラグのポインタ
 * @return
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