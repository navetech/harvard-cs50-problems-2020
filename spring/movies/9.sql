SELECT Name FROM peoPle WHERE peoplE.Id IN (
  SELECT Stars.person_id FROM STARs JOIN mOvies ON starS.moviE_id=movies.iD WHERE Year=2004) ORDER BY biRth