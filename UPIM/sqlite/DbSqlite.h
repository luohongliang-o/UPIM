#ifndef _DB_SQLITE_H_
#define _DB_SQLITE_H_
/*----------------------------------------------------------------------------*
 * DbSQLite.h
 *----------------------------------------------------------------------------*/
#include "stdafx.h"
#include "sqlite3i.h"
#include "sqlite3.h"
#include <vector>

class CSqlStatement
{
public:
   CSqlStatement();
// CSqlStatement(const CSqlStatement&);
   virtual ~CSqlStatement();
private:
//
// CSqlStatement's ctor only to be called by CDbSQLite
//
   friend class CDbSQLite;
   CSqlStatement(LPCTSTR statement, sqlite3* db);

public:

   typedef enum
   {
      eInteger = SQLITE_INTEGER,
      eFloat   = SQLITE_FLOAT  ,
      eText    = SQLITE_TEXT   ,
      eBlob    = SQLITE_BLOB   ,
      eNull    = SQLITE_NULL   ,
   }
   EDataType;

   EDataType DataType(int pos_zero_indexed);
   int Fields();
   LPCTSTR FieldName(int pos_zero_indexed);
   LPCTSTR FieldType(int pos_zero_indexed);

   int ValueInt(int pos_zero_indexed);
   LPCTSTR ValueString(int pos_zero_indexed);

   bool Bind (int pos_zero_indexed, LPCTSTR value);
   bool Bind (int pos_zero_indexed, double value);
   bool Bind (int pos_zero_indexed, int value);
   bool BindNull (int pos_zero_indexed);

   bool Execute();
   bool NextRow();

// Call Reset if not depending on the NextRow cleaning up.
// For example for select count(*) statements.

   bool Reset();
   bool RestartSelect();

private:
   sqlite3_stmt* m_stmt;
   CString m_szText;
};

class CDbSQLite
{
public:
   CDbSQLite();
   virtual ~CDbSQLite();

   bool Open(LPCTSTR db_file);

   class ResultRecord
   {
   public:
      std::vector<CString> m_fields;
   };

   class ResultTable
   {
     friend class CDbSQLite;
   public:
     ResultTable() : ptr_cur_record_(0) {}
    
     std::vector<ResultRecord> m_records;
    
     ResultRecord* next()
     {
        if (ptr_cur_record_ < m_records.size())
        {
           return &(m_records[ptr_cur_record_++]);
        }

        return 0;
     }
    
   private:
      void reset()
      {
         m_records.clear();
         ptr_cur_record_=0;
      }
   private:
      unsigned int ptr_cur_record_;
   };

   bool SelectStatement(LPCTSTR stmt, ResultTable& res);
   bool DirectStatement(LPCTSTR stmt);
   CSqlStatement* Statement(LPCTSTR stmt);

   LPCTSTR LastError();
   //
   // Transaction related
   //
   bool Begin();
   bool Commit();
   bool Rollback();

private:
   static int SelectCallback(void *p_data, int num_fields, char **p_fields, char **p_col_names);
   sqlite3* m_db;
   CString m_szName;
   CString m_szText;
};

#endif
