# FoodApp Server (C++ / Crow minimal starter)

**Environment:** Ubuntu (instructions below)

## What's included
- `main.cpp` — Crow-based HTTP + WebSocket server skeleton
- `include/db.hpp`, `src/db.cpp` — MySQL connection helper (using libmysqlclient)
- `include/translate.hpp`, `src/translate.cpp` — Google Translate helper (uses libcurl)
- `CMakeLists.txt` — Build script

**Note:** The Crow single-header (`crow_all.h`) and `nlohmann/json.hpp` are *not* included in this zip (too large / external). Please install them using the instructions below.

---

## Quick setup (Ubuntu)

1. Install system dependencies:

```bash
sudo apt update
sudo apt install -y g++ cmake libboost-all-dev libssl-dev libmysqlclient-dev libcurl4-openssl-dev
sudo apt install -y nlohmann-json3-dev   # provides <nlohmann/json.hpp>
```

2. Get Crow single header (place into `include/`):
   - Download `crow_all.h` from the Crow repo and put it in the project's `include/` folder:
     https://github.com/CrowCpp/Crow (search for `crow_all.h`)

3. Configure MySQL DB:
```sql
CREATE DATABASE foodapp;
USE foodapp;
CREATE TABLE orders (
  id INT AUTO_INCREMENT PRIMARY KEY,
  user VARCHAR(255),
  item VARCHAR(255),
  quantity INT,
  created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);
```

4. Provide Google Translate API key:
   - Open `src/translate.cpp` and replace `YOUR_API_KEY_HERE` with your key.

5. Build & run:
```bash
mkdir build && cd build
cmake ..
make
./server
```

6. Test endpoints:
- Root:
  ```bash
  curl http://localhost:8080/
  ```
- POST /order:
  ```bash
  curl -X POST http://localhost:8080/order -H "Content-Type: application/json" -d '{"user":"Bryan","item":"Tacos","quantity":3}'
  ```
- POST /translate:
  ```bash
  curl -X POST http://localhost:8080/translate -H "Content-Type: application/json" -d '{"text":"Hello","target":"es"}'
  ```
- WebSocket:
  Connect to `ws://localhost:8080/ws` from a WebSocket client (webpage or tester).

---
## Notes & Troubleshooting
- If you get missing-header errors for `crow_all.h`, download the single header and place it at `include/crow_all.h`.
- `nlohmann/json.hpp` is provided by `nlohmann-json3-dev` on Ubuntu; the code includes `<nlohmann/json.hpp>`.
- If `mysqlclient` link fails, make sure `libmysqlclient-dev` was installed and the library is discoverable.

---
## What's next
- Replace the simple SQL string concatenation with prepared statements to avoid SQL injection.
- Move configuration (DB creds, API key) to environment variables or a config file.
- Add authentication (JWT) and order-status WebSocket broadcasts.
