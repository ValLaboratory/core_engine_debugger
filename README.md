# core_engine_debugger
コアエンジンの開発・解析用

# 初期設定
## /tools/setup/setup_config.sh
利用するcore_engine のbranch に併せて設定を行う
```
L. 12 - 14
DATA_YEAR=yyyy
DATE_MONTH=mm
DATA_EDITION=nn

例：
2025年02月04版の場合
DATA_YEAR=2025
DATA_MONTH=02
DATA_EDITION=04
```

## /exp_configure.h
上記と同様に利用するデータ版を設定
```
L. 5 - 7
#define DATA_YEAR     "yyyy"
#define DATA_MONTH    "mm"
#define DATA_EDITION  "nn"

例：
2025年02月04版の場合
#define DATA_YEAR=2025
#define DATA_MONTH=02
#define DATA_EDITION=04
```
