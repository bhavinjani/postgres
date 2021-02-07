MODULES = $(patsubst %.c,%,$(wildcard src/*.c))
EXTENSION = min_to_max
DATA      = min_to_max--1.0.0.sql

PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)

