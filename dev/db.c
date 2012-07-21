
/*
  +------------------------------------------------------------------------+
  | Phalcon Framework                                                      |
  +------------------------------------------------------------------------+
  | Copyright (c) 2011-2012 Phalcon Team (http://www.phalconphp.com)       |
  +------------------------------------------------------------------------+
  | This source file is subject to the New BSD License that is bundled     |
  | with this package in the file docs/LICENSE.txt.                        |
  |                                                                        |
  | If you did not receive a copy of the license and are unable to         |
  | obtain it through the world-wide-web, please send an email             |
  | to license@phalconphp.com so we can send you a copy immediately.       |
  +------------------------------------------------------------------------+
  | Authors: Andres Gutierrez <andres@phalconphp.com>                      |
  |          Eduar Carvajal <eduar@phalconphp.com>                         |
  +------------------------------------------------------------------------+
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_phalcon.h"
#include "phalcon.h"

#include "Zend/zend_operators.h"
#include "Zend/zend_exceptions.h"
#include "Zend/zend_interfaces.h"

#include "kernel/main.h"
#include "kernel/memory.h"

#include "kernel/object.h"
#include "kernel/fcall.h"
#include "kernel/array.h"
#include "kernel/concat.h"
#include "kernel/operators.h"

/**
 * Phalcon\Db
 *
 * Phalcon\Db and its related classes provide a simple SQL database interface for Phalcon Framework.
 * The Phalcon_Db is the basic class you use to connect your PHP application to an RDBMS.
 * There is a different adapter class for each brand of RDBMS.
 *
 * This component is intended to lower level database operations. If you want to interact with databases using
 * high level abstraction use Phalcon\Model.
 *
 * Phalcon\Db is an abstract class. You only can use it with a database adapter like Phalcon\Db\Adapter\Mysql
 *
 * 
 */

/**
 * Phalcon\Db constructor, this method should not be called directly. Use Phalcon\Db::factory instead
 *
 * @param stdClass $descriptor
 */
PHP_METHOD(Phalcon_Db, __construct){

	zval *descriptor = NULL;

	PHALCON_MM_GROW();
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &descriptor) == FAILURE) {
		PHALCON_MM_RESTORE();
		RETURN_NULL();
	}

	phalcon_update_property_zval(this_ptr, SL("_descriptor"), descriptor TSRMLS_CC);
	
	PHALCON_MM_RESTORE();
}

/**
 * Sets a logger class to log all SQL operations sent to database server
 *
 * @param Phalcon\Logger $logger
 */
PHP_METHOD(Phalcon_Db, setLogger){

	zval *logger = NULL;

	PHALCON_MM_GROW();
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &logger) == FAILURE) {
		PHALCON_MM_RESTORE();
		RETURN_NULL();
	}

	if (Z_TYPE_P(logger) != IS_OBJECT) {
		PHALCON_THROW_EXCEPTION_STR(phalcon_db_exception_ce, "Parameter \\$logger must be an Object");
		return;
	}
	phalcon_update_property_zval(this_ptr, SL("_logger"), logger TSRMLS_CC);
	
	PHALCON_MM_RESTORE();
}

/**
 * Returns the active logger
 *
 * @return Phalcon\Logger
 */
PHP_METHOD(Phalcon_Db, getLogger){

	zval *t0 = NULL;

	PHALCON_MM_GROW();
	PHALCON_ALLOC_ZVAL_MM(t0);
	phalcon_read_property(&t0, this_ptr, SL("_logger"), PH_NOISY_CC);
	
	RETURN_CCTOR(t0);
}

/**
 * Sends arbitrary text to a related logger in the instance
 *
 * @param string $sqlStatement
 * @param int $type
 */
PHP_METHOD(Phalcon_Db, log){

	zval *sql_statement = NULL, *type = NULL, *logger = NULL;

	PHALCON_MM_GROW();
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zz", &sql_statement, &type) == FAILURE) {
		PHALCON_MM_RESTORE();
		RETURN_NULL();
	}

	PHALCON_INIT_VAR(logger);
	phalcon_read_property(&logger, this_ptr, SL("_logger"), PH_NOISY_CC);
	if (zend_is_true(logger)) {
		PHALCON_CALL_METHOD_PARAMS_2_NORETURN(logger, "log", sql_statement, type, PH_NO_CHECK);
	}
	
	PHALCON_MM_RESTORE();
}

/**
 * Sets a database profiler to the connection
 *
 * @param Phalcon\Db\Profiler $profiler
 */
PHP_METHOD(Phalcon_Db, setProfiler){

	zval *profiler = NULL;

	PHALCON_MM_GROW();
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &profiler) == FAILURE) {
		PHALCON_MM_RESTORE();
		RETURN_NULL();
	}

	if (Z_TYPE_P(profiler) == IS_OBJECT) {
		phalcon_update_property_zval(this_ptr, SL("_profiler"), profiler TSRMLS_CC);
	} else {
		PHALCON_THROW_EXCEPTION_STR(phalcon_db_exception_ce, "Profiler must be an object");
		return;
	}
	
	PHALCON_MM_RESTORE();
}

