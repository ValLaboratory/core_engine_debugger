#!/bin/bash
. ./setup_config.sh

# このスクリプトが存在する絶対パス
SCRIPT_DIR=$(cd $(dirname $0); pwd)

# プロジェクトのROOTディレクトリ
PJ_ROOT_DIR="${SCRIPT_DIR}/../.."

if [[ "${TARGET_TYPE}" =~ ^(fiexe|wip|old|all|!?)$ ]] ; then
  if [ ${TARGET_TYPE} == "" ] ; then
    TARGET_TYPE="all"
  fi
else
  echo "error : unknown target is defined."
  echo -e "\tUnknown target: \033[1;31m${TARGET_TYPE}\033[0m"
  echo -e "\t\tPlease choice specified values : \033[1m\033[32mfixed\033[0m\033[0m / \033[1m\033[1;32mwip\033[0m / \033[1m\033[1;32mall\033[0m"

  exit 1
fi

# Amazon S3 を参照して、対象データを取得(fixed or all)
echo -e "search target: Type -> \033[1;32m${TARGET_TYPE}\033[0m, Amazon S3 Bucke t-> \033[1;32m${S3_BUCKET_DIR}\033[0m"

# 確定した版のデータを取得し、指定のディレクトリへ展開
echo "start to searching fixed data."
if [[ "${TARGET_TYPE}" =~ ^(fiexd|all)$ ]] ; then
  EXIST_FIX_DATA=`${AWS_CLI} s3 ls ${S3_BUCKET_URL}/${S3_BUCKET_DIR}`
  if [ "${EXIST_FIX_DATA}" != "" ] ; then
    echo -e "\033[1;32mfound!\033[0m"

    # 一時ディレクトリを作成
    if [ -d ${PJ_ROOT_DIR}/.expcache/tmp ] ; then
      rm -rf ${PJ_ROOT_DIR}/.expcache/tmp
    fi
    mkdir -p ${PJ_ROOT_DIR}/.expcache/tmp

    # 一時ディレクトリへZIP ファイルをダウンロード
    echo "start to downloading a data file."
    ${AWS_CLI} s3 cp ${S3_BUCKET_URL}/${S3_BUCKET_DIR}/${DATA_ZIP} ${PJ_ROOT_DIR}/.expcache/tmp/${DATA_ZIP}

    # ダウンロードした圧縮ファイルを解凍
    echo "start to unzip a downloaded file."
    unzip ${PJ_ROOT_DIR}/.expcache/tmp/${DATA_ZIP} -d ${PJ_ROOT_DIR}/.expcache/tmp/

    # 各種ファイルを指定のディレクトリへ展開
    mkdir -p ${PJ_ROOT_DIR}/.expcache/${LOCAL_EXTRACTION_DIR}/{knb,dia,jpcom/no_e4}
    cp -rf ${PJ_ROOT_DIR}/.expcache/tmp/product/intra/knb ${PJ_ROOT_DIR}/.expcache/${LOCAL_EXTRACTION_DIR}/
    cp -rf ${PJ_ROOT_DIR}/.expcache/tmp/product/dia ${PJ_ROOT_DIR}/.expcache/${LOCAL_EXTRACTION_DIR}/
    cp -rf ${PJ_ROOT_DIR}/.expcache/tmp/product/jpcom ${PJ_ROOT_DIR}/.expcache/${LOCAL_EXTRACTION_DIR}/
    # JPCOM.KNB を読込む為にシンボリックリンクを作成
    if [ -f ${PJ_ROOT_DIR}/.expcache/${LOCAL_EXTRACTION_DIR}/knb/JPCOM.knb ] ; then
      rm -rf ${PJ_ROOT_DIR}/.expcache/${LOCAL_EXTRACTION_DIR}/knb/JPCOM.knb
    fi
    ln -s ${PJ_ROOT_DIR}/.expcache/${LOCAL_EXTRACTION_DIR}/jpcom/JPCOM.KNB ${PJ_ROOT_DIR}/.expcache/${LOCAL_EXTRACTION_DIR}/knb/JPCOM.knb

    # 後始末
    rm -rf ${PJ_ROOT_DIR}/.expcache/tmp

    echo -e "exp data sync: \033[1;32mdone!\033[0m"
    exit 0
  fi
  echo -e "\033[1;33mnot foundl\033[0m"
