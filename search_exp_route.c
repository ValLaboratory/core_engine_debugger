#include "search_exp_route.h"

/**
 * @brief   平均経路探索
 * @param   ExpNaviHandler :
 * @param   ExpInt32 : 出発駅コード (内部駅コード)
 * @param   ExpInt32 : 到着駅コード (内部駅コード)
 * @param   ExpDate : 検索開始日時
 * @param   ExpDiaVehicles* : 利用交通機関の種別フラグのポインタ
 * @return
 */
ExpRouteResHandler search_simple_average_route(ExpNaviHandler h_exp_navi, ExpInt32 n_from_st_id, ExpInt32 n_to_st_id, ExpDate n_date, ExpDiaVehicles* p_exp_vehicles) {
    ExpDataHandler  h_exp_data;
    ExpStationCode  n_exp_from_st_code, n_exp_to_st_code;

    // データ操作ハンドラを取得
    h_exp_data = ExpNavi_GetDataHandler(h_exp_navi);
    if(h_exp_data == NULL) {
        return NULL;
    }

    // 外部駅コードから内部駅コードを取得
    ExpStation_SetFromID(h_exp_data, n_from_st_id, &n_exp_from_st_code);    // 出発駅
    ExpStation_SetFromID(h_exp_data, n_to_st_id, &n_exp_to_st_code);        // 到着駅

    // 経路検索の日付を設定
    ExpNavi_SetDepartureDate(h_exp_navi, n_date);

    // 経路探索の情報を設定
    ExpNavi_ClearEntries(h_exp_navi);   // お作法的に
    ExpNavi_SetStationEntry(h_exp_navi, 1, &n_exp_from_st_code);
    ExpNavi_SetStationEntry(h_exp_navi, 2, &n_exp_to_st_code);

    return ExpRoute_Search2(h_exp_navi, p_exp_vehicles);
}