/**
 * Returns the first row in a SQL query result
 *
 * @param string $sqlQuery
 * @param int $fetchMode
 * @return array
 */
PHP_METHOD(Phalcon_Db, fetchOne){

	zval *sql_query = NULL, *fetch_mode = NULL, *result = NULL;
	zval *r0 = NULL;
	zval *a0 = NULL;

	PHALCON_MM_GROW();
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z|z", &sql_query, &fetch_mode) == FAILURE) {
		PHALCON_MM_RESTORE();
		RETURN_NULL();
	}

	if (!fetch_mode) {
		PHALCON_ALLOC_ZVAL_MM(fetch_mode);
		ZVAL_LONG(fetch_mode, 2);
	}
	
	PHALCON_INIT_VAR(result);
	PHALCON_CALL_METHOD_PARAMS_1(result, this_ptr, "query", sql_query, PH_NO_CHECK);
	if (Z_TYPE_P(result) == IS_OBJECT) {
		PHALCON_CALL_METHOD_PARAMS_1_NORETURN(result, "setfetchmode", fetch_mode, PH_NO_CHECK);
		
		PHALCON_ALLOC_ZVAL_MM(r0);
		PHALCON_CALL_METHOD(r0, result, "fetcharray", PH_NO_CHECK);
		RETURN_CTOR(r0);
	} else {
		PHALCON_ALLOC_ZVAL_MM(a0);
		array_init(a0);
		
		RETURN_CTOR(a0);
	}
	
	PHALCON_MM_RESTORE();
}

/**
 * Dumps the complete result of a query into an array
 *
 * @param string $sqlQuery
 * @param int $fetchMode
 * @return array
 */
PHP_METHOD(Phalcon_Db, fetchAll){

	zval *sql_query = NULL, *fetch_mode = NULL, *results = NULL, *result = NULL;
	zval *row = NULL;
	zval *r0 = NULL;

	PHALCON_MM_GROW();
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z|z", &sql_query, &fetch_mode) == FAILURE) {
		PHALCON_MM_RESTORE();
		RETURN_NULL();
	}

	if (!fetch_mode) {
		PHALCON_ALLOC_ZVAL_MM(fetch_mode);
		ZVAL_LONG(fetch_mode, 2);
	}
	
	PHALCON_INIT_VAR(results);
	array_init(results);
	
	PHALCON_INIT_VAR(result);
	PHALCON_CALL_METHOD_PARAMS_1(result, this_ptr, "query", sql_query, PH_NO_CHECK);
	if (Z_TYPE_P(result) == IS_OBJECT) {
		PHALCON_CALL_METHOD_PARAMS_1_NORETURN(result, "setfetchmode", fetch_mode, PH_NO_CHECK);
		ws_e7f0_0:
			
			PHALCON_INIT_VAR(r0);
			PHALCON_CALL_METHOD(r0, result, "fetcharray", PH_NO_CHECK);
			PHALCON_CPY_WRT(row, r0);
			if (!zend_is_true(row)) {
				goto we_e7f0_0;
			}
			phalcon_array_append(&results, row, PH_SEPARATE TSRMLS_CC);
			goto ws_e7f0_0;
		we_e7f0_0:
		if(0){}
	}
	
	
	RETURN_CTOR(results);
}

/**
 * Inserts data into a table using custom RBDM SQL syntax
 *
 * @param string $table
 * @param array $values
 * @param array $fields
 * @param boolean $automaticQuotes
 * @return boolean
 */
