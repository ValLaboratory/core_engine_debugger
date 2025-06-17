#!/bin/bash

# 取得するデータの種類#
# fixed : 確定した版データ
#   wip : 作業中のデータ
#   old : 旧置場所
#   all : 全て（デフォルト）
TARGET_TYPE="all"

# データの版
# ※注意：Github のbranch に合わせること
DATA_YEAR="2025"
DATA_MONTH="06"
DATA_EDITION="01"
DATA_ZIP="ekispert-data.zip"

# ローカルでの展開ディレクトリ
LOCAL_EXTRACTION_DIR=${DATA_YEAR}${DATA_MONTH}_${DATA_EDITION}

# Amazon S3 のバケット
AWS_CLI="/usr/local/bin/aws"
S3_BUCKET_URL="s3://ekispert-data"
S3_BUCKET_DIR=${DATA_YEAR}${DATA_MONTH}${DATA_EDITION}
