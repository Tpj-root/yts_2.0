import mysql.connector

conn = mysql.connector.connect(
    host="localhost",
    user="ytsuser",
    password="mypassword",
    database="yts_db"
)

cur = conn.cursor()

# # INSERT
# cur.execute(
#     "INSERT INTO test_movies (movie_id, title) VALUES (%s, %s)",
#     (71658, "The Toxic Avenger")
# )
# conn.commit()

# print("✔ Insert OK")

# SIMPLE SELECT QUERY
cur.execute("SELECT * FROM test_movies")

rows = cur.fetchall()

print("✔ Query Result:")
for row in rows:
    print(row)

cur.close()
conn.close()