PHP_METHOD(Phalcon_Db, insert){

	zval *table = NULL, *values = NULL, *fields = NULL, *automatic_quotes = NULL;
	zval *value = NULL, *key = NULL, *comma = NULL, *joined_values = NULL, *insert_sql = NULL;
	zval *r0 = NULL, *r1 = NULL, *r2 = NULL, *r3 = NULL, *r4 = NULL, *r5 = NULL, *r6 = NULL;
	zval *r7 = NULL;
	zval *i0 = NULL;
	HashTable *ah0;
	HashPosition hp0;
	zval **hd;
	char *hash_index;
	uint hash_index_len;
	ulong hash_num;
	int hash_type;

	PHALCON_MM_GROW();
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zz|zz", &table, &values, &fields, &automatic_quotes) == FAILURE) {
		PHALCON_MM_RESTORE();
		RETURN_NULL();
	}

	PHALCON_SEPARATE_PARAM(values);
	
	if (!fields) {
		PHALCON_ALLOC_ZVAL_MM(fields);
		ZVAL_NULL(fields);
	}
	
	if (!automatic_quotes) {
		PHALCON_ALLOC_ZVAL_MM(automatic_quotes);
		ZVAL_BOOL(automatic_quotes, 0);
	}
	
	if (Z_TYPE_P(values) == IS_ARRAY) { 
		PHALCON_ALLOC_ZVAL_MM(r0);
		phalcon_fast_count(r0, values TSRMLS_CC);
		if (!zend_is_true(r0)) {
			PHALCON_ALLOC_ZVAL_MM(i0);
			object_init_ex(i0, phalcon_db_exception_ce);
			PHALCON_ALLOC_ZVAL_MM(r1);
			PHALCON_CONCAT_SVS(r1, "Unable to insert into ", table, " without data");
			PHALCON_CALL_METHOD_PARAMS_1_NORETURN(i0, "__construct", r1, PH_CHECK);
			phalcon_throw_exception(i0 TSRMLS_CC);
			return;
		} else {
			if (zend_is_true(automatic_quotes)) {
				if (phalcon_valid_foreach(values TSRMLS_CC)) {
					ALLOC_HASHTABLE(ah0);
					zend_hash_init(ah0, 0, NULL, NULL, 0);
					zend_hash_copy(ah0, Z_ARRVAL_P(values), NULL, NULL, sizeof(zval*));
					zend_hash_internal_pointer_reset_ex(ah0, &hp0);
					fes_e7f0_1:
					if(zend_hash_get_current_data_ex(ah0, (void**) &hd, &hp0) != SUCCESS){
						goto fee_e7f0_1;
					} else {
						PHALCON_INIT_VAR(key);
						PHALCON_GET_FOREACH_KEY(key, ah0, hp0);
					}
					PHALCON_INIT_VAR(value);
					ZVAL_ZVAL(value, *hd, 1, 0);
					if (Z_TYPE_P(value) == IS_OBJECT) {
						PHALCON_INIT_VAR(r2);
						phalcon_get_class(r2, value TSRMLS_CC);
						if (PHALCON_COMPARE_STRING(r2, "Phalcon\\Db\\RawValue")) {
							PHALCON_INIT_VAR(r3);
							PHALCON_CALL_METHOD_PARAMS_1(r3, this_ptr, "escapestring", value, PH_NO_CHECK);
							phalcon_array_update(&values, key, &r3, PH_COPY | PH_SEPARATE TSRMLS_CC);
							goto fes_e7f0_1;
						}
					}
					
					PHALCON_INIT_VAR(r4);
					PHALCON_CALL_METHOD_PARAMS_1(r4, this_ptr, "escapestring", value, PH_NO_CHECK);
					
					PHALCON_INIT_VAR(r5);
					PHALCON_CONCAT_SVS(r5, "'", r4, "'");
					phalcon_array_update(&values, key, &r5, PH_COPY | PH_SEPARATE TSRMLS_CC);
					zend_hash_move_forward_ex(ah0, &hp0);
					goto fes_e7f0_1;
					fee_e7f0_1:
					zend_hash_destroy(ah0);
					efree(ah0);
				} else {
					return;
				}
			}
		}
		
		PHALCON_INIT_VAR(comma);
		ZVAL_STRING(comma, ", ", 1);
		
		PHALCON_INIT_VAR(joined_values);
		phalcon_fast_join(joined_values, comma, values TSRMLS_CC);
		if (Z_TYPE_P(fields) == IS_ARRAY) { 
			PHALCON_ALLOC_ZVAL_MM(r6);
			phalcon_fast_join(r6, comma, fields TSRMLS_CC);
			PHALCON_INIT_VAR(insert_sql);
			PHALCON_CONCAT_SVSVSVS(insert_sql, "INSERT INTO ", table, " (", r6, ") VALUES (", joined_values, ")");
		} else {
			PHALCON_INIT_VAR(insert_sql);
			PHALCON_CONCAT_SVSVS(insert_sql, "INSERT INTO ", table, " VALUES (", joined_values, ")");
		}
		
		PHALCON_ALLOC_ZVAL_MM(r7);
		PHALCON_CALL_METHOD_PARAMS_1(r7, this_ptr, "query", insert_sql, PH_NO_CHECK);
		RETURN_CTOR(r7);
	} else {
		PHALCON_THROW_EXCEPTION_STR(phalcon_db_exception_ce, "The second parameter for insert isn't an Array");
		return;
	}
	
	PHALCON_MM_RESTORE();
}

/**
 * Updates data on a table using custom RBDM SQL syntax
 *
 * @param string $table
 * @param array $fields
 * @param array $values
 * @param string $whereCondition
 * @param boolean $automaticQuotes
 * @return boolean
 */
