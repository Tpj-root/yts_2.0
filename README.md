# yts_2.0
This script fetches data from an external API, converts the JSON response into a structured format, and stores it in a MySQL database.



```
https://yts.lt/api


https://yts.bz/api
https://yts.bz/api/v2/movie_details.json
https://yts.bz/api/v2/movie_details.json?movie_id=1

https://movies-api.accel.li/api/v2/movie_details.json?movie_id=
https://movies-api.accel.li/api/v2/movie_details.json?movie_id=1

```



```
https://yts.lt/api/v2/movie_parental_guides.json?movie_id=10 



```







## Check API

```
https://yts.bz/api/v2/movie_details.json?movie_id=71658


"id": 71658,
      "url": "https://yts.bz/movies/the-toxic-avenger-2023",
      "imdb_code": "tt1633359",
      "title": "The Toxic Avenger",
      "title_english": "The Toxic Avenger"
```



## Find "movie_count": 73922,

Last checked : Mon 23 Mar 12:46:43 IST 2026


```
https://yts.bz/api/v2/list_movies.json?limit=50&page=1




https://yts.bz/api/v2/movie_details.json?movie_id=73922

```



## LASTDATE OF UPLOAD


```
https://yts.bz/api/v2/movie_details.json?movie_id=75100

      "date_uploaded": "2026-03-18 08:49:00",



LAST FILM

https://yts.bz/api/v2/movie_details.json?movie_id=75196

          "date_uploaded": "2026-03-23 00:06:00",


```






###  This way, we can get the movie count and the last movie ID.

```
https://yts.bz/api/v2/list_movies.json?limit=50&page=1

 "movie_count": 73922,

        "id": 75196,
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

