CREATE TABLE redis_STRING_value(
    rondb_key BIGINT UNSIGNED NOT NULL,
    ordinal INT UNSIGNED NOT NULL,
    value VARBINARY(29500) NOT NULL,
    PRIMARY KEY (rondb_key, ordinal),
    FOREIGN KEY (rondb_key) REFERENCES redis_string_key(rondb_key) ON UPDATE RESTRICT ON DELETE CASCADE
) ENGINE NDB,
COMMENT = "NDB_TABLE=PARTITION_BALANCE=RP_BY_LDM_X_8" PARTITION BY KEY (rondb_key)