PHP_METHOD(Phalcon_Db, update){

	zval *table = NULL, *fields = NULL, *values = NULL, *where_condition = NULL;
	zval *automatic_quotes = NULL, *i = NULL, *update_values = NULL, *update_sql = NULL;
	zval *field = NULL;
	zval *r0 = NULL, *r1 = NULL, *r2 = NULL, *r3 = NULL, *r4 = NULL, *r5 = NULL, *r6 = NULL;
	zval *r7 = NULL, *r8 = NULL, *r9 = NULL, *r10 = NULL, *r11 = NULL, *r12 = NULL, *r13 = NULL;
	zval *r14 = NULL, *r15 = NULL, *r16 = NULL, *r17 = NULL;
	zval *c0 = NULL;
	zval *t0 = NULL;
	HashTable *ah0;
	HashPosition hp0;
	zval **hd;

	PHALCON_MM_GROW();
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zzz|zz", &table, &fields, &values, &where_condition, &automatic_quotes) == FAILURE) {
		PHALCON_MM_RESTORE();
		RETURN_NULL();
	}

	PHALCON_SEPARATE_PARAM(values);
	
	if (!where_condition) {
		PHALCON_ALLOC_ZVAL_MM(where_condition);
		ZVAL_NULL(where_condition);
	}
	
	if (!automatic_quotes) {
		PHALCON_ALLOC_ZVAL_MM(automatic_quotes);
		ZVAL_BOOL(automatic_quotes, 0);
	}
	
	PHALCON_ALLOC_ZVAL_MM(r0);
	phalcon_fast_count(r0, fields TSRMLS_CC);
	PHALCON_ALLOC_ZVAL_MM(r1);
	phalcon_fast_count(r1, values TSRMLS_CC);
	PHALCON_ALLOC_ZVAL_MM(r2);
	is_not_equal_function(r2, r0, r1 TSRMLS_CC);
	if (zend_is_true(r2)) {
		PHALCON_THROW_EXCEPTION_STR(phalcon_db_exception_ce, "The number of values to update is not the same as fields");
		return;
	}
	
	PHALCON_INIT_VAR(i);
	ZVAL_LONG(i, 0);
	
	PHALCON_INIT_VAR(update_values);
	array_init(update_values);
	
	PHALCON_INIT_VAR(update_sql);
	PHALCON_CONCAT_SVS(update_sql, "UPDATE ", table, " SET ");
	if (phalcon_valid_foreach(fields TSRMLS_CC)) {
		ah0 = Z_ARRVAL_P(fields);
		zend_hash_internal_pointer_reset_ex(ah0, &hp0);
		fes_e7f0_2:
		if(zend_hash_get_current_data_ex(ah0, (void**) &hd, &hp0) != SUCCESS){
			goto fee_e7f0_2;
		}
		
		PHALCON_INIT_VAR(field);
		ZVAL_ZVAL(field, *hd, 1, 0);
		if (zend_is_true(automatic_quotes)) {
			PHALCON_INIT_VAR(r3);
			phalcon_array_fetch(&r3, values, i, PH_NOISY_CC);
			if (Z_TYPE_P(r3) == IS_OBJECT) {
				PHALCON_INIT_VAR(r4);
				phalcon_array_fetch(&r4, values, i, PH_NOISY_CC);
				PHALCON_INIT_VAR(r5);
				phalcon_get_class(r5, r4 TSRMLS_CC);
				if (PHALCON_COMPARE_STRING(r5, "Phalcon\\Db\\RawValue")) {
					PHALCON_INIT_VAR(r6);
					phalcon_array_fetch(&r6, values, i, PH_NOISY_CC);
					PHALCON_INIT_VAR(r7);
					PHALCON_CALL_METHOD(r7, r6, "getvalue", PH_NO_CHECK);
					PHALCON_INIT_VAR(r8);
					PHALCON_CALL_METHOD_PARAMS_1(r8, this_ptr, "escapestring", r7, PH_NO_CHECK);
					phalcon_array_update(&values, i, &r8, PH_COPY | PH_SEPARATE TSRMLS_CC);
					goto fes_e7f0_2;
				}
			}
			
			PHALCON_INIT_VAR(r9);
			phalcon_array_fetch(&r9, values, i, PH_NOISY_CC);
			
			PHALCON_INIT_VAR(r10);
			PHALCON_CALL_METHOD_PARAMS_1(r10, this_ptr, "escapestring", r9, PH_NO_CHECK);
			
			PHALCON_INIT_VAR(r11);
			PHALCON_CONCAT_SVS(r11, "'", r10, "'");
			phalcon_array_update(&values, i, &r11, PH_COPY | PH_SEPARATE TSRMLS_CC);
		}
		
		PHALCON_INIT_VAR(r12);
		phalcon_array_fetch(&r12, values, i, PH_NOISY_CC);
		
		PHALCON_INIT_VAR(r13);
		PHALCON_CONCAT_VSV(r13, field, " = ", r12);
		phalcon_array_append(&update_values, r13, PH_SEPARATE TSRMLS_CC);
		PHALCON_SEPARATE(i);
		increment_function(i);
		zend_hash_move_forward_ex(ah0, &hp0);
		goto fes_e7f0_2;
		fee_e7f0_2:
		if(0){}
	} else {
		return;
	}
	
	PHALCON_INIT_VAR(c0);
	ZVAL_STRING(c0, ", ", 1);
	
	PHALCON_ALLOC_ZVAL_MM(r14);
	phalcon_fast_join(r14, c0, update_values TSRMLS_CC);
	phalcon_concat_self(&update_sql, r14 TSRMLS_CC);
	
	PHALCON_INIT_VAR(t0);
	ZVAL_NULL(t0);
	
	PHALCON_ALLOC_ZVAL_MM(r15);
	is_not_equal_function(r15, where_condition, t0 TSRMLS_CC);
	if (zend_is_true(r15)) {
		PHALCON_ALLOC_ZVAL_MM(r16);
		PHALCON_CONCAT_SV(r16, " WHERE ", where_condition);
		phalcon_concat_self(&update_sql, r16 TSRMLS_CC);
	}
	
	PHALCON_ALLOC_ZVAL_MM(r17);
	PHALCON_CALL_METHOD_PARAMS_1(r17, this_ptr, "query", update_sql, PH_NO_CHECK);
	RETURN_CTOR(r17);
}

