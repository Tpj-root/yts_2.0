# yts_2.0
This script fetches data from an external API, converts the JSON response into a structured format, and stores it in a MySQL database.



```
https://yts.lt/api
https://yts.lt/api/v2/list_movies.json

```



```
https://yts.lt/api/v2/movie_parental_guides.json?movie_id=10 

```







## Example useful queries


```
SELECT m.movie_id, m.title, GROUP_CONCAT(g.name) AS genres
FROM movies m
LEFT JOIN movie_genres mg ON mg.movie_id = m.movie_id
LEFT JOIN genres g ON g.id = mg.genre_id
WHERE m.movie_id = 71658
GROUP BY m.movie_id;

```


```
SELECT t.hash, t.quality, t.size, t.seeds, t.peers, t.url
FROM torrents t
WHERE t.movie_id = 71658
ORDER BY t.seeds DESC;

```



```
SELECT m.movie_id, m.title, m.year
FROM movies m
JOIN movie_genres mg ON mg.movie_id = m.movie_id
JOIN genres g ON g.id = mg.genre_id
WHERE g.name = 'Action'
ORDER BY m.year DESC;

```

```
SELECT t.movie_id, m.title, t.quality, t.seeds
FROM torrents t
JOIN movies m ON m.movie_id = t.movie_id
ORDER BY t.seeds DESC
LIMIT 20;

```