else
  echo -e "search target: fixed or all is \033[1;33mskipped\033[0m."
fi

# 確定した版のデータを取得し、指定のディレクトリへ展開
echo "start to searching wip data."
if [[ "${TARGET_TYPE}" =~ ^(wip|all)$ ]] ; then
  EXIST_FIX_DATA=`${AWS_CLI} s3 ls ${S3_BUCKET_URL}/wip/${S3_BUCKET_DIR}`
  if [ "${EXIST_FIX_DATA}" != "" ] ; then
    echo -e "\033[1;32mfound!\033[0m"

    # 一時ディレクトリを作成
    if [ -d ${PJ_ROOT_DIR}/.expcache/tmp ] ; then
      rm -rf ${PJ_ROOT_DIR}/.expcache/tmp
    fi
    mkdir -p ${PJ_ROOT_DIR}/.expcache/tmp

    # 最新版のものを取得
    LASTEST_DATA_ZIP=$(${AWS_CLI} s3 ls ${S3_BUCKET_URL}/wip/${S3_BUCKET_DIR}/ | sort | tail -n 1 | awk -F ' ' '{ print $4 }')

    # 一時ディレクトリへZIP ファイルをダウンロード
    echo "start to downloading a data file."
    ${AWS_CLI} s3 cp ${S3_BUCKET_URL}/wip/${S3_BUCKET_DIR}/${LASTEST_DATA_ZIP} ${PJ_ROOT_DIR}/.expcache/tmp/${LASTEST_DATA_ZIP}

    # ダウンロードした圧縮ファイルを解凍
    echo "start to unzip a downloaded file."
    unzip ${PJ_ROOT_DIR}/.expcache/tmp/${LASTEST_DATA_ZIP} -d ${PJ_ROOT_DIR}/.expcache/tmp/

    # 各種ファイルを指定のディレクトリへ展開
    mkdir -p ${PJ_ROOT_DIR}/.expcache/${LOCAL_EXTRACTION_DIR}/{knb,dia,jpcom/no_e4}
    cp -rf ${PJ_ROOT_DIR}/.expcache/tmp/product/intra/knb ${PJ_ROOT_DIR}/.expcache/${LOCAL_EXTRACTION_DIR}/
    cp -rf ${PJ_ROOT_DIR}/.expcache/tmp/product/dia ${PJ_ROOT_DIR}/.expcache/${LOCAL_EXTRACTION_DIR}/
    cp -rf ${PJ_ROOT_DIR}/.expcache/tmp/product/jpcom ${PJ_ROOT_DIR}/.expcache/${LOCAL_EXTRACTION_DIR}/
    # JPCOM.KNB を読込む為にシンボリックリンクを作成
    if [ -f ${PJ_ROOT_DIR}/.expcache/${LOCAL_EXTRACTION_DIR}/knb/JPCOM.knb ] ; then
      rm -rf ${PJ_ROOT_DIR}/.expcache/${LOCAL_EXTRACTION_DIR}/knb/JPCOM.knb
    fi
    ln -s ${PJ_ROOT_DIR}/.expcache/${LOCAL_EXTRACTION_DIR}/jpcom/JPCOM.KNB ${PJ_ROOT_DIR}/.expcache/${LOCAL_EXTRACTION_DIR}/knb/JPCOM.knb

    # 後始末
    rm -rf ${PJ_ROOT_DIR}/.expcache/tmp

    echo -e "exp data sync: \033[1;32mdone!\033[0m"
    exit 0
  fi
  echo -e "\033[1;33mnot foundl\033[0m"
else
  echo -e "search target: wip or all is \033[1;33mskipped\033[0m."
fi

exit 0