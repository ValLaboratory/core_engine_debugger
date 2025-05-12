/* 標準ヘッダー */
#include <stdio.h>
#include <stdlib.h>

/* core_engine 定義ヘッダー */
#include "ExpType.h"
#include "ExpPublic.h"

/* 平均経路検索 */
ExpRouteResHandler search_simple_average_route(
            ExpNaviHandler  h_exp_navi,
            ExpInt32        n_from_st_id,   // 経路検索の出発駅   (内部駅コード)
            ExpInt32        n_to_st_id,     // 経路探索の到着駅   (内部駅コード)
            ExpDate         n_date,         // 検索開始日時       (書式: yyyymmdd)
            ExpDiaVehicles* p_exp_vehicles  // 利用交通機関の設定
        );