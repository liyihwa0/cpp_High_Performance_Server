namespace wa{
    namespace db{
        /**
         * @brief 表示一个SQL语句的类型
         * @ingroup SQLParser
         */
        enum SqlCommandType{
            ERROR = 0,
            CALC,
            SELECT,
            INSERT,
            UPDATE,
            DELETE,
            CREATE_TABLE,
            DROP_TABLE,
            CREATE_INDEX,
            DROP_INDEX,
            SYNC,
            SHOW_TABLES,
            DESC_TABLE,
            BEGIN, 
            COMMIT,
            CLOG_SYNC,
            ROLLBACK,
            LOAD_DATA,
            HELP,
            EXIT,
            EXPLAIN,
            SCF_SYNC,
            SET_VARIABLE,  ///< 设置变量
        };
    }
}