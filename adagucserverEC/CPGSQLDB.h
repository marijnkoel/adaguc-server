#ifndef CPGSQLDB_H
#define CPGSQLDB_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libpq-fe.h"                                   /* libpq header file */
#include "CTypes.h"
#include "CDebugger.h"

#define CPGSQLDB_MAX_STR_LEN 8191

#define CDB_UNKNOWN_ERROR               0
#define CDB_UNKNOWN_COLUMNNAME          1
#define CDB_INDEX_OUT_OF_BOUNDS         2
#define CDB_CONNECTION_ERROR            3
#define CDB_TABLE_CREATION_ERROR        4
#define CDB_NODATA                      5
class CDB{
public:
  const char *getErrorMessage(int e){
    if(e==CDB_UNKNOWN_ERROR)return "CDB_UNKNOWN_ERROR";
    if(e==CDB_UNKNOWN_COLUMNNAME)return "CDB_UNKNOWN_COLUMNNAME";
    if(e==CDB_INDEX_OUT_OF_BOUNDS)return "CDB_INDEX_OUT_OF_BOUNDS";
    return getErrorMessage(0);
    
  }
  class ColumnModel{
    private:
      CT::string *columnNames;
      size_t size;
    public:
      ColumnModel(size_t size){
        columnNames = new CT::string[size];
        this->size=size;
      }
      ~ColumnModel(){delete[] columnNames;}
     size_t getIndex(const char *name){
        for(size_t j=0;j<size;j++){if(columnNames[j].equals(name))return j;}
        throw(CDB_UNKNOWN_COLUMNNAME);
      }
      const char *getName(size_t index){
         if(index>=size)throw(CDB_INDEX_OUT_OF_BOUNDS);
         return columnNames[index].c_str();
        
      }
      void setColumn(size_t index,const char *name){
        if(index>=size)throw(CDB_INDEX_OUT_OF_BOUNDS);
        columnNames[index].copy(name);
      }
      size_t getSize(){return size;}
  };
  
  class Record{
    private:
      CT::string *values;
      ColumnModel *columnModel;
      size_t size;
    public:
      ~Record(){
       delete[] values;
      }
      Record(ColumnModel *columnModel){
        this->columnModel=columnModel;
        size=columnModel->getSize();
        values = new CT::string[size];
      }
      ColumnModel * getColumnModel(){
        return columnModel;
      }

      CT::string *get(size_t index){
        if(index>=size)throw(CDB_INDEX_OUT_OF_BOUNDS);
        return &(values[index]);
      }  
      CT::string *get(const char *name){
        return get(columnModel->getIndex(name));
      }
      void push(size_t index,const char *value){
        if(index>=size)throw(CDB_INDEX_OUT_OF_BOUNDS);
        values[index].copy(value);
      }
  };
  
  class Store{
    private:
      std::vector<Record*>records;
      ColumnModel *columnModel;
   
    public:
       Store(ColumnModel *columnModel){
        this->columnModel=columnModel;
      }
      ~Store(){
        for(size_t j=0;j<records.size();j++){delete records[j];}records.clear();
        delete columnModel;
      }
      Record *getRecord(size_t rowNumber){
        if(rowNumber>=records.size())throw(CDB_INDEX_OUT_OF_BOUNDS);
        return records[rowNumber];
      }
      size_t getSize(){
        return records.size();
      }
      size_t size(){
        return records.size();
      }
      void push(Record *record){
        records.push_back(record);
      }
      std::vector<Record*> getRecords(){
        return records;
      }
      ColumnModel * getColumnModel(){
        return columnModel;
      }
  };
};


class CPGSQLDB:public CDB{
 // This class must be used within the same thread!
  private:
    DEF_ERRORFUNCTION();
    PGconn     *connection;
    PGresult   *result;
    char        szTemp[CPGSQLDB_MAX_STR_LEN+1];
    char        LastErrorMsg[CPGSQLDB_MAX_STR_LEN+1];
    int dConnected;
    void clearResult();
    
  public:

    const char *getError();
    CPGSQLDB();
    ~CPGSQLDB();
    int close();
    int connect(const char * pszOptions);
    int checkTable(const char * pszTableName,const char *pszColumns);
    int query(const char *pszQuery);
    CT::string* query_select(const char *pszQuery);
    CT::string* query_select(const char *pszQuery,int dColumn);
    
    Store* queryToStore(const char *pszQuery);
};
#endif
