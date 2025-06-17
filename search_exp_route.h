/* 標準ヘッダー */
#include <stdio.h>
#include <stdlib.h>

/* core_engine 定義ヘッダー */
#include "ExpType.h"
#include "ExpPublic.h"

/* 平均経路検索 */
ExpRouteResHandler search_simple_average_route(
            ExpNaviHandler  h_exp_navi,
            ExpInt32        n_from_st_id,   // 平均経路検索の出発駅   (外部駅コード)
            ExpInt32        n_to_st_id,     // 平均経路探索の到着駅   (外部駅コード)
            ExpDate         n_date,         // 検索開始日付           (書式: yyyymmdd)
            ExpDiaVehicles* p_exp_vehicles  // 利用交通機関の設定
        );

/* ダイヤ経路探索 */
ExpRouteResHandler search_simple_time_search(
            ExpNaviHandler  h_exp_navi,
            ExpInt32        n_from_st_id,   // ダイヤ経路探索の出発駅  (外部駅コード)
            ExpInt32        n_to_st_id,     // ダイヤ経路探索の到着駅  (外部駅コード)
            ExpInt16        n_mode,         // 経路探索開始時刻切り替えモード　(0:出発時刻, 1:到着時刻)
            ExpDate         n_date,         // 探索開始日付            (書式: yyyymmdd)
            ExpInt16        n_time,         // 探索開始時刻(分)        単位: 分
                                            // 例: 08時15分の場合, 8 * 60 + 15 = 495
            ExpDiaVehicles* p_exp_vehicles  // 各種乗り物設定
        );