/**
 * Deletes data from a table using custom RBDM SQL syntax
 *
 * @param string $table
 * @param string $whereCondition
 * @return boolean
 */
PHP_METHOD(Phalcon_Db, delete){

	zval *table = NULL, *where_condition = NULL;
	zval *r0 = NULL, *r1 = NULL, *r2 = NULL, *r3 = NULL;

	PHALCON_MM_GROW();
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z|z", &table, &where_condition) == FAILURE) {
		PHALCON_MM_RESTORE();
		RETURN_NULL();
	}

	if (!where_condition) {
		PHALCON_ALLOC_ZVAL_MM(where_condition);
		ZVAL_STRING(where_condition, "", 1);
	}
	
	if (!PHALCON_COMPARE_STRING(where_condition, "")) {
		PHALCON_ALLOC_ZVAL_MM(r0);
		PHALCON_CONCAT_SVSV(r0, "DELETE FROM ", table, " WHERE ", where_condition);
		PHALCON_ALLOC_ZVAL_MM(r1);
		PHALCON_CALL_METHOD_PARAMS_1(r1, this_ptr, "query", r0, PH_NO_CHECK);
		RETURN_CTOR(r1);
	} else {
		PHALCON_ALLOC_ZVAL_MM(r2);
		PHALCON_CONCAT_SV(r2, "DELETE FROM ", table);
		PHALCON_ALLOC_ZVAL_MM(r3);
		PHALCON_CALL_METHOD_PARAMS_1(r3, this_ptr, "query", r2, PH_NO_CHECK);
		RETURN_CTOR(r3);
	}
	
	PHALCON_MM_RESTORE();
}

/**
 * Starts a transaction in the connection
 *
 * @return boolean
 */
PHP_METHOD(Phalcon_Db, begin){

	zval *c0 = NULL;
	zval *r0 = NULL;

	PHALCON_MM_GROW();
	phalcon_update_property_bool(this_ptr, SL("_autoCommit"), 0 TSRMLS_CC);
	phalcon_update_property_bool(this_ptr, SL("_underTransaction"), 1 TSRMLS_CC);
	
	PHALCON_INIT_VAR(c0);
	ZVAL_STRING(c0, "BEGIN", 1);
	
	PHALCON_ALLOC_ZVAL_MM(r0);
	PHALCON_CALL_METHOD_PARAMS_1(r0, this_ptr, "query", c0, PH_NO_CHECK);
	RETURN_CTOR(r0);
}

/**
 * Rollbacks the active transaction in the connection
 *
 * @return boolean
 */
PHP_METHOD(Phalcon_Db, rollback){

	zval *t0 = NULL;
	zval *c0 = NULL;
	zval *r0 = NULL;

	PHALCON_MM_GROW();
	PHALCON_ALLOC_ZVAL_MM(t0);
	phalcon_read_property(&t0, this_ptr, SL("_underTransaction"), PH_NOISY_CC);
	if (zend_is_true(t0)) {
		phalcon_update_property_bool(this_ptr, SL("_underTransaction"), 0 TSRMLS_CC);
		phalcon_update_property_bool(this_ptr, SL("_autoCommit"), 1 TSRMLS_CC);
		
		PHALCON_INIT_VAR(c0);
		ZVAL_STRING(c0, "ROLLBACK", 1);
		
		PHALCON_ALLOC_ZVAL_MM(r0);
		PHALCON_CALL_METHOD_PARAMS_1(r0, this_ptr, "query", c0, PH_NO_CHECK);
		RETURN_CTOR(r0);
	} else {
		PHALCON_THROW_EXCEPTION_STR(phalcon_db_exception_ce, "There is not an active transaction on relational manager");
		return;
	}
	
	PHALCON_MM_RESTORE();
}

