/* contrib/min_to_max/min_to_max--1.0.sql */

-- complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use "CREATE EXTENSION min_to_max" to load this file. \quit


CREATE TYPE ty_min_to_max AS(min_val bigint,max_val bigint);

CREATE OR REPLACE FUNCTION m2mbigint_sfunc(ty_min_to_max,bigint)
    RETURNS ty_min_to_max
    LANGUAGE 'c'
    COST 1
    VOLATILE STRICT PARALLEL SAFE 
AS 'm2m', 'm2mbigint';


CREATE OR REPLACE FUNCTION m2mbigint_ffunc(ty_min_to_max)
    RETURNS text
    LANGUAGE 'c'
    COST 1
    VOLATILE STRICT PARALLEL SAFE 
AS 'm2m', 'm2mbigint_ffunc';

DROP AGGREGATE IF EXISTS min_to_max(bigint);
DROP AGGREGATE IF EXISTS min_to_max(anyelement);
CREATE AGGREGATE min_to_max(bigint) (
  SFUNC=m2mbigint_sfunc,
  STYPE=ty_min_to_max,
  FINALFUNC=m2mbigint_ffunc,
  PARALLEL = SAFE,
  INITCOND='(0,0)'
);