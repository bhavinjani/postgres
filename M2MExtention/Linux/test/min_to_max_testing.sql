/*Preparation for testing*/

CREATE TABLE campaign (
  id bigserial primary key not null,
  campaigncode VARCHAR(10) NOT NULL, -- not unique
  article TEXT,
  name TEXT NOT NULL -- not unique
  );
  
  insert into campaign (
    campaigncode, article, name
)
select
    left(md5(i::text), 5),
    md5(random()::text),
    md5(random()::text)
from generate_series(1, 10000000) s(i);


create index idx_campaigncode_id_campaigncode on public.campaign
using btree (id,campaigncode);


/* End */


-- Test Case ☺ 
SELECT min_to_max(val) FROM (VALUES(5),(3),(6),(7),(9),(10),(7)) t(val);

-- Test Case 1 - With Null values
SELECT min_to_max(val) FROM (VALUES(5),(null),(999999999),(7),(9),(5000000),(40)) t(val);

-- Test Case 2 withthout Group by full on table 
-- SELECT min_to_max(id), min(id) || ' - > ' || max(id) FROM campaign;

-- Test Case 3 With Group by full on table 
-- explain (analyse, verbose)
SELECT min_to_max(id) FROM campaign ;

-- Test Case 4 with limited filters with group by
-- explain (analyse, verbose)
SELECT min_to_max(id),campaigncode FROM campaign
where campaigncode in ('00001','00003','00005','00007','00009','0000a')
group by campaigncode; 

-- extra
-- explain (analyse, verbose)
SELECT min(id)||' -> '||max(id),campaigncode FROM campaign
where campaigncode in ('00001','00003','00005','00007','00009','0000a') 
group by campaigncode ;




