SELECT p.name
FROM people p
WHERE p.id in(
    SELECT DISTINCT(d.person_id)
    FROM directors d
    JOIN movies m ON m.id = d.movie_id
    JOIN ratings r ON r.movie_id = m.id
    WHERE r.rating >= 9.0
    );
