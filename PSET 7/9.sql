SELECT p.name
FROM people p
WHERE p.id in (
    SELECT DISTINCT s.person_id
    FROM stars s
    JOIN movies m on s.movie_id=m.id
    WHERE m.year = 2004)
    ORDER BY p.birth;
