#bin/bash

echo "Starting simple HTTP server."
echo "Open URL http://localhost:5100 on Chrome to run Arx Libertatis PNaCl (or http://localhost:5100/x86_64 to run Arx on x86_64 NaCl)."
cd www-root
python -m SimpleHTTPServer 5100
