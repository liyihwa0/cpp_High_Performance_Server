namespace wa{
    namespace db{
        /**
         * @brief 表示一个SQL语句的类型
         * @ingroup SQLParser
         */
        enum SqlCommandType{
            SCF_ERROR = 0,
            SCF_CALC,
            SCF_SELECT,
            SCF_INSERT,
            SCF_UPDATE,
            SCF_DELETE,
            SCF_CREATE_TABLE,
            SCF_DROP_TABLE,
            SCF_CREATE_INDEX,
            SCF_DROP_INDEX,
            SCF_SYNC,
            SCF_SHOW_TABLES,
            SCF_DESC_TABLE,
            SCF_BEGIN,  ///< 事务开始语句，可以在这里扩展只读事务
            SCF_COMMIT,
            SCF_CLOG_SYNC,
            SCF_ROLLBACK,
            SCF_LOAD_DATA,
            SCF_HELP,
            SCF_EXIT,
            SCF_EXPLAIN,
            SCF_SET_VARIABLE,  ///< 设置变量
        };
    }
}