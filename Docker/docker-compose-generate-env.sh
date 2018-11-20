#/bin/bash
rm .env
echo "EXTERNALADDRESS=https://${HOSTNAME}/" >> .env
echo "ADAGUC_HOME=$HOME" >> .env
echo "ADAGUC_DATA_DIR=/data/adaguc-autowms" >> .env
echo "ADAGUC_AUTOWMS_DIR=/data/adaguc-autowms" >> .env
echo "ADAGUC_DATASET_DIR=/data/adaguc-datasets" >> .env
echo "ADAGUC_PORT=443" >> .env