/**
 * Commits the active transaction in the connection
 *
 * @return boolean
 */
PHP_METHOD(Phalcon_Db, commit){

	zval *t0 = NULL;
	zval *c0 = NULL;
	zval *r0 = NULL;

	PHALCON_MM_GROW();
	PHALCON_ALLOC_ZVAL_MM(t0);
	phalcon_read_property(&t0, this_ptr, SL("_underTransaction"), PH_NOISY_CC);
	if (zend_is_true(t0)) {
		phalcon_update_property_bool(this_ptr, SL("_underTransaction"), 0 TSRMLS_CC);
		phalcon_update_property_bool(this_ptr, SL("_autoCommit"), 1 TSRMLS_CC);
		
		PHALCON_INIT_VAR(c0);
		ZVAL_STRING(c0, "COMMIT", 1);
		
		PHALCON_ALLOC_ZVAL_MM(r0);
		PHALCON_CALL_METHOD_PARAMS_1(r0, this_ptr, "query", c0, PH_NO_CHECK);
		RETURN_CTOR(r0);
	} else {
		PHALCON_THROW_EXCEPTION_STR(phalcon_db_exception_ce, "There is not an active transaction on relational manager");
		return;
	}
	
	PHALCON_MM_RESTORE();
}

/**
 * Manually sets a "under transaction" state for the connection
 *
 * @param boolean $underTransaction
 */
PHP_METHOD(Phalcon_Db, setUnderTransaction){

	zval *under_transaction = NULL;

	PHALCON_MM_GROW();
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &under_transaction) == FAILURE) {
		PHALCON_MM_RESTORE();
		RETURN_NULL();
	}

	phalcon_update_property_zval(this_ptr, SL("_underTransaction"), under_transaction TSRMLS_CC);
	
	PHALCON_MM_RESTORE();
}

/**
 * Checks whether connection is under database transaction
 *
 * @return boolean
 */
PHP_METHOD(Phalcon_Db, isUnderTransaction){

	zval *t0 = NULL;

	PHALCON_MM_GROW();
	PHALCON_ALLOC_ZVAL_MM(t0);
	phalcon_read_property(&t0, this_ptr, SL("_underTransaction"), PH_NOISY_CC);
	
	RETURN_CCTOR(t0);
}

/**
 * Checks whether connection have auto commit
 *
 * @return boolean
 */
PHP_METHOD(Phalcon_Db, getHaveAutoCommit){

	zval *t0 = NULL;

	PHALCON_MM_GROW();
	PHALCON_ALLOC_ZVAL_MM(t0);
	phalcon_read_property(&t0, this_ptr, SL("_autoCommit"), PH_NOISY_CC);
	
	RETURN_CCTOR(t0);
}

/**
 * Returns database name in the internal connection
 *
 * @return string
 */
PHP_METHOD(Phalcon_Db, getDatabaseName){

	zval *t0 = NULL, *t1 = NULL, *t2 = NULL;
	int eval_int;

	PHALCON_MM_GROW();
	PHALCON_ALLOC_ZVAL_MM(t0);
	phalcon_read_property(&t0, this_ptr, SL("_descriptor"), PH_NOISY_CC);
	eval_int = phalcon_isset_property(t0, SL("name") TSRMLS_CC);
	if (eval_int) {
		PHALCON_ALLOC_ZVAL_MM(t1);
		phalcon_read_property(&t1, this_ptr, SL("_descriptor"), PH_NOISY_CC);
		PHALCON_ALLOC_ZVAL_MM(t2);
		phalcon_read_property(&t2, t1, SL("name"), PH_NOISY_CC);
		
		RETURN_CCTOR(t2);
	}
	PHALCON_MM_RESTORE();
	RETURN_STRING("", 1);
}

/**
 * Returns active schema name in the internal connection
 *
 * @return string
 */
