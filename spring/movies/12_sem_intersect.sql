SELECT title from movies WHERE
  movies.id IN (SELECT movies.id FROM movies JOIN stars JOIN people ON movies.id=stars.movie_id AND stars.person_id=people.id WHERE name="Johnny Depp")
  AND
  movies.id IN (SELECT movies.id FROM movies JOIN stars JOIN people ON movies.id=stars.movie_id AND stars.person_id=people.id WHERE name="Helena Bonham Carter")