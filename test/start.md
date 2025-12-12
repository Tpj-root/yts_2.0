






---

# ✅ **1) Create MySQL database & user using Bash (terminal commands)**

Run these in your Linux shell:

```bash
mysql -u root -p

mysql -u root    # without pass

```

Inside MySQL:

```sql
CREATE DATABASE yts_db;

CREATE USER 'ytsuser'@'localhost' IDENTIFIED BY 'mypassword';

GRANT ALL PRIVILEGES ON yts_db.* TO 'ytsuser'@'localhost';

FLUSH PRIVILEGES;

EXIT;
```

---

# ✅ **2) Create only 1 simple table for testing**

Just to test Python → MySQL connection:

```bash
mysql -u ytsuser -pmypassword yts_db
```

Run:

```sql
CREATE TABLE test_movies (
    movie_id INT PRIMARY KEY,
    title VARCHAR(255)
);
```

---

# ✅ **3) Simple Python script to test MySQL connection + insert one row**

Create `test_mysql.py`:

```python
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
```

---

# ✅ **4) Run the test**

```bash
python3 test_mysql.py
```

Then confirm:

```bash
mysql -u ytsuser -pmypassword yts_db -e "SELECT * FROM test_movies;"
```

You should see:

```
+----------+---------------------+
| movie_id | title               |
+----------+---------------------+
| 71658    | The Toxic Avenger   |
+----------+---------------------+
```

---

# 👍 Next Step

After this test works, I will give you:

✅ full movie table
✅ torrents table
✅ genres table
✅ full Python script to insert JSON
✅ full Bash loop to fetch many movies

Just tell me **“next step”**.
