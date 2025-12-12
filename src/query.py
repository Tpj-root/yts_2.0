import mysql.connector

conn = mysql.connector.connect(
    host="localhost",
    user="ytsuser",
    password="mypassword",
    database="yts_db"
)

cur = conn.cursor()

print("""
Choose Query:

1) Get movie + genres
2) Get torrents (sorted by seeds)
3) Movies with specific genre
4) Top seeded torrents (global)
""")

choice = input("Enter choice (1-4): ").strip()

# ask movie_id only for 1 and 2
movie_id = None
if choice in ("1", "2"):
    movie_id = input("Enter movie_id: ").strip()

if choice == "1":
    # movie + genres
    query = """
    SELECT m.movie_id, m.title, GROUP_CONCAT(g.name) AS genres
    FROM movies m
    LEFT JOIN movie_genres mg ON mg.movie_id = m.movie_id
    LEFT JOIN genres g ON g.id = mg.genre_id
    WHERE m.movie_id = %s
    GROUP BY m.movie_id;
    """
    cur.execute(query, (movie_id,))

elif choice == "2":
    # torrents sorted by seeds
    query = """
    SELECT t.hash, t.quality, t.size, t.seeds, t.peers, t.url
    FROM torrents t
    WHERE t.movie_id = %s
    ORDER BY t.seeds DESC;
    """
    cur.execute(query, (movie_id,))

elif choice == "3":
    # movies by genre
    genre = input("Enter genre name (example: Action): ").strip()
    query = """
    SELECT m.movie_id, m.title, m.year
    FROM movies m
    JOIN movie_genres mg ON mg.movie_id = m.movie_id
    JOIN genres g ON g.id = mg.genre_id
    WHERE g.name = %s
    ORDER BY m.year DESC;
    """
    cur.execute(query, (genre,))

elif choice == "4":
    # global top seeded torrents
    query = """
    SELECT t.movie_id, m.title, t.quality, t.seeds
    FROM torrents t
    JOIN movies m ON m.movie_id = t.movie_id
    ORDER BY t.seeds DESC
    LIMIT 20;
    """
    cur.execute(query)

else:
    print("Invalid choice")
    cur.close()
    conn.close()
    exit()

rows = cur.fetchall()

print("\n✔ Query Result:")
for row in rows:
    print(row)

cur.close()
conn.close()
