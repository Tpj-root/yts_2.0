import mysql.connector

conn = mysql.connector.connect(
    host="localhost",
    user="ytsuser",
    password="mypassword",
    database="yts_db"
)

cur = conn.cursor()

cur.execute("INSERT INTO test_movies (movie_id, title) VALUES (%s, %s)",
            (71658, "The Toxic Avenger"))

conn.commit()

cur.close()
conn.close()

print("✔ Insert OK")
