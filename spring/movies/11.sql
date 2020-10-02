SELECT title FROM movies JOIN ratings JOIN stars JOIN People ON
  movies.id=ratings.movie_id AND movies.id=stars.movie_id AND stars.person_id=people.id WHERE name="Chadwick Boseman"
  ORDER BY rating DESC LIMIT 5