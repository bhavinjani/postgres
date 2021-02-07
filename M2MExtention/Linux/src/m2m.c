/*------------------------------------------------------------------------
 * Author : Bhavin Jani
 * Package : m2m
 * Create Date: 06-02-2021
 * Updates	:
 * Purpose :A set of POC for the aggregation in combination for min(val) and max(val)
 * Proto type of Combine aggrigate for min & max
 *
 *
 *-------------------------------------------------------------------------
 */
#include <postgres.h>
#include <fmgr.h>
#include <executor/executor.h>
#include <utils/int8.h>
#include <utils/inval.h>
#include <string.h>
#include <access/heapam.h>
#include <funcapi.h>
#include <catalog/pg_type.h>

#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif

/* commented next line to Build on Window64 for the  */
// PGDLLEXPORT Datum m2mbigint(PG_FUNCTION_ARGS);

PG_FUNCTION_INFO_V1(m2mbigint);

Datum
m2mbigint(PG_FUNCTION_ARGS)
{

	// Get Composite type 
	HeapTupleHeader  tytup_lt;
	tytup_lt = PG_GETARG_HEAPTUPLEHEADER(0);
	
	// Get Running raw arguments
	Datum		el = PG_GETARG_DATUM(1);
	
	// Debud logs 
	// elog(DEBUG1, "m2m: m2m: %s %s", tytup_lt);



	
	// if Argument is null
	bool isnull = PG_ARGISNULL(0);

	// decalare generic variable to hold the value
	Datum min, max;


	//Delclare nulls for the the values
	bool  nulls[2];


	// Get atribute for the tupple from the composite type
	Datum tytup_min = GetAttributeByName(tytup_lt, "min_val", &isnull);
	Datum tytup_max = GetAttributeByName(tytup_lt, "max_val", &isnull);

	
	// Check if there is any 0 values -- Edge cases 

	tytup_min = ((tytup_min == 0) ? tytup_min = el : tytup_min);
	tytup_max = ((tytup_max == 0) ? tytup_max = el : tytup_max);

	/*
	if (tytup_min == 0) { tytup_min = el;}

	if (tytup_max == 0) {tytup_max = el;}
	*/


	if (isnull) {

		PG_RETURN_NULL();

	}

	else {

		// get min

		if (el > tytup_min) {

			min = tytup_min;
		}
		else
		{
			min = el;
		}


		// get max

		if (el < tytup_max && tytup_max > tytup_min) {

			max = tytup_max;
		}
		else
		{
			max = el;
		}



		// Prepare tupple sets and memory hold
		Datum values[2];
		TupleDesc tupdesc;
		HeapTuple tuple;
		
		// alocate memory for performace
		MemSet(values, 0, sizeof(values));
		MemSet(nulls, 0, sizeof(nulls));

		// create template
		tupdesc = CreateTemplateTupleDesc(2);

		Oid oid_min_val = get_fn_expr_argtype(fcinfo->flinfo, 1);
		TupleDescInitEntry(tupdesc, (AttrNumber)1, "min_val", INT8OID, -1, 0);
		TupleDescInitEntry(tupdesc, (AttrNumber)2, "max_val", oid_min_val, -1, 0);


		if (get_call_result_type(fcinfo, NULL, &tupdesc) != TYPEFUNC_COMPOSITE)
			ereport(ERROR,
				(errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
					errmsg("function returning record called in context " "that cannot accept type record")));

		tupdesc = BlessTupleDesc(tupdesc);

		values[0] = Int64GetDatum(min);
		values[1] = Int64GetDatum(max);

		
		tuple = heap_form_tuple(tupdesc, values, nulls);


		
		
		PG_RETURN_DATUM(HeapTupleGetDatum(tuple));


	}


}


/* commented next line to Build on Window64 for the  */
// PGDLLEXPORT Datum m2mbigint_ffunc(PG_FUNCTION_ARGS);

PG_FUNCTION_INFO_V1(m2mbigint_ffunc);

Datum
m2mbigint_ffunc(PG_FUNCTION_ARGS)
{

	// Get Composite type 
	HeapTupleHeader  tytup;
	tytup = PG_GETARG_HEAPTUPLEHEADER(0);

	// if Argument is null
	bool isnull = PG_ARGISNULL(0);

		// Get atribute for the tupple from the composite type
	Datum tytup_min = GetAttributeByName(tytup, "min_val", &isnull);
	Datum tytup_max = GetAttributeByName(tytup, "max_val", &isnull);
	
	 char finalresult[7000];
	 sprintf(finalresult, "_%u \-->\ %u", tytup_min,tytup_max);
	 	
	 
	 PG_RETURN_DATUM(finalresult);


	}


