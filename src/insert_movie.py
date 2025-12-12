#!/usr/bin/env python3
import sys, json, mysql.connector, datetime

# --- CONFIG: change to your DB settings ---
DB_CONFIG = {
    "host": "127.0.0.1",
    "user": "ytsuser",
    "password": "mypassword",
    "database": "yts_db",
    "autocommit": True
}
# ------------------------------------------

def dt_from_unix(u):
    try:
        return datetime.datetime.utcfromtimestamp(int(u))
    except:
        return None

def get_or_create_genre(cur, name):
    cur.execute("SELECT id FROM genres WHERE name=%s", (name,))
    r = cur.fetchone()
    if r: return r[0]
    cur.execute("INSERT INTO genres (name) VALUES (%s)", (name,))
    return cur.lastrowid

def upsert_movie(cur, m):
    date_uploaded = dt_from_unix(m.get("date_uploaded_unix")) or None
    params = (
        m["id"], m.get("imdb_code"), m.get("title"), m.get("title_english"),
        m.get("title_long"), m.get("slug"), m.get("year"), m.get("rating"),
        m.get("runtime"), m.get("language"), m.get("like_count"),
        m.get("description_intro"), m.get("description_full"),
        m.get("yt_trailer_code"), m.get("mpa_rating"),
        m.get("background_image"), m.get("small_cover_image"),
        m.get("medium_cover_image"), m.get("large_cover_image"),
        date_uploaded, m.get("date_uploaded_unix")
    )
    cur.execute("""
      INSERT INTO movies (movie_id, imdb_code, title, title_english, title_long, slug,
        year, rating, runtime, language, like_count, description_intro, description_full,
        yt_trailer_code, mpa_rating, background_image, small_cover_image,
        medium_cover_image, large_cover_image, date_uploaded, date_uploaded_unix)
      VALUES (%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s)
      ON DUPLICATE KEY UPDATE
        imdb_code=VALUES(imdb_code), title=VALUES(title), rating=VALUES(rating),
        runtime=VALUES(runtime), like_count=VALUES(like_count), description_full=VALUES(description_full),
        date_uploaded=VALUES(date_uploaded), date_uploaded_unix=VALUES(date_uploaded_unix)
    """, params)

def insert_torrents(cur, movie_id, torrents):
    if not torrents: return
    for t in torrents:
        date_uploaded = dt_from_unix(t.get("date_uploaded_unix")) or None
        cur.execute("""
          INSERT INTO torrents (hash, movie_id, url, quality, type, is_repack, video_codec, bit_depth,
                                audio_channels, seeds, peers, size, size_bytes, date_uploaded, date_uploaded_unix)
          VALUES (%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s)
          ON DUPLICATE KEY UPDATE seeds=VALUES(seeds), peers=VALUES(peers), size_bytes=VALUES(size_bytes)
        """, (
            t.get("hash"), movie_id, t.get("url"), t.get("quality"), t.get("type"),
            1 if str(t.get("is_repack")) not in ("0","false","False") else 0,
            t.get("video_codec"), t.get("bit_depth"), t.get("audio_channels"),
            t.get("seeds"), t.get("peers"), t.get("size"), t.get("size_bytes"),
            date_uploaded, t.get("date_uploaded_unix")
        ))

def insert_genres(cur, movie_id, genres):
    # remove existing mapping (optional) then reinsert (keeps sync)
    cur.execute("DELETE FROM movie_genres WHERE movie_id=%s", (movie_id,))
    for g in genres or []:
        gid = get_or_create_genre(cur, g)
        cur.execute("INSERT IGNORE INTO movie_genres (movie_id, genre_id) VALUES (%s,%s)", (movie_id, gid))

def main(path):
    with open(path, "r", encoding="utf-8") as f:
        payload = json.load(f)
    if payload.get("status") != "ok":
        print("status not ok", path); return

    movie = payload["data"].get("movie")
    if not movie: print("no movie", path); return

    conn = mysql.connector.connect(**DB_CONFIG)
    cur = conn.cursor()
    try:
        upsert_movie(cur, movie)
        insert_genres(cur, movie["id"], movie.get("genres", []))
        insert_torrents(cur, movie["id"], movie.get("torrents", []))
    finally:
        cur.close(); conn.close()
    print("Inserted/updated movie", movie["id"])

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: insert_movie.py PATH_TO_JSON"); sys.exit(1)
    main(sys.argv[1])
