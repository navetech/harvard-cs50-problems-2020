SELECT name FROM people WHERE people.id IN (
  SElect directors.person_id FROM directors JOIN movies Join ratings ON directors.movie_id=movies.id AND movies.id=ratings.movie_id WHERE rating >=9.0)