PHP_METHOD(Phalcon_Db, getDefaultSchema){

	zval *t0 = NULL, *t1 = NULL, *t2 = NULL, *t3 = NULL, *t4 = NULL, *t5 = NULL;
	int eval_int;

	PHALCON_MM_GROW();
	PHALCON_ALLOC_ZVAL_MM(t0);
	phalcon_read_property(&t0, this_ptr, SL("_descriptor"), PH_NOISY_CC);
	eval_int = phalcon_isset_property(t0, SL("schema") TSRMLS_CC);
	if (eval_int) {
		PHALCON_ALLOC_ZVAL_MM(t1);
		phalcon_read_property(&t1, this_ptr, SL("_descriptor"), PH_NOISY_CC);
		PHALCON_ALLOC_ZVAL_MM(t2);
		phalcon_read_property(&t2, t1, SL("schema"), PH_NOISY_CC);
		
		RETURN_CCTOR(t2);
	} else {
		PHALCON_ALLOC_ZVAL_MM(t3);
		phalcon_read_property(&t3, this_ptr, SL("_descriptor"), PH_NOISY_CC);
		eval_int = phalcon_isset_property(t3, SL("name") TSRMLS_CC);
		if (eval_int) {
			PHALCON_ALLOC_ZVAL_MM(t4);
			phalcon_read_property(&t4, this_ptr, SL("_descriptor"), PH_NOISY_CC);
			PHALCON_ALLOC_ZVAL_MM(t5);
			phalcon_read_property(&t5, t4, SL("name"), PH_NOISY_CC);
			
			RETURN_CCTOR(t5);
		}
	}
	PHALCON_MM_RESTORE();
	RETURN_STRING("", 1);
}

/**
 * Returns the username which has connected to the database
 *
 * @return string
 */
PHP_METHOD(Phalcon_Db, getUsername){

	zval *t0 = NULL, *t1 = NULL, *t2 = NULL;
	int eval_int;

	PHALCON_MM_GROW();
	PHALCON_ALLOC_ZVAL_MM(t0);
	phalcon_read_property(&t0, this_ptr, SL("_descriptor"), PH_NOISY_CC);
	eval_int = phalcon_isset_property(t0, SL("username") TSRMLS_CC);
	if (eval_int) {
		PHALCON_ALLOC_ZVAL_MM(t1);
		phalcon_read_property(&t1, this_ptr, SL("_descriptor"), PH_NOISY_CC);
		PHALCON_ALLOC_ZVAL_MM(t2);
		phalcon_read_property(&t2, t1, SL("username"), PH_NOISY_CC);
		
		RETURN_CCTOR(t2);
	}
	PHALCON_MM_RESTORE();
	RETURN_STRING("", 1);
}

/**
 * Returns the username which has connected to the database
 *
 * @return string
 */
PHP_METHOD(Phalcon_Db, getHostName){

	zval *t0 = NULL, *t1 = NULL, *t2 = NULL;
	int eval_int;

	PHALCON_MM_GROW();
	PHALCON_ALLOC_ZVAL_MM(t0);
	phalcon_read_property(&t0, this_ptr, SL("_descriptor"), PH_NOISY_CC);
	eval_int = phalcon_isset_property(t0, SL("host") TSRMLS_CC);
	if (eval_int) {
		PHALCON_ALLOC_ZVAL_MM(t1);
		phalcon_read_property(&t1, this_ptr, SL("_descriptor"), PH_NOISY_CC);
		PHALCON_ALLOC_ZVAL_MM(t2);
		phalcon_read_property(&t2, t1, SL("host"), PH_NOISY_CC);
		
		RETURN_CCTOR(t2);
	}
	PHALCON_MM_RESTORE();
	RETURN_STRING("", 1);
}

/**
 * This method is executed before every SQL statement sent to the database system
 *
 * @param string $sqlStatement
 */
PHP_METHOD(Phalcon_Db, _beforeQuery){

	zval *sql_statement = NULL, *logger = NULL, *profiler = NULL;
	zval *c0 = NULL, *c1 = NULL;
	zval *r0 = NULL, *r1 = NULL;

	PHALCON_MM_GROW();
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &sql_statement) == FAILURE) {
		PHALCON_MM_RESTORE();
		RETURN_NULL();
	}

	PHALCON_INIT_VAR(logger);
	phalcon_read_property(&logger, this_ptr, SL("_logger"), PH_NOISY_CC);
	if (zend_is_true(logger)) {
		PHALCON_INIT_VAR(c0);
		ZVAL_BOOL(c0, 1);
		PHALCON_ALLOC_ZVAL_MM(r0);
		PHALCON_CALL_METHOD_PARAMS_1(r0, this_ptr, "getconnectionid", c0, PH_NO_CHECK);
		PHALCON_ALLOC_ZVAL_MM(r1);
		PHALCON_CONCAT_SVSV(r1, "[", r0, "] ", sql_statement);
		PHALCON_INIT_VAR(c1);
		ZVAL_LONG(c1, 7);
		PHALCON_CALL_METHOD_PARAMS_2_NORETURN(logger, "log", r1, c1, PH_NO_CHECK);
	}
	
	PHALCON_INIT_VAR(profiler);
	phalcon_read_property(&profiler, this_ptr, SL("_profiler"), PH_NOISY_CC);
	if (zend_is_true(profiler)) {
		PHALCON_CALL_METHOD_PARAMS_1_NORETURN(profiler, "startprofile", sql_statement, PH_NO_CHECK);
	}
	
	PHALCON_MM_RESTORE();
}

