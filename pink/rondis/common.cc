#include <stdarg.h>
#include "pink/include/redis_conn.h"
#include <ndbapi/NdbApi.hpp>
#include <ndbapi/Ndb.hpp>

#include "common.h"

#define FOREIGN_KEY_RESTRICT_ERROR 256

int execute_no_commit(NdbTransaction *trans, int &ret_code, bool allow_fail)
{
    printf("Execute NoCommit\n");
    if (trans->execute(NdbTransaction::NoCommit) != 0)
    {
        ret_code = trans->getNdbError().code;
        return -1;
    }
    return 0;
}

int execute_commit(Ndb *ndb, NdbTransaction *trans, int &ret_code)
{
    // printf("Execute transaction\n");
    if (trans->execute(NdbTransaction::Commit) != 0)
    {
        ret_code = trans->getNdbError().code;
        return -1;
    }
    ndb->closeTransaction(trans);
    return 0;
}

/**
 * @brief Writes formatted data to a buffer.
 *
 * This function writes formatted data to the provided buffer using a format string
 * and a variable number of arguments, similar to printf. It ensures that the
 * formatted string does not exceed the specified buffer size.
 *
 * @param buffer A pointer to the buffer where the formatted string will be written.
 * @param bufferSize The size of the buffer.
 * @param format A format string that specifies how subsequent arguments are converted for output.
 * @param ... Additional arguments specifying the data to be formatted.
 * @return The number of characters written, excluding the null terminator. If the output
 *         is truncated due to the buffer size limit, the return value is the number of
 *         characters (excluding the null terminator) which would have been written if
 *         enough space had been available.
 */
int write_formatted(char *buffer, int bufferSize, const char *format, ...)
{
    int len = 0;
    va_list arguments;
    va_start(arguments, format);
    len = vsnprintf(buffer, bufferSize, format, arguments);
    va_end(arguments);
    return len;
}

void assign_ndb_err_to_response(
    std::string *response,
    const char *app_str,
    Uint32 error_code)
{
    char buf[512];
    write_formatted(buf, sizeof(buf), "-ERR NDB(%u) %s\r\n", error_code, app_str);
    printf("%s", buf);
    response->assign(buf);
}

void assign_generic_err_to_response(
    std::string *response,
    const char *app_str)
{
    char buf[512];
    write_formatted(buf, sizeof(buf), "-ERR %s\r\n", app_str);
    printf("%s", buf);
    response->assign(buf);
}

/*
    Generic Errors
*/

void failed_no_such_row_error(std::string *response)
{
    // Redis stuff
    response->assign("$-1\r\n");
}

void failed_large_key(std::string *response)
{
    assign_generic_err_to_response(response,
                                   "Support up to 3000 bytes long keys");
}
