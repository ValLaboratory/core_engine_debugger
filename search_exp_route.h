/* 標準ヘッダー */
#include <stdio.h>
#include <stdlib.h>

/* core_engine 定義ヘッダー */
#include "ExpType.h"
#include "ExpPublic.h"

/* 共通関数 */
#include "common.h"

/* 平均経路検索 */
ExpRouteResHandler search_simple_average_route(
            ExpNaviHandler  h_exp_navi,
            const ExpInt32* p_eki_code,     // 駅コードの配列のポインタ
            ExpInt16        n_buf_size,     // 駅コードの配列のサイズ
            ExpDate         n_date,         // 検索開始日付           (書式: yyyymmdd)
            ExpDiaVehicles* p_exp_vehicles  // 利用交通機関の設定
        );

/* ダイヤ経路探索 */
ExpRouteResHandler search_simple_time_search(
            ExpNaviHandler  h_exp_navi,
            const ExpInt32* p_eki_codes,    // 駅コードの配列のポインタ
            ExpInt16        n_buf_size,     // 駅コードの配列のサイズ
            ExpInt16        n_mode,         // 経路探索開始時刻切り替えモード　(0:出発時刻, 1:到着時刻)
            ExpDate         n_date,         // 探索開始日付            (書式: yyyymmdd)
            ExpInt16        n_time,         // 探索開始時刻(分)        単位: 分
                                            // 例: 08時15分の場合, 8 * 60 + 15 = 495
            ExpDiaVehicles* p_exp_vehicles  // 各種乗り物設定
        );

/* 経路探索結果からダイヤリストを表字 */
void display_dia_list(
            ExpRouteResHandler  h_result_route, // 経路探索結果の操作ハンドラー
            ExpInt16            n_result_no,    // 経路探索結果の順序番号
            ExpInt16            n_seq_no        // 経路結果の指定経路順序番号の区間番号
        );