/**
 * This method is executed after every SQL statement sent to the database system
 *
 * @param string $sqlStatement
 */
PHP_METHOD(Phalcon_Db, _afterQuery){

	zval *sql_statement = NULL;
	zval *t0 = NULL, *t1 = NULL;

	PHALCON_MM_GROW();
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &sql_statement) == FAILURE) {
		PHALCON_MM_RESTORE();
		RETURN_NULL();
	}

	PHALCON_ALLOC_ZVAL_MM(t0);
	phalcon_read_property(&t0, this_ptr, SL("_profiler"), PH_NOISY_CC);
	if (zend_is_true(t0)) {
		PHALCON_ALLOC_ZVAL_MM(t1);
		phalcon_read_property(&t1, this_ptr, SL("_profiler"), PH_NOISY_CC);
		PHALCON_CALL_METHOD_PARAMS_1_NORETURN(t1, "stopprofile", sql_statement, PH_NO_CHECK);
	}
	
	PHALCON_MM_RESTORE();
}

/**
 * Instantiates Phalcon\Db adapter using given parameters
 *
 * @param string $adapterName
 * @param stdClass $options
 * @return Phalcon\Db\Adapter
 */
PHP_METHOD(Phalcon_Db, factory){

	zval *adapter_name = NULL, *options = NULL, *descriptor = NULL, *value = NULL;
	zval *key = NULL, *class_name = NULL;
	zval *r0 = NULL, *r1 = NULL;
	zval *i0 = NULL, *i1 = NULL;
	HashTable *ah0;
	HashPosition hp0;
	zval **hd;
	char *hash_index;
	uint hash_index_len;
	ulong hash_num;
	int hash_type;
	zend_class_entry *ce0;

	PHALCON_MM_GROW();
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zz", &adapter_name, &options) == FAILURE) {
		PHALCON_MM_RESTORE();
		RETURN_NULL();
	}

	if (!zend_is_true(adapter_name)) {
		PHALCON_THROW_EXCEPTION_STR(phalcon_db_exception_ce, "A valid adapter name is required");
		return;
	}
	if (Z_TYPE_P(options) != IS_ARRAY) { 
		if (Z_TYPE_P(options) != IS_OBJECT) {
			PHALCON_THROW_EXCEPTION_STR(phalcon_db_exception_ce, "The parameter 'options' must be an Array or Object");
			return;
		}
	}
	
	if (Z_TYPE_P(options) == IS_ARRAY) { 
		PHALCON_INIT_VAR(descriptor);
		object_init(descriptor);
		if (phalcon_valid_foreach(options TSRMLS_CC)) {
			ah0 = Z_ARRVAL_P(options);
			zend_hash_internal_pointer_reset_ex(ah0, &hp0);
			fes_e7f0_3:
			if(zend_hash_get_current_data_ex(ah0, (void**) &hd, &hp0) != SUCCESS){
				goto fee_e7f0_3;
			} else {
				PHALCON_INIT_VAR(key);
				PHALCON_GET_FOREACH_KEY(key, ah0, hp0);
			}
			PHALCON_INIT_VAR(value);
			ZVAL_ZVAL(value, *hd, 1, 0);
			phalcon_update_property_zval_zval(descriptor, key, value TSRMLS_CC);
			zend_hash_move_forward_ex(ah0, &hp0);
			goto fes_e7f0_3;
			fee_e7f0_3:
			if(0){}
		} else {
			return;
		}
	} else {
		PHALCON_CPY_WRT(descriptor, options);
	}
	
	PHALCON_INIT_VAR(class_name);
	PHALCON_CONCAT_SV(class_name, "Phalcon\\Db\\Adapter\\", adapter_name);
	
	PHALCON_ALLOC_ZVAL_MM(r0);
	PHALCON_CALL_FUNC_PARAMS_1(r0, "class_exists", class_name);
	if (zend_is_true(r0)) {
		ce0 = phalcon_fetch_class(class_name TSRMLS_CC);
		PHALCON_ALLOC_ZVAL_MM(i0);
		object_init_ex(i0, ce0);
		PHALCON_CALL_METHOD_PARAMS_1_NORETURN(i0, "__construct", descriptor, PH_CHECK);
		
		RETURN_CTOR(i0);
	}
	
	PHALCON_ALLOC_ZVAL_MM(i1);
	object_init_ex(i1, phalcon_db_exception_ce);
	
	PHALCON_ALLOC_ZVAL_MM(r1);
	PHALCON_CONCAT_SVS(r1, "Database adapter class ", class_name, " was not found");
	PHALCON_CALL_METHOD_PARAMS_1_NORETURN(i1, "__construct", r1, PH_CHECK);
	phalcon_throw_exception(i1 TSRMLS_CC);
	return;
}

